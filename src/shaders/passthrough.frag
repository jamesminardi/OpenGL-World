#version 460 core

out vec4 FragColor;

void main()
{
    vec3 color = vec3(1.0f, 0.5f, 0.2f);
    FragColor = vec4(color, 1.0);
}