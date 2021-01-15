#include <stdio.h>
#include <stdlib.h>
#include <regex.h>


static int reg_match(const char *reg_expr, char *string) {
	regex_t regex;
	regmatch_t match[1];
	regcomp(&regex, reg_expr, REG_EXTENDED);
	int check_match = regexec(&regex, string, 1, match, 0);
	printf("End match: %d\n", match[0].rm_eo);
	regfree(&regex);
	return match[0].rm_eo;
}
int main() {
	char *string = "i=12423;";
	//int ret = regcomp(&regex, "^[a-z]+$",REG_EXTENDED);
	//ret = regexec(&regex, string,0, NULL, 0);
	//printf("%d", ret);
	const char *reg_expr = "^([a-zA-Z][a-zA-Z0-9]*)";
	int ret = reg_match(reg_expr, string);
	if (ret)
		printf("match!\n");
	else
		printf("NO MATCH!\n");


}