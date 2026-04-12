#ifndef HASH_FUNCTONS_H
#define HASH_FUNCTONS_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <assert.h>

//------------------------------------------------------------------//

const size_t MAX_ITERATIONS = 1024 * 1024;

//------------------------------------------------------------------//

size_t CountHashDjb2    (char* const string);
size_t CountHashChecksum(char* const string);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* HASH_FUNCTONS_H */