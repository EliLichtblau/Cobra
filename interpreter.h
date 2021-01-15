#include "lexer.h"
#include "parser.h"

#if !defined(INTERPRETER_H)
#define INTERPRETER_H


typedef union {
	long integer;

} Value;


Value* eval_expr(Expression *);

#endif