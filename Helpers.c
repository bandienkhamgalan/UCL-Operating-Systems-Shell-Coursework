#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include "Helpers.h"

size_t trimWhitespace(char *toTrim)
{
	assert(toTrim != NULL);
	size_t length = strlen(toTrim);

	if(length == 0)
		return 0;

	// find index of first non-whitespace character
	size_t firstNonSpaceChar = 0;
	while(isspace(toTrim[firstNonSpaceChar]) && firstNonSpaceChar < length)
		++firstNonSpaceChar;

	if(firstNonSpaceChar == length) {
		// string is 100% whitespace
		toTrim[0] = '\0';
		return 0;
	}

	// find index of last non-whitespace character
	size_t lastNonSpaceChar = length - 1;
	while(isspace(toTrim[lastNonSpaceChar]) && lastNonSpaceChar > firstNonSpaceChar)
		--lastNonSpaceChar;

	size_t index = 0;
	length = lastNonSpaceChar - firstNonSpaceChar + 1;
	while(firstNonSpaceChar <= lastNonSpaceChar)
	{
		toTrim[index] = toTrim[firstNonSpaceChar];
		++firstNonSpaceChar;
		++index;
	}
	toTrim[index] = '\0';
	return length;
}

bool containsSpaces(char *toCheck)
{
	assert(toCheck != NULL);
	size_t length = strlen(toCheck);
	for(int index = 0 ; index < length ; ++index)
		if(isspace(toCheck[index]))
			return true;
	return false;
}

bool parseAssignmentString(char *toParse, char **name, char **value)
{
	assert(toParse != NULL && name != NULL && value != NULL);
	char* parsedValue = toParse;
	char* parsedName = strsep(&parsedValue, "=");
	if(parsedValue == NULL || parsedName == NULL)
		return false;
	if(strlen(parsedName) == 0 || strlen(parsedValue) == 0 || containsSpaces(parsedName) || containsSpaces(parsedValue))
		return false;
	*value = parsedValue;
	*name = parsedName;
	return true;
}