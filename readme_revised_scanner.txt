Revised Version of Lex Scanner:

1. Add #include "y.tab.hpp"
2. Add return for every token to yacc
3. Add option flags (DebugToken DebugLine to determine whether to print token and source code(line)
4. Remove symbol table in scanner and move to symboltable.cpp and symboltable.hpp