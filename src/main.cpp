#include <stdlib.h>
#include <stdio.h>
#include "hash_table.h"

//------------------------------------------------------------------//

int main()
{
    HashTable_t hash_table = {};

    do
    {
        if (HashTableCtor(&hash_table, 13, CountHashChecksum))
            break;

        char* const strings[] = {"penis", "penis2", "penis3", "artem", "lolkek", "jopsa"};
        const size_t size     = sizeof(strings) / sizeof(strings[0]);

        bool error = false;

        for (size_t i = 0; i < size; i++)
        {
            if (HashTablePutElement(&hash_table, strings[i]))
            {
                error = true;
                break;
            }
        }

        if (error)
            break;


        for (size_t i = 0; i < size; i++)
        {
            int hash_table_pos = -1;
            int list_pos       = -1;

            if (HashTableFindElement(&hash_table, strings[i], &hash_table_pos, &list_pos))
                break;
            
            printf("found %s: at %d %d\n", strings[i], hash_table_pos, list_pos);
        }

        if (error)
            break;
    }
    while (0);
    
    HashTableDtor(&hash_table);

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------//
