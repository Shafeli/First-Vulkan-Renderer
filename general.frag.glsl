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

layout(location = 0) smooth in vec3 inPosition;
layout(location = 1) smooth in vec3 inNormal;
layout(location = 2) smooth in vec2 inTexCoord;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 fragTexCoord;

layout(set = 1, binding = 1) uniform sampler2D texSampler;

const vec3 light2Position = vec3(0.0, 0.0, 0.0);
const vec3 light2Color = vec3(1.0, 1.0, 1.0);

float attenuation(float dist, float constant, float linear, float quadratic)
{
    return 1.0 / (constant + linear * dist + quadratic * dist * dist);
}

vec3 lighting(vec3 lightPos, vec3 pos, vec3 N, vec3 color)
{
    vec3 L = normalize(lightPos - pos);
    float NdotL = max(dot(N, L), 0);

    float lightDist = length(lightPos - pos);
    float atten = 1; //attenuation(lightDist, lightAttenuation.x, lightAttenuation.y, lightAttenuation.z);

    return color * NdotL * atten;
}

void main()
{
    fragTexCoord = inTexCoord;
    vec3 pos = (objectMatrix * vec4(inPosition, 1)).xyz;
    vec3 N = normalize(inNormal);

    vec3 light1 = lighting(lightPosition, pos, N, lightColor);
    vec3 light2 = lighting(light2Position, pos, N, light2Color);

    vec3 finalColor = objectColor.rgb * (light1 + light2);

    vec4 texColor = texture(texSampler, fragTexCoord);
    outColor = texColor * vec4(finalColor, 1.0);
}
