#version 460 core

layout(location = 0) in vec3 inPosition;         // Original position of the vertex
layout(location = 1) in vec3 inPositionOffset;    // Offset to be applied to the position
layout(location = 2) in vec4 inColor;            // Input color
layout(location = 3) in float inSize;            // Size factor

uniform mat4 uMVP;  // Model-View-Projection matrix

out vec4 fragColor;

void main()
{
    fragColor = inColor;

    // Convert the 3D position offset to a 4D homogeneous coordinate (with w = 1.0f)
    vec4 positionWithOffset = vec4(inPositionOffset, 1.0);

    // Apply scaling only to the x, y, z components (not the w component)
    vec4 scaledPosition = vec4(inPosition * inSize, 1.0); // Scales x, y, z components

    // Apply the transformation to the final position (apply MVP matrix to scaled position + offset)
    gl_Position = uMVP * (scaledPosition + positionWithOffset);
}
