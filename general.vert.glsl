#version 450

layout(push_constant) uniform Constants
{
    mat4 objectMatrix;
    vec4 objectColor;
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
layout(location = 2) in vec2 inTexCoord;

layout(location = 1) out vec3 outPosition;
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec2 outTexCoord;

void main()
{
    outPosition = inPosition;
    outNormal = inNormal;
    outTexCoord = inTexCoord;

    gl_Position = projectionMatrix * viewMatrix * objectMatrix * vec4(inPosition, 1);
}