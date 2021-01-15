#include "lexer.h"

#ifndef PARSER_H
#define PARSER_H

#define INT_TO_EXPR_NAME \
	GEN_TOKEN(EXPR_ADD)  \
	GEN_TOKEN(EXPR_MULT)  \
	GEN_TOKEN(EXPR_INT)  \
	GEN_TOKEN(EXPR_VAR)  \
	GEN_TOKEN(EXPR_BOOL)  \
	GEN_TOKEN(EXPR_SUB)  \
	GEN_TOKEN(EXPR_DIV)  \
	GEN_TOKEN(EXPR_POW)  \
	GEN_TOKEN(EXPR_GREATER)  \
	GEN_TOKEN(EXPR_LESS)  \
	GEN_TOKEN(EXPR_GREATEREQUAL)  \
	GEN_TOKEN(EXPR_LESSEQUAL)  \
	GEN_TOKEN(EXPR_EQUAL)  \
	GEN_TOKEN(EXPR_NOTEQUAL)  \
	GEN_TOKEN(EXPR_OR)  \
	GEN_TOKEN(EXPR_AND)  \
	GEN_TOKEN(EXPR_NOT)  \
	GEN_TOKEN(EXPR_FUNCTIONCALL)	\
	GEN_TOKEN(EXPR_COMMA)	\

#define GEN_EXPR(STRING) #STRING,


typedef enum {
	EXPR_ADD,
	EXPR_MULT,
	EXPR_INT,
	EXPR_VAR,
	EXPR_BOOL,
	EXPR_SUB,
	EXPR_DIV,
	EXPR_POW,
	EXPR_GREATER,
	EXPR_LESS,
	EXPR_GREATEREQUAL,
	EXPR_LESSEQUAL,
	EXPR_EQUAL,
	EXPR_NOTEQUAL,
	EXPR_OR,
	EXPR_AND,
	EXPR_NOT,
	EXPR_FUNCTIONCALL,
	EXPR_COMMA,

} ExpressionType;

typedef struct expr {
	ExpressionType type;
	union {
		long value;
		char *name;
		struct {
			struct expr *expr1;
			struct expr *expr2;
		};
	};
	struct expr *next;
}  Expression;




typedef struct {
	Expression *expression;
	TokenList *tokenList;

} ParseList;

/*
ParseList* parse_plus(TokenList *tokenList);
ParseList* parse_mult(TokenList *tokenList);
ParseList* parse_int(TokenList *tokenList);*/
Expression* parse_expr(TokenList *tokenList);
ParseList *parse_or(TokenList *tokenList);
ParseList *parse_and(TokenList *tokenList);
ParseList *parse_equality(TokenList *tokenList);
ParseList *parse_relation(TokenList *tokenList);
ParseList *parse_add(TokenList *tokenList);
ParseList *parse_mult(TokenList *tokenList);
ParseList *parse_power(TokenList *tokenList);
ParseList *parse_unary(TokenList *tokenList);
ParseList *parse_primary(TokenList *tokenList);
ParseList *parse_not(TokenList *tokenList);
ParseList *parse_function(TokenList *tokenList);

ParseList *parse_S(TokenList *tokenList);





Expression *newExpression(void);








void printParseList(ParseList *parseList);
void printExpression(Expression *expr);

#endif