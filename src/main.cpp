#include <stdlib.h>
#include <stdio.h>
#include "hash_table.h"

//------------------------------------------------------------------//

int main()
{
    HashTable_t    hash_table = {};
    HashTableErr_t error      = HT_SUCCESS;

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

            for (size_t data_num = 0; data_num < DATA_FILES_COUNT; data_num++)
            {
                if ((error = HashTableLoadData(&hash_table, HT_DATA_FILES_TABLE[data_num])))
                    break;
            }

            if (error)
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

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------//
