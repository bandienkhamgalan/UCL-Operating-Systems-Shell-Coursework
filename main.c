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
	Shell* shell = InitializeShell();
	Shell_PromptUser(shell);
	Shell_Free(shell);

	return 0;
}