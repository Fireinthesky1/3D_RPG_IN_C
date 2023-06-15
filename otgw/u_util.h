#pragma once

#include <stdio.h>

// read entire file into buffer
const char* load_file(const char* filename)
{
    FILE* f;
    fopen_s(&f, filename, "r");

    if (f == NULL)
    {
        perror("ERROR::FAILED TO OPEN FILE:: ");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    unsigned long length = (unsigned long)ftell(f);
    rewind(f);
    char* buffer = (char*)calloc(length+1, sizeof(char));

    if (buffer == NULL)
    {
        printf("FAILED TO ALLOCATE MEMORY FOR FILE");
        return NULL;
    }

    fread(buffer, 1, length, f);

    fclose(f);

    buffer[length] = '\0';

    return buffer;
}