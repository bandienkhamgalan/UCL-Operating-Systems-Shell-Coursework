#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
#define HashTable_Size 4096

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

HashTable* HashTable_Make(size_t size);
void HashTable_Set(HashTable *hashTable, char *_key, char *_value);
char* HashTable_Get(HashTable *hashTable, char *key);
void HashTable_Free(HashTable *hashTable);
bool HashTable_Delete(HashTable *hashTable, char *key);
size_t HashTable_Hash(char *toHash);

#endif