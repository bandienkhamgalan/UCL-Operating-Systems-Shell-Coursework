#include <stdio.h>
#include <stdlib.h>
#include "Profile.h"
#include "Helpers.h"
#include "Variable.h"

HashTable* ParseEnvironmentVariables(char *filename)
{
	FILE* profile = fopen(filename, "r");
	if(profile == NULL)
		return NULL;

	HashTable* toReturn = HashTable_Make(1024);
	
	size_t lineBufferSize = 32;
	char *lineBuffer = calloc(sizeof(char), lineBufferSize);
	Variable variable;
	int counter = 0;
	while(getline(&lineBuffer, &lineBufferSize, profile) != -1)
	{
		trimWhitespace(lineBuffer);
		char *name;
		char *value;
		if(ParseAssignmentString(lineBuffer, &name, &value))
		{
			HashTable_Set(toReturn, name, value);
		} else {
			printf("Error parsing line %d of profile file", counter);
		}
		++counter;
	}
	free(lineBuffer);

	return toReturn;
}