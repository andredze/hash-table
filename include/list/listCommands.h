#ifndef LIST_COMMANDS_H
#define LIST_COMMANDS_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "listTypes.h"
#include "listDebug.h"
#include <x86intrin.h>

//——————————————————————————————————————————————————————————————————————————————————————————

typedef enum ListInsertType
{
    LIST_INSERT_AFTER,
    LIST_INSERT_BEFORE
} ListInsertType_t;

//——————————————————————————————————————————————————————————————————————————————————————————

ListErr_t ListCtor         (List_t* list, size_t capacity);
ListErr_t ListDtor         (List_t* list);
ListErr_t ListEraseElem    (List_t* list, int  pos);
ListErr_t ListPushBack     (List_t* list,           elem_t  value, int* insert_pos);
ListErr_t ListInsertAfter  (List_t* list, int  pos, elem_t  value, int* insert_pos);
ListErr_t ListInsertBefore (List_t* list, int  pos, elem_t  value, int* insert_pos);

#if defined(NOAVX)
    int ListElemsEqual(elem_t elem1, elem_t elem2);
#else
    int ListElemsEqual(__m256i mm_elem1, elem_t elem2);
#endif

inline ListErr_t ListFindElement(List_t* list, elem_t item, int* item_pos)__attribute__((always_inline));

//------------------------------------------------------------------//

inline ListErr_t ListFindElement(List_t* list, elem_t item, int* item_pos)
{
    DPRINTF("> Start ListFindElement() " SPEC "\n", item);

    DEBUG_LIST_CHECK(list, "START_FIND_ELEMENT_", 0);

    int    node_pos  = list->data[0].next;
    size_t list_size = list->size;

    #ifndef NOAVX

        __m256i mm_item = _mm256_load_si256((__m256i*) item);

    #endif /* NOAVX */

    for (size_t i = 0; i < list_size; i++)
    {
        //------------------------------------------------------------------//
        #ifdef NOAVX

            bool elems_equal = ListElemsEqual(item, list->data[node_pos].value);

        #else

            bool elems_equal = ListElemsEqual(mm_item, list->data[node_pos].value);

        #endif /* NOAVX */
        //------------------------------------------------------------------//

        if (elems_equal)
        {
            *item_pos = node_pos;

            return LIST_SUCCESS;
        }

        // else: continue
        node_pos = list->data[node_pos].next;
        //------------------------------------------------------------------//
    }

    // item was not found
    *item_pos = -1;

    DPRINTF("> End   ListFindElement()" SPEC "\n", item);

    return LIST_SUCCESS;
}

//------------------------------------------------------------------//

ListErr_t ListGetHead      (List_t* list,           int*    head);
ListErr_t ListGetTail      (List_t* list,           int*    tail);
ListErr_t ListGetValue     (List_t* list, int  pos, elem_t* value);
ListErr_t ListGetSize      (List_t* list, size_t* size_dest);

ListErr_t ListReallocDownLinear (List_t* list);
ListErr_t ListReallocUpLinear   (List_t* list);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_COMMANDS_H */
