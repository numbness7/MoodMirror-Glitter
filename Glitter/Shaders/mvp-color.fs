#version 330 core
out vec4 FragColor;

uniform vec4 aColor;
uniform float creationDate;
uniform float elapsedTime;
uniform float SHAPE_SPAWN_TIME;
uniform float MAXIMUM_BATCHES;
uniform float SHAPE_MAX_FADE;

float calcFade(float elapsedTime, float creationDate, float blendPoint, float minFade, float maxFade);

void main(){
    float fade = calcFade(elapsedTime, creationDate, 0.5f, 0.5f, 1.0f);
    vec4 newColor = aColor;
    newColor.a = (1 - fade)*aColor.a;
    FragColor = newColor;
}

float calcFade(float elapsedTime, float creationDate, float blendPoint, float minFade, float maxFade){
    float fade = 0.0f;
    float timePassed = elapsedTime - creationDate;
    float normalizer = 1.0f/(SHAPE_SPAWN_TIME*MAXIMUM_BATCHES);
    float timePassedNorm = timePassed*normalizer;
    float mi = (minFade - maxFade)/blendPoint;
    float bi = minFade - mi*blendPoint;
    float mf = (maxFade - minFade)/(1.0f-blendPoint);
    float bf = maxFade - mf;
    if(timePassedNorm <= blendPoint) fade = mi*timePassedNorm + bi;
    else if (timePassedNorm > blendPoint) fade = mf*timePassedNorm + bf;


    if(fade > SHAPE_MAX_FADE) fade = SHAPE_MAX_FADE;
    if(fade < 0.0f) fade = 0.0f;
    return fade;
    
}