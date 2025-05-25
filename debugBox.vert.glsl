#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 fragColor;

layout(set = 0, binding = 0) uniform CameraUBO 
{
    mat4 view;
    mat4 proj;
};

layout(push_constant) uniform PushConstants 
{
    mat4 model;
};

void main() 
{
    gl_Position = proj * view * model * vec4(inPosition, 1.0);
    fragColor = inColor;
}