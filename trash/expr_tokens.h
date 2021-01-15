/* 
 * Parsing tokens for Cobra
*/
#include <stdio.h>

/* Tokens */
#define TOK_LPAREN 0 
#define TOK_RPAREN 1 
#define TOK_LBRACE 2 
#define TOK_RBRACE 3 
#define TOK_EQUAL 4 
#define TOK_NOTEQUAL 5 
#define TOK_ASSIGN 6 
#define TOK_GREATER 7 
#define TOK_GREATEREQUAL 8 
#define TOK_LESS 9 
#define TOK_LESSEQUAL 10 
#define TOK_OR 11 
#define TOK_AND 12 
#define TOK_NOT 13 
#define TOK_SEMI 14 
#define TOK_INT 15 
#define TOK_BOOL_TYPE 16 
#define TOK_PRINT 17 
#define TOK_IF 18 
#define TOK_ELSE 19  
#define TOK_FOR 20 
#define TOK_WHILE 21 
#define TOK_ADD 22
#define TOK_SUB 23
#define TOK_MULT 24
#define TOK_DIV 25
#define TOK_POW 26
#define TOK_DEF 27
#define TOK_DOUBLE 28
#define TOK_COMMA 29

#define TOK_VAR 30
#define TOK_STRING 31

/* convert passed token to */
#define INT_TO_TOKEN_NAME \
	GEN_TOKEN(TOK_LPAREN)  \
	GEN_TOKEN(TOK_RPAREN)  \
	GEN_TOKEN(TOK_LBRACE)  \
	GEN_TOKEN(TOK_RBRACE)  \
	GEN_TOKEN(TOK_EQUAL)  \
	GEN_TOKEN(TOK_NOTEQUAL)  \
	GEN_TOKEN(TOK_ASSIGN)  \
	GEN_TOKEN(TOK_GREATER)  \
	GEN_TOKEN(TOK_GREATEREQUAL)  \
	GEN_TOKEN(TOK_LESS)  \
	GEN_TOKEN(TOK_LESSEQUAL)  \
	GEN_TOKEN(TOK_OR)  \
	GEN_TOKEN(TOK_AND)  \
	GEN_TOKEN(TOK_NOT)  \
	GEN_TOKEN(TOK_SEMI)  \
	GEN_TOKEN(TOK_INT)  \
	GEN_TOKEN(TOK_BOOL_TYPE)  \
	GEN_TOKEN(TOK_PRINT)  \
	GEN_TOKEN(TOK_IF)  \
	GEN_TOKEN(TOK_ELSE)  \
	GEN_TOKEN(TOK_FOR)  \
	GEN_TOKEN(TOK_WHILE)  \
	GEN_TOKEN(TOK_ADD)  \
	GEN_TOKEN(TOK_SUB)  \
	GEN_TOKEN(TOK_MULT)  \
	GEN_TOKEN(TOK_DIV)  \
	GEN_TOKEN(TOK_POW)  \
	GEN_TOKEN(TOK_DEF)  \
	GEN_TOKEN(TOK_DOUBLE)  \
	GEN_TOKEN(TOK_COMMA)  \
	GEN_TOKEN(TOK_VAR)  \
	GEN_TOKEN(TOK_STRING)  \

#define GEN_TOKEN(STRING) #STRING,


#define MAX(a, b) ((a) > (b) ? (a) : (b))




typedef unsigned char TOKEN; 

typedef union {
	char *name; //If variable (class name, function etc) or string
	long integer;
	double floating_point;
} Token;

typedef struct expr {
	TOKEN TOK_ID;
	Token *token;
	struct expr *next;
} Expr;



typedef struct {
	Expr *head;
	Expr *tail;
} Expr_List;




/* Read strings from file */
typedef struct {
	char *string;
	int capacity;
	int size;
} String_Buffer;







Expr_List* parse(FILE *f);








