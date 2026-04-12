#ifndef LIST_DEBUG_H
#define LIST_DEBUG_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "listTypes.h"
#include "listGraph.h"
#include <time.h>
#include <sys/stat.h>
#include <stdarg.h>

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG

//==========================================================================================

#define LIST_CALL_DUMP(list_ptr, name, fmt, ...)                                                  \
        BEGIN                                                                                     \
        ListDumpInfo_t dump_info = {LIST_SUCCESS, name, __PRETTY_FUNCTION__, __FILE__, __LINE__}; \
        if (ListDump((list_ptr), &dump_info, (fmt), ##__VA_ARGS__))                               \
        {                                                                                         \
            return LIST_DUMP_ERROR;                                                               \
        }                                                                                         \
        END

#define DEBUG_LIST_CHECK(list, fmt, ...)                \
        BEGIN                                           \
        ListErr_t ret = LIST_SUCCESS;                   \
        if ((ret = ListCheck((list),                    \
                             __PRETTY_FUNCTION__,       \
                             __FILE__,                  \
                             __LINE__,                  \
                             (fmt), ##__VA_ARGS__)))    \
        {                                               \
            return ret;                                 \
        }                                               \
        END

//==========================================================================================

#else

#define LIST_CALL_DUMP(list_ptr, name, fmt, ...)    ;
#define DEBUG_LIST_CHECK(list, fmt, ...)            ;

//==========================================================================================

#endif /* LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG

int LinearSearch(int* array, size_t size, int elem);

int SetDirectories(char* log_filename, char* image_dir, char* dot_dir);

ListErr_t ListCheck(List_t*     list,
                    const char* func,
                    const char* file,
                    int         line,
                    const char* fmt, ...);

ListErr_t ListVerify          (List_t* list);
ListErr_t ListVerifyNext      (List_t* list, size_t* next_count_ptr);
ListErr_t ListVerifyPrev      (List_t* list, size_t* prev_count_ptr);
ListErr_t ListVerifyFree      (List_t* list, size_t* free_count_ptr);

//——————————————————————————————————————————————————————————————————————————————————————————

ListErr_t vListDump(
    List_t*         list,
    ListDumpInfo_t* dump_info,
    const char*     fmt,
    va_list         args);

ListErr_t ListDump(
    List_t*         list,
    ListDumpInfo_t* dump_info,
    const char*     fmt, ...);

int       ListDumpStruct      (List_t* list, ListDumpInfo_t* dump_info, FILE* fp);
int       ListDumpData        (List_t* list, ListDumpInfo_t* dump_info, FILE* fp);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_DEBUG_H */
