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
ListErr_t ListFindElement  (List_t* list, elem_t item, int* item_pos);
ListErr_t ListGetHead      (List_t* list,           int*    head);
ListErr_t ListGetTail      (List_t* list,           int*    tail);
ListErr_t ListGetValue     (List_t* list, int  pos, elem_t* value);
ListErr_t ListGetSize      (List_t* list, size_t* size_dest);

ListErr_t ListReallocDownLinear (List_t* list);
ListErr_t ListReallocUpLinear   (List_t* list);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_COMMANDS_H */
