#ifndef GRAPH_COMMON_H
#define GRAPH_COMMON_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <assert.h>
#include <string.h>

//——————————————————————————————————————————————————————————————————————————————————————————

int ProcessFreeEdgeCase(int   pos,
                        int   prev,
                        int   next,
                        int   next_limits_cross,
                        FILE* fp);

//——————————————————————————————————————————————————————————————————————————————————————————

int MakeNode(const char* name,
             const char* label,
             const char* color,
             const char* fillcolor,
             const char* fontcolor,
             const char* shape,
             FILE* fp);

int MakeWrongNode(int         pos,
                  int         value,
                  const char* connection,
                  FILE*       fp);

//——————————————————————————————————————————————————————————————————————————————————————————

int MakeDefaultEdge(int         index1,
                    int         index2,
                    const char* color,
                    const char* constraint,
                    const char* dir,
                    const char* style,
                    const char* arrowhead,
                    const char* arrowtail,
                    FILE*       fp);

int MakeWrongEdge(int         pos,
                  const char* connection,
                  FILE*       fp);

int MakeEdge(const char* node1,
             const char* node2,
             const char* color,
             const char* constraint,
             const char* dir,
             const char* style,
             const char* arrowhead,
             const char* arrowtail,
             FILE*       fp);

//——————————————————————————————————————————————————————————————————————————————————————————

int PrintArg(const char* arg_name,
             const char* arg_value,
             int*        is_first_arg,
             FILE*       fp);

//——————————————————————————————————————————————————————————————————————————————————————————

const int MAX_NODE_NAME_LEN = 32;
const int MAX_LABEL_LEN     = 256;

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* GRAPH_COMMON_H */
