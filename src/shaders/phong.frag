// Fragment shader computing full-phong lighting for a directional light

#version 460 core

// location 0 is gl_FragCoord
in vec4 vColor;
in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vWorldPosition;

// By default, OpenGL draws the 0th output
// COLOR MUST BE AT LOCATION 0
out vec4 fColor;
out vec4 fragCoordOut;

uniform sampler2D myTexture;

uniform vec3 LightColor;
uniform vec3 LightPos;
uniform vec3 ViewPos;
uniform float Shininess; // exponent for sharper highlights
uniform float SpecularStrength; // Extra factor to adjust shininess
uniform float AmbientStrength; // Extra factor to adjust shininess
uniform float DiffuseStrength;

void main() {
    vec4 fragCoord_tmp = gl_FragCoord;
    fragCoordOut = fragCoord_tmp;

    vec4 color_tmp = vColor;

    // Point Light
    vec3 light_dir = normalize(LightPos - vWorldPosition);

    // Ambient
    vec3 ambient = AmbientStrength * LightColor;

    // Diffuse
    float diff = max(0.0, dot(vNormal, light_dir));
    vec3 diffuse = diff * LightColor * DiffuseStrength;


    // Specular
    vec3 view_dir    = normalize(ViewPos - vWorldPosition);
    vec3 halfway_dir = normalize(light_dir + view_dir);
    float spec = max(0.0, dot(vNormal, halfway_dir));
    float spec_pow = pow(spec, Shininess);
    vec3 specular = LightColor * spec_pow * SpecularStrength;

    vec3 result = min( ((diffuse + ambient + specular) * texture(myTexture, vTexCoord).rgb ), vec3(1.0) );

    fColor = vec4(result, color_tmp.a);

}