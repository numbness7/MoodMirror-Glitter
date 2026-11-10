#version 330 core

uniform sampler2D TEXTURE;

in vec2 ourTexCoord;
out vec4 FragColor;

void main(){
    float f = texture(TEXTURE, ourTexCoord).r;
    if(f >= 0.0f)
        FragColor = vec4(f,0.0f,0.0f,1.0f);
    else
        FragColor = vec4(0.0f,-f,0.0f,1.0f);
}