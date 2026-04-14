#ifndef INPUT_H
#define INPUT_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "my_printfs.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/stat.h>

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct BufferData
{
    char* buffer;
    int lines_count;
} BufferData_t;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct PtrDataParams
{
    char** ptrdata;
    int lines_count;
} PtrDataParams_t;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct FileInfo
{
    FILE* stream;
    const char* filepath;
    size_t size;
} FileInfo_t;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct InputCtx
{
    FileInfo_t input_file_info;
    BufferData_t buffer_data;
    PtrDataParams_t ptrdata_params;
    FileInfo_t output_file_info;
} InputCtx_t;

//——————————————————————————————————————————————————————————————————————————————————————————

int  OpenFile           (FileInfo_t* file_info, const char* mode);
int  ReadAndParseFile   (InputCtx_t* InputCtx);
int  CountSize          (InputCtx_t* InputCtx);
int  ReadText           (InputCtx_t* InputCtx);
int  AllocateBuffer     (InputCtx_t* InputCtx);
int  FillBuffer         (InputCtx_t* InputCtx);
int  ParseText          (InputCtx_t* InputCtx);
int  CountLines         (InputCtx_t* InputCtx);
int  AllocatePtrdata    (InputCtx_t* InputCtx);
int  FillPtrdata        (InputCtx_t* InputCtx);
void InputCtxDtor       (InputCtx_t* input_ctx);
void AddNullTerminators (char* ptr);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* INPUT_H */
