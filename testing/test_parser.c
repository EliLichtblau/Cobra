#include "../lexer.h"
#include "../parser.h"
#include <stdio.h>
/*
int main(int argc, char *argv[]) {
	Expression *expr;
	if (argc < 2) {
		return 0;
	}
	FILE *fp = fopen(argv[1], "r");

	if (fp != NULL) {
		TokenList *list;
		ParseList *parseList;
		Expression *expr;

		list = tokenize(fp);
		//printTokList(list);
		
		


		expr = parse(list);
		printExpression(expr);
		printf("\n");
	}

	fclose(fp);
	return 0;
}*/


int main(int argc, char *argv[]) {

	FILE *fp = fopen("test_lexer_file.txt", "r");

	if (fp != NULL) {
		TokenList *list = tokenize(fp);
		printTokList(list);
		Expression *expr = parse_expr(list);
		printExpression(expr);
		printf("\n");
	}

	fclose(fp);
	return 0;
}