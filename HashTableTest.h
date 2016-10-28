#include "greatest/greatest.h"
#include "HashTable.h"

TEST HashTable_Make_SetsEntriesTo0() {
    HashTable* hashTable = HashTable_Make(8);
    ASSERT_EQ(hashTable->entries, 0);
    HashTable_Free(hashTable);
    PASS();    
} 

TEST HashTable_Set_IncrementsEntries() {
    HashTable* hashTable = HashTable_Make(8);
    HashTable_Set(hashTable, "sup", "yes");
    ASSERT_EQ(hashTable->entries, 1);
    HashTable_Free(hashTable);
    PASS();    
} 

TEST HashTable_Set_DuplicateElementDoesNotIncrementEntries() {
    HashTable* hashTable = HashTable_Make(8);
    HashTable_Set(hashTable, "sup", "yes");
    ASSERT_EQ(hashTable->entries, 1);
    HashTable_Set(hashTable, "sup", "no");
    ASSERT_EQ(hashTable->entries, 1);
    HashTable_Free(hashTable);
    PASS();    
} 

TEST HashTable_Set_AddedElementCanBeRetrieved() {
    HashTable* hashTable = HashTable_Make(8);
    HashTable_Set(hashTable, "sup", "yes");
    char* value = HashTable_Get(hashTable, "sup");
    ASSERT(value != NULL);
    ASSERT_STR_EQ(value, "yes");
    HashTable_Free(hashTable);
    PASS();      
}

TEST HashTable_Set_DuplicateElementCanBeRetrieved() {
    HashTable* hashTable = HashTable_Make(8);
    HashTable_Set(hashTable, "sup", "yes");
    HashTable_Set(hashTable, "sup", "no");
    char* value = HashTable_Get(hashTable, "sup");
    ASSERT(value != NULL);
    ASSERT_STR_EQ(value, "no");
    HashTable_Free(hashTable);
    PASS();      
}

TEST HashTable_Set_HandlesCollisionsGracefully() {
    HashTable* hashTable = HashTable_Make(1);
    char *keys[] = { "hello", "world", "what", "is", "up", "yes", "no", "will", "this", "break", "the", "hash", "table", "chair", "fork", "spoon", "Mongolia", "juice", "water"};
    char *values[] = { "world", "this", "is", "love", "down", "no", "yes", "they", "will", "the", "hash", "table", "chair", "table", "spoon", "fork", "China", "Coke", "air"};
    for(size_t i = 0 ; i < sizeof(keys) / sizeof(char *) ; ++i)
        HashTable_Set(hashTable, keys[i], values[i]);
    ASSERT_EQ(19, sizeof(keys) / sizeof(char *));
    ASSERT_EQ(hashTable->entries, sizeof(keys) / sizeof(char *));
    for(size_t i = 0 ; i < sizeof(keys) / sizeof(char *) ; ++i)
    {
        char* value = HashTable_Get(hashTable, keys[i]);
        ASSERT(value != NULL);
        ASSERT_STR_EQ(value, values[i]);
    }
    HashTable_Free(hashTable);
    PASS();      
}

TEST HashTable_Get_NoElementReturnsNull() {
    HashTable* hashTable = HashTable_Make(1);
    ASSERT_EQ(NULL, HashTable_Get(hashTable, "test"));
    ASSERT_EQ(NULL, HashTable_Get(hashTable, "yes"));
    ASSERT_EQ(NULL, HashTable_Get(hashTable, "no"));
    ASSERT_EQ(NULL, HashTable_Get(hashTable, "up"));
    ASSERT_EQ(NULL, HashTable_Get(hashTable, "12345678"));
    HashTable_Free(hashTable);
    PASS();      
}

TEST HashTable_Delete_RemovesOnlyMatchingElementAndDecrementsEntries() {
    HashTable* hashTable = HashTable_Make(2);
    char *keys[] = { "hello", "world", "what", "is", "up", "yes", "no", "will", "this", "break", "the", "hash", "table", "chair", "fork", "spoon", "Mongolia", "juice", "water"};
    char *values[] = { "world", "this", "is", "love", "down", "no", "yes", "they", "will", "the", "hash", "table", "chair", "table", "spoon", "fork", "China", "Coke", "air"};
    for(size_t i = 0 ; i < sizeof(keys) / sizeof(char *) ; ++i)
        HashTable_Set(hashTable, keys[i], values[i]);
    ASSERT_EQ(19, sizeof(keys) / sizeof(char *));
    ASSERT_EQ(hashTable->entries, sizeof(keys) / sizeof(char *));
    ASSERT_EQ(true, HashTable_Delete(hashTable, "hello"));
    ASSERT_EQ(18, -1 + sizeof(keys) / sizeof(char *));
    ASSERT_EQ(hashTable->entries, -1 + sizeof(keys) / sizeof(char *));
    ASSERT_EQ(NULL, HashTable_Get(hashTable, "hello"));
    for(size_t i = 1 ; i < sizeof(keys) / sizeof(char *) ; ++i)
    {
        char* value = HashTable_Get(hashTable, keys[i]);
        ASSERT(value != NULL);
        ASSERT_STR_EQ(value, values[i]);
    }
    HashTable_Free(hashTable);
    PASS();     
}

TEST HashTable_Delete_NonExistantElementKeepsAllElementsAndDoesNotChangeEntries() {
    HashTable* hashTable = HashTable_Make(2);
    char *keys[] = { "hello", "world", "what", "is", "up", "yes", "no", "will", "this", "break", "the", "hash", "table", "chair", "fork", "spoon", "Mongolia", "juice", "water"};
    char *values[] = { "world", "this", "is", "love", "down", "no", "yes", "they", "will", "the", "hash", "table", "chair", "table", "spoon", "fork", "China", "Coke", "air"};
    for(size_t i = 0 ; i < sizeof(keys) / sizeof(char *) ; ++i)
        HashTable_Set(hashTable, keys[i], values[i]);
    ASSERT_EQ(19, sizeof(keys) / sizeof(char *));
    ASSERT_EQ(hashTable->entries, sizeof(keys) / sizeof(char *));
    ASSERT_EQ(false, HashTable_Delete(hashTable, "they"));
    ASSERT_EQ(19, sizeof(keys) / sizeof(char *));
    ASSERT_EQ(hashTable->entries, sizeof(keys) / sizeof(char *));
    for(size_t i = 0 ; i < sizeof(keys) / sizeof(char *) ; ++i)
    {
        char* value = HashTable_Get(hashTable, keys[i]);
        ASSERT(value != NULL);
        ASSERT_STR_EQ(value, values[i]);
    }
    HashTable_Free(hashTable);
    PASS();     
}

SUITE(HashTableTest)
{
    RUN_TEST(HashTable_Make_SetsEntriesTo0);
    RUN_TEST(HashTable_Set_IncrementsEntries);
    RUN_TEST(HashTable_Set_DuplicateElementDoesNotIncrementEntries);
    RUN_TEST(HashTable_Set_AddedElementCanBeRetrieved);
    RUN_TEST(HashTable_Set_DuplicateElementCanBeRetrieved);
    RUN_TEST(HashTable_Set_HandlesCollisionsGracefully);
    RUN_TEST(HashTable_Get_NoElementReturnsNull);
    RUN_TEST(HashTable_Delete_RemovesOnlyMatchingElementAndDecrementsEntries);
    RUN_TEST(HashTable_Delete_NonExistantElementKeepsAllElementsAndDoesNotChangeEntries);
}