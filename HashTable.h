#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>

typedef struct HashTableEntry
{
	char* key;
	char* value;
	struct HashTableEntry* next;
} HashTableEntry;

typedef struct
{
	HashTableEntry* table;
	size_t entries;
	size_t _size;
	size_t _collisions; 
} HashTable;

/** @brief	Initialize an empty HashTable struct of the specified size. 
 *
 * 			The hash table uses chaining with linked lists so it can hold elements greater than the table size. 
 *			Greater size will improve look-up time but increase memory usage. 
 *
 *			This function will dynamically allocated a HashTable struct. Caller must free the HashTable
 *			struct by calling HashTable_Free(HashTable*). Simply calling free() will cause memory leaks!
 *
 *  @return pointer to dynamically allocated HashTable struct
 */
HashTable* HashTable_Make(size_t size);

/** @brief	Free a dynamically allocated Shell struct
 *
 *  @param toFree */
void HashTable_Free(HashTable *hashTable);

/** @brief	Set the string value for a key, overwriting the value if the key already exists
 *
 * 			The hash table will make a copy of the key and value strings, so caller may
 *			modify or free the strings after calling this function. 
 *
 *	@param hashTable
 *	@param _key
 *	@param _value 
 */
void HashTable_Set(HashTable *hashTable, char *_key, char *_value);

/** @brief	Get the string value for a key, if it exists
 *
 *			The function will return a copy of the string value, which must be freed by the caller. 
 *
 *	@param hashTable
 *	@param _key
 *	@return string value (must be freed by caller) if key exists, NULL otherwise
 */
char* HashTable_Get(HashTable *hashTable, char *key);

/** @brief	Delete the string value for a key, if it exists
 *
 *	@param hashTable
 *	@param key
 *	@return true if key exists and value was successfully deleted, false otherwise
 */
bool HashTable_Delete(HashTable *hashTable, char *key);

#endif