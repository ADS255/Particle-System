#version 460 core

layout(location = 0) in vec3 inPosition;         // Quad vertex position (local)
layout(location = 1) in vec2 inTexCoords;        // Texture coordinates (local)
layout(location = 2) in vec3 inPositionOffset;   // Particle world position (center)
layout(location = 3) in vec4 inColor;            // Particle color
layout(location = 4) in float inSize;            // Particle size

uniform mat4 uView;  // View matrix
uniform mat4 uProj;  // Projection matrix

out vec4 fragColor; // outputting for later stages
out vec2 texCoords; // outputting for later stages

void main()
{
    fragColor = inColor;
    texCoords = inTexCoords;

    // Extract camera right and up vectors from the View Matrix
    vec3 right = vec3(uView[0][0], uView[1][0], uView[2][0]); // First column
    vec3 up    = vec3(uView[0][1], uView[1][1], uView[2][1]); // Second column

    // Scale by particle size
    vec3 billboardPos = (right * inPosition.x + up * inPosition.y) * inSize;

    // Final world position of vertex
    vec4 worldPos = vec4(inPositionOffset + billboardPos, 1.0);

    // Apply matrices
    gl_Position = uProj * uView * worldPos;
}


