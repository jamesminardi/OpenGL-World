#version 460 core

in float vHeight;

out vec4 fColor;

void main()
{
    float h = (vHeight)/64.0f;	// shift and scale the height into a grayscale value
    fColor = vec4(h, h, h, 1.0);
}