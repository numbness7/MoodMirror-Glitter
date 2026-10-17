#version 330 core
in vec2 ourTexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec4 aColor;
uniform bool distanceFade = false;


void main(){
    if (!distanceFade)
        FragColor = aColor*vec4(texture(ourTexture, ourTexCoord).xyz,1.0f);
    else{
        vec4 textureColor = texture(ourTexture, ourTexCoord);
        //Jank
        //FragColor = aColor*vec4(textureColor.x, textureColor.y, textureColor.z, 1.0f);
        const float redChannel = 0;
        const float greenChannel = 1;
        const float blueChannel = 2;
        const float alphaChannel = 3;
        const float nrChannels = 4;
        float dist = 
              textureColor.x*pow(2,-(redChannel)*8)
            + textureColor.y*pow(2,-(greenChannel)*8)
            + textureColor.z*pow(2,-( blueChannel)*8)
            + textureColor.a*pow(2,-( alphaChannel)*8);
        FragColor = aColor*vec4(1.0f,0.0f,0.0f,dist*8.0f);
    }
}