// LEARN OPENGL
#version 310 es
precision highp float;
precision highp int;
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    vec4 col = texture(screenTexture, TexCoords).rgba;
    FragColor = vec4(col);
} 

