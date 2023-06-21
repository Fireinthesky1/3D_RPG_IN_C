#version 460 core

out vec4 o_fragment_color;

//in vec3 o_color;
//in vec2 o_texture_coordinate;

//uniform sampler2D u_texture;

void
main()
{
    //o_fragment_color = texture(u_texture, o_texture_coordinate);
    o_fragment_color = vec4(0.46, 0.36, 0.86, 1.0);
}