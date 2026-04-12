#include "listGraph.h"

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG

//==========================================================================================

ListErr_t ListCreateDumpGraph(List_t* list,
                              const char* image_name,
                              const char* dot_dir)
{
    assert(list != NULL);

    if (image_name == NULL)
    {
        PRINTERR("Given image name is null\n");
        return LIST_IMAGE_NAME_NULL;
    }
    if (strlen(image_name) > MAX_FILENAME_LEN / 2)
    {
        PRINTERR("Too big filename for graph image");
        return LIST_FILENAME_TOOBIG;
    }

    char filename[MAX_FILENAME_LEN] = {};

    sprintf(filename, "%s/%s.dot", dot_dir, image_name);
    
    FILE* fp = fopen(filename, "w");

    if (fp == NULL)
    {
        PRINTERR("Opening graph logfile failed");
        // fix double free here
        return LIST_LOGFILE_OPEN_ERROR;
    }

    fprintf(fp, "digraph GG\n{\n\t"
    R"(graph [splines=ortho];
    ranksep=0.75;
    nodesep=0.5;
    node [
        fontname  = "Arial",
        shape     = "Mrecord",
        style     = "filled",
        color     = "#3E3A22",
        fillcolor = "#E3DFC9",
        fontcolor = "#3E3A22"
    ];
    edge [constraint=false];)""\n");

    MakeListNodes(list, fp);

    /* make all edges */
    for (int pos = 1; pos < (int) list->capacity; pos++)
    {
        MakeListEdge(pos, list, fp);
    }

    MakeHeadTailFree(list, fp);

    fprintf(fp, "}\n");

    fclose(fp);

    return LIST_SUCCESS;
}

//------------------------------------------------------------------------------------------

int MakeListNodes(List_t* list,
                  FILE*   fp)
{
    assert(list != NULL);
    assert(fp   != NULL);

    /* place all nodes in one rank */
    fprintf(fp, "\t{ rank=same; ");

    for (size_t ind = 0; ind < list->capacity; ind++)
    {
        fprintf(fp, "node%zu; ", ind);
    }

    fprintf(fp, "}\n");

    /* Create invisible edges for all nodes */
    for (int i = 0; i < (int) list->capacity - 1; i++)
    {
        MakeDefaultEdge(i, i + 1, NULL, NULL, NULL, "invis", NULL, NULL, fp);
    }

    MakeDefaultNode(0, "#3E3A22", "#ecede8", "#3E3A22", "record", list, fp);

    for (int i = 1; (size_t) i < list->capacity; i++)
    {
        /* if free */
        if (list->data[i].prev == -1)
        {
            MakeDefaultNode(i, "#006400", "#C0FFC0", "#005300", NULL, list, fp);
        }
        else
        {
            MakeDefaultNode(i, NULL, NULL, NULL, NULL, list, fp);
        }
    }

    return 0;
}

//------------------------------------------------------------------------------------------

int MakeDefaultNode(int index,
                    const char* color,
                    const char* fillcolor,
                    const char* fontcolor,
                    const char* shape,
                    List_t* list,
                    FILE* fp)
{
    assert(fp        != NULL);
    assert(list      != NULL);

    char name[MAX_NODE_NAME_LEN] = "";

    sprintf(name, "node%d", index);

    char label[MAX_LABEL_LEN] = "";
    int  current_pos = 0;

    current_pos += sprintf(label, "{ idx = %d | value = ", index);

    if (list->data[index].value == LIST_POISON)
    {
        current_pos += sprintf(current_pos + label, "PZN");
    }
    else
    {
        current_pos += sprintf(current_pos + label, SPEC, list->data[index].value);
    }

    sprintf(current_pos + label, " | { prev = %d | next = %d }}",
                                 list->data[index].prev,
                                 list->data[index].next);

    MakeNode(name, label, color, fillcolor, fontcolor, shape, fp);

    return 0;
}

//------------------------------------------------------------------------------------------

