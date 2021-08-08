ifeq ($(shell uname -s), Darwin)
	javaa := ./javaa_mac
else
	javaa := ./javaa
endif

all: compiler

compiler: lex.yy.cpp y.tab.cpp symboltable.cpp symboltable.hpp
	g++ y.tab.cpp symboltable.cpp -o compiler -ll -ly -std=c++11 -Wno-deprecated-register

lex.yy.cpp: ada_scanner.l
	lex -o lex.yy.cpp ada_scanner.l

y.tab.cpp: ada_parser.y
	yacc -d ada_parser.y -o y.tab.cpp

clean:
	rm compiler lex.yy.cpp y.tab.*

run: compiler
	./compiler $(file).ada
	$(javaa) $(file).jasm
	java $(file)