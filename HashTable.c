#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "HashTable.h"

size_t HashTable_Hash(char *toHash);

HashTable* HashTable_Make(size_t size)
{
	HashTable* new = malloc(sizeof(HashTable));
	assert(new != NULL);
	new->entries = 0;
	new->_size = size;
	new->_collisions = 0;
	new->table = calloc(sizeof(HashTableEntry), new->_size);
	assert(new->table != NULL);
	for(int i = 0 ; i < new->_size ; ++i)
	{
		new->table[i].key = NULL;
		new->table[i].value = NULL;
		new->table[i].next = NULL;
	}

	return new;
}

void HashTable_Set(HashTable *hashTable, char *_key, char *_value)
{
	assert(hashTable != NULL && _key != NULL && _value != NULL);

	char *key = strdup(_key);
	char *value = strdup(_value);

	size_t hash = HashTable_Hash(key) % hashTable->_size;
	HashTableEntry* entry = &(hashTable->table[hash]);
	if(entry->key == NULL)
	{
		entry->key = key;
		entry->value = value;
		++(hashTable->entries);
		return;
	}
	else
	{
		while(1)
		{
			if(strcmp(entry->key, key) == 0)
			{
				free(entry->value);
				free(key);
				entry->value = value;
				return;
			}

			if(entry->next == NULL)
			{
				HashTableEntry* appendage = malloc(sizeof(HashTableEntry));
				assert(appendage != NULL);

				appendage->next = NULL;
				appendage->key = key;
				appendage->value = value;

				entry->next = appendage;
				++(hashTable->entries);
				++(hashTable->_collisions);
				return;
			}
			entry = entry->next;
		}
	}
}

char* HashTable_Get(HashTable *hashTable, char *key)
{
	assert(hashTable != NULL && key != NULL);

	size_t hash = HashTable_Hash(key) % hashTable->_size;
	HashTableEntry* entry = &(hashTable->table[hash]);
	do
	{
		if(entry->key && strcmp(key, entry->key) == 0)
			return strdup(entry->value);
		entry = entry->next;
	} while(entry != NULL);

	return NULL;
}

bool HashTable_Delete(HashTable *hashTable, char *key)
{
	assert(hashTable != NULL && key != NULL);

	size_t hash = HashTable_Hash(key) % hashTable->_size;
	HashTableEntry* entry = &(hashTable->table[hash]);
	HashTableEntry* previous = NULL;
	do
	{
		if(entry->key && strcmp(key, entry->key) == 0)
		{			
			// first element
			free(entry->key);
			free(entry->value);
			entry->key = entry->value = NULL;

			HashTableEntry* next = entry->next;
			if(next != NULL)
			{
				entry->key = next->key;
				entry->value = next->value;
				entry->next = next->next;
				free(next);
			}

			if(previous != NULL)
				previous->next = entry;
			
			--(hashTable->entries);
			
			return true;
		}
		previous = entry;
		entry = entry->next;
	} while(entry != NULL);

	return false;
}

void HashTable_Free(HashTable *hashTable)
{
	assert(hashTable != NULL);
	for(int i = 0 ; i < hashTable->_size ; ++i)
	{
		HashTableEntry* entry = &(hashTable->table[i]);
		free(entry->key);
		free(entry->value);
		entry = entry->next;
		while(entry != NULL)
		{
			free(entry->key);
			free(entry->value);
			HashTableEntry* nextEntry = entry->next;
			free(entry);

			entry = nextEntry;
		}
	}	

	free(hashTable->table);
	free(hashTable);
}

size_t HashTable_Hash(char *toHash)
{
	assert(toHash != NULL);

	// djb2 hashing algorithm
	size_t hash = 5381;
    
    int c;
	while( (c = *(toHash++)) != 0 )
        hash = ((hash << 5) + hash) + c;

    return hash;
}