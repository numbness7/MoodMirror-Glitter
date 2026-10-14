#version 330 core
in vec2 ourTexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture1;
uniform vec4 aColor;
uniform float creationDate;
uniform float elapsedTime;
uniform float SHAPE_SPAWN_TIME;
uniform float MAXIMUM_BATCHES;
uniform float SHAPE_MAX_FADE;

float calcFade(float elapsedTime, float creationDate, float blendPointPre, float blendPointPost, float blendPointEnd, float minFade, float maxFade);

void main(){
    float fade = calcFade(elapsedTime, creationDate, 0.3f, 0.5f, 0.7f, 0.0f, 1.0f);
    vec4 newColor = aColor;
    newColor.a = (1 - fade)*aColor.a;
    FragColor = newColor*texture(ourTexture1, ourTexCoord);
}

float calcFade(float elapsedTime, float creationDate, float blendPointPre, float blendPointPost, float minFade, float maxFade){
    float fade = 0.0f;
    float timePassed = elapsedTime - creationDate;
    float normalizer = 1.0f/(SHAPE_SPAWN_TIME*MAXIMUM_BATCHES);
    float timePassedNorm = timePassed*normalizer;
    float mi = (minFade - maxFade)/blendPointPre;
    float bi = minFade - mi*blendPointPre;
    float mf = (maxFade - minFade)/(blendPointEnd-blendPointPost);
    float bf = maxFade - mf*blendPointEnd;
    if(timePassedNorm <= blendPointPre) fade = mi*timePassedNorm + bi;
    else if (timePassedNorm > blendPointPre && timePassedNorm <= blendPointPost) fade = minFade;
    else if (timePassedNorm > blendPointPost && timePassedNorm <= blendPointEnd) fade = mf*timePassedNorm + bf;
    else if (timePassedNorm > blendPointEnd) fade = maxFade;


    if(fade > SHAPE_MAX_FADE) fade = SHAPE_MAX_FADE;
    if(fade < 0.0f) fade = 0.0f;
    return fade;
    
}