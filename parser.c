#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include "parser.h"


static const char *NUM_TO_TOKEN[] = {
    INT_TO_TOKEN_NAME
};

static const char *EXPR_TO_NAME[] = {
    INT_TO_EXPR_NAME
};

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

void initTokenList(TokenList *list) {
	list->head = NULL;
	list->tail = NULL;
}

void initExpression(Expression *expr){
	expr->next = NULL;
	expr->expr1 = NULL;
	expr->expr2 = NULL;
}

void initParseList(ParseList *parseList) {
	parseList->expression = malloc(sizeof(Expression));
	initExpression(parseList->expression);
	parseList->tokenList = malloc(sizeof(TokenList));
	parseList->tokenList->head = NULL;
	parseList->tokenList->tail = NULL;
}






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



Expression *copyExpression(Expression *expr) {
	if (expr == NULL)
		return NULL;

	Expression *copy = malloc(sizeof(Expression));

	copy->type = expr->type;
	if (copy->type != EXPR_INT && copy->type != EXPR_VAR) {
		copy->expr1 = copyExpression(expr->expr1);
		copy->expr2 = copyExpression(expr->expr2);
	} else {
		switch (copy->type) {
			case EXPR_VAR:
				copy->name = expr->name;
			case EXPR_INT:
				copy->value = expr->value;
		}
		
	}
	

	
	return copy;

}
/*
ExpressionList* copyExpressionList(ExpressionList *exprlist) {
	ExpressionList *copy = malloc(sizeof(ExpressionList));
	initExpressionList(copy);
	if (exprlist->head == NULL) {
		printf("Bug\n");
	}
	copy->head = copyExpression(exprlist->head);
	return copy;
}
*/

ParseList *pushExpression(ParseList *parseList, Expression *expr) {
	//expr->next = parseList->expressionList->head;
	//parseList->expressionList->head = expr;
	//printExpression(expr);
	//printf("\n");
	parseList->expression = expr;
	return parseList;
}

Expression *newExpression(void){
	Expression *expr = malloc(sizeof(Expression));
	initExpression(expr);
	return expr;
}


Expression* parse_expr(TokenList *tokenList) {
	//old
	//ParseList *parseList = parse_plus(tokenList);
	ParseList *parseList = parse_S(tokenList);
	return parseList->expression;
}


ParseList *parse_S(TokenList *tokenList) {
	return parse_or(tokenList);
}

ParseList *parse_or(TokenList *tokenList) {
	ParseList *after_parse_and = parse_and(tokenList);
	Token *head = peek(tokenList);

	if (head == NULL)
		return after_parse_and;

	unsigned char tok_type = head->TOK_TYPE;
	if (tok_type == TOK_OR) {
		Expression *expr = newExpression();
		pop(tokenList);

		expr->expr1 = copyExpression(after_parse_and->expression);
		expr->type = EXPR_OR;
		expr->expr2 = parse_or(after_parse_and->tokenList)->expression;
		return pushExpression(after_parse_and, expr);
	}

	return after_parse_and;
}

ParseList *parse_and(TokenList *tokenList) {
	ParseList *after_parse_equality = parse_not(tokenList);
	Token *head = peek(tokenList);

	if (head == NULL)
		return after_parse_equality;

	unsigned char tok_type = head->TOK_TYPE;
	if (tok_type == TOK_AND) {
		Expression *expr = newExpression();
		pop(tokenList);

		expr->expr1 = copyExpression(after_parse_equality->expression);
		expr->type = EXPR_AND;
		expr->expr2 = parse_and(after_parse_equality->tokenList)->expression;
		return pushExpression(after_parse_equality, expr);
	}

	return after_parse_equality;
}
ParseList *parse_not(TokenList *tokenList) {
	Token *head = peek(tokenList);
	if (head == NULL) 
		return NULL;
	
	unsigned char tok_type = head->TOK_TYPE;
	if (tok_type == TOK_NOT) {
		Expression *expr = newExpression();
		pop(tokenList);
		ParseList* parseStep = parse_equality(tokenList);
		expr->expr1 = copyExpression(parseStep->expression); //I should reason out if copy is necessary
		expr->type = EXPR_NOT;
		expr->expr2 = NULL; //No expression 2

		return pushExpression(parseStep, expr);
	}
	/* Default case */
	return parse_equality(tokenList);

}



