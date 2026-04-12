#include "hash_functions.h"

//------------------------------------------------------------------//

size_t CountHashDjb2(char* const string)
{
    assert(string);

    size_t hash = 5381;

    for (size_t i = 0; (*string != '\0') && (i < MAX_ITERATIONS); i++)
    {
        hash = ((hash << 5) + hash) + *string;
    }

    return hash;

}

//------------------------------------------------------------------//

size_t CountHashChecksum(char* const string)
{
    assert(string);

    size_t hash = 0;

    for (size_t i = 0; (*string != '\0') && (i < MAX_ITERATIONS); i++)
    {
        hash += *string;
    }

    return hash;

}

//------------------------------------------------------------------//