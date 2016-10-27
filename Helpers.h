#ifndef HELPERS_H
#define HELPERS_H

#include <stdbool.h>

size_t trimWhitespace(char *toTrim); // In-place trimming
void getCurrentWorkingDirectory(char** heapString, size_t bufferSize);
bool containsSpaces(char *toCheck);
bool parseAssignmentString(char *toParse, char **name, char **value);

#endif