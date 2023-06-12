#version 460 core

in vec4 vposition;

void
main()
{
    gl_Position = vposition;
}