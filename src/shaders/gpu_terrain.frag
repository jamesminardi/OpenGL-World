#version 460 core

in float Height;

out vec4 FragColor;

void main()
{
    float h = (16)/64.0f;
    FragColor = vec4(h, h, h, 1.0);
}