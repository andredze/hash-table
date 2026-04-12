#ifndef LIST_TYPES_H
#define LIST_TYPES_H

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG
    #define DEBUG
#endif

//——————————————————————————————————————————————————————————————————————————————————————————

#include "my_printfs.h"
#include <stdlib.h>
#include <stdint.h>

//——————————————————————————————————————————————————————————————————————————————————————————

typedef char* elem_t;

#define SPEC "%s"

const elem_t LIST_POISON = NULL;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct Node
{
    elem_t value;
    int    next;
    int    prev;
} Node_t;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct List
{
    Node_t*  data;
    size_t   capacity;
    size_t   size;
    int      free;
    int      is_sorted;
} List_t;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef enum ListErr
{
    LIST_SUCCESS,
    LIST_DUMP_ERROR,
    LIST_CALLOC_ERROR,
    LIST_DATA_REALLOC_ERROR,
    LIST_LOGFILE_OPEN_ERROR,
    LIST_FILENAME_TOOBIG,
    LIST_IMAGE_NAME_NULL,
    LIST_CTX_NULL,
    LIST_DATA_NULL,
    LIST_CAPACITY_EXCEEDS_MAX,
    LIST_HEAD_NEGATIVE,
    LIST_HEAD_TOOBIG,
    LIST_TAIL_NEGATIVE,
    LIST_TAIL_TOOBIG,
    LIST_FREE_NEGATIVE,
    LIST_FREE_TOOBIG,
    LIST_NEXT_NEGATIVE,
    LIST_NEXT_TOOBIG,
    LIST_PREV_NEGATIVE,
    LIST_PREV_TOOBIG,
    LIST_FREE_NEXT_NEGATIVE,
    LIST_FREE_NEXT_TOOBIG,
    LIST_FREE_PREV_NOT_NULL,
    LIST_POSITION_NEGATIVE,
    LIST_POSITION_TOO_BIG,
    LIST_NO_SUCH_ELEMENT,
    LIST_NEXT_IS_CYCLED,
    LIST_PREV_IS_CYCLED,
    LIST_FREE_IS_CYCLED,
    LIST_FREE_VALUE_NOT_PZN,
    LIST_FILLED_VALUE_IS_PZN,
    LIST_SIZE_EXCEEDS_CAPACITY,
    LIST_SIZE_IS_WRONG,
    LIST_CAP_IS_WRONG,
    LIST_NEXT_WRONG,
    LIST_PREV_WRONG,
    LIST_FLAG_IS_WRONG
} ListErr_t;

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG

//——————————————————————————————————————————————————————————————————————————————————————————

const char* const LIST_STR_ERRORS[] =
{
    [LIST_SUCCESS]               = "LIST_SUCCESS",
    [LIST_DUMP_ERROR]            = "LIST_DUMP_ERROR",
    [LIST_CALLOC_ERROR]          = "LIST_CALLOC_ERROR",
    [LIST_DATA_REALLOC_ERROR]    = "LIST_DATA_REALLOC_ERROR",
    [LIST_LOGFILE_OPEN_ERROR]    = "LIST_LOGFILE_OPEN_ERROR",
    [LIST_FILENAME_TOOBIG]       = "LIST_FILENAME_TOOBIG",
    [LIST_IMAGE_NAME_NULL]       = "LIST_IMAGE_NAME_NULL",
    [LIST_CTX_NULL]              = "LIST_CTX_NULL",
    [LIST_DATA_NULL]             = "LIST_DATA_NULL",
    [LIST_CAPACITY_EXCEEDS_MAX]  = "LIST_CAPACITY_EXCEEDS_MAX",
    [LIST_HEAD_NEGATIVE]         = "LIST_HEAD_NEGATIVE",
    [LIST_HEAD_TOOBIG]           = "LIST_HEAD_TOOBIG",
    [LIST_TAIL_NEGATIVE]         = "LIST_TAIL_NEGATIVE",
    [LIST_TAIL_TOOBIG]           = "LIST_TAIL_TOOBIG",
    [LIST_FREE_NEGATIVE]         = "LIST_FREE_NEGATIVE",
    [LIST_FREE_TOOBIG]           = "LIST_FREE_TOOBIG",
    [LIST_NEXT_NEGATIVE]         = "LIST_NEXT_NEGATIVE",
    [LIST_NEXT_TOOBIG]           = "LIST_NEXT_TOOBIG",
    [LIST_PREV_NEGATIVE]         = "LIST_PREV_NEGATIVE",
    [LIST_PREV_TOOBIG]           = "LIST_PREV_TOOBIG",
    [LIST_FREE_NEXT_NEGATIVE]    = "LIST_FREE_NEXT_NEGATIVE",
    [LIST_FREE_NEXT_TOOBIG]      = "LIST_FREE_NEXT_TOOBIG",
    [LIST_FREE_PREV_NOT_NULL]    = "LIST_FREE_PREV_NOT_NULL",
    [LIST_POSITION_NEGATIVE]     = "LIST_POSITION_NEGATIVE",
    [LIST_POSITION_TOO_BIG]      = "LIST_POSITION_TOO_BIG",
    [LIST_NO_SUCH_ELEMENT]       = "LIST_NO_SUCH_ELEMENT",
    [LIST_NEXT_IS_CYCLED]        = "LIST_NEXT_IS_CYCLED",
    [LIST_PREV_IS_CYCLED]        = "LIST_PREV_IS_CYCLED",
    [LIST_FREE_IS_CYCLED]        = "LIST_FREE_IS_CYCLED",
    [LIST_FREE_VALUE_NOT_PZN]    = "LIST_FREE_VALUE_NOT_PZN",
    [LIST_FILLED_VALUE_IS_PZN]   = "LIST_FILLED_VALUE_IS_PZN",
    [LIST_SIZE_EXCEEDS_CAPACITY] = "LIST_SIZE_EXCEEDS_CAPACITY",
    [LIST_SIZE_IS_WRONG]         = "LIST_SIZE_IS_WRONG",
    [LIST_CAP_IS_WRONG]          = "LIST_CAP_IS_WRONG",
    [LIST_NEXT_WRONG]            = "LIST_NEXT_WRONG",
    [LIST_PREV_WRONG]            = "LIST_PREV_WRONG",
    [LIST_FLAG_IS_WRONG]         = "LIST_FLAG_IS_WRONG"
};

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct ListDumpInfo
{
    ListErr_t   error;
    const char* image_name;
    const char* func;
    const char* file;
    int         line;
} ListDumpInfo_t;

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

const size_t LIST_MIN_CAPACITY = 2;
const size_t LIST_MAX_CAPACITY = 1024 * 1024 * 1024;

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_TYPES_H */