int MakeListEdge(int     pos,
                 List_t* list,
                 FILE*   fp)
{
    assert(list != NULL);
    assert(fp   != NULL);

    int next = list->data[pos].next;
    int prev = list->data[pos].prev;

    if (prev == -1 && (next == 0 || next == -1))
    {
        return 0;
    }

    int next_limits_cross = (((size_t) next < list->capacity && next >= 0) == 0);
    int prev_limits_cross = (((size_t) prev < list->capacity && prev >= 0) == 0);

    if (ProcessFreeEdgeCase(pos, prev, next, next_limits_cross, fp) == 1)
    {
        return 0;
    }

    if (!(prev_limits_cross) && list->data[prev].next != pos)
    {
        if (list->data[list->data[prev].next].prev != prev)
        {
            MakeDefaultNode(prev, "#530000", "#FFC0C0", "#400000", NULL, list, fp);
        }
        MakeDefaultEdge(pos, prev, "#640000", NULL, NULL, "dashed", "dot", NULL, fp);
    }

    if (ProcessUncrossedLimitsEdge(pos, next,
                                   prev_limits_cross,
                                   next_limits_cross,
                                   list, fp) == 1)
    {
        return 0;
    }

    if (next_limits_cross)
    {
        MakeWrongNode(pos, next, "next", fp);
        MakeWrongEdge(pos,       "next", fp);
    }
    else
    {
        if (next != 0)
        {
            MakeDefaultEdge(pos, next, "#000064", NULL, NULL, NULL, NULL, NULL, fp);
        }
    }
    if (prev_limits_cross)
    {
        MakeWrongNode(pos, prev, "prev", fp);
        MakeWrongEdge(pos,       "prev", fp);
    }

    return 0;
}

//------------------------------------------------------------------------------------------

int ProcessUncrossedLimitsEdge(int     pos,
                               int     next,
                               int     prev_limits_cross,
                               int     next_limits_cross,
                               List_t* list,
                               FILE*   fp)
{
    assert(list != NULL);
    assert(fp   != NULL);

    if (next_limits_cross || prev_limits_cross)
    {
        return 0;
    }

    if (next == 0)
    {
        return 1;
    }

    if (list->data[next].prev == pos)
    {
        // NOTE: next and prev with different arrow heads
        MakeDefaultEdge(pos, next, "#000064", NULL, "both", NULL, "normal", "normal", fp);

        return 1;
    }

    DPRINTF("next.prev = %d\n", list->data[next].prev);

    if (list->data[next].prev < (int) list->capacity &&
        list->data[next].prev > 0                    &&
        list->data[list->data[next].prev].next != next)
    {
        MakeDefaultNode(next, "#530000", "#FFC0C0", "#400000", NULL, list, fp);
    }

    MakeDefaultEdge(pos, next, "#640000", NULL, NULL, "dashed", NULL, NULL, fp);

    return 1;
}

//------------------------------------------------------------------------------------------

int MakeHeadTailFree(List_t* list, FILE* fp)
{
    assert(list != NULL);
    assert(fp   != NULL);

    fprintf(fp,
    R"(    node [
        shape = "box",
        color = "#70421A",
        fontcolor = "#70421A",
        fillcolor = "#DEB887"
    ];
    edge [
        color = "#70421A",
        arrowhead=none,
        constraint=true
    ];
    tail; head; free;)""\n");

    /* Make edges to the elements */
    if (list->data[0].prev >= 0)
    {
        char name[MAX_NODE_NAME_LEN] = "";
        sprintf (name, "node%d", list->data[0].prev);
        MakeEdge("tail", name, NULL, NULL, NULL, NULL, NULL, NULL, fp);
    }
    if (list->data[0].next >= 0)
    {
        char name[MAX_NODE_NAME_LEN] = "";
        sprintf (name, "node%d", list->data[0].next);
        MakeEdge("head", name, NULL, NULL, NULL, NULL, NULL, NULL, fp);
    }
    if (list->free >= 0)
    {
        char name[MAX_NODE_NAME_LEN] = "";
        sprintf (name, "node%d", list->free);
        MakeEdge("free", name, NULL, NULL, NULL, NULL, NULL, NULL, fp);
    }

    /* Place on one rank */
    fprintf(fp, "\t{ rank=same; tail; head; free; }\n");

    return 0;
}

//==========================================================================================

#endif /* LIST_DEBUG */

//------------------------------------------------------------------------------------------
