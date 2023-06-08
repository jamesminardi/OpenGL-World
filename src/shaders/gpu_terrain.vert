#version 460 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec2 aTex;

//out vec2 TexCoord;
out vec4 vPos;


void main()
{
    gl_Position = vec4(aPos, 1.0);
    vPos = vec4(aPos, 1.0);
//    TexCoord = aTex;
}