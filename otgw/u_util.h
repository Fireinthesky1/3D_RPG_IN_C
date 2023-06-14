#pragma once

#include <stdio.h>
#include <stdlib.h> // TODO(James): Check if this is neccessary

// read entire file into buffer
// TODO(James): Figure out how to make this work without the "}" dependency
// TODO(James): Crash when attempting to free the memory allocated here after
// use Look into allocated and freeing memory
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
    char* shader = (char*)malloc(sizeof(length));

    if (shader == NULL)
    {
        printf("FAILED TO ALLOCATE MEMORY FOR FILE");
        return NULL;
    }

    for (int i = 0; i < length; ++i)
    {
        shader[i] = '\0';
    }

    for (int i = 0; i < length; ++i)
    {
        char c = getc(f);
        if (c == '}') { shader[i] = c; break; }
        shader[i] = c;
        //putc(shader[i], stdout); //TESTING CODE
    }

    fclose(f);
    return shader;
}