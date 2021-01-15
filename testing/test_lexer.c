#include "../lexer.h"
#include <stdio.h>
int main(int argc, char *argv[]) {
	if (argc < 2) {
		FILE *fp = fopen("test_lexer_file.txt", "r");

		if (fp != NULL) {
			TokenList *list = tokenize(fp);
			printTokList(list);
		}

		fclose(fp);
		return 0;
	}
	FILE *fp = fopen(argv[1], "r");

	if (fp != NULL) {
		TokenList *list = tokenize(fp);
		printTokList(list);
	}

	fclose(fp);
	return 0;
}