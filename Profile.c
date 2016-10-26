#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Profile.h"
#include "Helpers.h"

HashTable* ParseEnvironmentVariables(char *filename)
{
	FILE* profile = fopen(filename, "r");
	if(profile == NULL)
		return NULL;

	HashTable* toReturn = HashTable_Make(1024);
	
	size_t lineBufferSize = 32;
	char *lineBuffer = calloc(sizeof(char), lineBufferSize);
	int counter = 0;
	while(getline(&lineBuffer, &lineBufferSize, profile) != -1)
	{
		trimWhitespace(lineBuffer);
		char *value = lineBuffer;
		char *name = strsep(&value, "=");
		if(value == NULL || strlen(name) == 0 || strlen(value) == 0)
		{
			printf("Error parsing line %d of profile file\n", counter);
		} else {
			HashTable_Set(toReturn, name, value);
		}
		++counter;
	}
	free(lineBuffer);

	fclose(profile);

	return toReturn;
}