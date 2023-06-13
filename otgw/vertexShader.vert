#version 460 core

in vec3 vposition;

void
main()
{
    gl_Position = vec4(vposition.x, vposition.y, vposition.z, 1.0f);
}