#include "expr_tokens.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>


static void push_char(String_Buffer *str, char x);
static void reset_buffer(String_Buffer *str);
static void init_string_buffer(String_Buffer *str);
static void erase_start(String_Buffer *str, int n);
static int regex_match(const char *reg_expr, char *string);
static Expr* match_token(String_Buffer *str);
static void push_expr_list(Expr_List *list, Expr *expr);


/* For Debugging, print token names*/
static const char *NUM_TO_TOKEN[] = {
    INT_TO_TOKEN_NAME
};


static void push_char(String_Buffer *str, char x) {
	/* Increase size if necessary */
	if (str->size >= str->capacity-1) {
		char *copied = malloc(str->size+1);
		strcpy(copied, str->string);
		str->capacity *= 2;
		str->string = realloc(str->string, str->capacity);
		strcpy(str->string, copied);
		free(copied);
	} 
	str->string[str->size] = x;
	str->string[str->size+1] = '\0';
	str->size++;
	
	
}


/* remove n characters from front of string */
static void erase_start(String_Buffer *str, int n) {
	char *copy = malloc(str->size+1 - n);
	strcpy(copy, &((str->string)[n]));
	str->size -= n;

	/* Incase longgg string, keep small */
	if (str->size < str->capacity / 4) {
		str->capacity = MAX(str->size * 2, 16);
		str->string = realloc(str->string, str->capacity);
	}
	strcpy(str->string, copy);
	free(copy);
}



static void reset_buffer(String_Buffer *str) {
	free(str->string);
	init_string_buffer(str);
}


static void init_string_buffer(String_Buffer *str) {
	str->capacity = 16;
	str->size = 0;
	str->string = malloc(str->capacity);
}


/* 
 * Given a regex expression and a string
 * Return 1 on match, 0 on no match.
*/
static int regex_match(const char *reg_expr, char *string) {
	regex_t regex;
	regmatch_t match[1];
	regcomp(&regex, reg_expr, REG_EXTENDED);
	int check_match = regexec(&regex, string, 1, match, 0);
	regfree(&regex);
	if (!check_match) 
		return match[0].rm_eo;
	return 0;
}







static Expr* match_token(String_Buffer *str) {
	char *token = str->string;
	int length;
	Expr *expr = malloc(sizeof(Expr));
	expr->token = malloc(sizeof(Token));

	if (regex_match("^\\(", token)) {
		expr->TOK_ID = TOK_LPAREN;
		erase_start(str, 1);
	} else if (regex_match("^\\)", token)) {
		expr->TOK_ID = TOK_RPAREN;
		erase_start(str, 1);
	} else if (regex_match("^\\{", token)) {
		expr->TOK_ID = TOK_LBRACE;
		erase_start(str, 1);
	} else if (regex_match("^\\}", token)) {
		expr->TOK_ID = TOK_RBRACE;
		erase_start(str, 1);
	} else if (regex_match("^==", token)) {
		expr->TOK_ID = TOK_EQUAL;
		erase_start(str, 2);
	} else if (regex_match("^!=", token)) {
		expr->TOK_ID = TOK_NOTEQUAL;
		erase_start(str, 2);
	} else if (regex_match("^<=", token)) {
		expr->TOK_ID = TOK_LESSEQUAL;
		erase_start(str, 2);
	} else if (regex_match("^>=", token)) {
		expr->TOK_ID = TOK_GREATEREQUAL;
		erase_start(str, 2);
	} else if (regex_match("^>", token)) {
		expr->TOK_ID = TOK_GREATER;
		erase_start(str, 1);
	} else if (regex_match("^<", token)) {
		expr->TOK_ID = TOK_LESS;
		erase_start(str, 1);
	} else if (regex_match("^=", token)) {
		expr->TOK_ID = TOK_EQUAL;
		erase_start(str, 1);
	} else if (regex_match("^\\+", token)) {
		expr->TOK_ID = TOK_ADD;
		erase_start(str, 1);
	} else if (regex_match("^\\-", token)) {
		expr->TOK_ID = TOK_SUB;
		erase_start(str, 1);
	} else if (regex_match("^/", token)) {
		expr->TOK_ID = TOK_DIV;
		erase_start(str, 1);
	} else if (regex_match("^\\*\\*", token)) {
		expr->TOK_ID = TOK_POW;
		erase_start(str, 2);
	} else if (regex_match("^\\*", token)) {
		expr->TOK_ID = TOK_MULT;
		erase_start(str, 1);
	} else if (regex_match("^;", token)) {
		expr->TOK_ID = TOK_SEMI;
		erase_start(str, 1);
	} else if (regex_match("^,", token)){
		expr->TOK_ID = TOK_COMMA;
		erase_start(str, 1);
	} else if ((length=regex_match("^(\\-)?(([0-9]*\\.([0-9]+))|[0-9]+\\.[0-9]*)", token))) {
		expr->TOK_ID = TOK_DOUBLE;
		char *tmp;
		expr->token->floating_point = strtod(str->string, &tmp);
		//erase_start(str, strlen(str->string) - strlen(tmp));
		erase_start(str, length);
	} 


	else if ((length = regex_match("^((\\-)?[0-9]+)", token))) {
		expr->TOK_ID = TOK_INT;
		expr->token->integer = atoi(token);
		erase_start(str, length);
	} 

	else if ((length=regex_match("^([a-zA-Z][a-zA-Z0-9_]*)", token))) {
		if (strcmp(token, "or") == 0) {
			expr->TOK_ID = TOK_OR;
			erase_start(str, 2);
		} else if (strcmp(token, "def") == 0) {
			expr->TOK_ID = TOK_DEF;
			erase_start(str, 3);
		}
		
		else if (strcmp(token, "and") == 0) {
			expr->TOK_ID = TOK_AND;
			erase_start(str, 3);
		}
		else if (strcmp(token, "not") == 0) {
			expr->TOK_ID = TOK_NOT;
			erase_start(str, 3);
		}
		else if (strcmp(token, "print") == 0) {
			expr->TOK_ID = TOK_PRINT;
			erase_start(str, 5);
		}
		else if (strcmp(token, "if") == 0) {
			expr->TOK_ID = TOK_IF;
			erase_start(str, 2);
		}
		else if (strcmp(token, "else") == 0) {
			expr->TOK_ID = TOK_ELSE;
			erase_start(str, 4);
		}
		else if (strcmp(token, "for") == 0) {
			expr->TOK_ID = TOK_FOR;
			erase_start(str, 3);
		}
		else if (strcmp(token, "while") == 0) {
			expr->TOK_ID = TOK_WHILE;
			erase_start(str, 5);
		} else {
			expr->TOK_ID = TOK_VAR;
			expr->token->name = malloc(length+1);
			strncpy(expr->token->name, token, length);
			expr->token->name[length] = '\0';
			erase_start(str, length);
		}

	} else {
		printf("Lexer Error: %s\n", str->string);
		exit(-1);
	}



	return expr;
}



