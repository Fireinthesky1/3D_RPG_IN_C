#version 460 core
layout (location = 0) in vec3 i_position;
layout (location = 1) in vec3 i_color;
layout (location = 2) in vec2 i_texture_coordinate;

out vec3 o_color;
out vec2 o_texture_coordinate;

uniform float u_delta_sin;
uniform float u_delta_cos;

void
main()
{
    gl_Position = vec4(i_position.x + u_delta_sin, i_position.y + u_delta_cos, u_delta_sin, 2 + u_delta_sin / 2);
    o_color = i_color;
    o_texture_coordinate = vec2(i_texture_coordinate.x + u_delta_sin, i_texture_coordinate.y + u_delta_sin);
}