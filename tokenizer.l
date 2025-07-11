%{
#include "grammar.tab.h"
#include <stdlib.h>
#include <string.h>
%}

%%

[ \t\r\n]+                  ; // Ignore whitespace
"//".*                      ; // Ignore single-line comments

"if"                        { return IF; }
"else"                      { return ELSE; }
"while"                     { return WHILE; }

[0-9]+(\.[0-9]+)?            { yylval.number = atof(yytext); return NUMBER; }
\"[^\"]*\"                  { yylval.identifier = strdup(yytext); return STRING; }
[a-zA-Z_][a-zA-Z0-9_]*       { yylval.identifier = strdup(yytext); return ID; }

"="                         { return '='; }
"*"                         { return '*'; }
"/"                         { return '/'; }
"+"                         { return '+'; }
"-"                         { return '-'; }
"<="                        { return LE; }
">="                        { return GE; }
"=="                        { return EQ; }
"!="                        { return NE; }
"<"                         { return '<'; }
">"                         { return '>'; }
"("                         { return '('; }
")"                         { return ')'; }
"{"                         { return '{'; }
"}"                         { return '}'; }
"["                         { return '['; }
"]"                         { return ']'; }
","                         { return ','; }
";"                         { return ';'; }

.                           { fprintf(stderr, "Lexer Error: Unknown character: %s\n", yytext); exit(1); }

%%

int yywrap(void) {
    return 1;
}



