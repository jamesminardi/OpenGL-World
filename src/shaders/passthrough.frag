#version 460 core

in vec3 vColor;
in vec2 vTexCoord;

out vec4 fColor;

uniform sampler2D myTexture;

void main()
{

    fColor = texture(myTexture, vTexCoord);
}