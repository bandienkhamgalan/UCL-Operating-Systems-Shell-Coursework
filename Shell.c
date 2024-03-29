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

int Shell_ChangeWorkingDirectory(Shell* shell, char *directory);
int Shell_UpdateCurrentWorkingDirectory(Shell* shell);

Shell* InitializeShell()
{
	Shell* shell = Shell_Make();
	if(Shell_LoadProfile(shell, "profile") == -1)
	{	
		printf("Error: Could not load profile file\n");
	}
	
	char* home = Shell_GetVariable(shell, "HOME");
	if(home != NULL)
	{
		int error;
		if((error = Shell_ChangeWorkingDirectory(shell, home)) != 0)
			printf("Error: Could not change working directory to $HOME: %s\n", strerror(error));
		free(home);
	}
	else
	{
		printf("Error: HOME was not set, please set manually...\n");
	}
	Shell_UpdateCurrentWorkingDirectory(shell);

	char* paths = Shell_GetVariable(shell, "PATH");
	if(paths != NULL)
	{
		Shell_LoadSearchPathsFromString(shell, paths);
		free(paths);
	}
	else
	{
		printf("Error: PATH was not set, please set manually...\n");
	}

	return shell;
}

void Shell_LoadSearchPathsFromString(Shell* shell, char* paths)
{
	assert(shell != NULL && paths != NULL);
	for(size_t index = 1; index < shell->searchPathsCapacity ; ++index)
	{
		free(shell->searchPaths[index]);
		shell->searchPaths[index] = NULL;
	}
	shell->searchPathMaxLength = 0;

	size_t count = 1;
	char* delimiters = ":";
	char* path = strtok(paths, delimiters);
	struct stat statbuf;
	while(path != NULL)
	{
		if(stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
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
		char** elements = splitBySpace(lineBuffer);
		size_t numElements = strarraylen(elements);
		char *name = NULL;
		char *value = NULL;
		if(numElements == 1 && parseAssignmentString(lineBuffer, &name, &value))
		{	
			++variableCounter;
			Shell_SetVariable(shell, name, value);
		}
		else
			printf("Error parsing line %d of file %s\n", lineCounter, filename);

		free(elements);		
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
	new->searchPathMaxLength = 0;
	new->searchPaths = calloc(sizeof(char*), new->searchPathsCapacity);  
	assert(new->searchPaths != NULL);
	for(size_t index = 0; index < new->searchPathsCapacity ; ++index)
		new->searchPaths[index] = NULL;
	new->searchPaths[0] = malloc(sizeof(char) * 2);
	strcpy(new->searchPaths[0], ".");
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
	if(strcmp(name, "PATH") == 0)
	{
		char* path = HashTable_Get(shell->variables, "PATH");
		Shell_LoadSearchPathsFromString(shell, path);
		free(path);
	}
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

	int currentError;
	size_t lineBufferSize = 64;
	char *lineBuffer = calloc(sizeof(char), lineBufferSize);

	printf("%s > ", shell->workingDirectory);
	while(getline(&lineBuffer, &lineBufferSize, stdin) != -1)
	{
		trimWhitespace(lineBuffer);
		if(strcmp(lineBuffer, "exit") == 0)
			break;

		char** elements = splitBySpace(lineBuffer);
		size_t numElements = strarraylen(elements);
		if(numElements >= 1)
		{
			char* command = elements[0];

			char *name = NULL;
			char *value = NULL;
			if(numElements == 1 && parseAssignmentString(lineBuffer, &name, &value))
			{
				Shell_SetVariable(shell, name, value);			
			}
			else if(strcmp(command, "cd") == 0)
			{
				if(numElements == 1)
				{
					char* home = Shell_GetVariable(shell, "HOME");
					if(home != NULL)
					{
						if((currentError = Shell_ChangeWorkingDirectory(shell, home)) != 0)
							printf("Could not change working directory: %s\n", strerror(currentError));
						free(home);
					}
					else
					{
						printf("cd: must provide argument or set $HOME\n");
					}
				}
				else if(numElements == 2)
				{
					char* directory = elements[1];
					if((currentError = Shell_ChangeWorkingDirectory(shell, directory)) != 0)
						printf("Could not change working directory: %s\n", strerror(currentError));
				}
				else
				{
					printf("cd: invalid number of arguments\n");
				}
			}
			/*else if(strcmp(command, "splitBySpace") == 0)
			{
				printf("%zu components\n", numElements);
				for(size_t index = 0 ; index < numElements ; ++index)
					printf("%zu\t%s\t(%zu characters)\n", index + 1, elements[index], strlen(elements[index]));
			}
			else if(strcmp(command, "printPath") == 0)
			{
				size_t searchPathsCount = strarraylen(shell->searchPaths);
				printf("%zu directories\n", searchPathsCount);
				for(size_t index = 0 ; index < searchPathsCount ; ++index)
					printf("%zu\t%s\t(%zu characters)\n", index + 1, shell->searchPaths[index], strlen(shell->searchPaths[index]));
			}*/
			else
			{
				Shell_RunCommand(shell, command, elements);
			}
		}
		else
		{
			printf("You entered an invalid string...\n");
		}

		free(elements);

		printf("%s > ", shell->workingDirectory);
	}
	free(lineBuffer);
}

void Shell_RunCommand(Shell* shell, char* command, char** arguments)
{
	assert(shell != NULL && shell->searchPaths != NULL && command != NULL && arguments != NULL);
	size_t searchPathsCount = strarraylen(shell->searchPaths);
	size_t bufferCapacity = shell->searchPathMaxLength + 1 + strlen(command) + 1;
	char* buffer = calloc(sizeof(char), bufferCapacity);
	struct stat statbuf;
	bool found = false;
	for(size_t index = 0 ; index < searchPathsCount ; ++index)
	{
		char* searchPath = shell->searchPaths[index];
		strcpy(buffer, searchPath);
		strcat(buffer, "/");
		strcat(buffer, command);
		if(stat(buffer, &statbuf) == 0 && S_ISREG(statbuf.st_mode))
		{
        	pid_t pid = fork();

        	if(pid == -1)
        	{
        		int errsv = errno;
        		printf("Error: failed to execute %s: %s...\n", buffer, strerror(errsv));
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
        		found = true;
        		break;
        	}
		}
	}
	if(!found)
	{
		printf("%s: command not found\n", command);
	}

	free(buffer);
}