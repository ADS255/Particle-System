#version 460 core

layout(std430, binding = 0) buffer DataBuffer {
    float data[]; //SSBO containing particle data
};

layout(location = 0) in vec3 inPosition;         // Quad vertex position (local)
layout(location = 1) in vec2 inTexCoords;         // Texture coordinates (local)

uniform mat4 uView;  // View matrix
uniform mat4 uProj;  // Projection matrix

out vec4 fragColor;
out vec2 texCoords;

void main()
{
    int index = gl_InstanceID * 13;  // 3 (position) + 4 (color) + 1 (size) 13 floats per particle

    if (data[index + 11] <= 0) {
        // If particle lifetime < 0
        // Push vertex outside clip space to ensure it's clipped and discarded before rasterization
        gl_Position = vec4(-9999.0, -9999.0, -9999.0, 1.0);
        return;
    }

    // Extract data from SSBO
    vec3 pos = vec3(data[index], data[index+1], data[index+2]); // Position
    vec4 colour = vec4(data[index+3], data[index+4], data[index+5], data[index+6]); // Color
    float size = data[index+7]; // Size

    // Use color modified by values from another SSBO
    fragColor = colour * vec4(1.0, 1.0, 1.0, 1.0);
    texCoords = inTexCoords;

    // Extract camera right and up vectors from the View Matrix
    vec3 right = vec3(uView[0][0], uView[1][0], uView[2][0]); // First column
    vec3 up    = vec3(uView[0][1], uView[1][1], uView[2][1]); // Second column

    // Billboard transformation and scaling
    vec3 billboardPos = (right * inPosition.x + up * inPosition.y) * size;

    // Compute world position of vertex
    vec4 worldPos = vec4(pos + billboardPos, 1.0);

    // Final projection
    gl_Position = uProj * uView * worldPos;
}


