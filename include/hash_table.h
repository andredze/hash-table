#ifndef HASH_TABLE_H
#define HASH_TABLE_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "my_printfs.h"
#include "hash_functions.h"
#include "listCommands.h"

//------------------------------------------------------------------//

const size_t HT_MAX_CAPACITY = 1024 * 1024 * 1024;
char * const HT_NODE_POISON = NULL;

//------------------------------------------------------------------//

typedef char* HashTableElem_t;

//------------------------------------------------------------------//

typedef struct HashTable
{
    size_t      capacity;
    List_t*     data; 

    uint32_t (* hash_function)(const HashTableElem_t elem);
}
HashTable_t;

//------------------------------------------------------------------//

typedef enum HashTableErr
{
    HT_SUCCESS,
    HT_MEMALLOC_ERR,
    HT_EXCEEDS_MAX_CAPACITY,
    HT_CAPACITY_IS_ZERO,
    HT_LIST_ERROR,
    HT_NULL_ERR
}
HashTableErr_t;

//------------------------------------------------------------------//

HashTableErr_t HashTableCtor        (HashTable_t* hash_table, size_t capacity, uint32_t (* hash_function)(HashTableElem_t elem));
HashTableErr_t HashTablePutElement  (HashTable_t* hash_table, HashTableElem_t item);
HashTableErr_t HashTableVerify      (HashTable_t* hash_table);
HashTableErr_t HashTableDump        (HashTable_t* hash_table);
void           HashTableDtor        (HashTable_t* hash_table);

HashTableErr_t HashTableFindElement(HashTable_t*    hash_table, 
                                    HashTableElem_t item, 
                                    int*            hash_table_pos,
                                    int*            list_pos);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* HASH_TAB_H */
