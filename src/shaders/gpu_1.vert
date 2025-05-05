#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoords;

uniform mat4 uView;
uniform mat4 uProj;

out vec4 fragColor;
out vec2 texCoords;

layout(std430, binding = 2) buffer PositionBuffer {
    vec3 pos[];
};

layout(std430, binding = 3) buffer ColourBuffer {
    vec4 colour[];
};

layout(std430, binding = 4) buffer SizeBuffer {
    float size[];
};

layout(std430, binding = 6) buffer lifetimeBuffer {
    float lifetime[];
};

void main()
{
    int index = gl_InstanceID;

    vec3 particlePos = pos[index]; // Or read pos[index];
    float particleSize = size[index];         // Hardcoded for now
    vec4 particleColor = vec4(1,1,1,1); // Hardcoded for now

    fragColor = particleColor;
    texCoords = inTexCoords;

    // Extract camera right and up vectors from view matrix
    vec3 right = normalize(vec3(uView[0][0], uView[1][0], uView[2][0]));
    vec3 up    = normalize(vec3(uView[0][1], uView[1][1], uView[2][1]));

    // Move the quad corner according to inPosition * size
    vec3 offset = (right * inPosition.x + up * inPosition.y) * particleSize;

    vec3 worldPosition = particlePos + offset;

    gl_Position = uProj * uView * vec4(worldPosition, 1.0);
}

