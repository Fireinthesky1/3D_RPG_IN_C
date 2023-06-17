#pragma once

#include "r_shader.h"

void s_bind_shader_source(struct shader* i_shader)
{
    glShaderSource(i_shader->id, 1, &(i_shader->source_code), NULL);
}

void s_compile_shader(struct shader* i_shader)
{
    char* shader_type_as_string;

    // TODO(JAMES): come back and fill out for all shader types
    if (i_shader->type == GL_VERTEX_SHADER)
    {
        shader_type_as_string = "VERTEX SHADER";
    }
    else
    {
        shader_type_as_string = "FRAGMENT SHADER";
    }

    glCompileShader(i_shader->id);

    GLint vertex_compiled;
    glGetShaderiv(i_shader->id, GL_COMPILE_STATUS, &vertex_compiled);
    if (vertex_compiled != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(i_shader->id, 1024, &log_length, message);
        printf
        ("ERROR::%s COMPILATION FAILED::%s\n\n",shader_type_as_string,message);
    }
}