%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_registry.h"
#include "code_emitter.h"

extern int yylex();
extern FILE *yyin;

// Custom error handler
void yyerror(const char *message) {
    fprintf(stderr, "Parsing Error: %s\n", message);
    exit(1);
}
%}

%union {
    double number;
    char* identifier;
    char* expression;
    struct ArgumentNode* arg_list;
}

// Tokens
%token <number> NUMBER
%token <identifier> ID
%token <identifier> STRING
%token IF ELSE WHILE
%token LE GE EQ NE

%type <expression> expression
%type <arg_list> arguments expressions

// Operator precedence
%left '+' '-'
%left '*' '/'
%right UMINUS

%%

// Starting rule
program:
    statement_sequence { finalize_code_output(); }
;

statement_sequence:
      /* Empty sequence */
    | statement_sequence statement
;

statement:
      assignment_statement ';'
    | function_call ';'
    | if_structure
    | while_structure
;

assignment_statement:
    ID '=' expression {
        if (!is_symbol_registered($1)) {
            register_symbol($1);
            emit_declaration($1, $3);
        } else {
            emit_assignment($1, $3);
        }
    }
;

function_call:
      ID '(' arguments ')' { emit_function_call($1, $3); }
    | ID '(' expression ')' { 
          ArgumentNode* temp = create_argument_list($3, NULL);
          emit_function_call($1, temp);
      }
    | ID '(' STRING ')' { emit_message_call($1, $3); }
;

arguments:
      /* Empty */ { $$ = NULL; }
    | expressions { $$ = $1; }
;

expressions:
      expression { $$ = create_argument_list($1, NULL); }
    | expressions ',' expression { $$ = append_argument($1, $3); }
;

if_structure:
    IF '(' expression ')' { emit_if_block_start($3); } '{' statement_sequence '}' optional_else
;

optional_else:
      { emit_if_block_end(); }
    | ELSE { emit_else_block_start(); } '{' statement_sequence '}' { emit_else_block_end(); }
;

while_structure:
    WHILE '(' expression ')' { emit_while_block_start($3); } '{' statement_sequence '}' { emit_while_block_end(); }
;

expression:
      expression '+' expression { asprintf(&$$, "(%s + %s)", $1, $3); }
    | expression '-' expression { asprintf(&$$, "(%s - %s)", $1, $3); }
    | expression '*' expression { asprintf(&$$, "(%s * %s)", $1, $3); }
    | expression '/' expression { asprintf(&$$, "(%s / %s)", $1, $3); }
    | expression '<' expression { asprintf(&$$, "(%s < %s)", $1, $3); }
    | expression '>' expression { asprintf(&$$, "(%s > %s)", $1, $3); }
    | expression LE expression   { asprintf(&$$, "(%s <= %s)", $1, $3); }
    | expression GE expression   { asprintf(&$$, "(%s >= %s)", $1, $3); }
    | expression EQ expression   { asprintf(&$$, "(%s == %s)", $1, $3); }
    | expression NE expression   { asprintf(&$$, "(%s != %s)", $1, $3); }
    | '-' expression %prec UMINUS { asprintf(&$$, "(-%s)", $2); }
    | '(' expression ')'         { $$ = $2; }
    | NUMBER                     { asprintf(&$$, "%f", $1); }
    | ID                         { asprintf(&$$, "%s", $1); }
    | ID '[' expression ']'       { asprintf(&$$, "%s[%s]", $1, $3); }
;

%%

// Main driver
int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <source.wiz> <input.csv>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Cannot open source file");
        return 1;
    }

    start_code_generation(argv[2]);
    yyparse();
    return 0;
}


