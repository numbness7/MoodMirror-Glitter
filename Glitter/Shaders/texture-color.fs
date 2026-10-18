#version 330 core
in vec2 ourTexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;
uniform sampler2D borderTexture;
uniform vec4 aColor;
#define borderCase 0;
#define SCALE 0.05f; // Can only scale down
vec4 getDistColor(vec4 textureColor, int theCase);
float getDist(vec4 textureColor);
vec4 getDistColor(vec4 textureColor, int theCase, float borderDistScale);
#define redChannel      0.0f;
#define greenChannel    1.0f;
#define blueChannel     2.0f;
#define alphaChannel    3.0f;
#define nrChannels      4.0f;
float maxColor = pow(2,8) - 1;
float maxDist = pow(2,8*4)-1;
float getColorDist(float color, float channel);


void main(){
    vec4 borderTextureColor = texture(borderTexture, ourTexCoord);
    vec4 textureColor = texture(ourTexture, ourTexCoord);
    vec4 borderColor = getDistColor(borderTextureColor, borderCase, 1.0f);
    FragColor = scaleDown(SCALE, getDist(borderColor))*aColor*borderColor*textureColor;
}

vec4 scaleDown(float scale, float dist){
    if(scale > dist) return vec4(1.0f);
    else return vec4(0.0f);
}

float getDist(vec4 textureColor){
        float dist = getColorDist(textureColor.x,redChannel)+getColorDist(textureColor.y,greenChannel)+getColorDist(textureColor.z,blueChannel)+getColorDist(textureColor.a,alphaChannel);
        return dist;
}

vec4 getDistColor(vec4 textureColor, int theCase, float borderDistScale){
    float dist = getDist(textureColor);
    if (theCase == 0){
        return vec4(1.0f)*vec4(textureColor.x, textureColor.y, textureColor.z + textureColor.a,1.0f);
    }
    else if (theCase == 1) {
        return vec4(1.0f)*textureColor;
    }
    else if (theCase == 2){
        return vec4(1.0f)*vec4(1.0f,1.0f,1.0f,dist*borderDistScale);
    }
    else if (theCase == 3){
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