#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include "Shell.h"
#include "HashTable.h"
#include "Helpers.h"

void Shell_LoadSearchPathsFromString(Shell* shell, char* paths)
{
	
}

int Shell_LoadProfile(Shell* shell, char *filename)
{
	assert(shell != NULL && filename != NULL);

	FILE* profile = fopen(filename, "r");
	if(profile == NULL)
		return -1;
	
	size_t lineBufferSize = 32;
	char *lineBuffer = calloc(sizeof(char), lineBufferSize);
	int lineCounter = 0;
	int variableCounter = 0;
	while(getline(&lineBuffer, &lineBufferSize, profile) != -1)
	{
		trimWhitespace(lineBuffer);
		char *name = NULL;
		char *value = NULL;
		if(!parseAssignmentString(lineBuffer, &name, &value))
		{
			printf("Error parsing line %d of file %s\n", lineCounter, filename);
		} else {
			++variableCounter;
			Shell_UpdateVariable(shell, name, value);
		}
		++lineCounter;
	}
	free(lineBuffer);
	fclose(profile);

	return variableCounter;
}

Shell* Shell_Make()
{
	Shell* new = malloc(sizeof(Shell));
	assert(new != NULL);
	new->workingDirectoryBufferSize = 64; 
	new->workingDirectory = calloc(sizeof(char), new->workingDirectoryBufferSize);
	new->variables = HashTable_Make(2048);
	new->searchPaths = NULL;
	new->searchPathsCount = 0;
	return new;
}

void Shell_Free(Shell* toFree)
{
	assert(toFree != NULL);
	free(toFree->workingDirectory);
	if(toFree->searchPaths != NULL)
		free(toFree->searchPaths);
	HashTable_Free(toFree->variables);
	free(toFree);
}

int Shell_ChangeWorkingDirectory(Shell* shell, char *directory)
{
	assert(shell != NULL && directory != NULL);

	if(chdir(directory) != 0)
	{
		return errno;
	}

	return Shell_UpdateCurrentWorkingDirectory(shell);
}

void Shell_UpdateVariable(Shell* shell, char* name, char* value)
{
	assert(shell != NULL && name != NULL && value != NULL);
	HashTable_Set(shell->variables, name, value);
}

int Shell_UpdateCurrentWorkingDirectory(Shell* shell)
{
	assert(shell != NULL && shell->workingDirectory != NULL);
	if(getcwd(shell->workingDirectory, shell->workingDirectoryBufferSize) == NULL)
	{
		int errsv = errno;
		if(errsv == ERANGE)
		{
			shell->workingDirectoryBufferSize *= 2; 
			shell->workingDirectory = realloc(shell->workingDirectory, sizeof(char) * shell->workingDirectoryBufferSize);
			assert(shell->workingDirectory != NULL);
			if(getcwd(shell->workingDirectory, shell->workingDirectoryBufferSize) == NULL)
			{
				return errno;
			}
			else
			{
				return 0;
			}
		}
		return errno;
	}
	else
	{
		return 0;
	}
}

void Shell_PromptUser(Shell *shell)
{
	assert(shell != NULL && shell->workingDirectory != NULL);
	printf("%s > ", shell->workingDirectory);
}