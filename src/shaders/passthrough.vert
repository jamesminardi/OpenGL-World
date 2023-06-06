#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 mvp;

out vec3 vColor;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
    vColor = vec3(1.0);
}