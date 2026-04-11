#include <stdlib.h>
#include <stdio.h>
#include "hash_table.h"

//------------------------------------------------------------------//

int main()
{
    HashTable_t hash_table = {};

    do
    {
        if (HashTableCtor(&hash_table, 1024))
            break;

                
    }
    while (0);
    
    HashTableDtor(&hash_table);

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------//
