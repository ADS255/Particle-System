#version 460 core

// Uniform variable for a 2D texture sampler
uniform sampler2D uTexSlot;

// Input variables from the vertex shader
in vec4 fragColor;   // Fragment color
in vec2 texCoords;   // Texture coordinates for sampling the texture

// Output variable for the final fragment color
out vec4 outColor;

void main()
{
    // Sample the texture at the given texture coordinates
    vec4 texColor = texture(uTexSlot, texCoords);

    // Multiply the sampled texture color by the fragment color
    // This applies the fragment color as a modulation to the texture
    outColor = texColor * fragColor;
}
