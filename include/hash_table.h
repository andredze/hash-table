#ifndef HASH_TABLE_H
#define HASH_TABLE_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "my_printfs.h"

//------------------------------------------------------------------//

struct Node_t;

//------------------------------------------------------------------//

const size_t HT_MAX_CAPACITY = 1024 * 1024 * 1024;

//------------------------------------------------------------------//

typedef struct HashTable
{
    size_t      capacity;
    Node_t**    data; 
}
HashTable_t;

//------------------------------------------------------------------//

typedef enum HashTableErr
{
    HT_SUCCESS,
    HT_MEMALLOC_ERR,
    HT_EXCEEDS_MAX_CAPACITY,

}
HashTableErr_t;

//------------------------------------------------------------------//

HashTableErr_t HashTableCtor        (HashTable_t* hash_table, size_t capacity);
HashTableErr_t HashTableFindElement (HashTable_t* hash_table);
HashTableErr_t HashTableVerify      (HashTable_t* hash_table);
HashTableErr_t HashTableDump        (HashTable_t* hash_table);
void           HashTableDtor        (HashTable_t* hash_table);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* HASH_TAB_H */
