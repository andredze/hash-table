#include "graphCommon.h"

//------------------------------------------------------------------------------------------

int ProcessFreeEdgeCase(int   pos,
                        int   prev,
                        int   next,
                        int   next_limits_cross,
                        FILE* fp)
{
    assert(fp != NULL);

    if (prev != -1)
    {
        return 0;
    }

    if (next_limits_cross)
    {
        MakeWrongNode(pos, next, "next", fp);
        MakeWrongEdge(pos,       "next", fp);

        return 0;
    }

    MakeDefaultEdge(pos, next, "#006400", NULL, NULL, "dashed", NULL, NULL, fp);

    return 1;
}

//------------------------------------------------------------------------------------------

int MakeNode(const char* name,
             const char* label,
             const char* color,
             const char* fillcolor,
             const char* fontcolor,
             const char* shape,
             FILE* fp)
{
    assert(name != NULL);
    assert(fp   != NULL);

    fprintf(fp, "\t%s", name);

    int is_first_arg = 1;

    PrintArg("label",     label,     &is_first_arg, fp);
    PrintArg("color",     color,     &is_first_arg, fp);
    PrintArg("fillcolor", fillcolor, &is_first_arg, fp);
    PrintArg("fontcolor", fontcolor, &is_first_arg, fp);
    PrintArg("shape",     shape,     &is_first_arg, fp);

    if (is_first_arg == 0)
    {
        fprintf(fp, "]");
    }

    fprintf(fp, ";\n");

    return 0;
}

//------------------------------------------------------------------------------------------

int MakeWrongNode(int         pos,
                  int         value,
                  const char* connection,
                  FILE*       fp)
{
    assert(connection != NULL);
    assert(fp         != NULL);

    char name [MAX_NODE_NAME_LEN] = "";
    char label[MAX_LABEL_LEN]     = "";

    sprintf (name,  "wrong_%s_%d", connection, pos);
    sprintf (label, "idx = %d",    value);

    MakeNode(name, label, "#640000", "#FFC0C0", "#640000", "octagon", fp);

    return 0;
}

//------------------------------------------------------------------------------------------

int MakeDefaultEdge(int         index1,
                    int         index2,
                    const char* color,
                    const char* constraint,
                    const char* dir,
                    const char* style,
                    const char* arrowhead,
                    const char* arrowtail,
                    FILE*       fp)
{
    assert(fp != NULL);

    char node1[MAX_NODE_NAME_LEN] = "";
    char node2[MAX_NODE_NAME_LEN] = "";

    sprintf (node1, "node%d", index1);
    sprintf (node2, "node%d", index2);

    MakeEdge(node1, node2, color, constraint, dir, style, arrowhead, arrowtail, fp);

    return 0;
}

//------------------------------------------------------------------------------------------

int MakeWrongEdge(int         pos,
                  const char* connection,
                  FILE*       fp)
{
    assert(connection != NULL);
    assert(fp         != NULL);

    char node1[MAX_NODE_NAME_LEN] = "";
    char node2[MAX_NODE_NAME_LEN] = "";

    sprintf(node1, "node%d",                  pos);
    sprintf(node2, "wrong_%s_%d", connection, pos);

    if (strcmp(connection, "prev"))
    {
        MakeEdge(node1, node2, "#640000", "true", NULL, NULL, "dot", NULL, fp);
    }
    else
    {
        MakeEdge(node1, node2, "#640000", "true", NULL, NULL, NULL, NULL, fp);
    }

    return 0;
}

//------------------------------------------------------------------------------------------

int MakeEdge(const char* node1,
             const char* node2,
             const char* color,
             const char* constraint,
             const char* dir,
             const char* style,
             const char* arrowhead,
             const char* arrowtail,
             FILE*       fp)
{
    assert(node1 != NULL);
    assert(node2 != NULL);

    fprintf(fp, "\t%s->%s", node1, node2);

    int is_first_arg = 1;

    PrintArg("color",      color,      &is_first_arg, fp);
    PrintArg("constraint", constraint, &is_first_arg, fp);
    PrintArg("dir",        dir,        &is_first_arg, fp);
    PrintArg("style",      style,      &is_first_arg, fp);
    PrintArg("arrowhead",  arrowhead,  &is_first_arg, fp);
    PrintArg("arrowtail",  arrowtail,  &is_first_arg, fp);

    if (is_first_arg == 0)
    {
        fprintf(fp, "]");
    }

    fprintf(fp, ";\n");

    return 0;
}

//------------------------------------------------------------------------------------------

int PrintArg(const char* arg_name,
             const char* arg_value,
             int*        is_first_arg,
             FILE*       fp)
{
    assert(arg_name     != NULL);
    assert(is_first_arg != NULL);

    if (arg_value != NULL)
    {
        if (*is_first_arg)
        {
            fprintf(fp, " [");
            *is_first_arg = 0;
        }
        else
        {
            fprintf(fp, ", ");
        }

        fprintf(fp, "%s = \"%s\"", arg_name, arg_value);
    }

    return 0;
}

//------------------------------------------------------------------------------------------
