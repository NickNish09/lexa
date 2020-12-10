program:  lexAnalyser.l syntaxAnalyser.y
	bison -d --verbose syntaxAnalyser.y
	flex lexAnalyser.l
	gcc -Wall syntaxAnalyser.tab.c lex.yy.c -ll -o program.out