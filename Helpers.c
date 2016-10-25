#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
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