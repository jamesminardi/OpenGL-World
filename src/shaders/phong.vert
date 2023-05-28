// Vertex shader for a full-phong directional light calculated in the fragment shader

#version 430 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Color;
layout (location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 Normal;

// location 0 is gl_Position
out vec4 vColor;
out vec2 vTexCoord;
out vec3 vNormal; // interpolate the normalized surface
out vec3 vWorldPosition;

uniform mat4 MVPMatrix;
uniform mat4 ModelMatrix;


void main() {

    vWorldPosition = vec3(ModelMatrix * vec4(Position, 1.0));
    vec4 transformed_position = MVPMatrix * vec4(Position, 1.0);
    gl_Position =  transformed_position;
    vColor = vec4(Color, 1.0);
    vNormal = normalize(Normal);
    vTexCoord = aTexCoord;
}