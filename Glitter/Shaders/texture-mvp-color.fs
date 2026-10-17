#version 330 core
in vec2 ourTexCoord;
out vec4 FragColor;

uniform sampler2D borderTexture;
uniform int borderCase = 0;

uniform vec4 aColor;
uniform sampler2D ourTexture;
uniform float destructionTimePassed;
uniform float destructionTime;
uniform int layer;
uniform int maxLayer;
uniform float SHAPE_MAX_FADE;
uniform float SHAPE_MIN_FADE;
uniform int MID_LAYER;
uniform bool DO_FADE;
uniform bool IGNORE_TEXTURE;
uniform float BORDER_DIST_SCALE;

in vec3 oPos;

float getDist(vec4 textureColor);
float calcFade();
vec4 getDistColor(vec4 textureColor, int theCase, float borderDistScale);

void main(){
    
    
    float fade = 0.0f;
    if (DO_FADE)
        fade = calcFade();
    vec4 fadedColor = aColor;
    fadedColor.a = (1.0f - fade)*fadedColor.a;


    vec4 borderTextureColor = texture(borderTexture, ourTexCoord);
    vec4 textureColor = texture(ourTexture, ourTexCoord);
    vec4 borderColor = getDistColor(borderTextureColor, borderCase, BORDER_DIST_SCALE);
    float dist = getDist(borderTextureColor);

    if (IGNORE_TEXTURE)
        FragColor = borderColor*fadedColor;
    else
        FragColor = borderColor*fadedColor*textureColor;
}

float calcFade(){
    float fade = 0.0f;
    int midLayer = MID_LAYER;
    int midLayerMinus = maxLayer - midLayer;
    float timePassed = destructionTimePassed;
    float normalizer = destructionTime;
    float timePassedNorm = timePassed/normalizer;
    float maxFadeLayerRatio;
    float minFadeLayerRatio;
    float maxFade;
    float minFade;
    float slope;
    float b;
    if(layer > midLayer){ // Fade in
        maxFadeLayerRatio = float(layer - midLayer)/float(midLayerMinus);
        minFadeLayerRatio = float(layer - midLayer-1)/float(midLayerMinus);
        maxFade = maxFadeLayerRatio*(SHAPE_MAX_FADE);
        minFade = minFadeLayerRatio*(SHAPE_MAX_FADE);
        slope = (minFade - maxFade)/(1.0f - 0.0f);
        b = minFade - slope*1.0f;
    }
    else{ // Fade out
        maxFadeLayerRatio = float((midLayer) - layer)/float(midLayer);
        minFadeLayerRatio = float((midLayer-1) - layer)/float(midLayer);
        maxFade = maxFadeLayerRatio*(SHAPE_MAX_FADE);
        minFade = minFadeLayerRatio*(SHAPE_MAX_FADE);
        slope = (maxFade - minFade)/(1.0f - 0.0f);
        b = maxFade - slope*1.0f;
    }

    fade = b + slope*timePassedNorm;
    if(fade > SHAPE_MAX_FADE) fade = SHAPE_MAX_FADE;
    else if (fade < SHAPE_MIN_FADE) fade = SHAPE_MIN_FADE;
    return fade;
}

float getDist(vec4 textureColor){
        const float redChannel = 0;
        const float greenChannel = 1;
        const float blueChannel = 2;
        const float alphaChannel = 3;
        const float nrChannels = 4;
        float dist = 
              textureColor.x*pow(2,-(redChannel)*8)
            + textureColor.y*pow(2,-(greenChannel)*8)
            + textureColor.z*pow(2,-( blueChannel)*8);
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
        return vec4(1.0f)*vec4(1.0f,1.0f,1.0f,1.0f - dist*borderDistScale);
    }
    else {
        return vec4(1.0f);
    }
}