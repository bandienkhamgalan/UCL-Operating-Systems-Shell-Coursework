#ifndef HELPERS_H
#define HELPERS_H

#include <stdbool.h>

/** @brief	Removes the leading and trailing whitespace of a string in-place. 
 *
 *  @param toTrim string to trim, will be modified
 *  @return length of string after trimming
 */
size_t trimWhitespace(char *toTrim);

/** @brief	Splits a variable assignment string into name and value, by separating the string on the '=' token, 
 *
 *  @param toParse string containing variable assignment, contents will be modified
 *	@param name non-null pointer to string, will be set to name
 *	@param value non-null pointer to string, will be set to value
 *  @return true if parsing succeeded, false otherwise
 */
bool parseAssignmentString(char *toParse, /*out*/ char **name, /*out*/char **value);

/** @brief	Computes the length of a NULL-terminated array of strings
 *
 *  @param toCount array
 *  @return length of array
 */
size_t strarraylen(char **toCount);

/** @brief	Splits a string into components delimited by space character. 
 *
 *			This function will split a string into components delimited by space, but ignored spaces escaped by
 *			'\' character and spaces inside a quoted string. It also removes escape characters and quotes. 
 *			
 *			e.g. 	ls /Volumes/Macintosh\ HD => 2 components: {ls, /Volumes/Macintosh HD}
 *					ls "/Volumes/Macintosh HD" => 2 components: {ls, /Volumes/Macintosh HD}
 *
 *			This function will return an empty array if parsing fails. 
 *
 *  @param toParse string to parse, will be modified
 *  @return NULL-terminated malloc-ed array of strings, array must be freed by caller
 */
char** splitBySpace(char *toParse); 

#endif