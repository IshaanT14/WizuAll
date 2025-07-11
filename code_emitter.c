#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code_emitter.h"
#include "symbol_registry.h"

static FILE *output_file = NULL;

// Begin code generation and embed dataset
void start_code_generation(const char *csv_file) {
    output_file = fopen("out.c", "w");
    fprintf(output_file, "#include <stdio.h>\n#include <math.h>\n\n");

    FILE *data_input = fopen(csv_file, "r");
    if (data_input) {
        fprintf(output_file, "double data[] = {");
        double temp;
        int first_value = 1;
        while (fscanf(data_input, "%lf,", &temp) == 1) {
            if (!first_value) fprintf(output_file, ", ");
            fprintf(output_file, "%f", temp);
            first_value = 0;
        }
        fprintf(output_file, "};\n\n");
        fclose(data_input);
    } else {
        fprintf(stderr, "Warning: Failed to load dataset %s\n", csv_file);
    }

    fprintf(output_file, "int data_size = sizeof(data) / sizeof(data[0]);\n\n");
    fprintf(output_file, "int main() {\n");
}

// Finalize the generated code
void finalize_code_output() {
    fprintf(output_file, "    return 0;\n}\n");
    fclose(output_file);
}

// Emit code for declaring a new variable
void emit_declaration(const char *var_name, const char *expression) {
    if (strcmp(var_name, "i") == 0) {
        fprintf(output_file, "    int %s = %s;\n", var_name, expression);
    } else {
        fprintf(output_file, "    double %s = %s;\n", var_name, expression);
    }
}

// Emit code for assigning a value to an existing variable
void emit_assignment(const char *var_name, const char *expression) {
    fprintf(output_file, "    %s = %s;\n", var_name, expression);
}

// No inline expressions generated here
void emit_addition(double operand1, double operand2) {}
void emit_subtraction(double operand1, double operand2) {}
void emit_multiplication(double operand1, double operand2) {}
void emit_division(double operand1, double operand2) {}
void emit_negation(double operand) {}
void emit_variable_reference(const char *var_name) {}

// Create a new argument node
ArgumentNode* create_argument_list(const char* value, ArgumentNode* next) {
    ArgumentNode* new_node = malloc(sizeof(ArgumentNode));
    new_node->value = strdup(value);
    new_node->next = next;
    return new_node;
}

// Append an argument to an existing list
ArgumentNode* append_argument(ArgumentNode* list, const char* value) {
    ArgumentNode* new_arg = create_argument_list(value, NULL);
    if (!list) return new_arg;
    ArgumentNode* cursor = list;
    while (cursor->next) cursor = cursor->next;
    cursor->next = new_arg;
    return list;
}

// Emit a function or print call
void emit_function_call(const char *function_name, ArgumentNode *args) {
    if (strcmp(function_name, "print") == 0) {
        if (args) {
            fprintf(output_file, "    printf(\"%%f\", %s);\n", args->value);
        } else {
            fprintf(output_file, "    printf(\"\\n\");\n");
        }
        return;
    }

    if (strcmp(function_name, "printmsg") == 0) {
        if (args) {
            fprintf(output_file, "    printf(\"%%s\", %s);\n", args->value);
        }
        return;
    }

    fprintf(output_file, "    %s(", function_name);
    for (ArgumentNode *current = args; current; current = current->next) {
        fprintf(output_file, "%s", current->value);
        if (current->next) fprintf(output_file, ", ");
    }
    fprintf(output_file, ");\n");
}

// Begin an if-block
void emit_if_block_start(const char *condition_expr) {
    fprintf(output_file, "    if (%s) {\n", condition_expr);
}

// End an if-block
void emit_if_block_end() {
    fprintf(output_file, "    }\n");
}

// Begin an else-block
void emit_else_block_start() {
    fprintf(output_file, "    } else {\n");
}

// End an else-block
void emit_else_block_end() {
    fprintf(output_file, "    }\n");
}

// Begin a while-loop
void emit_while_block_start(const char *condition_var) {
    fprintf(output_file, "    while (%s) {\n", condition_var);
}

// End a while-loop
void emit_while_block_end() {
    fprintf(output_file, "    }\n");
}

// Emit a message-specific function call
void emit_message_call(const char *function_name, const char *message) {
    if (strcmp(function_name, "printmsg") == 0) {
        fprintf(output_file, "    printf(%s);", message);
    } else {
        fprintf(stderr, "Error: Unknown string operation for %s\n", function_name);
    }
}
