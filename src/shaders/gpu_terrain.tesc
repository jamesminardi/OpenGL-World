#version 460 core

layout(vertices=4) out;

in vec4 vPos[];
out vec4 tcPos[];

uniform mat4 model;
uniform mat4 view;

//in vec2 TexCoord[];
//out vec2 TextureCoord[];

uniform float tessLevelInner = 4.0; // controlled by keyboard buttons
uniform float tessLevelOuter = 4.0; // controlled by keyboard buttons

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
//    TextureCoord[gl_InvocationID] = TexCoord[gl_InvocationID];
    tcPos[gl_InvocationID] = vPos[gl_InvocationID];

    if(gl_InvocationID == 0)
    {
//        const float MIN_TESS_LEVEL = 4.0;
//        const float MAX_TESS_LEVEL = 64.0;
//        const float MIN_DISTANCE = 20;
//        const float MAX_DISTANCE = 800;
//
//        vec4 eyeSpacePos00 = view * model * gl_in[0].gl_Position;
//        vec4 eyeSpacePos01 = view * model * gl_in[1].gl_Position;
//        vec4 eyeSpacePos10 = view * model * gl_in[2].gl_Position;
//        vec4 eyeSpacePos11 = view * model * gl_in[3].gl_Position;
//
//        // "distance" from camera scaled between 0 and 1
//        float distance00 = clamp( (abs(eyeSpacePos00.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );
//        float distance01 = clamp( (abs(eyeSpacePos01.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );
//        float distance10 = clamp( (abs(eyeSpacePos10.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );
//        float distance11 = clamp( (abs(eyeSpacePos11.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );
//
//        float tessLevel0 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance10, distance00) );
//        float tessLevel1 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance00, distance01) );
//        float tessLevel2 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance01, distance11) );
//        float tessLevel3 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance11, distance10) );

        gl_TessLevelOuter[0] = tessLevelOuter;
        gl_TessLevelOuter[1] = tessLevelOuter;
        gl_TessLevelOuter[2] = tessLevelOuter;
        gl_TessLevelOuter[3] = tessLevelOuter;

        gl_TessLevelInner[0] = tessLevelInner;
        gl_TessLevelInner[1] = tessLevelInner;
    }
}