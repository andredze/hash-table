#include "hash_functions.h"
#include "my_printfs.h"

//------------------------------------------------------------------//

uint64_t CountHashAlwaysOne(char* const string)
{
    assert(string);

    return 1;
}

//------------------------------------------------------------------//

uint64_t CountHashFirstLetter(char* const string)
{
    assert(string);

    return *string;
}

//------------------------------------------------------------------//

uint64_t CountHashLength(char* const string)
{
    assert(string);

    return strlen(string);
}

//------------------------------------------------------------------//

uint64_t CountHashChecksum(char* const string)
{
    assert(string);

    uint64_t hash = 0;

    for (size_t i = 0; (string[i] != '\0') && (i < MAX_ITERATIONS); i++)
    {
        hash += string[i];
    }

    return hash;
}

//------------------------------------------------------------------//

uint64_t CountHashRotateLeft(char* const string)
{
    assert(string);

    uint64_t hash = *string;
    
    for (size_t i = 0; (string[i + 1] != '\0') && (i < MAX_ITERATIONS); i++)
    {
        hash = (hash << ROL_BYTES) | (hash >> (sizeof(uint64_t) * 8 - ROL_BYTES)); // == rol hash, 3

        hash = hash ^ string[i + 1];
    }

    return hash;
}

//------------------------------------------------------------------//

uint64_t CountHashRotateRight(char* const string)
{
    assert(string);

    uint64_t hash = *string;
    
    for (size_t i = 0; (string[i + 1] != '\0') && (i < MAX_ITERATIONS); i++)
    {
        hash = (hash >> ROL_BYTES) | (hash << (sizeof(uint64_t) * 8 - ROL_BYTES)); // == ror hash, 3

        hash = hash ^ string[i + 1];
    }

    return hash;
}

//------------------------------------------------------------------//

uint64_t CountHashDjb2(char* const string)
{
    assert(string);

    uint64_t hash = 5381;

    for (size_t i = 0; (string[i] != '\0') && (i < MAX_ITERATIONS); i++)
    {
        hash = ((hash << 5) + hash) + string[i];
    }

    return hash;
}

//------------------------------------------------------------------//

uint64_t CountHashCrc32(char* const string)
{
    assert(string);

    uint64_t hash = 0xffffffff;
    DPRINTF("string = %s\n", string);

    for (size_t i = 0; (string[i] != '\0') && (i < MAX_ITERATIONS); i++)
    {
        DPRINTF("text\n");
        DPRINTF("string let= %c\n", string[i]);
        DPRINTF("index = %d\n", (uint8_t) string[i] ^ (hash & 0xff));

        hash = POLYNOM_8_LOOKUP_TABLE[(uint8_t) string[i] ^ (hash & 0xff)] ^ (hash >> 8);
    }

    return hash ^ 0xffffffff;
}

//------------------------------------------------------------------//
