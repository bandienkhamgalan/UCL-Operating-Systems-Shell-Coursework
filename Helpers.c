#define _GNU_SOURCE
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
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

	// remove leading whitespace by shifting characters in string so first non-white space character is in index 0
	size_t index = 0;
	length = lastNonSpaceChar - firstNonSpaceChar + 1;
	while(firstNonSpaceChar <= lastNonSpaceChar)
	{
		toTrim[index] = toTrim[firstNonSpaceChar];
		++firstNonSpaceChar;
		++index;
	}
	// truncate string with NULL termination character to remove trailing whitespace
	toTrim[index] = '\0';

	return length;
}

bool parseAssignmentString(char *toParse, char **name, char **value)
{
	assert(toParse != NULL && name != NULL && value != NULL);
	char* parsedValue = toParse;
	char* parsedName = strsep(&parsedValue, "=");
	if(parsedValue == NULL || parsedName == NULL)
		return false;
	if(strlen(parsedName) == 0 || strlen(parsedValue) == 0)
		return false;
	*value = parsedValue;
	*name = parsedName;
	return true;
}

size_t strarraylen(char** toCount)
{
	assert(toCount != NULL);
	size_t count = 0;
	while(*toCount++)
		++count;
	return count;
}

char** splitBySpace(char *toParse)
{
	assert(toParse != NULL);
	size_t length = strlen(toParse);

	size_t capacity = 2;
	char** strings = calloc(sizeof(char*), capacity);
	assert(strings != NULL);
	size_t stringsIndex = 0;

	// Find start indices of tokens
	size_t index = 0;
	while(index < length)
	{
		// Seek to first non-space character
		while(isspace(toParse[index]) && index < length)
			++index;

		size_t startIndex = index;
		bool escaped = false;
		bool quoted = false;
		bool success = false;
		for( ; index < length ; ++index)
		{
			char current = toParse[index];
			if(current == '\\')
			{
				if(escaped)
					escaped = false;
				else
				{
					if(index + 1 < length && (toParse[index + 1] == '\\' || toParse[index + 1] == ' ' || toParse[index + 1] == '\"'))
						escaped = true;
					else
						break;
				}
			}
			else if(current == '\"')
			{
				if(escaped)
					escaped = false;
				else
					quoted = !quoted;
			}
			else if(isspace(current))
			{
				if(escaped)
				{
					if(!quoted && current == ' ')
						escaped = false;
					else
						break;
				}
				else
				{
					if(quoted)
						continue;
					else
					{
						success = true;
						break;
					}
				}
			}
		}

		if(startIndex == index)
			break;

		if((success || index == length) && !quoted)
		{
			toParse[index] = '\0';
			if(stringsIndex + 1 >= capacity)
			{
				strings = realloc(strings, sizeof(char*) * capacity * 2);
				assert(strings != NULL);
				capacity *= 2;
			}
			strings[stringsIndex] = toParse + startIndex;
			++stringsIndex;
		}
		else
		{
			strings[0] = NULL;
			return strings;
		}

		++index;
	}

	// Copy tokens to output array, while removing escape characters
	for(index = 0 ; index < stringsIndex ; ++index)
	{
		char* string = strings[index];
		size_t length = strlen(string);
		size_t realIndex = 0;
		size_t stringIndex = 0;
		bool escaped = false;
		for( ; stringIndex < length ; ++stringIndex)
		{
			if(string[stringIndex] == '\\' && !escaped)
				escaped = true;
			else if(string[stringIndex] == '\"' && !escaped)
			{
				// do not copy quotation marks
			}
			else
			{
				string[realIndex] = string[stringIndex];
				++realIndex;
				escaped = false;
			}
		}
		string[realIndex] = '\0';
	}


	for(size_t i = stringsIndex ; i < capacity ; ++i)
		strings[i] = NULL;

	return strings;
}