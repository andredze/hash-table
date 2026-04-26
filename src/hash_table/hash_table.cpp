#include <math.h>
#include "hash_table.h"

//------------------------------------------------------------------//

static void HashTableDebugPrintElem(int elem_index, char* item)
{
    DPRINTF("elem_index = %d (%s | ", elem_index, item);
    
    for (int i = 0; i < STR_MAX_SIZE; i++)
    {
        DPRINTF("%d ", (char) item[i]);
    }

    DPRINTF(")\n");
}

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
    hash_table->size     = 0;

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

    for (size_t i = 0; i < hash_table->capacity; i++)
    {
        if (ListCtor(&hash_table->data[i], 2) != LIST_SUCCESS)
        {
            return HT_LIST_ERROR;
        }
    }

    return HT_SUCCESS;
}

//------------------------------------------------------------------//

static HashTableErr_t HashTableGetElemList(HashTable_t*     hash_table, 
                                           HashTableElem_t  item, 
                                           List_t**         list_ptr_ptr)
{

#if defined(NOCRC32ASM)
    size_t hash_value = CountHashCrc32(item);
#elif defined(HIST)
    size_t hash_value = hash_table->hash_function(item);
#elif defined(FULLASM)
    size_t hash_value = CountHashCrc32Asm(item);
#else
    size_t hash_value = CountHashCrc32AsmInline(item);
#endif /* NOCRC32ASM */

    size_t elem_index = hash_value % hash_table->capacity;

    HashTableDebugPrintElem(elem_index, item);

    *list_ptr_ptr = &hash_table->data[elem_index];

    return HT_SUCCESS;
}

//------------------------------------------------------------------//

HashTableErr_t HashTablePutElement(HashTable_t *hash_table, HashTableElem_t item)
{
    assert(hash_table);

    if (hash_table->capacity == 0)
    {
        PRINTERR("Can't put element in an empty hash table");

        return HT_CAPACITY_IS_ZERO;
    }

    List_t*        list_ptr = NULL;
    HashTableErr_t error    = HT_SUCCESS;

    DPRINTF("putting %s\n", item);

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

        hash_table->size++;

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

    hash_table->size++;

    return HT_SUCCESS;
}

//------------------------------------------------------------------//

HashTableErr_t HashTableVerify(HashTable_t* hash_table)
{
    assert(hash_table);

#ifdef LIST_DEBUG
    for (size_t i = 0; i < hash_table->capacity; i++)
    {
        ListErr_t error = LIST_SUCCESS;

        if ((error = ListVerify(hash_table->data[i])))
        {
            PRINTERR("Error in bucket %zu: %s", i, LIST_STR_ERRORS[error]);
            
            return HT_LIST_ERROR;
        }
    }
#endif /* LIST_DEBUG */

    return HT_SUCCESS;
}

//------------------------------------------------------------------//

HashTableErr_t HashTableFindElement(HashTable_t*    hash_table,
                                    HashTableElem_t item,
                                    int*            hash_table_pos,
                                    int*            list_pos)
{
    if (hash_table->capacity == 0)
    {
        if (hash_table_pos)
        {
            *hash_table_pos = -1;
        }
        if (list_pos)
        {
            *list_pos = -1;
        }

        return HT_SUCCESS;
    }

#if defined(NOCRC32ASM)
    size_t hash_value = CountHashCrc32(item);
#elif defined(HIST)
    size_t hash_value = hash_table->hash_function(item);
#else
    size_t hash_value = CountHashCrc32AsmInline(item);
#endif /* NOCRC32ASM */

    size_t elem_index = hash_value % hash_table->capacity;

    HashTableDebugPrintElem(elem_index, item);

    List_t* list_ptr = &hash_table->data[elem_index];

    int pos = -1;

    ListFindElement(list_ptr, item, &pos);

    // (pos == -1) if element was not found

    if (hash_table_pos)
    {    
        *hash_table_pos = (int) elem_index;
    }
    if (list_pos)
    {
        *list_pos = pos;
    }

#ifdef LIST_DEBUG
    char debug_name[DEBUG_NAME_LEN] = {};

    snprintf(debug_name, sizeof(debug_name), "hash_table_%zu", elem_index);

    ListDumpInfo_t dump_info = {LIST_SUCCESS, debug_name, __PRETTY_FUNCTION__, __FILE__, __LINE__};

    ListDump(list_ptr, &dump_info, "dumping hash_table[%d]", elem_index);
#endif /* LIST_DEBUG */

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

#if defined(NOAVX)

    free(hash_table->words);

#else

    _mm_free(hash_table->words);

#endif /* NOAVX */

    hash_table->words = NULL;
}

