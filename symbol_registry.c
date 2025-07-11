#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_registry.h"

#define MAX_REGISTRY_SIZE 1024

typedef struct {
    char *identifier;
    double value;
} RegistryEntry;

static RegistryEntry registry[MAX_REGISTRY_SIZE];
static int registry_count = 0;

// Add a new symbol if it doesn't already exist
void register_symbol(const char *identifier) {
    for (int i = 0; i < registry_count; ++i) {
        if (strcmp(registry[i].identifier, identifier) == 0) return;
    }
    registry[registry_count].identifier = strdup(identifier);
    registry[registry_count].value = 0.0;
    registry_count++;
}

// Retrieve the value of a symbol, or return 0.0 if not found
double fetch_symbol_value(const char *identifier) {
    for (int i = 0; i < registry_count; ++i) {
        if (strcmp(registry[i].identifier, identifier) == 0)
            return registry[i].value;
    }
    return 0.0;
}

// Assign a value to an existing symbol, or insert if absent
void update_symbol_value(const char *identifier, double value) {
    for (int i = 0; i < registry_count; ++i) {
        if (strcmp(registry[i].identifier, identifier) == 0) {
            registry[i].value = value;
            return;
        }
    }
    register_symbol(identifier);
    registry[registry_count - 1].value = value;
}

// Check if a symbol is already registered
int is_symbol_registered(const char *identifier) {
    for (int i = 0; i < registry_count; ++i) {
        if (strcmp(registry[i].identifier, identifier) == 0)
            return 1;
    }
    return 0;
}
