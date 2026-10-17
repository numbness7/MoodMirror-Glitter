#version 330 core
in vec2 ourTexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;
uniform sampler2D borderTexture;
uniform vec4 aColor;
uniform int borderCase = 0;
vec4 getDistColor(vec4 textureColor, int theCase);
float getDist(vec4 textureColor);
vec4 getDistColor(vec4 textureColor, int theCase, float borderDistScale);
const float redChannel = 0;
const float greenChannel = 1;
const float blueChannel = 2;
const float alphaChannel = 3;
const float nrChannels = 4;
const float maxColor = pow(2,8) - 1;
const float maxDist = pow(2,8*4)-1;
float getColorDist(float color, float channel);


void main(){
    vec4 borderTextureColor = texture(borderTexture, ourTexCoord);
    vec4 textureColor = texture(ourTexture, ourTexCoord);
    vec4 borderColor = getDistColor(borderTextureColor, borderCase, 1.0f);
    FragColor = aColor*borderColor*textureColor;
}
float getDist(vec4 textureColor){
        float dist = getColorDist(textureColor.x,redChannel)+getColorDist(textureColor.y,greenChannel)+getColorDist(textureColor.z,blueChannel)+getColorDist(textureColor.a,alphaChannel);
        return dist;
}

vec4 getDistColor(vec4 textureColor, int theCase, float borderDistScale){
    if (theCase == 0){
        return vec4(1.0f)*vec4(textureColor.x, textureColor.y, textureColor.z + textureColor.a,1.0f);
    }
    else if (theCase == 1) {
        return vec4(1.0f)*textureColor;
    }
    else if (theCase == 2){
        float dist = getDist(textureColor);
        return vec4(1.0f)*vec4(1.0f,1.0f,1.0f,dist*borderDistScale);
    }
    else if (theCase == 3){
        float dist = getDist(textureColor);
        float invertedDist = 1.0f - dist*borderDistScale; 
        return vec4(1.0f)*vec4(1.0f,1.0f,1.0f,invertedDist);
    }
    else {
        return vec4(1.0f);
    }
}
float getColorDist(float color, float channel){
   return color*(maxColor/maxDist)*pow(2,(nrChannels-channel-1)*8); 
}