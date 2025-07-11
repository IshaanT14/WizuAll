#ifndef EMITTER_H
#define EMITTER_H

#include <stdio.h>

// Structure representing a node in the function argument list
typedef struct ArgumentNode {
    char* value;
    struct ArgumentNode* next;
} ArgumentNode;

// File setup and teardown
void start_code_generation(const char *csv_file);
void finalize_code_output();

// Variable-related code generation
void emit_declaration(const char *var_name, const char *expression);
void emit_assignment(const char *var_name, const char *expression);

// Arithmetic operations
void emit_addition(double operand1, double operand2);
void emit_subtraction(double operand1, double operand2);
void emit_multiplication(double operand1, double operand2);
void emit_division(double operand1, double operand2);
void emit_negation(double operand);
void emit_variable_reference(const char *var_name);

// Control structure generation
void emit_if_block_start(const char *condition_expr);
void emit_if_block_end();
void emit_else_block_start();
void emit_else_block_end();
void emit_while_block_start(const char *condition_var);
void emit_while_block_end();

// Argument list utilities
ArgumentNode* create_argument_list(const char *value, ArgumentNode *next);
ArgumentNode* append_argument(ArgumentNode *list, const char *value);

// Function invocation generation
void emit_function_call(const char *function_name, ArgumentNode *args);
void emit_message_call(const char *function_name, const char *message);

#endif // EMITTER_H
