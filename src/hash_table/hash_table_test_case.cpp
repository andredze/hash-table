#include "hash_table_test_case.h"

//------------------------------------------------------------------//

HashTableErr_t HashTableRunTestCase(HashTable_t* hash_table, Word_t* words)
{
    assert(hash_table);

    int pos = 0;

    int misses_count = 0;

    HashTableErr_t error = HT_SUCCESS;

    for (size_t test_num = 0; test_num < TESTS_COUNT; test_num++)
    {
        for (size_t i = 0; i < hash_table->size; i++)
        {
#ifdef FULLASM
            if ((error = HashTableFindElementAsm(hash_table, words[i], NULL, &pos)))
            {
                break;
            }
#else
            if ((error = HashTableFindElement(hash_table, words[i], NULL, &pos)))
            {
                break;
            }
#endif /* FULLASM */

            DPRINTF("word %s pos %d\n", words[i], pos);
        }
    }

    printf("Tests ended: misses count = %d\n", misses_count);

    if (misses_count > 0)
    {
        return HT_FAILED_TESTS;
    }

    return HT_SUCCESS;
}

//------------------------------------------------------------------//