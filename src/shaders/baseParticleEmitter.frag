#version 460 core

uniform sampler2D uTexSlot;

in vec4 fragColor;
in vec2 texCoords;

out vec4 outColor;

void main()
{
    //outColor = fragColor; // Set alpha to 1.0 for fully opaque

    outColor = texture(uTexSlot,texCoords);
}