ParseList *parse_equality(TokenList *tokenList) {
	Expression *expr;
	ParseList *after_parse_relation = parse_relation(tokenList);
	Token *head = peek(tokenList);

	if (head == NULL)
		return after_parse_relation;

	unsigned char tok_type = head->TOK_TYPE;
	if (tok_type == TOK_EQUAL) {
		expr = newExpression();
		pop(tokenList);

		expr->expr1 = copyExpression(after_parse_relation->expression);
		expr->type = EXPR_EQUAL;
		expr->expr2 = parse_equality(after_parse_relation->tokenList)->expression;
		return pushExpression(after_parse_relation, expr);
	} else if (tok_type == TOK_NOTEQUAL) {
		expr = newExpression();
		pop(tokenList);

		expr->expr1 = copyExpression(after_parse_relation->expression);
		expr->type = EXPR_NOTEQUAL;
		expr->expr2 = parse_equality(after_parse_relation->tokenList)->expression;
		return pushExpression(after_parse_relation, expr);
	} 

	return after_parse_relation;
}


ParseList *parse_relation(TokenList *tokenList) {
	Expression *expr;
	ParseList *after_parse_add = parse_add(tokenList);
	Token *head = peek(tokenList);

	if (head == NULL)
		return after_parse_add;

	unsigned char tok_type = head->TOK_TYPE;

	switch (tok_type) {
		case TOK_LESS:
			expr = newExpression();
			pop(tokenList);
			expr->expr1 = copyExpression(after_parse_add->expression);
			expr->type = EXPR_LESS;
			expr->expr2 = parse_relation(after_parse_add->tokenList)->expression;
			return pushExpression(after_parse_add, expr);

		case TOK_LESSEQUAL:
			expr = newExpression();
			pop(tokenList);
			expr->expr1 = copyExpression(after_parse_add->expression);
			expr->type = EXPR_LESSEQUAL;
			expr->expr2 = parse_relation(after_parse_add->tokenList)->expression;
			return pushExpression(after_parse_add, expr);

		case TOK_GREATER:
			expr = newExpression();
			pop(tokenList);
			expr->expr1 = copyExpression(after_parse_add->expression);
			expr->type = EXPR_GREATER;
			expr->expr2 = parse_relation(after_parse_add->tokenList)->expression;
			return pushExpression(after_parse_add, expr);

		case TOK_GREATEREQUAL:
			expr = newExpression();
			pop(tokenList);
			expr->expr1 = copyExpression(after_parse_add->expression);
			expr->type = EXPR_GREATEREQUAL;
			expr->expr2 = parse_relation(after_parse_add->tokenList)->expression;
			return pushExpression(after_parse_add, expr);
	}


	return after_parse_add;
}

ParseList *parse_add(TokenList *tokenList) {
	Expression *expr;
	ParseList *after_parse_mult = parse_mult(tokenList);
	Token *head = peek(tokenList);

	if (head == NULL)
		return after_parse_mult;

	unsigned char tok_type = head->TOK_TYPE;

	switch (tok_type) {
		case TOK_ADD:
			expr = newExpression();
			pop(tokenList);

			expr->expr1 = copyExpression(after_parse_mult->expression);
			expr->type = EXPR_ADD;
			expr->expr2 = parse_add(after_parse_mult->tokenList)->expression;
			return pushExpression(after_parse_mult, expr);

		case TOK_SUB:
			expr = newExpression();
			pop(tokenList);

			expr->expr1 = copyExpression(after_parse_mult->expression);
			expr->type = EXPR_SUB;
			expr->expr2 = parse_add(after_parse_mult->tokenList)->expression;
			return pushExpression(after_parse_mult, expr);
	}

	

	return after_parse_mult;
}

ParseList *parse_mult(TokenList *tokenList) {
	Expression *expr;
	ParseList *after_parse_power = parse_power(tokenList);
	Token *head = peek(tokenList);

	if (head == NULL)
		return after_parse_power;

	unsigned char tok_type = head->TOK_TYPE;

	switch (tok_type) {
		case TOK_MULT:
			expr = newExpression();
			pop(tokenList);

			expr->expr1 = copyExpression(after_parse_power->expression);
			expr->type = EXPR_MULT;
			expr->expr2 = parse_mult(after_parse_power->tokenList)->expression;
			return pushExpression(after_parse_power, expr);

		case TOK_DIV:
			expr = newExpression();
			pop(tokenList);

			expr->expr1 = copyExpression(after_parse_power->expression);
			expr->type = EXPR_DIV;
			expr->expr2 = parse_mult(after_parse_power->tokenList)->expression;
			return pushExpression(after_parse_power, expr);
	}

	
	return after_parse_power;
}

