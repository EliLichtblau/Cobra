#include "parse_stmt.h"
#include "lexer.h"
#include "parser.h"
#include <stdlib.h>


Token* peek(TokenList *list) {
	return list->head;
}

TokenList* pop(TokenList *list) {
	if (list == NULL ||
		list->head == NULL) {
		printf("Empty list!!! error\n");
		exit(-1);
	}
	Token *tmp = list->head;
	list->head = list->head->next;
	//free(tmp);
	if (list->head == NULL)
		list->tail = NULL;
	return list;
}


/* removes token if token type matches specified type */
TokenList* conditionlPop(TokenList *list, unsigned char toktype) {
	Token *head = peek(list);
	if (head == NULL) {
		printf("Empty list!!! error\n");
		exit(-1);
	}

	if (head->TOK_TYPE != toktype) {
		printf("Tok type mismatch: %d %d", head->TOK_TYPE, toktype);
	}
	pop(list);

}



Statement* newStatement() {
	Statement *stmt = malloc(sizeof(Statement));
	stmt->type = NOOP;
	return stmt;
}




Statement* parseStatement(TokenList *tokenlist) {
	Token *head;

	head = peek(tokenlist);

	if (head == NULL)
		return newStatement();

	Statement *after_parse_options = parse_options(tokenlist);
	//arecursive call
	Statement *rec_call = parseStatement(tokenlist);
	Statement *ret = newStatement();
	ret->type = SEQ;
	ret->stmt1 = after_parse_options;
	ret->stmt2 = rec_call;
	return ret;
}


Statement *parse_options(TokenList *tokenlist) {
	Token *head;

	head = peek(tokenlist);

	if (head == NULL) {
		printf("Missing some statement... parse statement options\n");
		exit(-1);
	}

	switch (head->TOK_TYPE) {
		case TOK_VAR:
			return parse_assign(tokenlist);

		case TOK_PRINT:
			return parse_print(tokenlist);

		case TOK_IF:
			return parse_if(tokenlist);
		case TOK_WHILE:
			return parse_while(tokenlist);

		case TOK_FOR:
			return parse_for(tokenlist);

	}

	printf("bad token in stmt options\n");
	exit(-1);
	return NULL;
}



Statement *parse_assign(TokenList *tokenlist) {
	Token *head;

	head = peek(tokenlist);

	if (head == NULL) {
		printf("Missing some statment... parse assign options\n");
		exit(-1);
	}

	if (head->TOK_TYPE == TOK_VAR) {
		pop(tokenlist);
		conditionlPop(tokenlist, TOK_EQUAL);
		ParseList *pl = parse_S(tokenlist);
		conditionlPop(tokenlist, TOK_SEMI);
		Statement *stmt = newStatement();
		stmt->type = ASSIGN;
		stmt->name = head->tokObject->name;
		stmt->exprAssign = pl->expression;
		return stmt;
	}


	printf("Error parssing assign, expected identfier\n");
	exit(-1);
	return NULL;
}


/* We are gauranteed that print is the first token */
Statement *parse_print(TokenList *tokenlist) {
	Token *head;

	head = peek(tokenlist);
	if (head == NULL) {
		printf("Missing some statment... parse print\n");
		exit(-1);
	}

	conditionlPop(tokenlist, TOK_PRINT);
	conditionlPop(tokenlist, TOK_LPAREN);
	ParseList *pl = parse_S(tokenlist);
	conditionlPop(tokenlist, TOK_RPAREN);
	conditionlPop(tokenlist, TOK_SEMI);

	Statement *stmt = newStatement();
	stmt->type = PRINT;
	stmt->exprPrint = pl->expression;

	return stmt;
}

Statement* parse_if(TokenList *tokenlist){}
Statement* parse_while(TokenList *tokenlist){}
Statement* parse_for(TokenList *tokenlist){}


int main() {

	return 0;
}