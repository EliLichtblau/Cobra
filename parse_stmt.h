#include "lexer.h"
#include "parser.h"
#ifndef PARSER_STMT_H
#define PARSER_STMT_H

#define INT_TO_STMT_NAME \
	GEN_TOKEN(NOOP)  \
	GEN_TOKEN(SEQ)  \
	GEN_TOKEN(ASSIGN)  \
	GEN_TOKEN(IF)  \
	GEN_TOKEN(FOR)  \
	GEN_TOKEN(WHILE)  \
	GEN_TOKEN(PRINT)  \


#define GEN_EXPR(STRING) #STRING,


typedef enum {
	NOOP,
	SEQ,
	ASSIGN,
	IF,
	FOR,
	WHILE,
	PRINT,
} StatementType;




typedef struct stmt {
	StatementType type;
	union {

		//seq case
		struct {
			struct stmt *stmt1;
			struct stmt *stmt2;
		};

		//Assign case
		struct {
			char *name;
			Expression *exprAssign;
		};

		//if case
		struct {
			Expression *exprIf;
			struct stmt *stmt1If;
			struct stmt *stmt2If;
		};

		//for case

		//while case

		//print case
		struct {
			Expression *exprPrint;
		};



	};


} Statement;





Statement* parse_statment(TokenList *tokenlist);
Statement* parse_options(TokenList *tokenlist);
Statement* parse_assign(TokenList *tokenlist);
Statement* parse_print(TokenList *tokenlist);
Statement* parse_if(TokenList *tokenlist);
Statement* parse_else(TokenList *tokenlist);
Statement* parse_for(TokenList *tokenlist);
Statement* parse_while(TokenList *tokenlist);







#endif
