#include "hash_table_test_case.h"

//------------------------------------------------------------------//

HashTableErr_t HashTableRunTestCase(HashTable_t* hash_table)
{
    assert(hash_table);

    int pos = 0;

    int misses_count = 0;

    HashTableErr_t error = HT_SUCCESS;

    for (size_t test_num = 0; test_num < TESTS_COUNT; test_num++)
    {
        for (size_t i = 0; i < hash_table->size; i++)
        {
            if ((error = HashTableFindElement(hash_table, hash_table->words[i], NULL, &pos)))
            {
                break;
            }
            
            misses_count += (pos == -1);
        }
    }

    printf("Tests ended: misses count = %d\n", misses_count);

    return HT_SUCCESS;
}

//------------------------------------------------------------------//