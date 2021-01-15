#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include <stdlib.h>





Value* eval_expression(Expression *expression) {
	Value *evaluation;
	if (expression == NULL)
		return NULL;

	evaluation = malloc(sizeof(Value));
	ExpressionType type = expression->type;
	if (type == EXPR_INT) {
		evaluation->integer = expression->value;
	} else {
		Value *tmp1 = eval_expression(expression->expr1);
		Value *tmp2 = eval_expression(expression->expr2);

		//DO SOME TYPE CHECK FUN
		if (expression->type == EXPR_ADD)
			evaluation->integer = tmp1->integer + tmp2->integer;
		else
			evaluation->integer = tmp1->integer * tmp2->integer;
	}

	return evaluation;

}



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

	expr = parse(list);
	printExpression(expr);
	printf("\n");
	Value *val = eval_expression(expr);


	printf("\n");
	printf("%ld\n", val->integer);
	return 0;
}
