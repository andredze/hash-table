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
#include "input.h"

//------------------------------------------------------------------//

const size_t HT_MAX_CAPACITY     = 1024 * 1024 * 1024;
char * const HT_NODE_POISON      = NULL;
const int    DEBUG_NAME_LEN      = 128;
const int    HT_MAX_FILENAME_LEN = 100;
const int    MAX_COMMAND_LENGTH  = 256;

const char * const GNUPLOT_SCRIPT_PATH     = "hist/make_hist.gp";
const char * const VARIANCE_DATA_FILE_PATH = "hist/variance.md";
const char * const DOT_DIR_PATH            = "log/dot";

//------------------------------------------------------------------//

typedef char* HashTableElem_t;

typedef char Word_t[STR_MAX_SIZE];

const Word_t ZERO_DATA = {};

//------------------------------------------------------------------//

typedef struct HashTable
{
    size_t      capacity;
    size_t      size;

    List_t*     data;

    uint32_t (* hash_function)(const HashTableElem_t elem);

    Word_t*     words;
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
    HT_NULL_ERR,
    HT_FILE_ERROR,
    HT_SYSTEM_ERR,
    HT_FILENAME_TOOBIG,
    HT_DATA_ALREADY_LOADED_ERR,
    HT_FAILED_TESTS
}
HashTableErr_t;

//------------------------------------------------------------------//

typedef struct HashFunctionHistCase
{
    uint32_t (* hash_function) (char* const string);
    const char* hist_name;
    const char* hist_title;
}
HashFunctionHistCase_t;

//------------------------------------------------------------------//

const HashFunctionHistCase_t HT_HASH_FUNC_HIST_CASES_TABLE[] = {
    {CountHashAlwaysOne  , "always_one"  , "\\\"value is always one\\\""    },
    {CountHashFirstLetter, "first_letter", "\\\"first letter ASCII code\\\""},
    {CountHashLength     , "strlen"      , "strlen"                         },
    {CountHashChecksum   , "checksum"    , "checksum"                       },
    {CountHashRotateLeft , "rol"         , "\\\"rotate left + xor\\\""      },
    {CountHashRotateRight, "ror"         , "\\\"rotate right + xor\\\""     },
    {CountHashDjb2       , "djb2"        , "gnu (djb2)"                     },
    {CountHashCrc32      , "crc32"       , "crc32"                          },
    {CountHashCrc32Asm   , "crc32_asm"   , "crc32 asm"                      }
};

//------------------------------------------------------------------//

const size_t HISTOGRAMS_COUNT = sizeof(HT_HASH_FUNC_HIST_CASES_TABLE) / 
                                sizeof(HT_HASH_FUNC_HIST_CASES_TABLE[0]);

//------------------------------------------------------------------//

#ifdef DEBUG
const char * const TEST_DATA_FILE_PATH = "data/test.txt"; 
#else
const char * const TEST_DATA_FILE_PATH = "data/sorted.txt"; 
#endif /* DEBUG */

const char * const HIST_DATA_FILE_PATH = "data/sorted.txt"; 

//------------------------------------------------------------------//

HashTableErr_t HashTableCtor(HashTable_t* hash_table, 
                             size_t       capacity, 
                             uint32_t  (* hash_function)(HashTableElem_t elem));

HashTableErr_t HashTablePutElement      (HashTable_t* hash_table, HashTableElem_t item);
HashTableErr_t HashTableVerify          (HashTable_t* hash_table);
HashTableErr_t HashTableDump            (HashTable_t* hash_table);
void           HashTableDtor            (HashTable_t* hash_table);
HashTableErr_t HashTableLoadData        (HashTable_t* hash_table, const char* data_file_path);

HashTableErr_t HashTableDrawHistogram(HashTable_t* hash_table, 
                                      const char*  hist_name,
                                      const char*  hist_title);

HashTableErr_t HashTableFindElement(HashTable_t*    hash_table, 
                                    HashTableElem_t item, 
                                    int*            hash_table_pos,
                                    int*            list_pos);

HashTableErr_t HashTableCountVariance(HashTable_t* hash_table, 
                                      double*      variance_dest,
                                      double*      std_deviation_dest);

extern "C" HashTableErr_t HashTableFindElementAsm(HashTable_t*    hash_table,
                                                  HashTableElem_t item,
                                                  int*            hash_table_pos,
                                                  int*            list_pos);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* HASH_TABLE_H */
