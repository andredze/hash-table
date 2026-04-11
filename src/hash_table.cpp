#include "hash_table.h"

//------------------------------------------------------------------//

HashTableErr_t HashTableCtor(HashTable_t* hash_table, size_t capacity)
{
    assert(hash_table);

    if (capacity > HT_MAX_CAPACITY)
    {
        PRINTERR("Hash table capacity: %zu exceeds maximum: %zu", 
                 capacity, HT_MAX_CAPACITY);

        return HT_EXCEEDS_MAX_CAPACITY;
    }

    hash_table->capacity = capacity;

    hash_table->data = (Node_t**) calloc(capacity, sizeof(Node_t**));

    if (hash_table->data == NULL)
    {
        PRINTERR("Memory allocation for hash table failed");

        return HT_MEMALLOC_ERR;
    }

    return HT_SUCCESS;
}

//------------------------------------------------------------------//

void HashTableDtor(HashTable_t* hash_table)
{
    assert(hash_table);

    free(hash_table->data);

    hash_table->data = NULL;

    hash_table->capacity = 0;
}

//------------------------------------------------------------------//