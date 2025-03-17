#version 460 core

uniform sampler2D uTexSlot;

in vec4 fragColor;
in vec2 texCoords;

out vec4 outColor;

void main()
{
    vec4 texColor = texture(uTexSlot, texCoords);
    outColor = texColor * fragColor;
}
