#ifndef LIST_GRAPH_H
#define LIST_GRAPH_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "listDebug.h"
#include "listTypes.h"
#include "graphCommon.h"

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef LIST_DEBUG

//——————————————————————————————————————————————————————————————————————————————————————————

ListErr_t ListCreateDumpGraph(
    List_t* list,
    const char* image_name,
    const char* dot_dir);

int MakeListNodes(
    List_t* list,
    FILE*   fp);

int MakeListEdge(
    int     pos,
    List_t* list,
    FILE*   fp);

int MakeHeadTailFree(
    List_t*     list,
    FILE*       fp);

//——————————————————————————————————————————————————————————————————————————————————————————

int ProcessUncrossedLimitsEdge(
    int     pos,
    int     next,
    int     prev_limits_cross,
    int     next_limits_cross,
    List_t* list,
    FILE*   fp);

//——————————————————————————————————————————————————————————————————————————————————————————

int MakeDefaultNode(
    int         index,
    const char* color,
    const char* fillcolor,
    const char* fontcolor,
    const char* shape,
    List_t*     list,
    FILE*       fp);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* LIST_GRAPH_H */
