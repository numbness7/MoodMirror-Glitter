#version 330 core
in vec2 ourTexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec4 aColor;

void main(){
    FragColor = aColor*texture(ourTexture, ourTexCoord);
}