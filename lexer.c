#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>


static void push_char(StringBuffer *str, char x);
static void reset_buffer(StringBuffer *str);
static void init_StringBuffer(StringBuffer *str);
static void erase_start(StringBuffer *str, int n);
static int regex_match(const char *reg_expr, char *string);
static Token* match_token(StringBuffer *str);
static void push_TokenList(TokenList *list, Token *token);


/* For Debugging, print token names*/
static const char *NUM_TO_TOKEN[] = {
    INT_TO_TOKEN_NAME
};


static void push_char(StringBuffer *str, char x) {
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
static void erase_start(StringBuffer *str, int n) {
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



static void reset_buffer(StringBuffer *str) {
	free(str->string);
	init_StringBuffer(str);
}


static void init_StringBuffer(StringBuffer *str) {
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





static Token* match_token(StringBuffer *str) {
	char *tok = str->string;
	int length;
	Token *token = malloc(sizeof(Token));
	token->tokObject = malloc(sizeof(TokObject));

	if (regex_match("^\\(", tok)) {
		token->TOK_TYPE = TOK_LPAREN;
		erase_start(str, 1);
	} else if (regex_match("^\\)", tok)) {
		token->TOK_TYPE = TOK_RPAREN;
		erase_start(str, 1);
	} else if (regex_match("^\\{", tok)) {
		token->TOK_TYPE = TOK_LBRACE;
		erase_start(str, 1);
	} else if (regex_match("^\\}", tok)) {
		token->TOK_TYPE = TOK_RBRACE;
		erase_start(str, 1);
	} else if (regex_match("^==", tok)) {
		token->TOK_TYPE = TOK_EQUAL;
		erase_start(str, 2);
	} else if (regex_match("^!=", tok)) {
		token->TOK_TYPE = TOK_NOTEQUAL;
		erase_start(str, 2);
	} else if (regex_match("^<=", tok)) {
		token->TOK_TYPE = TOK_LESSEQUAL;
		erase_start(str, 2);
	} else if (regex_match("^>=", tok)) {
		token->TOK_TYPE = TOK_GREATEREQUAL;
		erase_start(str, 2);
	} else if (regex_match("^>", tok)) {
		token->TOK_TYPE = TOK_GREATER;
		erase_start(str, 1);
	} else if (regex_match("^<", tok)) {
		token->TOK_TYPE = TOK_LESS;
		erase_start(str, 1);
	} else if (regex_match("^=", tok)) {
		token->TOK_TYPE = TOK_ASSIGN;
		erase_start(str, 1);
	} else if (regex_match("^\\+", tok)) {
		token->TOK_TYPE = TOK_ADD;
		erase_start(str, 1);
	} else if (regex_match("^\\-", tok)) {
		token->TOK_TYPE = TOK_SUB;
		erase_start(str, 1);
	} else if (regex_match("^/", tok)) {
		token->TOK_TYPE = TOK_DIV;
		erase_start(str, 1);
	} else if (regex_match("^\\*\\*", tok)) {
		token->TOK_TYPE = TOK_POW;
		erase_start(str, 2);
	} else if (regex_match("^\\*", tok)) {
		token->TOK_TYPE = TOK_MULT;
		erase_start(str, 1);
	} else if (regex_match("^;", tok)) {
		token->TOK_TYPE = TOK_SEMI;
		erase_start(str, 1);
	} else if (regex_match("^,", tok)){
		token->TOK_TYPE = TOK_COMMA;
		erase_start(str, 1);
	} else if ((length=regex_match("^(\\-)?(([0-9]*\\.([0-9]+))|[0-9]+\\.[0-9]*)", tok))) {
		token->TOK_TYPE = TOK_DOUBLE;
		char *tmp;
		token->tokObject->floating_point = strtod(str->string, &tmp);
		//erase_start(str, strlen(str->string) - strlen(tmp));
		erase_start(str, length);
	} 


	else if ((length = regex_match("^((\\-)?[0-9]+)", tok))) {
		token->TOK_TYPE = TOK_INT;
		token->tokObject->integer = atoi(tok);
		erase_start(str, length);
	} 

	else if ((length=regex_match("^([a-zA-Z][a-zA-Z0-9_]*)", tok))) {
		if (strcmp(tok, "or") == 0) {
			token->TOK_TYPE = TOK_OR;
			erase_start(str, 2);
		} else if (strcmp(tok, "def") == 0) {
			token->TOK_TYPE = TOK_DEF;
			erase_start(str, 3);
		}
		
		else if (strcmp(tok, "and") == 0) {
			token->TOK_TYPE = TOK_AND;
			erase_start(str, 3);
		}
		else if (strcmp(tok, "not") == 0) {
			token->TOK_TYPE = TOK_NOT;
			erase_start(str, 3);
		}
		else if (strcmp(tok, "print") == 0) {
			token->TOK_TYPE = TOK_PRINT;
			erase_start(str, 5);
		}
		else if (strcmp(tok, "if") == 0) {
			token->TOK_TYPE = TOK_IF;
			erase_start(str, 2);
		}
		else if (strcmp(tok, "else") == 0) {
			token->TOK_TYPE = TOK_ELSE;
			erase_start(str, 4);
		}
		else if (strcmp(tok, "for") == 0) {
			token->TOK_TYPE = TOK_FOR;
			erase_start(str, 3);
		}
		else if (strcmp(tok, "while") == 0) {
			token->TOK_TYPE = TOK_WHILE;
			erase_start(str, 5);
		}else if (strcmp(tok, "True") == 0) {
			token->TOK_TYPE = TOK_BOOL;
			token->tokObject->integer = 1;
			erase_start(str, 4);
		}else if (strcmp(tok, "False") == 0) {
			token->TOK_TYPE = TOK_BOOL;
			token->tokObject->integer = 0;
			erase_start(str, 5);
		} else {
			token->TOK_TYPE = TOK_VAR;
			token->tokObject->name = malloc(length+1);
			strncpy(token->tokObject->name, tok, length);
			token->tokObject->name[length] = '\0';
			erase_start(str, length);
		}

	} else {
		printf("Lexer Error: %s\n", str->string);
		exit(-1);
	}



	return token;
}




static void push_TokenList(TokenList *list, Token *token) {
	if (list == NULL)
		return;
	token->next = NULL; //Thought this was default..
	if (list->head == NULL) {
		list->head = token;
		list->tail = token;
	} else {
		list->tail->next = token;
		list->tail = token;
	}
}




void printTokList(TokenList *list) {
	Token *tmp, *t;
	tmp = list->head;
	while (tmp != NULL) {
		if (tmp->TOK_TYPE != TOK_VAR && tmp->TOK_TYPE != TOK_INT
			&& tmp->TOK_TYPE != TOK_DOUBLE)
			printf("(%s); ", NUM_TO_TOKEN[tmp->TOK_TYPE]);
		else {
			if (tmp->TOK_TYPE == TOK_VAR) {
				printf("(type %s: name: %s); ", NUM_TO_TOKEN[tmp->TOK_TYPE], tmp->tokObject->name);
				//free(tmp->tokObject->name);
			} else if (tmp->TOK_TYPE == TOK_INT){
				printf("(type %s: name: %ld); ", NUM_TO_TOKEN[tmp->TOK_TYPE], tmp->tokObject->integer);
			} else {
				printf("(type %s: name: %lf);", NUM_TO_TOKEN[tmp->TOK_TYPE], tmp->tokObject->floating_point);
			}
		}
		t =tmp;
		tmp = tmp->next;
		//free(t->tokObject);
		//free(t);
	}
	printf("\n");
}



TokenList* tokenize(FILE *f) {
	char c;
	Token *token;
	TokenList *list = malloc(sizeof(TokenList));
	StringBuffer *str = malloc(sizeof(StringBuffer));
	init_StringBuffer(str);
	char string[128];
	while (fgets(string, 128, f)) {
		for(int i = 0; i < 128; i++) {
			c = string[i];
			if (c == '\0')
				break;
			if (c != ' ' && c != '\t' && c != '\r' && c!='\n') 
				push_char(str, c);
			else {

				while (str->size != 0) {
					token = match_token(str);
					push_TokenList(list, token);
				}
				
				reset_buffer(str);
			}
		}
	}
	while (str->size != 0) {
		token = match_token(str);
		push_TokenList(list, token);
	}
	
	reset_buffer(str);
	//Honestly not sure why p4a has this..
	//So we're just not gonna
	/*
	printf("here\n");
	token->TOK_TYPE = TOK_EOF;
	push_TokenList(list, token);*/
	return list;

}



/*

int main() {


	FILE *f = fopen("test_input.cobra", "r");
	if (f != NULL) {
		TokenList *list = tokenize(f);
		printTokList(list);

	}
	str = malloc(sizeof(StringBuffer));
	str->capacity = 8;
	str->size = 0;
	str->string = malloc(str->capacity);
	
	for(int i = 0; i<26; i++) {
		printf("%s size: %d capacity: %d\n", str->string, str->size, str->capacity);
		push_char(str, (char)( ((int)'a')+i));
		printf("here\n");
	}
	fclose(f);

	return 0;
*/