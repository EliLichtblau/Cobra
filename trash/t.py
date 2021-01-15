s = """#define TOK_LPAREN 0
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
#define TOK_INT_TYPE 15
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
#define TOK_POW 26"""


with open("tmp.txt", "w") as f:
	for word in s.split():
		if (word[0]=='T'):
			f.write("tok({0})  \\\n".format(word))