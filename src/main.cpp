#include <stdlib.h>
#include <stdio.h>
#include "hash_table.h"
#include "hash_table_test_case.h"

//------------------------------------------------------------------//

int main(int argc, char* argv[])
{
    HashTable_t    hash_table = {};
    HashTableErr_t error      = HT_SUCCESS;

#ifdef HIST
    FILE* variance_data_file = fopen(VARIANCE_DATA_FILE_PATH, "w"); 

    double variance      = 0.0;
    double std_deviation = 0.0;

    fprintf(variance_data_file, "| %-20s | %15s | %15s |\n"
                                "|-------|------|------|\n", 
            "hash func", "variance", "std deviation");

    for (size_t i = 0; i < HISTOGRAMS_COUNT; i++)
    {
        do
        {
            const char* hash_func_name = HT_HASH_FUNC_HIST_CASES_TABLE[i].hist_name;
            const char* hist_title     = HT_HASH_FUNC_HIST_CASES_TABLE[i].hist_title;

            fprintf(stderr, "Ctoring\n");

            if ((error = HashTableCtor(&hash_table, 5003, HT_HASH_FUNC_HIST_CASES_TABLE[i].hash_function)))
                break;

            fprintf(stderr, "Loading data\n");

            if ((error = HashTableLoadData(&hash_table, DATA_FILE_PATH)))
                break;

            fprintf(stderr, "Drawing histogram %s\n", hash_func_name);

            if ((error = HashTableDrawHistogram(&hash_table, hash_func_name, hist_title)))
                break;

            fprintf(stderr, "Counting variance for %s\n", hash_func_name);

            if ((error = HashTableCountVariance(&hash_table, &variance, &std_deviation)))
                break;

            fprintf(variance_data_file, "| %-20s | %15.2f | %15.2f |\n", 
                    hash_func_name, variance, std_deviation);
        }
        while (0);
         
        HashTableDtor(&hash_table);

        if (error)
        {
            fclose(variance_data_file);
            return error;
        }
    }

    fclose(variance_data_file);
#endif /* HIST */

#ifdef TEST

    alignas(64) char var1[64] = {};
    alignas(64) char var2[64] = {};

    fprintf(stderr, "%s == %s is %d\n", var1, var2, ListElemsEqual(var1, var2));

    strcpy(var1, "peniiiiiiiiiiiiiiiiiiiiiii");
    strcpy(var2, "peniiiiiiiiiiiiiiiiiiiiiii");
    
    fprintf(stderr, "%s == %s is %d\n", var1, var2, ListElemsEqual(var1, var2));
    
    strcpy(var1, "peniiiiiiiiiiiiiiiiiiiiiii");
    strcpy(var2, "peniiiiiiiiiiiiiiiiiiii222");

    fprintf(stderr, "%s == %s is %d\n", var1, var2, ListElemsEqual(var1, var2));

    do
    {
        fprintf(stderr, "Ctoring\n");

        if ((error = HashTableCtor(&hash_table, 5003, CountHashCrc32)))
            break;

        fprintf(stderr, "Loading data\n");

        if ((error = HashTableLoadData(&hash_table, DATA_FILE_PATH)))
            break;

        fprintf(stderr, "Running test case\n");

        if ((error = HashTableRunTestCase(&hash_table)))
            break;
    }
    while (0);

    HashTableDtor(&hash_table);
#endif /* TEST */

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------//
