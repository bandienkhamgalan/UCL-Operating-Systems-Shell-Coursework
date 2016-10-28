#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include "HashTable.h"
#include "Helpers.h"
#include "Shell.h"

int main(int argc, char *argv[])
{
	Shell* shell = Shell_Make();
	if(Shell_LoadProfile(shell, "profile") == -1)
	{
		printf("Could not load profile file\n");
	}
	
	int currentError;
	char* home = HashTable_Get(shell->variables, "HOME");
	if(home != NULL)
	{
		if((currentError = Shell_ChangeWorkingDirectory(shell, home)) != 0)
			printf("Could not change working directory to $HOME: %s\n", strerror(currentError));
	}
	Shell_UpdateCurrentWorkingDirectory(shell);
	
	size_t lineBufferSize = 64;
	char *lineBuffer = calloc(sizeof(char), lineBufferSize);
	
	Shell_PromptUser(shell);
	while(getline(&lineBuffer, &lineBufferSize, stdin) != -1)
	{
		trimWhitespace(lineBuffer);
		if(strcmp(lineBuffer, "exit") == 0)
			break;

		char *name = NULL;
		char *value = NULL;
		if(parseAssignmentString(lineBuffer, &name, &value))
		{
			printf("Updating %s variable to %s\n", name, value);
			Shell_UpdateVariable(shell, name, value);
		}
		else
		{
			char** elements = splitBySpace(lineBuffer);
			size_t numElements = strarraylen(elements);
			if(numElements >= 1)
			{
				char* command = elements[0];
				if(strcmp(command, "cd") == 0)
				{
					if(numElements == 1)
					{
						char* home = HashTable_Get(shell->variables, "HOME");
						if(home != NULL)
						{
							if((currentError = Shell_ChangeWorkingDirectory(shell, home)) != 0)
								printf("Could not change working directory: %s\n", strerror(currentError));
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
				else
				{
					printf("Executing command %s...\n", command);
				}
			}
			//printf("you entered %zu elements\n", numElements);
			free(elements);
		}

		Shell_PromptUser(shell);
	}
	free(lineBuffer);

	Shell_Free(shell);

	return 0;
}