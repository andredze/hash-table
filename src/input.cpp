#include "input.h"

//------------------------------------------------------------------------------------------

int ReadAndParseFile(InputCtx_t* InputCtx)
{
    if (OpenFile(&InputCtx->input_file_info, "rb"))
    {
        return 1;
    }
    if (ReadText(InputCtx))
    {
        DPRINTF("<Error during parsing text>\n");
        return 1;
    }
    fclose(InputCtx->input_file_info.stream);

    if (ParseText(InputCtx))
    {
        DPRINTF("<ptr_data is a NULL pointer>\n");
        return 1;
    }

    return 0;
}

//------------------------------------------------------------------------------------------

int OpenFile(FileInfo_t* file_info, const char* mode)
{
    assert(file_info != NULL);
    assert(file_info->filepath != NULL);

    file_info->stream = fopen(file_info->filepath, mode);
    if (file_info->stream == NULL)
    {
        DPRINTF("\n<Can not open the file %s>\n", file_info->filepath);
        return 1;
    }

    return 0;
}

//------------------------------------------------------------------------------------------

int CountSize(InputCtx_t* InputCtx)
{
    assert(InputCtx != NULL);
    assert(InputCtx->input_file_info.filepath != NULL);

    struct stat file_stat_info = {};

    if (stat(InputCtx->input_file_info.filepath, &file_stat_info) == -1)
    {
        DPRINTF("\n<Error occured with stat()>\n");
        return 1;
    }
    InputCtx->input_file_info.size = (size_t) file_stat_info.st_size;
    DPRINTF("size = %zu\n", InputCtx->input_file_info.size);

    return 0;
}

//------------------------------------------------------------------------------------------

int ReadText(InputCtx_t* InputCtx)
{
    assert(InputCtx != NULL);

    if (CountSize(InputCtx))
    {
        return 1;
    }
    if (AllocateBuffer(InputCtx))
    {
        return 1;
    }
    if (FillBuffer(InputCtx))
    {
        return 1;
    }

    return 0;
}

//------------------------------------------------------------------------------------------

int AllocateBuffer(InputCtx_t* InputCtx)
{
    assert(InputCtx != NULL);

    char* buffer = (char*) calloc(InputCtx->input_file_info.size / sizeof(char) + 1,
                                  sizeof(char));
    if (buffer == NULL)
    {
        DPRINTF("<Memory allocation for buffer failed>");
        return 1;
    }
    InputCtx->buffer_data.buffer = buffer;

    return 0;
}

//------------------------------------------------------------------------------------------

int FillBuffer(InputCtx_t* InputCtx)
{
    assert(InputCtx != NULL);
    assert(InputCtx->input_file_info.stream != NULL);

    size_t fread_return = fread(InputCtx->buffer_data.buffer,
                                sizeof(char),
                                InputCtx->input_file_info.size,
                                InputCtx->input_file_info.stream);
    DPRINTF("fread return = %zu\n", fread_return);

    if (fread_return != InputCtx->input_file_info.size)
    {
        if (ferror(InputCtx->input_file_info.stream))
        {
            DPRINTF("\n<Error with reading the file>\n");
            DPRINTF("<fread_return = %zu>\n", fread_return);
            return 1;
        }
        if (feof(InputCtx->input_file_info.stream))
        {
            DPRINTF("\n<EOF reached>\n");
            return 1;
        }
    }

    DPRINTF("BUFFER:\n");

#ifdef DEBUG
    puts(InputCtx->buffer_data.buffer);
#endif

    // adding NULL-terminator
    InputCtx->buffer_data.buffer[InputCtx->input_file_info.size] = '\0';

    return 0;
}

//------------------------------------------------------------------------------------------

int ParseText(InputCtx_t* InputCtx)
{
    assert(InputCtx != NULL);

    if (CountLines(InputCtx))
    {
        return 1;
    }
    if (AllocatePtrdata(InputCtx))
    {
        return 1;
    }
    if (FillPtrdata(InputCtx))
    {
        return 1;
    }

    return 0;
}

//------------------------------------------------------------------------------------------

int CountLines(InputCtx_t* InputCtx)
{
    assert(InputCtx != NULL);

    char* ptr = InputCtx->buffer_data.buffer;
    char* endptr = strchr(ptr, '\0');
    int lines_count = 0;

    while (ptr < endptr)
    {
        assert(ptr != NULL);

        ptr = strchr(ptr, '\n');
        lines_count += 1;

        if (ptr == NULL)
        {
            break;
        }

        ptr++; // skip '\n'
    }

    InputCtx->buffer_data.lines_count = lines_count;
    InputCtx->ptrdata_params.lines_count = lines_count;

    return 0;
}

//------------------------------------------------------------------------------------------

int AllocatePtrdata(InputCtx_t* InputCtx)
{
    assert(InputCtx != NULL);

    InputCtx->ptrdata_params.ptrdata = (char**) calloc ( (size_t) InputCtx->buffer_data.lines_count,
                                                     sizeof(char*));
    if (InputCtx->ptrdata_params.ptrdata == NULL)
    {
        DPRINTF("\n<Memory allocation for ptrdata failed>\n");
        return 1;
    }

    return 0;
}

//------------------------------------------------------------------------------------------

int FillPtrdata(InputCtx_t* InputCtx)
{
    assert(InputCtx != NULL);

    // fill ptrdata and switch \r and \n to \0
    char* ptr = InputCtx->buffer_data.buffer;

    for (int i = 0; i < InputCtx->buffer_data.lines_count; i++)
    {
        InputCtx->ptrdata_params.ptrdata[i] = ptr;

        assert(ptr != NULL);

        ptr = strchr(ptr, '\n');
        if (ptr == NULL)
        {
            break;
        }

        ptr++;

        AddNullTerminators(ptr);
    }

    return 0;
}

//------------------------------------------------------------------------------------------

void AddNullTerminators(char* ptr)
{
    assert(ptr != NULL);

    *(ptr - 1) = '\0'; // switch \n to \0
    if (*(ptr - 2) == '\r') // if windows file (\r\n at end)
    {
        *(ptr - 2) = '\0'; // switch \r to \0
    }
}

//------------------------------------------------------------------------------------------

void InputCtxDtor(InputCtx_t* input_ctx)
{
    free(input_ctx->buffer_data.buffer);
    free(input_ctx->ptrdata_params.ptrdata);

    input_ctx->buffer_data.buffer         = NULL;
    input_ctx->buffer_data.lines_count    = 0;
    input_ctx->ptrdata_params.ptrdata     = NULL;
    input_ctx->ptrdata_params.lines_count = 0;
}

//------------------------------------------------------------------------------------------
