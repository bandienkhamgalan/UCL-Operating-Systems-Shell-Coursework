#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include "HashTable.h"
#include "Helpers.h"
#include "Profile.h"
#include "Array.h"

typedef struct
{
	char* workingDirectory;
	HashTable* environmentVariables;
	Array* searchPaths;
} Shell;

int main(int argc, char *argv[])
{
	Shell shell;
	assert((shell.environmentVariables = ParseEnvironmentVariables("profile")) != NULL);
	
	//char* path = HashTable_Get(shell.environmentVariables, "PATH");
	char* home = HashTable_Get(shell.environmentVariables, "HOME");
	if(home == NULL)
	{
		char *cwd = getcwd(NULL, 0);
		printf("No HOME set in profile: setting to %s\n", cwd);
		HashTable_Set(shell.environmentVariables, "HOME", cwd);
		free(cwd);
		home = HashTable_Get(shell.environmentVariables, "HOME");
	}
	shell.workingDirectory = home;
	if(chdir(shell.workingDirectory) != 0) {
		int errsv = errno;
		printf("Error occurred changing directory: %d\n", errsv);
	} else {
		size_t lineBufferSize = 64;
		char *lineBuffer = calloc(sizeof(char), lineBufferSize);
		char *cwd = getcwd(NULL, 0);
		printf("%s > ", cwd);
		while(getline(&lineBuffer, &lineBufferSize, stdin) != -1)
		{
			trimWhitespace(lineBuffer);
			printf("you entered %zu characters\n", strlen(lineBuffer));
			printf("%s > ", cwd);
			if(strcmp(lineBuffer, "exit") == 0)
				break;
		}
		free(cwd);
		free(lineBuffer);
	}

	HashTable_Free(shell.environmentVariables);

	return 0;
}