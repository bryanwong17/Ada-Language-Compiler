## Build Project2 by MakeFile

1. Run `Makefile` -> make
2. If make is not successful -> make clean, then make again
3. If for example want to test HelloWorld.ada -> ./parser < HelloWorld.ada

## Or, compile manually

1. lex -o lex.yy.cpp ada_scanner.l
2. yacc -d -v ada_parser.y -o y.tab.cpp
3. g++ y.tab.cpp symboltable.cpp -o parser -ll -std=c++11 -Wno-deprecated-register
4. If for example want to test HelloWorld.ada -> ./parser < HelloWorld.ada