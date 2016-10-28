#ifndef SHELL_H
#define SHELL_H

#include "HashTable.h"

typedef struct
{
	char* workingDirectory;
	int workingDirectoryBufferSize;
	HashTable* variables;
	char** searchPaths;
	size_t searchPathsCapacity;
	size_t searchPathMaxLength;
} Shell;

// Returns number of variables parsed from file, -1 if file could not be read
int Shell_LoadProfile(Shell* shell, char *filename);
void Shell_PromptUser(Shell *shell);
Shell* Shell_Make();
void Shell_SetVariable(Shell* shell, char* name, char* value);
char* Shell_GetVariable(Shell* shell, char* name);
void Shell_Free(Shell* toFree);
// Returns 0 if succeeded, otherwise the errno of chdir() syscall
int Shell_ChangeWorkingDirectory(Shell* shell, char *directory);
// Returns 0 if succeeded, otherwise the errno of getcwd() syscall
int Shell_UpdateCurrentWorkingDirectory(Shell* shell);
void Shell_LoadSearchPathsFromString(Shell* shell, char* paths);
void Shell_RunCommand(Shell* shell, char* command, char** arguments);


#endif