ParseList *parse_power(TokenList *tokenList) {
	Expression *expr;
	ParseList *after_parse_function = parse_function(tokenList);
	/* unary returns NULL on no token */
	if (after_parse_function == NULL) 
		return after_parse_function;
	
	Token *head = peek(tokenList);

	if (head == NULL)
		return after_parse_function;

	unsigned char tok_type = head->TOK_TYPE;

	switch (tok_type) {
		case TOK_POW:
			expr = newExpression();
			pop(tokenList);

			expr->expr1 = copyExpression(after_parse_function->expression);
			expr->type = EXPR_POW;
			expr->expr2 = parse_power(after_parse_function->tokenList)->expression;
			return pushExpression(after_parse_function, expr);
	}

	
	return after_parse_function;
}


ParseList *parse_function(TokenList *tokenList) {
	Token *head = peek(tokenList);
	Expression *expr;
	ParseList *parseList;
	

	if (head == NULL) {
		return parse_unary(tokenList);
	}


	unsigned char tok_type = head->TOK_TYPE;
	if (tok_type == TOK_VAR) {
		//this is kinda ugly, but I think
		//handling it at lexing is worse...
		//lookahead again, check for lparen
		Token *after_head = head->next;
		if (after_head != NULL) {
			tok_type = after_head->TOK_TYPE;
			if (tok_type == TOK_LPAREN) {
				//parse until comma...
				expr = newExpression();
				
				pop(tokenList);
				parseList = parse_function(tokenList);
				expr->type = EXPR_FUNCTIONCALL;
				expr->expr1 = copyExpression(parseList->expression);
				expr->expr2 = NULL;
				return pushExpression(parseList, expr);
			}
		}

	}



	return parse_unary(tokenList);;
}



/* 
 * I'm calling  a comma a unary operator.. sue me
*/
ParseList *parse_unary(TokenList *tokenList) {
	/*Token *head = peek(tokenList);
	if (head == NULL) 
		return NULL;
	
	unsigned char tok_type = head->TOK_TYPE;
	if (tok_type == TOK_COMMA) {
		Expression *expr = newExpression();
		pop(tokenList);
		ParseList* parseStep = parse_unary(tokenList);
		expr->expr1 = copyExpression(parseStep->expression); //I should reason out if copy is necessary
		expr->type = EXPR_NOT;
		expr->expr2 = NULL; //No expression 2

		return pushExpression(parseStep, expr);
	}*/

	Expression *expr;
	ParseList *after_parse_primary = parse_primary(tokenList);
	Token *head = peek(tokenList);
	if (head == NULL)
		return after_parse_primary;
	
	unsigned char tok_type = head->TOK_TYPE;
	if (tok_type == TOK_COMMA) {
		expr = newExpression();
		pop(tokenList);
		expr->expr1 = copyExpression(after_parse_primary->expression);
		expr->type = EXPR_COMMA;
		expr->expr2 = parse_unary(after_parse_primary->tokenList)->expression;
		
		return pushExpression(after_parse_primary, expr);
	}



	/* Default case */
	return after_parse_primary;


}












ParseList *parse_primary(TokenList *tokenList) {
	Token *head = peek(tokenList);
	Expression *expr;
	ParseList *parseList;
	//Need to change structure a bit for
	//better error messages - later problem
	if (head == NULL) {	
		printf("Missing Value!\n");
		exit(-1);
	}

	unsigned char tok_type = head->TOK_TYPE;

	switch (tok_type) {

		case TOK_INT:
			expr = newExpression();
			parseList = malloc(sizeof(ParseList));
			initParseList(parseList);
			parseList->tokenList = pop(tokenList);
			expr->type = EXPR_INT;
			expr->value = head->tokObject->integer;
			return pushExpression(parseList, expr);

		//Bools are just tagged integers..
		case TOK_BOOL:
			expr = newExpression();
			parseList = malloc(sizeof(ParseList));
			initParseList(parseList);
			parseList->tokenList = pop(tokenList);
			expr->type = EXPR_BOOL;
			expr->value = head->tokObject->integer;
			return pushExpression(parseList, expr);

		case TOK_VAR:
			//printf("Called TOK_VAR\n");
			expr = newExpression();
			parseList = malloc(sizeof(ParseList));
			initParseList(parseList);
			parseList->tokenList = pop(tokenList);
			expr->type = EXPR_VAR;
			expr->name = head->tokObject->name;
			//pushExpression(parseList, expr);
			
			return pushExpression(parseList, expr);
			


		case TOK_LPAREN:
			expr = newExpression();
			parseList = malloc(sizeof(ParseList));
			initParseList(parseList);
			pop(tokenList);
			parseList = parse_S(tokenList);
			//now match closing parenthesis necessary
			//if this fails, so does the program
			Token *tmp = peek(tokenList);
			
			if (tmp == NULL || tmp->TOK_TYPE != TOK_RPAREN) {
				printf("Missing match ')'\n");
				exit(-1);
			}
			pop(tokenList);
			
			return parseList;
			
	}

	printf("Invalid tokens remaing: ");
	printTokList(tokenList);
	printExpression(expr);
	exit(-1);

	return NULL; //keep compiler happy
}

















