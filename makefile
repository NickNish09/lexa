program:  lexAnalyser.l syntaxAnalyser.y
	bison -d syntaxAnalyser.y
	flex lexAnalyser.l
	gcc syntaxAnalyser.tab.c lex.yy.c -ll -o program.out