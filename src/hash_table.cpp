#include "hash_table.h"

// HashLoad
// HashDumpHist

//------------------------------------------------------------------//

HashTableErr_t HashTableCtor(HashTable_t *hash_table, size_t capacity,
                             uint32_t (* hash_function)(HashTableElem_t elem))
{
    assert(hash_table);

    if (capacity > HT_MAX_CAPACITY)
    {
        PRINTERR("Hash table capacity: %zu exceeds maximum: %zu",
                 capacity, HT_MAX_CAPACITY);

        return HT_EXCEEDS_MAX_CAPACITY;
    }

    hash_table->capacity = capacity;

    hash_table->data = (List_t *)calloc(capacity, sizeof(List_t));

    if (hash_table->data == NULL)
    {
        PRINTERR("Memory allocation for hash table failed");

        return HT_MEMALLOC_ERR;
    }

    if (hash_function == NULL)
    {
        PRINTERR("No hash function provided");

        return HT_NULL_ERR;
    }

    hash_table->hash_function = hash_function;

    return HT_SUCCESS;
}

//------------------------------------------------------------------//

static int HashTableElemsEqual(HashTableElem_t elem1, HashTableElem_t elem2)
{
    assert(elem1);
    assert(elem2);

    if (strcmp(elem1, elem2) == 0)
    {
        return 1;
    }

    return 0;
}

//------------------------------------------------------------------//

static HashTableErr_t HashTableGetElemList(HashTable_t*     hash_table, 
                                           HashTableElem_t  item, 
                                           List_t**         list_ptr_ptr)
{
    size_t hash_value = hash_table->hash_function(item);

    if (hash_table->capacity == 0)
    {
        PRINTERR("Can't put element in an empty hash table");

        return HT_CAPACITY_IS_ZERO;
    }

    size_t elem_index = hash_value % hash_table->capacity;

    *list_ptr_ptr = &hash_table->data[elem_index];

    return HT_SUCCESS;
}

//------------------------------------------------------------------//

HashTableErr_t HashTablePutElement(HashTable_t *hash_table, HashTableElem_t item)
{
    List_t*        list_ptr = NULL;
    HashTableErr_t error    = HT_SUCCESS;

    if ((error = HashTableGetElemList(hash_table, item, &list_ptr)))
    {
        return error;
    }

    //------------------------------------------------------------------//

    // if list was not ctored
    if (list_ptr->capacity == 0)
    {
        if (ListCtor(list_ptr, 1) != LIST_SUCCESS)
        {
            return HT_LIST_ERROR;
        }
        // insert first element
        if (ListInsertAfter(list_ptr, 0, item, NULL) != LIST_SUCCESS)
        {
            return HT_LIST_ERROR;
        }

        return HT_SUCCESS;
    }

    //------------------------------------------------------------------//

    int pos = -1;

    if (ListFindElement(list_ptr, item, &pos) != LIST_SUCCESS)
    {
        return HT_LIST_ERROR;
    }

    // (pos == -1) if element was not found
    if (pos != -1)
    {
        return HT_SUCCESS;
    }

    if (ListPushBack(list_ptr, item, NULL) != LIST_SUCCESS)
    {
        return HT_LIST_ERROR;
    }

    return HT_SUCCESS;
}

//------------------------------------------------------------------//

HashTableErr_t HashTableVerify(HashTable_t* hash_table)
{

    return HT_SUCCESS;
}

//------------------------------------------------------------------//

HashTableErr_t HashTableFindElement(HashTable_t*    hash_table, 
                                    HashTableElem_t item, 
                                    int*            hash_table_pos,
                                    int*            list_pos)
{
    size_t hash_value = hash_table->hash_function(item);

    if (hash_table->capacity == 0)
    {
        PRINTERR("Can't put element in an empty hash table");

        return HT_CAPACITY_IS_ZERO;
    }

    size_t elem_index = hash_value % hash_table->capacity;

    List_t* list_ptr = &hash_table->data[elem_index];

    int pos = -1;

    if (ListFindElement(list_ptr, item, &pos) != LIST_SUCCESS)
    {
        return HT_LIST_ERROR;
    }

    // (pos == -1) if element was not found

    *hash_table_pos = (int) elem_index;
    *list_pos       = pos;  

    return HT_SUCCESS;
}

//------------------------------------------------------------------//

void HashTableDtor(HashTable_t *hash_table)
{
    assert(hash_table);

    size_t capacity = hash_table->capacity;

    for (size_t i = 0; i < capacity; i++)
    {
        if (hash_table->data[i].data != NULL)
        {
            ListDtor(&hash_table->data[i]);
        }
    }

    free(hash_table->data);

    hash_table->data = NULL;

    hash_table->capacity = 0;
}

//------------------------------------------------------------------//