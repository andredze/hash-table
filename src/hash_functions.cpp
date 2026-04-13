#include "hash_functions.h"

//------------------------------------------------------------------//

uint32_t CountHashAlwaysZero(char* const string)
{
    assert(string);

    return 0;
}

//------------------------------------------------------------------//

uint32_t CountHashFirstLetter(char* const string)
{
    assert(string);

    return *string;
}

//------------------------------------------------------------------//

uint32_t CountHashLength(char* const string)
{
    assert(string);

    return strlen(string);
}

//------------------------------------------------------------------//

uint32_t CountHashChecksum(char* const string)
{
    assert(string);

    uint32_t hash = 0;

    for (size_t i = 0; (string[i] != '\0') && (i < MAX_ITERATIONS); i++)
    {
        hash += string[i];
    }

    return hash;
}

//------------------------------------------------------------------//

uint32_t CountHashRollingLeft(char* const string)
{
    assert(string);

    uint32_t hash = *string;
    
    for (size_t i = 0; (string[i + 1] != '\0') && (i < MAX_ITERATIONS); i++)
    {
        hash = (hash << ROL_BYTES) | (hash >> (sizeof(uint32_t) * 8 - ROL_BYTES)); // == rol hash, 3

        hash = hash ^ string[i + 1];
    }

    return hash;
}

//------------------------------------------------------------------//

uint32_t CountHashRollingRight(char* const string)
{
    assert(string);

    uint32_t hash = *string;
    
    for (size_t i = 0; (string[i + 1] != '\0') && (i < MAX_ITERATIONS); i++)
    {
        hash = (hash >> ROL_BYTES) | (hash << (sizeof(uint32_t) * 8 - ROL_BYTES)); // == ror hash, 3

        hash = hash ^ string[i + 1];
    }

    return hash;
}

//------------------------------------------------------------------//

uint32_t CountHashDjb2(char* const string)
{
    assert(string);

    uint32_t hash = 5381;

    for (size_t i = 0; (*string != '\0') && (i < MAX_ITERATIONS); i++)
    {
        hash = ((hash << 5) + hash) + *string;
    }

    return hash;
}

//------------------------------------------------------------------//

uint32_t CountHashCrc32(char* const string)
{
    assert(string);

    uint32_t hash = 0xffffffff;

    for (size_t i = 0; (*string != '\0') && (i < MAX_ITERATIONS); i++)
    {
        hash = POLYNOM_8_LOOKUP_TABLE[string[i] ^ hash & 0xff] ^ (hash >> 8);
        i++;
    }

    return hash ^ 0xffffffff;
}

//------------------------------------------------------------------//
