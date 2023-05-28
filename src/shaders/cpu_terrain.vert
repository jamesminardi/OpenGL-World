#version 460 core
layout (location = 0) in vec3 aPos;

out float vHeight;

uniform mat4 mvp;

void main()
{
    vHeight = aPos.y;
    gl_Position = mvp * vec4(aPos, 1.0);
}