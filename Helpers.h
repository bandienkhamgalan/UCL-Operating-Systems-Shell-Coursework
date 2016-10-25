#ifndef HELPERS_H
#define HELPERS_H

#include <stdbool.h>

// In-place trimming
size_t trimWhitespace(char *toTrim);
bool ParseAssignmentString(char* toParse, char **name, char **value);

#endif