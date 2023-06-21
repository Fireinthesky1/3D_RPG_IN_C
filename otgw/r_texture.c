#pragma once

#include "r_texture.h"

void t_bind_texture(struct texture* texture)
{
    // Check to see this dereference works
    glBindTexture(texture->type, *(texture->id));
}

void t_gen_mip_map(struct texture* texture)
{
    glGenerateMipmap(texture->type);
}

void t_initialize_id(struct texture* texture)
{
    glGenTextures(1, texture->id);
}

void t_set_tex_param(struct texture* texture, GLenum name, GLenum value)
{
    glTexParameteri(texture->type, name, value);
}

int t_specify_image(struct texture* texture)
{
    switch (texture->type)
    {
    case GL_TEXTURE_2D:
        glTexImage2D
        (texture->type,
            0,
            GL_RGB,
            texture->width,
            texture->height,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            texture->image_data);
        break;
    case GL_TEXTURE_3D:
        //FILL THIS OUT
        break;
    default:
        printf("ERROR::CANNOT SPECIFY TEXTURE IMAGE");
        return -1;
    }
    return 0;
}