static void push_expr_list(Expr_List *list, Expr *expr) {
	if (list == NULL)
		return;
	expr->next = NULL; //Thought this was default..
	if (list->head == NULL) {
		list->head = expr;
		list->tail = expr;
	} else {
		list->tail->next = expr;
		list->tail = expr;
	}
}

void p(FILE *f) {

	char c;
	Expr *expr;
	Expr_List *list = malloc(sizeof(Expr_List));
	String_Buffer *str = malloc(sizeof(String_Buffer));
	init_string_buffer(str);
	char string[128];
	while (fgets(string, 128, f)) {
		for(int i = 0; i < 128; i++) {
			char c = string[i];
			if (c == '\0')
				break;
			if (c != ' ' && c != '\t' && c != '\r' && c!='\n') 
				push_char(str, c);
			else {

				while (str->size != 0) {
					expr = match_token(str);
					//printf("%s\n", NUM_TO_TOKEN[expr->TOK_ID]);
					push_expr_list(list, expr);
				}
				
				reset_buffer(str);
			}
		}
	}
	while (str->size != 0) {
			expr = match_token(str);
			//printf("%s\n", NUM_TO_TOKEN[expr->TOK_ID]);
			push_expr_list(list, expr);
		}
		
		reset_buffer(str);

	
	/*
	while (fscanf(f, "%c", &c) != EOF) {

		if (c != ' ' && c != '\t' && c != '\r' && c!='\n') 
			push_char(str, c);
		else {

			while (str->size != 0) {
				expr = match_token(str);
				//printf("%s\n", NUM_TO_TOKEN[expr->TOK_ID]);
				push_expr_list(list, expr);
			}
			
			reset_buffer(str);
		}
		
	}*/
	
	

	Expr *tmp, *t;
	tmp = list->head;
	while (tmp != NULL) {
		if (tmp->TOK_ID != TOK_VAR && tmp->TOK_ID != TOK_INT
			&& tmp->TOK_ID != TOK_DOUBLE)
			printf("%s\n", NUM_TO_TOKEN[tmp->TOK_ID]);
		else {
			if (tmp->TOK_ID == TOK_VAR) {
				printf("type %s: name: %s\n", NUM_TO_TOKEN[tmp->TOK_ID], tmp->token->name);
				free(tmp->token->name);
			} else if (tmp->TOK_ID == TOK_INT){
				printf("type %s: name: %ld\n", NUM_TO_TOKEN[tmp->TOK_ID], tmp->token->integer);
			} else {
				printf("type %s: name: %lf\n", NUM_TO_TOKEN[tmp->TOK_ID], tmp->token->floating_point);
			}
		}
		t =tmp;
		tmp = tmp->next;
		free(t->token);
		free(t);
	}

	free(str->string);
	free(str);
	free(list->head);

	free(list->tail);
	free(list);

} 




int main() {


	FILE *f = fopen("test_input.cobra", "r");
	if (f != NULL)
		p(f);
	/*str = malloc(sizeof(String_Buffer));
	str->capacity = 8;
	str->size = 0;
	str->string = malloc(str->capacity);
	*/
	/*for(int i = 0; i<26; i++) {
		printf("%s size: %d capacity: %d\n", str->string, str->size, str->capacity);
		push_char(str, (char)( ((int)'a')+i));
		printf("here\n");
	}*/
	fclose(f);

	return 0;
}