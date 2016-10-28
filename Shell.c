#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "Shell.h"
#include "HashTable.h"
#include "Helpers.h"

void Shell_LoadSearchPathsFromString(Shell* shell, char* paths)
{
	assert(shell != NULL && paths != NULL);
	for(size_t index = 0; index < shell->searchPathsCapacity ; ++index)
	{
		free(shell->searchPaths[index]);
		shell->searchPaths[index] = NULL;
	}
	shell->searchPathMaxLength = 0;

	size_t count = 0;
	char* delimiters = ":";
	char* path = strtok(paths, delimiters);
	struct stat statbuf;
	while(path != NULL)
	{
		if(stat(path, &statbuf) == -1)
		{
        	int errsv = errno;
        	printf("Error adding %s to search path: %s\n", path, strerror(errsv));
		}
		else if(!S_ISDIR(statbuf.st_mode))
			printf("Error adding %s to search path: not a directory\n", path);
		else
		{
			if(count + 1 >= shell->searchPathsCapacity)
			{
				size_t newCapacity = shell->searchPathsCapacity * 2;
				shell->searchPaths = realloc(shell->searchPaths, sizeof(char*) * newCapacity);
				assert(shell->searchPaths != NULL);
				for(size_t index = shell->searchPathsCapacity; index < newCapacity ; ++index)
					shell->searchPaths[index] = NULL;
				shell->searchPathsCapacity = newCapacity;
			}
			shell->searchPaths[count] = strdup(path);
			size_t length = strlen(path);
			if(length > shell->searchPathMaxLength)
				shell->searchPathMaxLength = length;
		}		

		path = strtok(NULL, delimiters);
		++count;
	}
}

int Shell_LoadProfile(Shell* shell, char *filename)
{
	assert(shell != NULL && filename != NULL);

	FILE* profile = fopen(filename, "r");
	if(profile == NULL)
		return -1;
	
	size_t lineBufferSize = 32;
	char *lineBuffer = calloc(sizeof(char), lineBufferSize);
	assert(lineBuffer != NULL);
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
			Shell_SetVariable(shell, name, value);
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
	assert(new->workingDirectory != NULL);
	new->variables = HashTable_Make(2048);
	new->searchPathsCapacity = 8;
	new->searchPaths = calloc(sizeof(char*), new->searchPathsCapacity);
	new->searchPathMaxLength = 0;
	assert(new->searchPaths != NULL);
	for(size_t index = 0; index < new->searchPathsCapacity ; ++index)
		new->searchPaths[index] = NULL;
	return new;
}

void Shell_Free(Shell* toFree)
{
	assert(toFree != NULL);
	free(toFree->workingDirectory);
	for(size_t index = 0; index < toFree->searchPathsCapacity ; ++index)
		free(toFree->searchPaths[index]);
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

void Shell_SetVariable(Shell* shell, char* name, char* value)
{
	assert(shell != NULL && name != NULL && value != NULL);
	HashTable_Set(shell->variables, name, value);
}

char* Shell_GetVariable(Shell* shell, char* name)
{
	assert(shell != NULL && name != NULL);
	return HashTable_Get(shell->variables, name);
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

void Shell_RunCommand(Shell* shell, char* command, char** arguments)
{
	assert(shell != NULL && shell->searchPaths != NULL && command != NULL && arguments != NULL);
	size_t searchPathsCount = strarraylen(shell->searchPaths);
	size_t bufferCapacity = shell->searchPathMaxLength + 1 + strlen(command) + 1;
	char* buffer = calloc(sizeof(char), bufferCapacity);
	struct stat statbuf;
	for(size_t index = 0 ; index < searchPathsCount ; ++index)
	{
		char* searchPath = shell->searchPaths[index];
		strcpy(buffer, searchPath);
		strcat(buffer, "/");
		strcat(buffer, command);
		printf("Checking %s...\n", buffer);
		if(stat(buffer, &statbuf) == 0 && S_ISREG(statbuf.st_mode))
		{
			printf("Executing %s\n", buffer);
        	pid_t pid = fork();

        	if(pid == -1)
        	{
        		int errsv = errno;
        		printf("failed to execute execute %s: %s...\n", buffer, strerror(errsv));
        	} 
        	else if(pid == 0)
        	{
        		// in child
        		execv(buffer, arguments);
        		exit(EXIT_FAILURE);
        	}
        	else
        	{
        		int status;
        		waitpid(pid, &status, 0);
        		//printf("Process %d finished executing with status %d\n", pid, status);
        	}
		}
	}
	free(buffer);
}