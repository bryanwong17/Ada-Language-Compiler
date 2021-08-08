%{

using namespace std;

#include "y.tab.hpp"

#define MAX_LINE_LENG 256
#define LIST strcat(buf, yytext)
#define token(t) { LIST; if (DebugToken) printf("<%s>\n", #t); }
#define tokenChar(t) { LIST; if (DebugToken) printf("<'%c'>\n", t); }
#define tokenInteger(t,i) { LIST; if (DebugToken) printf("<%s: %d>\n", #t, i); }
#define tokenString(t,s) { LIST; if (DebugToken) printf("<%s: %s>\n", #t, s); }

int line = 1;
int DebugToken = 0;
int DebugLine = 1;
char buf[MAX_LINE_LENG];

%}
DELIMITER               ([,:.;()[\]])
ARITHMETIC              ([-+*/])
REMAINDER               (%)
RELATIONAL              ([<>=])
ID 					          	([A-Za-z][0-9A-Za-z]*)
DIGIT                   ([0-9]+)
INTEGER                 ({DIGIT})
REAL                    ([-+]?{DIGIT}\.{DIGIT}?([Ee](-+)?{DIGIT})?)
BOOLEAN                 ("true"|"false")
LOGICAL                 ("and"|"or"|"not")
STRING                  (\"(\"\"|[^"\n])*\")
SINGLE_COMMENT          (\-\-[^\n]*)
BEGIN_COMMENT           (\{)
END_COMMENT             ([^\n]*\})

%x COMMENT

%%

{DELIMITER} |
{ARITHMETIC}  |
{REMAINDER}  |
{RELATIONAL}            { tokenChar(yytext[0]); return yytext[0]; }

{BOOLEAN}				        {
                          tokenString(BOOLEAN, yytext);
                          if (string(yytext) == "true") yylval.b_val = true;
                          if (string(yytext) == "false") yylval.b_val = false;
                          return BOOLConst;
                        }

{LOGICAL}			        	{
                          if (string(yytext) == "and")
                          {
                            token('AND');
                            return AND;
                          };
                          if (string(yytext) == "not") 
                          {
                            token('NOT');
                            return NOT;
                          }
                          if (string(yytext) == "or")
                          {
                            token('OR');
                            return OR;
                          }
                        }

"<="                    { token('<='); return LE; } 
"=>"                    { token('=>'); return GE; }
">="                    { token('>='); return GE; }
"/="                    { token('/='); return NE; }
":="                    { token(':='); return ASSIGN; }
".."                    { token('..'); return RANGE; }
"integer"               { token(INTEGER); return INTEGER; }
"string"                { token(STRING); return STRING; }
"boolean"               { token(BOOLEAN); return BOOLEAN; }
"float"                 { token(FLOAT); return FLOAT; }
"begin"                 { token(BEGIN_TOK); return BEGIN_TOKEN; }
"break"                 { token(BREAK); return BREAK; }
"character"             { token(CHARACTER); return CHARACTER; }
"case"                  { token(CASE); return CASE; }
"continue"              { token(CONTINUE); return CONTINUE; }
"constant"              { token(CONSTANT); return CONSTANT; }
"declare"               { token(DECLARE); return DECLARE; }
"do"                    { token(DO); return DO; }
"else"                  { token(ELSE); return ELSE; }
"end"                   { token(END); return END; }
"exit"                  { token(EXIT); return EXIT; }
"for"                   { token(FOR); return FOR; }
"if"                    { token(IF); return IF; }
"in"                    { token(IN); return IN; }
"loop"                  { token(LOOP); return LOOP; }
"print"                 { token(PRINT); return PRINT; }
"println"               { token(PRINTLN); return PRINTLN; }
"procedure"             { token(PROCEDURE); return PROCEDURE; }
"program"               { token(PROGRAM); return PROGRAM; }
"read"                  { token(READ); return READ; }
"return"                { token(RETURN); return RETURN; }
"then"                  { token(THEN); return THEN; }
"while"                 { token(WHILE); return WHILE; }


{INTEGER}               {
                          tokenInteger(INTEGER, atoi(yytext));
                          yylval.i_val = atoi(yytext);
                          return INTConst;
                        }

{REAL}                  {
                          tokenString(REAL, yytext);
                          yylval.d_val = atof(yytext);
                          return FLOATConst;
                        }

{ID}         		        {
                          tokenString(ID, yytext);
                          yylval.s_val = new string(yytext);
                          return ID;
                        }

{STRING}                {
                          char s[MAX_LINE_LENG] = {0};
                          int idx = 0;
                          for (int i = 1; i < yyleng - 1; ++i) {
                            if (yytext[i] == '"') ++i;
                            s[idx++] = yytext[i];
                          }
                          tokenString(STRING, s);
                          yylval.s_val = new string(s);
                          return STRConst;
                        }

  /* comment */
{SINGLE_COMMENT}        { LIST; }

{BEGIN_COMMENT}         {
                          LIST;
                          BEGIN COMMENT;
                        }

<COMMENT>[^\n]          {
                          LIST;
                        }

<COMMENT>\n             {
                          LIST;
                          if (DebugLine) printf("%d: %s", line, buf);
                          line++;
                          buf[0] = '\0';
                        }

<COMMENT>{END_COMMENT}  {
                          LIST;
                          BEGIN 0;
                        }

\n					           	{
                          LIST;
                          if (DebugLine) printf("%d: %s", line, buf);
                          line++;
                          buf[0] = '\0';
                        }

[ \t]*		              { LIST; }

<<EOF>>			        		{ LIST;
						              if (DebugLine) printf("EOF line: %d %s\n", line, buf);
                          line++;
                          buf[0] = '\0';
						              return 0;
						            }

.                       {
                          LIST;
                          if (DebugLine) printf("%d:%s\n", line, buf);
                          printf("bad character:'%s'\n",yytext);
                          exit(-1);
                        }

%%