#include "listDebug.h"

#ifdef LIST_DEBUG

//==========================================================================================

ListErr_t ListCheck(List_t*     list,
                    const char* func,
                    const char* file,
                    int         line,
                    const char* fmt,
                    ...)
{
    ListErr_t verify_status = LIST_SUCCESS;

    if ((verify_status = ListVerify(list)))
    {
        PRINTERR("%s (ListVerify not passed! Check \"list_log.html\")", LIST_STR_ERRORS[verify_status]);

        ListDumpInfo_t check_dump_info = {verify_status, "error_dump", func, file, line};

        va_list args = {};
        va_start(args, fmt);

        if (vListDump(list, &check_dump_info, fmt, args))
        {
            return LIST_DUMP_ERROR;
        }

        va_end(args);
    }

    return verify_status;
}

//------------------------------------------------------------------------------------------

ListErr_t ListVerify(List_t* list)
{
    if (list == NULL)
    {
        return LIST_CTX_NULL;
    }
    if (list->capacity > LIST_MAX_CAPACITY)
    {
        return LIST_CAPACITY_EXCEEDS_MAX;
    }
    if (list->size >= list->capacity)
    {
        return LIST_SIZE_EXCEEDS_CAPACITY;
    }
    if (list->data == NULL)
    {
        return LIST_DATA_NULL;
    }
    if (list->data[0].next < 0)
    {
        return LIST_HEAD_NEGATIVE;
    }
    if ((size_t) list->data[0].next >= list->capacity)
    {
        return LIST_HEAD_TOOBIG;
    }
    if (list->data[0].prev < 0)
    {
        return LIST_TAIL_NEGATIVE;
    }
    if ((size_t) list->data[0].prev >= list->capacity)
    {
        return LIST_TAIL_TOOBIG;
    }
    if (list->free < -1)
    {
        return LIST_FREE_NEGATIVE;
    }
    if (list->free != -1 && (size_t) list->free >= list->capacity)
    {
        return LIST_FREE_TOOBIG;
    }
    if (list->is_sorted != 0 && list->is_sorted != 1)
    {
        return LIST_FLAG_IS_WRONG;
    }

    size_t next_count = 0;
    size_t prev_count = 0;
    size_t free_count = 0;

    ListErr_t error = LIST_SUCCESS;

    if ((error = ListVerifyNext(list, &next_count)) != LIST_SUCCESS)
    {
        return error;
    }
    if ((error = ListVerifyPrev(list, &prev_count)) != LIST_SUCCESS)
    {
        return error;
    }
    if ((error = ListVerifyFree(list, &free_count)) != LIST_SUCCESS)
    {
        return error;
    }

    if (next_count != list->size || prev_count != list->size)
    {
        return LIST_SIZE_IS_WRONG;
    }
    if (next_count + free_count + 1 != list->capacity)
    {
        return LIST_CAP_IS_WRONG;
    }

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

int LinearSearch(int* array, size_t size, int elem)
{
    assert(array != NULL);

    for (int i = 0; i < (int) size; i++)
    {
        if (array[i] == elem)
        {
            return i;
        }
    }

    return -1;
}

//------------------------------------------------------------------------------------------

ListErr_t ListVerifyNext(List_t* list, size_t* next_count_ptr)
{
    assert(list       != NULL);
    assert(list->data != NULL);

    int*   next_nodes = (int*) calloc(list->capacity, sizeof(list->data[0].next));
    size_t next_count = 0;

    for (int i = list->data[0].next; i != 0; i = list->data[i].next)
    {
        if (list->data[i].value == LIST_POISON)
        {
            free(next_nodes);
            return LIST_FILLED_VALUE_IS_PZN;
        }
        if (LinearSearch(next_nodes, list->capacity, i) != -1)
        {
            free(next_nodes);
            return LIST_NEXT_IS_CYCLED;
        }
        if (list->data[i].next < 0)
        {
            free(next_nodes);
            return LIST_NEXT_NEGATIVE;
        }
        if ((size_t) list->data[i].next >= list->capacity)
        {
            free(next_nodes);
            return LIST_NEXT_TOOBIG;
        }
        if (list->data[list->data[i].next].prev != i)
        {
            free(next_nodes);
            return LIST_PREV_WRONG;
        }

        next_nodes[next_count++] = i;
    }

    free(next_nodes);

    *next_count_ptr = next_count;

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListVerifyPrev(List_t* list, size_t* prev_count_ptr)
{
    assert(list       != NULL);
    assert(list->data != NULL);

    int*   prev_nodes = (int*) calloc(list->capacity, sizeof(list->data[0].prev));
    size_t prev_count = 0;

    for (int i = list->data[0].prev; i != 0; i = list->data[i].prev)
    {
        if (LinearSearch(prev_nodes, list->capacity, i) != -1)
        {
            free(prev_nodes);
            return LIST_PREV_IS_CYCLED;
        }
        if (list->data[i].prev < 0)
        {
            free(prev_nodes);
            return LIST_PREV_NEGATIVE;
        }
        if ((size_t) list->data[i].prev >= list->capacity)
        {
            free(prev_nodes);
            return LIST_PREV_TOOBIG;
        }
        if (list->data[list->data[i].prev].next != i)
        {
            free(prev_nodes);
            return LIST_NEXT_WRONG;
        }

        prev_nodes[prev_count++] = i;
    }

    free(prev_nodes);

    *prev_count_ptr = prev_count;

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListVerifyFree(List_t* list, size_t* free_count_ptr)
{
    assert(list       != NULL);
    assert(list->data != NULL);

    int*   free_nodes = (int*) calloc(list->capacity, sizeof(list->data[0].next));
    size_t free_count = 0;

    for (int i = list->free; i > 0; i = list->data[i].next)
    {
        if (list->data[i].value != LIST_POISON)
        {
            free(free_nodes);
            return LIST_FREE_VALUE_NOT_PZN;
        }
        if (LinearSearch(free_nodes, list->capacity, i) != -1)
        {
            free(free_nodes);
            return LIST_FREE_IS_CYCLED;
        }
        if (list->data[i].next < -1)
        {
            free(free_nodes);
            return LIST_FREE_NEXT_NEGATIVE;
        }
        if (list->data[i].next != -1 && (size_t) list->data[i].next >= list->capacity)
        {
            free(free_nodes);
            return LIST_FREE_NEXT_TOOBIG;
        }
        if (list->data[i].prev != -1)
        {
            free(free_nodes);
            return LIST_FREE_PREV_NOT_NULL;
        }

        free_nodes[free_count++] = i;
    }

    free(free_nodes);

    *free_count_ptr = free_count;

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t vListDump(List_t*         list,
                    ListDumpInfo_t* dump_info,
                    const char*     fmt,
                    va_list         args)
{
    static int calls_count = 1;

    static char log_filename [MAX_FILENAME_LEN] = "";
    static char image_dir    [MAX_FILENAME_LEN] = "";
    static char dot_dir      [MAX_FILENAME_LEN] = "";

    if (calls_count == 1)
    {
        SetDirectories(log_filename, image_dir, dot_dir);
    }

    char image_name[MAX_FILENAME_LEN] = {};
    sprintf(image_name, "%04d_%s", calls_count, dump_info->image_name);

    FILE* fp = fopen(log_filename, calls_count == 1 ? "w" : "a");

    if (fp == NULL)
    {
        PRINTERR("Opening logfile %s failed", log_filename);
        return LIST_LOGFILE_OPEN_ERROR;
    }

    calls_count++;

//     fprintf(fp, "<pre>\n<h3><font color=blue>%s%d</font></h3>",
//                 dump_info->reason,
//                 dump_info->command_arg);

    fprintf(fp, "<pre>\n<h3><font color=blue>");

    vfprintf(fp, fmt, args);

    fprintf(fp, "</font></h3>");

    fprintf(fp, dump_info->error == LIST_SUCCESS ?
                "<font color=green><b>" :
                "<font color=red><b>ERROR: ");

    fprintf(fp, "%s (code %d)</b></font>\n"
                "LIST DUMP called from %s at %s:%d\n\n",
                LIST_STR_ERRORS[dump_info->error],
                dump_info->error,
                dump_info->func,
                dump_info->file,
                dump_info->line);

    if (ListDumpStruct(list, dump_info, fp))
    {
        fclose(fp);
        return LIST_SUCCESS;
    }

    if (dump_info->error == LIST_CAPACITY_EXCEEDS_MAX ||
        dump_info->error == LIST_HEAD_NEGATIVE        ||
        dump_info->error == LIST_HEAD_TOOBIG)
    {
        fclose(fp);
        return LIST_SUCCESS;
    }

    ListErr_t graph_error = LIST_SUCCESS;
    if ((graph_error = ListCreateDumpGraph(list, image_name, dot_dir)))
    {
        fclose(fp);
        return graph_error;
    }

    fprintf(fp, "\n<img src = svg/%s.svg width = 100%%>\n\n"
                "============================================================="
                "=============================================================\n\n",
                image_name);

    fclose(fp);

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

ListErr_t ListDump (List_t*         list,
                    ListDumpInfo_t* dump_info,
                    const char*     fmt, ...)
{
    va_list args = {};
    va_start(args, fmt);

    ListErr_t status = vListDump(list, dump_info, fmt, args);

    va_end(args);

    return status;
}

//------------------------------------------------------------------------------------------

int SetDirectories(char* log_filename, char* image_dir, char* dot_dir)
{
    assert(log_filename != NULL);
    assert(image_dir    != NULL);
    assert(dot_dir      != NULL);

    time_t rawtime = time(NULL);

    struct tm* info = localtime(&rawtime);

    char time_dir[MAX_FILENAME_LEN / 2] = "";
    char dir[MAX_FILENAME_LEN] = "";

    strftime(time_dir, sizeof(time_dir), "%d%m%Y_%H%M%S", info);

    snprintf(dir, 100, "log/%s", time_dir);
    mkdir(dir);

    sprintf(image_dir, "log/%s/svg", time_dir);

    DPRINTF("image_dir = %s;\n", image_dir);
    mkdir(image_dir);

    sprintf(dot_dir, "log/%s/dot", time_dir);

    DPRINTF("dot_dir   = %s;\n", dot_dir);
    mkdir(dot_dir);

    sprintf(log_filename, "log/%s/list_log.html", time_dir);

    return 0;
}

//------------------------------------------------------------------------------------------

int ListDumpStruct(List_t* list, ListDumpInfo_t* dump_info, FILE* fp)
{
    assert(dump_info != NULL);
    assert(fp        != NULL);

    fprintf(fp, "list [%p]:\n\n"
                "—————————————————————————————————————————————————————————————"
                "—————————————————————————————————————————————————————————————\n\n",
                list);

    if (dump_info->error == LIST_CTX_NULL)
    {
        return 0;
    }

    fprintf(fp, "is_sorted = %d;\n"
                "capacity  = %zu;\n"
                "size = %zu;\n"
                "free = %d;\n",
                list->is_sorted,
                list->capacity,
                list->size,
                list->free);

    if (dump_info->error == LIST_DATA_NULL)
    {
        return 0;
    }

    fprintf(fp, "head = %d;\n"
                "tail = %d;\n",
                list->data[0].next,
                list->data[0].prev);

    ListDumpData(list, dump_info, fp);

    fprintf(fp, "—————————————————————————————————————————————————————————————"
                "—————————————————————————————————————————————————————————————\n\n");

    return 0;
}

//------------------------------------------------------------------------------------------

int ListDumpData(List_t* list, ListDumpInfo_t* dump_info, FILE* fp)
{
    assert(list      != NULL);
    assert(dump_info != NULL);
    assert(fp        != NULL);

    fprintf(fp, "data [%p]:\n[\n", list->data);

    if (dump_info->error == LIST_CAPACITY_EXCEEDS_MAX)
    {
        return 0;
    }

    fprintf(fp, "\tindex  ");

    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(fp, "%4zu ", i);
    }

    fprintf(fp, "\n\tvalue [");

    for (size_t i = 0; i < list->capacity; i++)
    {
        if (list->data[i].value == LIST_POISON)
        {
            fprintf(fp, " PZN ");
        }
        else
        {
            fprintf(fp, "%4d ", list->data[i].value);
        }
    }

    fprintf(fp, "];\n\tnext  [");

    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(fp, "%4d ", list->data[i].next);
    }

    fprintf(fp, "];\n\tprev  [");

    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(fp, "%4d ", list->data[i].prev);
    }

    fprintf(fp, "];\n];\n");

    return 0;
}

//==========================================================================================

#endif /* LIST_DEBUG */

//------------------------------------------------------------------------------------------
