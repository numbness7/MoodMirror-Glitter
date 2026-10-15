#version 330 core
out vec4 FragColor;

uniform vec4 aColor;
uniform float destructionTimePassed;
uniform float destructionTime;
uniform int layer;
uniform int maxLayer;
uniform float SHAPE_MAX_FADE;
uniform float SHAPE_MIN_FADE;
uniform int MID_LAYER;
uniform bool DO_FADE;

in vec3 oPos;

float calcFade();
float calcFadeSimple();

void main(){
    float fade = 0.0f;
    if (DO_FADE)
        fade = calcFade();
    vec4 newColor = aColor;
    
    
    newColor.a = (1.0f - fade)*aColor.a;

    FragColor = newColor;
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

float calcFadeSimple(){
    float fade = 0.0f;
    int midLayer = maxLayer/2;
    float timePassed = destructionTimePassed;
    float normalizer = destructionTime;
    float timePassedNorm = timePassed/normalizer;
    if (layer == 0){
        float slope = (SHAPE_MAX_FADE - SHAPE_MIN_FADE)/(1.0f - 0.0f);
        float b = SHAPE_MIN_FADE;
        fade = b + slope*timePassedNorm;
    }
    else {
        float maxFade = float(layer)/float(maxLayer);
        float slope = (float(layer-1)/float(maxLayer) - maxFade)/(1.0f - 0.0f);
        float b = maxFade;
        fade = b + slope*timePassedNorm;
    }
    if(fade > SHAPE_MAX_FADE) fade = SHAPE_MAX_FADE;
    else if (fade < SHAPE_MIN_FADE) fade = SHAPE_MIN_FADE;
    return fade;
}