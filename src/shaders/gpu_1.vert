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

    vec3 particlePos = pos[index];
    float particleSize = size[index];
    float particleLifetime = lifetime[index]; 
    vec4 particleColor = colour[index];

    fragColor = particleColor;
    texCoords = inTexCoords;

    if (particleLifetime <= 0) {
        // If particle lifetime < 0
        // Push vertex outside clip space to ensure it's clipped and discarded before rasterization
        gl_Position = vec4(-9999.0, -9999.0, -9999.0, 1.0);
        return;
    }

    // Extract camera right and up vectors from view matrix
    vec3 right = normalize(vec3(uView[0][0], uView[1][0], uView[2][0]));
    vec3 up    = normalize(vec3(uView[0][1], uView[1][1], uView[2][1]));

    // Move the quad corner according to inPosition * size
    vec3 offset = (right * inPosition.x + up * inPosition.y) * particleSize;

    vec3 worldPosition = particlePos + offset;

    gl_Position = uProj * uView * vec4(worldPosition, 1.0);
}

