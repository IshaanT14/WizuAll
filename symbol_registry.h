#ifndef SYMBOL_REGISTRY_H
#define SYMBOL_REGISTRY_H

// Fetch the numeric value associated with a given identifier
double fetch_symbol_value(const char *identifier);

// Update or assign a value to an existing symbol
void update_symbol_value(const char *identifier, double value);

// Insert a new symbol into the registry
void register_symbol(const char *identifier);

// Check whether a symbol is already registered
int is_symbol_registered(const char *identifier);

#endif // SYMBOL_REGISTRY_H