/* OLD SHIT*/

/*
ParseList* parse_plus(TokenList *tokenList) {
	//printTokList(tokenList);
	ParseList *after_parse_mult = parse_mult(tokenList);
	Token *head = peek(after_parse_mult->tokenList);

	if (head == NULL)
		return after_parse_mult;

	unsigned char token_type = head->TOK_TYPE;

	if (token_type == TOK_ADD) {
		Expression *expr = malloc(sizeof(Expression));
		initExpression(expr);
		pop(tokenList);
		
		//expr->expr1 = copyExpressionList(after_parse_mult->expressionList)->head;
		expr->expr1 = copyExpression(after_parse_mult->expression);
		
		expr->type = EXPR_ADD;
		expr->expr2 = parse_plus(after_parse_mult->tokenList)->expression;
		return pushExpression(after_parse_mult, expr);
	} 

	return after_parse_mult;
}


ParseList* parse_mult(TokenList *tokenList) {
	Expression *expr_int;
	ParseList *parse_postInt = parse_int(tokenList);
	expr_int = parse_postInt->expression;

	Token *head;
	head = peek(parse_postInt->tokenList);
	if (head == NULL)
		return parse_postInt;

	unsigned char token_type = head->TOK_TYPE;
	if (token_type == TOK_MULT) {
		Expression *expr = malloc(sizeof(Expression));
		pop(tokenList);
		expr->type = EXPR_MULT;
		//expr->expr1 = copyExpressionList(expr_int)->head;
		expr->expr1 = copyExpression(expr_int);
		expr->expr2 = parse_mult(parse_postInt->tokenList)->expression;
		return pushExpression(parse_postInt, expr);
	}

	//printTokList(parse_postInt->tokenList);
	//printParseList(parse_postInt); 
	return parse_postInt;
}

ParseList* parse_int(TokenList *tokenList) {
	Token *head;
	ParseList *parseList;
	head = peek(tokenList);
	
	unsigned char token_type = head->TOK_TYPE;

	if (token_type == TOK_INT) {
		Expression *expr = malloc(sizeof(Expression));
		expr->next = NULL;
		parseList = malloc(sizeof(ParseList));
		initParseList(parseList);
		parseList->tokenList = pop(tokenList);
		expr->type = EXPR_INT;
		expr->value = head->tokObject->integer;
		return pushExpression(parseList, expr);
	} else {
		printf("Parse Error: expected integer, recieved %s\n", NUM_TO_TOKEN[token_type]);
		exit(-1);
	}


}*/


/* Assumes not type int */
void printExpression(Expression *expr) {
	if (expr == NULL)
		return;

	//printf("here\n");
	ExpressionType type = expr->type;

	if (type != EXPR_INT && type != EXPR_VAR) {
		printf("(");
		printf("%s: ", EXPR_TO_NAME[type]);
		
		printExpression(expr->expr1);
		printExpression(expr->expr2);
		printf(")");
	} else {
		if (type == EXPR_INT)
			printf("%ld ", expr->value);
		else if (type == EXPR_VAR)
			printf("%s ", expr->name);
	}
	

}

void printParseList(ParseList *parseList) {
	Expression *expr;
	if (parseList == NULL || (expr = parseList->expression) == NULL)
		return;


	printExpression(expr);
	printf("\n");
		

}
/*
int main() {
	FILE *f = fopen("test.txt", "r");
	TokenList *list;
	ParseList *parseList;
	Expression *expr;
	if (f != NULL) {
		list = tokenize(f);
		//printTokList(list);

	}

	fclose(f);

	expr = parser(list);
	printExpression(expr);
	printf("\n");

	return 0;
}*/