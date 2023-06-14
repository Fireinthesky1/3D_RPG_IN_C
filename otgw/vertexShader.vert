#version 460 core
layout (location = 0) in vec3 vposition;

uniform vec4 deltaPosition;

void
main()
{
    gl_Position = 
    vec4(vposition.x, vposition.y, vposition.z, 1.0f) + deltaPosition;
}