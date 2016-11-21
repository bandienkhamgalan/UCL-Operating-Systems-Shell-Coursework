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

/** @brief	Initialize a Shell struct with contents of a profile file.  
 *
 * 		  	This function expects a file named "profie" in the current working directory, and will
 *          report an error to stdout if the file does not exist. This function expects assignments
 *			to shell variables PATH and HOME in profile file, and will report an error to stdout
 *			if they are not assigned.     
 *
 *			This function will dynamically allocate a Shell struct. Caller must free the Shell
 *			struct by calling Shell_Free(Shell*). Simply calling free() will cause memory leaks!
 *
 *  @return Pointer to dynamically allocated Shell struct
 */
Shell* InitializeShell();

/** @brief	Initialize a Shell struct with default values. 
 *
 *			This function will dynamically allocate a Shell struct. Caller must free the Shell
 *			struct by calling Shell_Free(Shell*). Simply calling free() will cause memory leaks!
 *
 *  @return pointer to dynamically allocated Shell struct
 */
Shell* Shell_Make();

/** @brief	Free a dynamically allocated Shell struct
 *
 *  @param toFree
 */
void Shell_Free(Shell* toFree);

/** @brief 	Update a Shell struct with variable assignments defined in the specified profile file. 
 *  	   	Use Shell_GetVariable(Shell*, char*) function to retrieve the value of a shell variable. 
 *
 *  @param shell
 *  @param filename Path to profile file
 *  @return Number of variables parsed, -1 if error occurred e.g. if file does not exist 
 */
int Shell_LoadProfile(Shell* shell, char *filename);

/** @brief 	Retrieve the value of a shell variable. 
 *
 *			The function will return a copy of the value, so the string must be freed by the caller. 
 *
 *  @param shell
 *  @param name variable name
 *  @return String value of specified shell variable (must be freed by caller), NULL if not defined
 */
char* Shell_GetVariable(Shell* shell, char* name);

/** @brief 	Sets the value of a shell variable.  
 *			Note that PATH and HOME are special variables. Setting PATH
 *			will update the search paths of the shell, and setting HOME
 *			will update the default working directory of the shell. 
 *
 *  @param shell
 *  @param name
 *	@param value
 */
void Shell_SetVariable(Shell* shell, char* name, char* value);


/** @brief 	Sets the value of a shell variable.  
 *			Note that PATH and HOME are special variables. Setting PATH
 *			will update the search paths of the shell, and setting HOME
 *			will update the default working directory of the shell. 
 *
 *  @param shell
 *  @param name
 *	@param value
 */
void Shell_PromptUser(Shell *shell);

/** @brief	Change the current working directory of a Shell struct
 *
 *  @param shell
 *	@param directory
 *	@return 0 if succeeded, otherwise the errno of chdir() syscall
 */
int Shell_ChangeWorkingDirectory(Shell* shell, char *directory);

/** @brief	Free a dynamically allocated Shell struct
 *
 *  @param toFree
 */
void Shell_LoadSearchPathsFromString(Shell* shell, char* paths);

/** @brief	Run an executable if exists in the search paths or current working directory. 
 * 			The function reports an error to stdout if the executable cannot be found. 
 *
 *  @param shell
 *	@param command
 *	@param arguments NULL terminated array of strings
 */
void Shell_RunCommand(Shell* shell, char* command, char** arguments);

#endif