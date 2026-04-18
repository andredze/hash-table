#ifndef HASH_TABLE_TEST_CASE_H
#define HASH_TABLE_TEST_CASE_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "hash_table.h"

//------------------------------------------------------------------//

const int TESTS_COUNT = 1000;

//------------------------------------------------------------------//

HashTableErr_t HashTableRunTestCase(HashTable_t* hash_table);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* HASH_TABLE_TEST_CASE_H */