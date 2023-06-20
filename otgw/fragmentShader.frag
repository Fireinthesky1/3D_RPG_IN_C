#version 460 core

out vec4 o_fragment_color;

in vec3 o_color;
in vec2 o_texture_coordinate;

uniform sampler2D u_texture;

void
main()
{
    o_fragment_color = texture(u_texture, o_texture_coordinate);
}