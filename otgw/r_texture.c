#pragma once

#include "r_texture.h"

void t_bind_texture(struct texture* texture)
{
    glBindTexture(texture->type, texture->id);
}

void t_gen_mip_map(struct texture* texture)
{
    glGenerateMipmap(texture->type);
}

void t_set_tex_param(struct texture* texture, GLenum name, GLenum value)
{
    glTexParameteri(texture->type, name, value);
}

void t_specify_image(struct texture* texture)
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
        break;
    default:
        printf("ERROR::CANNOT SPECIFY TEXTURE IMAGE");
        break;
    }
}