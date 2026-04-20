#include "listCommands.h"

//------------------------------------------------------------------------------------------

static ListErr_t ListInsert(List_t* list,
                            int     pos,
                            elem_t  value,
                            int*    insert_pos);

static ListErr_t ListCheckPos(List_t* list, int pos);

static ListErr_t ListRealloc            (List_t* list);
static ListErr_t ListReallocLinear      (List_t* list, size_t capacity);

//------------------------------------------------------------------------------------------

ListErr_t ListCtor(List_t* list, size_t capacity)
{
    DPRINTF("> Start ListCtor(capacity = %zu)\n", capacity);

    if (list == NULL)
    {
        PRINTERR("LIST_CTX_NULL");
        return LIST_CTX_NULL;
    }
    if (capacity > LIST_MAX_CAPACITY)
    {
        PRINTERR("LIST_CAPACITY_EXCEEDS_MAX");
        return LIST_CAPACITY_EXCEEDS_MAX;
    }

    if (capacity < LIST_MIN_CAPACITY)
    {
        capacity = LIST_MIN_CAPACITY;
    }

    list->data = (Node_t*) calloc(capacity, sizeof(Node_t));

    if (list->data == NULL)
    {
        PRINTERR("LIST_CALLOC_ERROR");
        return LIST_CALLOC_ERROR;
    }

    list->is_sorted = 1;

    list->capacity = capacity;
    list->size     = 0;
    list->free     = 1;

    /* Filling the null element */
    list->data[0].prev  = 0; /* tail */
    list->data[0].next  = 0; /* head */
    
    strncpy(list->data[0].value, LIST_POISON, sizeof(list->data[0].value));

    /* Filling the free list */
    for (int i = 1; i < (int) capacity; i++)
    {
        list->data[i].prev  = -1;
        list->data[i].next  = i + 1;
        
        strncpy(list->data[0].value, LIST_POISON, sizeof(list->data[0].value));
    }

    /* Last free element addresses to null */
    list->data[capacity - 1].next = -1;

    LIST_CALL_DUMP(list, "ctor", "DUMP AFTER CTOR, CAPACITY = %zu", capacity);

    DPRINTF("> End   ListCtor\n\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListPushBack(List_t* list, elem_t value, int* insert_pos)
{
    return ListInsertBefore(list, 0, value, insert_pos);
}

//------------------------------------------------------------------------------------------

static ListErr_t ListCheckPos(List_t* list, int pos)
{
    assert(list != NULL);

    if (pos < 0)
    {
        PRINTERR("List position is negative");
        return LIST_POSITION_NEGATIVE;
    }
    if ((size_t) pos >= list->capacity)
    {
        PRINTERR("List position is too big");
        return LIST_POSITION_TOO_BIG;
    }

    /* Check that pos is in list */
    if (list->data[pos].prev == -1)
    {
        PRINTERR("List doesn't have an element with pos = %d", pos);
        return LIST_NO_SUCH_ELEMENT;
    }

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListInsertAfter(List_t* list, int pos, elem_t value, int* insert_pos)
{
    DPRINTF("> Start ListInsertAfter(pos = %d, value = " SPEC ")\n", pos, value);

    DEBUG_LIST_CHECK(list, "ERROR DUMP AT START OF INSERT AFTER pos = %d", pos);

    ListErr_t error = LIST_SUCCESS;
    if ((error = ListCheckPos(list, pos)) != LIST_SUCCESS)
    {
        return error;
    }

    if ((error = ListInsert(list, pos, value, insert_pos)))
    {
        return error;
    }

    DEBUG_LIST_CHECK(list, "ERROR DUMP AT END OF INSERT AFTER pos = %d", pos);

    LIST_CALL_DUMP(list, "insert", "DUMP AT END OF INSERT_AFTER pos=%d (value=" SPEC ")", pos, value);

    DPRINTF("> End   ListInsertAfter\n\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListInsertBefore(List_t* list, int pos, elem_t value, int* insert_pos)
{
    DPRINTF("> Start ListInsertBefore(pos = %d, value = " SPEC ")\n", pos, value);

    DEBUG_LIST_CHECK(list, "ERROR DUMP AT START OF INSERT BEFORE pos = %d", pos);

    ListErr_t error = LIST_SUCCESS;
    if ((error = ListCheckPos(list, pos)) != LIST_SUCCESS)
    {
        return error;
    }

    /* Insert before position is equivalent to inserting after previous position */
    if ((error = ListInsert(list, list->data[pos].prev, value, insert_pos)))
    {
        return error;
    }

    DEBUG_LIST_CHECK(list, "ERROR DUMP AT END OF INSERT BEFORE pos = %d", pos);

    LIST_CALL_DUMP(list, "insert", "DUMP AT END OF INSERT_BEFORE pos=%d (value=" SPEC ")", pos, value);

    DPRINTF("> End   ListInsertBefore\n\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

static ListErr_t ListInsert(List_t* list,
                            int     pos,
                            elem_t  value,
                            int*    insert_pos)
{
    assert(list != NULL);

    ListErr_t error = LIST_SUCCESS;

    if (list->free == -1)
    {
        if ((error = ListRealloc(list)))
        {
            return error;
        }
    }

    if (pos != list->data[0].prev)
    {
        list->is_sorted = 0;
    }

    int cur_index = list->free;
    int pos_next  = list->data[pos].next;

    /* set new free element */
    list->free = list->data[list->free].next;

    /* connect previous element to current */
    list->data[pos].next      = cur_index;

    /* connect next element to current */
    list->data[pos_next].prev = cur_index;

    /* connect current element to previous and next */
    list->data[cur_index].prev  = pos;
    list->data[cur_index].next  = pos_next;

    strncpy(list->data[cur_index].value, value, sizeof(list->data[cur_index].value));

    if (insert_pos != NULL)
    {
        *insert_pos = cur_index;
    }

    list->size++;

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

static ListErr_t ListRealloc(List_t* list)
{
    DPRINTF("\t> Start ListRealloc()\n");

    DEBUG_LIST_CHECK(list, "ERROR DUMP AT START OF REALLOC with capacity = %zu", list->capacity);

    size_t  capacity = list->capacity * 2 + 1;

    Node_t* new_data = (Node_t*) realloc(list->data, capacity * sizeof(list->data[0]));

    if (new_data == NULL)
    {
        PRINTERR("List data realloc failed");
        return LIST_DATA_REALLOC_ERROR;
    }

    list->data     = new_data;

    list->free = (int) list->capacity;

    list->capacity = capacity;

    /* Filling the free list */
    for (int i = list->free; i < (int) list->capacity; i++)
    {
        list->data[i].prev  = -1;
        list->data[i].next  = i + 1;

        strncpy(list->data[i].value, LIST_POISON, sizeof(list->data[i].value));
    }

    /* Last free element addresses to nothing */
    list->data[list->capacity - 1].next = -1;

    DEBUG_LIST_CHECK(list, "ERROR DUMP AT END OF REALLOC with capacity = %zu", list->capacity);

    DPRINTF("\t> End   ListRealloc\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListReallocDownLinear(List_t* list)
{
    DPRINTF("\t> Start ListReallocDownLinear()\n");

    DEBUG_LIST_CHECK(list, "ERROR DUMP AT START OF LINEAR REALLOC DOWN with capacity = %zu", list->capacity);

    if (list->size >= list->capacity / 2)
    {
        printf("List is not large enough for realloc down\n");
        return LIST_SUCCESS;
    }

    size_t capacity = list->capacity / 2 + 1;

    ListErr_t error = LIST_SUCCESS;
    if ((error = ListReallocLinear(list, capacity)))
    {
        return error;
    }

    DEBUG_LIST_CHECK(list, "ERROR DUMP AT END OF LINEAR REALLOC DOWN with capacity = %zu", list->capacity);

    LIST_CALL_DUMP(list, "realloc_down", "DUMP AFTER REALLOC DOWN LINEAR to capacity = %zu", list->capacity);

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListReallocUpLinear(List_t* list)
{
    DPRINTF("\t> Start ListReallocUpLinear()\n");

    DEBUG_LIST_CHECK(list, "ERROR DUMP AT START OF LINEAR REALLOC UP with capacity = %zu", list->capacity);

    size_t capacity = list->capacity * 2 + 1;

    ListErr_t error = LIST_SUCCESS;
    if ((error = ListReallocLinear(list, capacity)))
    {
        return error;
    }

    DEBUG_LIST_CHECK(list, "ERROR DUMP AT END OF LINEAR REALLOC UP with capacity = %zu", list->capacity);

    LIST_CALL_DUMP(list, "realloc_up", "DUMP AFTER REALLOC UP LINEAR to capacity = %zu", list->capacity);

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

static ListErr_t ListReallocLinear(List_t* list, size_t capacity)
{
    DPRINTF("\t> Start ListReallocLinear()\n");

    Node_t* new_data = (Node_t*) calloc(capacity, sizeof(list->data[0]));

    if (new_data == NULL)
    {
        PRINTERR("List data realloc failed");
        return LIST_DATA_REALLOC_ERROR;
    }

    new_data[0].next = 1;
    new_data[0].prev = (int) list->size;

    int ind = 1;

    for (int pos = list->data[0].next; pos > 0; pos = list->data[pos].next)
    {
        DPRINTF("ind = %d;\n", ind);
        new_data[ind].prev  = ind - 1;
        new_data[ind].next  = ind + 1;
        strncpy(new_data[ind].value, list->data[pos].value, sizeof(new_data[ind].value));
        ind++;
    }

    new_data[list->size].next = 0;

    list->free = ind;

    for (; (size_t) ind < capacity; ind++)
    {
        new_data[ind].prev  = -1;
        new_data[ind].next  = ind + 1;

        strncpy(new_data[ind].value, LIST_POISON, sizeof(new_data[ind].value));
    }

    new_data[capacity - 1].next = -1;

    free(list->data);

    list->capacity  = capacity;
    list->data      = new_data;
    list->is_sorted = 1;

    DPRINTF("\t> End   ListReallocLinear\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListEraseElem(List_t* list, int pos)
{
    DPRINTF("> Start ListErase(pos = %d)\n", pos);

    DEBUG_LIST_CHECK(list, "ERROR DUMP AT START OF ERASE pos = %d", pos);

    ListErr_t error = LIST_SUCCESS;
    if ((error = ListCheckPos(list, pos)) != LIST_SUCCESS)
    {
        return error;
    }

    if (pos != list->data[0].prev)
    {
        list->is_sorted = 0;
    }

    int next_ind = list->data[pos].next;
    int prev_ind = list->data[pos].prev;

    /* Connect pos to free and set poisons */
    list->data[pos].prev  = -1;
    list->data[pos].next  = list->free;

    strncpy(list->data[pos].value, LIST_POISON, sizeof(list->data[pos].value));

    list->free = pos;

    DPRINTF(R"(    prev_ind = %d;
    next_ind = %d;
    head     = %d;
    tail     = %d;
    pos      = %d;)",
    prev_ind,
    next_ind,
    list->data[0].next,
    list->data[0].prev,
    pos);

    list->data[prev_ind].next = next_ind;
    list->data[next_ind].prev = prev_ind;

    list->size--;

    DEBUG_LIST_CHECK(list, "ERROR DUMP AT END OF ERASE pos = %d", pos);

    LIST_CALL_DUMP(list, "erase", "DUMP AFTER ERASE OF ELEMENT at pos = %d", pos);

    DPRINTF("> End   ListErase\n\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListGetHead(List_t* list, int* head)
{
    DPRINTF("> Start ListGetHead()\n");

    DEBUG_LIST_CHECK(list, "START_GET_HEAD_", 0);

    *head = list->data[0].next;

    DPRINTF("> End   ListGetHead\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListGetTail(List_t* list, int* tail)
{
    DPRINTF("> Start ListGetTail()\n");

    DEBUG_LIST_CHECK(list, "START_GET_TAIL_", 0);

    *tail = list->data[0].prev;

    DPRINTF("> End   ListGetTail\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListGetSize(List_t* list, size_t* size_dest)
{
    DPRINTF("> Start ListGetSize()\n");

    DEBUG_LIST_CHECK(list, "START_GET_SIZE_", 0);

    *size_dest = list->size;

    DPRINTF("> End   ListGetSize\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListGetValue(List_t* list, int pos, elem_t value)
{
    DPRINTF("> Start ListGetValue()\n");

    DEBUG_LIST_CHECK(list, "START_GET_VALUE_", pos);

    ListErr_t error = LIST_SUCCESS;
    if ((error = ListCheckPos(list, pos)) != LIST_SUCCESS)
    {
        return error;
    }

    strncpy(value, list->data[pos].value, sizeof(list->data[pos].value));

    DPRINTF("> End   ListGetValue\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListDtor(List_t* list)
{
    DPRINTF("> Start ListDtor()\n");

    if (list == NULL)
    {
        PRINTERR("LIST_CTX_NULL");
        return LIST_CTX_NULL;
    }

    free(list->data);

    DPRINTF("> End   ListDtor\n\n");

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

int ListElemsEqual(elem_t elem1, elem_t elem2)
{
    assert(elem1);
    assert(elem2);

    __m512i mm_elem1 = _mm512_loadu_si512((void*) elem1);
    __m512i mm_elem2 = _mm512_loadu_si512((void*) elem2);

    __mmask32 mm_mask = _mm512_cmpeq_epi16_mask(mm_elem1, mm_elem2);

    unsigned int mask = _cvtmask32_u32(mm_mask);

    DPRINTF("comparing %s with %s, mask = %u\n", elem1, elem2, mask);

    if (~mask == 0)
    {
        return 1;
    }

    return 0;
}

//------------------------------------------------------------------//

ListErr_t ListFindElement(List_t* list, elem_t item, int* item_pos)
{
    DPRINTF("> Start ListFindElement() " SPEC "\n", item);

    DEBUG_LIST_CHECK(list, "START_FIND_ELEMENT_", 0);

    ListErr_t error      = LIST_SUCCESS;
    size_t    list_size  = list->size;
    int       node_pos   = 0;

    if ((error = ListGetHead(list, &node_pos)) != LIST_SUCCESS)
    {
        return error;
    }

    for (size_t i = 0; (i < list_size) && (node_pos != 0); i++)
    {
        if (ListElemsEqual(list->data[node_pos].value, item) == 1)
        {
            *item_pos = node_pos;

            return LIST_SUCCESS;
        }

        // else continue
        node_pos = list->data[node_pos].next;
    }

    *item_pos = -1;

    DPRINTF("> End   ListFindElement()" SPEC "\n", item);

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------