//------------------------------------------------------------------//

HashTableErr_t HashTableCountVariance(HashTable_t* hash_table, 
                                      double*      variance_dest,
                                      double*      std_deviation_dest)
{
    assert(hash_table);

    if ((variance_dest == NULL) || (std_deviation_dest == NULL))
    {
        PRINTERR("Given null ptr");
        return HT_NULL_ERR;
    }

    size_t size_sum = 0;

    for (size_t i = 0; i < hash_table->capacity; i++)
    {
        size_sum += hash_table->data[i].size;
    }

    double mean = (double) size_sum / hash_table->capacity;

    size_t deviation_squared_sum = 0;

    for (size_t i = 0; i < hash_table->capacity; i++)
    {
        size_t diff = hash_table->data[i].size - mean;
        deviation_squared_sum += diff * diff;
    }

    double variance      = (double) deviation_squared_sum / hash_table->capacity;
    double std_deviation = sqrt(variance);
    *variance_dest       = variance;
    *std_deviation_dest  = std_deviation;

    return HT_SUCCESS;
}

//------------------------------------------------------------------//

HashTableErr_t HashTableDrawHistogram(HashTable_t* hash_table, 
                                      const char*  hist_name,
                                      const char*  hist_title)
{
    assert(hash_table);
    assert(hist_name);
    assert(hist_title);

    FILE* hist_file = fopen("hist/data.dat", "w");

    for (size_t i = 0; i < hash_table->capacity; i++)
    {
        fprintf(hist_file, "%zu\n", hash_table->data[i].size);
    }

    fclose(hist_file);

    FILE* gnuplot_terminal = popen("gnuplot -persist", "w");

    if (gnuplot_terminal == NULL)
    {
        PRINTERR("Failed entering gnuplot");
        return HT_SYSTEM_ERR;
    }

    char command[MAX_COMMAND_LENGTH] = {};

    snprintf(command, sizeof(command), "call \'%s\' \"hist/%s.png\" \"%s\"\n", 
                      GNUPLOT_SCRIPT_PATH, hist_name, hist_title);

    fprintf(gnuplot_terminal, "%s", command);

    pclose(gnuplot_terminal);

    return HT_SUCCESS;
}

//------------------------------------------------------------------//

HashTableErr_t HashTableLoadData(HashTable_t* hash_table, const char* data_file_path)
{
    assert(hash_table);

    if (hash_table->words != NULL)
    {
        PRINTERR("Can only load data once in a hash table");
        
        return HT_DATA_ALREADY_LOADED_ERR;
    }

    InputCtx_t data_ctx = {.input_file_info = {.filepath = data_file_path}};

    fprintf(stderr, "Reading file\n");

    if (ReadAndParseFile(&data_ctx) == 1)
    {
        PRINTERR("Failed parsing/reading data file %s", data_file_path);
    
        InputCtxDtor(&data_ctx);
        
        return HT_FILE_ERROR;
    }

    HashTableErr_t error = HT_SUCCESS;

    size_t words_data_size = data_ctx.ptrdata_params.lines_count * sizeof(Word_t);

#if defined(NOAVX)

    hash_table->words = (Word_t*) calloc(data_ctx.ptrdata_params.lines_count, sizeof(Word_t));

#else

    hash_table->words = (Word_t*) _mm_malloc(words_data_size, sizeof(Word_t));

    // initialize words with zeros
    for (size_t i = 0; i < data_ctx.ptrdata_params.lines_count; i++)
    {
        strncpy(hash_table->words[i], ZERO_DATA, sizeof(hash_table->words[i]));
    }

#endif /* NOAVX */

    if (hash_table->words == NULL)
    {
        PRINTERR("Failed memory allocation for hash_table->words");
        return HT_MEMALLOC_ERR;
    }

    fprintf(stderr, "Filling table\n");

    for (int i = 0; i < data_ctx.ptrdata_params.lines_count; i++)
    {
        strncpy(hash_table->words[i], data_ctx.ptrdata_params.ptrdata[i], 
                sizeof(hash_table->words[i]));
     
        if ((error = HashTablePutElement(hash_table, hash_table->words[i])))
        {
            InputCtxDtor(&data_ctx);

            return error;
        }

        DPRINTF("Putting %s; size = %zu\n", data_ctx.ptrdata_params.ptrdata[i], hash_table->size);
    }

    InputCtxDtor(&data_ctx);    

    return HT_SUCCESS;
}

//------------------------------------------------------------------//