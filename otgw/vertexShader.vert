#version 460 core
layout (location = 0) in vec3 i_position;
//layout (location = 1) in vec3 i_color;
//layout (location = 2) in vec2 i_texture_coordinate;

//out vec3 o_color;
//out vec2 o_texture_coordinate;

uniform mat3 u_rotation_matrix;

void
main()
{
    gl_Position = vec4(i_position * u_rotation_matrix, 1.0);
    //o_color = i_color;
    //o_texture_coordinate = i_texture_coordinate;
}