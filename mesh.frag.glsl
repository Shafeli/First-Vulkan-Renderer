#version 450

layout(push_constant) uniform Constants 
{
    mat4 objectMatrix;
    vec4 objectColor;
    vec4 useTexture;
};

layout(std140, binding = 0) uniform Camera 
{
    mat4 projectionMatrix;
    mat4 viewMatrix;

    vec3 lightPosition;
    float pad0;

    vec3 lightColor;
    float pad1;

    vec3 lightAttenuation;
    float pad2;

    vec3 lightPosition2;
    float pad3;

    vec3 lightColor2;
    float pad4;

    vec3 lightAttenuation2;
    float pad5;
};

const float ambientStrength = 0.1;

layout(set = 1, binding = 0) uniform sampler2D texture0;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexcoord0;
layout(location = 4) flat in int inUseTexture;

layout(location = 0) out vec4 outColor;

vec3 lighting(vec3 lightPos, vec3 pos, vec3 N, vec3 color)
{
    vec3 L = normalize(lightPos - pos);
    float NdotL = max(dot(N, L), 0.0);
    return color * NdotL;
}

void main()
{
    vec3 pos = (objectMatrix * vec4(inPosition, 1.0)).xyz;
    
    // Transform and normalize normal
    vec3 N = normalize((objectMatrix * vec4(inNormal, 0.0)).xyz);

    // Lighting
    vec3 light1 = lighting(lightPosition, pos, N, lightColor);
    vec3 light2 = lighting(lightPosition2, pos, N, lightColor2);
    vec3 ambient = ambientStrength * lightColor;

    // Use texture or fallback color
    vec3 baseColor = (inUseTexture == 1) ? texture(texture0, inTexcoord0).rgb : objectColor.rgb;

    vec3 finalColor = (ambient + light1 + light2) * baseColor;

    outColor = vec4(finalColor, 1.0);
}

