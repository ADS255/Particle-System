#version 460 core

in vec4 fragColor;
out vec4 outColor;

void main()
{
    outColor = fragColor; // Set alpha to 1.0 for fully opaque
}
