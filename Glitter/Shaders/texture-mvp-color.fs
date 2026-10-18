#version 330 core
in vec2 ourTexCoord;
out vec4 FragColor;

uniform sampler2D borderTexture;
uniform int borderCase = 0;

uniform sampler2D ourTexture;
uniform float destructionTimePassed;
uniform float destructionTime;
uniform int layer;
uniform int maxLayer;
uniform float SHAPE_MAX_FADE;
uniform float SHAPE_MIN_FADE;
uniform int MID_LAYER;
uniform bool DO_FADE;
const float SCALE = 1.0f;
uniform float BORDER_THICKNESS;
uniform vec4 INNER_COLOR;
uniform vec4 OUTER_COLOR;
uniform bool SOLID_BORDER = false;


const float redChannel = 0;
const float greenChannel = 1;
const float blueChannel = 2;
const float alphaChannel = 3;
const float nrChannels = 4;
const float maxColor = pow(2,8) - 1;
const float maxDistData = pow(2,8*4)-1;
const float maxColorToDistRatio = (maxColor/maxDistData);
const float maxDistNormal = sqrt(2);

float getColorDist(float color, float channel);
float calcFade();
float distFromCenter(float dist);
bool isOuter(float dist, float thickness);
float getDist(vec4 textureColor);
bool isInner(float dist, float thickness);

float distFromCenter(float dist){
    return 1.0f - dist;
}

bool isOuter(float dist, float thickness){
    return dist <= thickness;
}

bool isInner(float dist, float thickness){
    return dist > thickness;
}



void main(){
    
    vec4 borderTextureColor = texture(borderTexture, ourTexCoord);
    vec4 textureColor = texture(ourTexture, ourTexCoord);
    
    float dist = getDist(borderTextureColor);
    float AMOUNT_INNER = dist/BORDER_THICKNESS;
    vec4 borderColor;

    if(SOLID_BORDER)
        borderColor = isInner(dist, BORDER_THICKNESS) ? INNER_COLOR : OUTER_COLOR;
    else
        borderColor = isInner(dist, BORDER_THICKNESS) ? INNER_COLOR : (AMOUNT_INNER*INNER_COLOR + (1.0f-AMOUNT_INNER)*OUTER_COLOR);


    float fade = 0.0f;
    if(DO_FADE){
        fade = calcFade();
    }

    FragColor = vec4(INNER_COLOR.xyz, INNER_COLOR.a*(1.0f-fade));
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
        float dist = getColorDist(textureColor.x,redChannel)+getColorDist(textureColor.y,greenChannel)+getColorDist(textureColor.z,blueChannel)+getColorDist(textureColor.a,alphaChannel);
        float distNormalized = dist*maxDistNormal;
        return distNormalized;
}

float getColorDist(float color, float channel){
   return color*maxColorToDistRatio*pow(2,(nrChannels-channel-1)*8); 
}