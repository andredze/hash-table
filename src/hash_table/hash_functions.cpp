#include "hash_functions.h"
#include "my_printfs.h"

//------------------------------------------------------------------//

uint32_t CountHashAlwaysOne(char* const string)
{
    assert(string);

    return 1;
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

uint32_t CountHashRotateLeft(char* const string)
{
    assert(string);

    uint32_t hash = *string;
    
    for (size_t i = 0; (string[i + 1] != '\0') && (i < MAX_ITERATIONS); i++)
    {
        // rol hash, 3
        hash = (hash << ROTATE_BYTES) | (hash >> (sizeof(uint32_t) * 8 - ROTATE_BYTES)); 

        hash = hash ^ string[i + 1];
    }

    return hash;
}

//------------------------------------------------------------------//

uint32_t CountHashRotateRight(char* const string)
{
    assert(string);

    uint32_t hash = *string;
    
    for (size_t i = 0; (string[i + 1] != '\0') && (i < MAX_ITERATIONS); i++)
    {
        // ror hash, 3
        hash = (hash >> ROTATE_BYTES) | (hash << (sizeof(uint32_t) * 8 - ROTATE_BYTES));

        hash = hash ^ string[i + 1];
    }

    return hash;
}

//------------------------------------------------------------------//

uint32_t CountHashDjb2(char* const string)
{
    assert(string);

    uint32_t hash = 5381;

    for (size_t i = 0; (string[i] != '\0') && (i < MAX_ITERATIONS); i++)
    {
        hash = ((hash << 5) + hash) + string[i];
    }

    return hash;
}

//------------------------------------------------------------------//

uint32_t CountHashCrc32(char* const string)
{
    assert(string);

    uint32_t hash = 0xFFFFFFFF;

    for (size_t i = 0; (string[i] != '\0') && (i < MAX_ITERATIONS); i++)
    {
        hash = POLYNOM_8_LOOKUP_TABLE[(uint8_t) string[i] ^ (hash & 0xff)] ^ (hash >> 8);
    }

    return hash ^ 0xFFFFFFFF;
}

//------------------------------------------------------------------//

uint32_t CountHashCrc32AsmInline(char* string)
{
    assert(string);

    uint32_t crc = 0xFFFFFFFF;

    asm ("crc32 %[crc], DWORD PTR [%[string] + 0 ]\n" 
         "crc32 %[crc], DWORD PTR [%[string] + 8 ]\n" 
         "crc32 %[crc], DWORD PTR [%[string] + 16]\n" 
         "crc32 %[crc], DWORD PTR [%[string] + 24]\n" 
         : [crc]    "+r" (crc) 
         : [string] "r"  (string)
         : "memory");

    // no remaining bytes as STR_MAX_SIZE % 8 == 0

    return crc ^ 0xFFFFFFFF;
}

//------------------------------------------------------------------//
