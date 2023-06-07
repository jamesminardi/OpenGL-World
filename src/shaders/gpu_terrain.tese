#version 460 core
layout(triangles, equal_spacing, ccw) in;

//uniform sampler2D heightMap;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


in vec3 tPos[];
//in vec2 TextureCoord[];

//out float Height;

void main()
{


//    float u = gl_TessCoord.x;
//    float v = gl_TessCoord.y;

//    vec2 t00 = TextureCoord[0].xy;
//    vec2 t01 = TextureCoord[1].xy;
//    vec2 t10 = TextureCoord[2].xy;
//    vec2 t11 = TextureCoord[3].xy;
//
//    vec2 t0 = (t01 - t00) * u + t00;
//    vec2 t1 = (t11 - t10) * u + t10;
//    vec2 texCoord = (t1 - t0) * v + t0;

//    Height = texture(heightMap, texCoord).y * 64.0 - 16.0;

//    vec4 p00 = gl_in[0].gl_Position;
//    vec4 p01 = gl_in[1].gl_Position;
//    vec4 p10 = gl_in[2].gl_Position;
//    vec4 p11 = gl_in[3].gl_Position;
//
//    vec4 uVec = p01 - p00;
//    vec4 vVec = p10 - p00;
//    vec4 normal = normalize( vec4(cross(vVec.xyz, uVec.xyz), 0) );
//
//    vec4 p0 = (p01 - p00) * u + p00;
//    vec4 p1 = (p11 - p10) * u + p10;
////    vec4 p = (p1 - p0) * v + p0 + normal * Height;
//    vec4 p = (p1 - p0) * v + p0 + normal * 1.0;


    vec3 p0 = gl_TessCoord.x * tPos[0]; // x is one corner
    vec3 p1 = gl_TessCoord.y * tPos[1]; // y is the 2nd corner
    vec3 p2 = gl_TessCoord.z * tPos[2]; // z is the 3rd corner (ignore when using quads)
    vec3 pos = normalize (p0 + p1 + p2);
    gl_Position = projection * view * model * vec4 (pos, 1.0);

}