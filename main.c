#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "HashTable.h"
#include "Profile.h"

int main(int argc, char *argv[])
{
	HashTable* table = ParseEnvironmentVariables("profile");
	printf("HOME:\t%s\n", HashTable_Get(table, "HOME"));
	printf("PATH:\t%s\n", HashTable_Get(table, "PATH"));
	char *cwd = getcwd(NULL, 0);
	printf("%s\n", cwd);
	free(cwd);
	return 0;
}