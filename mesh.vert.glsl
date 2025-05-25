#version 450

layout(push_constant) uniform Constants
{
    mat4 objectMatrix;
    vec4 objectColor;
    vec4 useTexture;
};

layout(binding = 0) uniform Camera
{
    mat4 projectionMatrix;
    mat4 viewMatrix;

    vec3 lightPosition;
    vec3 lightColor;
    vec3 lightAttenuation;
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexcoord0;

layout(location = 0) out vec3 outPosition;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outTexcoord0;
layout(location = 3) out vec4 outObjectColor;
layout(location = 4) flat out int outUseTexture;

void main()
{
    outPosition = (objectMatrix * vec4(inPosition, 1)).xyz;
    outNormal = normalize(transpose(inverse(mat3(objectMatrix))) * inNormal);
    outTexcoord0 = inTexcoord0;
    outUseTexture = int(useTexture.x);
    outObjectColor = objectColor;

    gl_Position = projectionMatrix * viewMatrix * objectMatrix * vec4(inPosition, 1.0);
}