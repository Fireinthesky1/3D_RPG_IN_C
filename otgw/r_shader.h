#pragma once

#include <glad/glad.h>
#include <stdio.h>

struct shader
{
    const GLchar* source_code;
    GLenum type;
    GLuint id;
};

void s_bind_shader_source(struct shader* shader);
void s_compile_shader(struct shader *shader);