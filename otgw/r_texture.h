#pragma once

#include <glad/glad.h>
#include <stdio.h>

struct texture
{
    const unsigned char* image_data;
    GLint width;
    GLint height;
    GLenum type;
    GLuint *id;
};

void t_bind_texture(struct texture* texture);
void t_gen_mip_map(struct texture* texture);
void t_initialize_id(struct texture* texture);
void t_set_tex_param(struct texture* texture, GLenum name, GLenum value);
int  t_specify_image(struct texture* texture);