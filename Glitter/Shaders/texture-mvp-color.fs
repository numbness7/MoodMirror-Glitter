#version 330 core
in vec2 ourTexCoord;
out vec4 FragColor;

uniform sampler2D borderTexture;
uniform int borderCase;

uniform sampler2D ourTexture;
uniform float destructionTimePassed;
uniform float destructionTime;
uniform int layer;
uniform int maxLayer;
uniform float ABOVE_SLOPE;
uniform float BELOW_SLOPE;
uniform float ABOVE_INTERCEPT;
uniform float BELOW_INTERCEPT;
uniform int MID_LAYER;
uniform bool DO_FADE;
const float SCALE = 1.0f;
uniform float BORDER_THICKNESS;
uniform vec4 INNER_COLOR;
uniform vec4 OUTER_COLOR;


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
float calcFade(float destructionTimePassed, float destructionTime, int totalLayers, int midLayer, int layer);
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
    return !isOuter(dist,thickness);
}



void main(){
    
    vec4 borderTextureColor = texture(borderTexture, ourTexCoord);
    vec4 textureColor = texture(ourTexture, ourTexCoord);
    
    float dist = getDist(borderTextureColor);
    float AMOUNT_INNER = dist/BORDER_THICKNESS;
    vec4 borderColor;

    switch (borderCase){
        case 0:
            borderColor = isInner(dist, BORDER_THICKNESS) ? INNER_COLOR : OUTER_COLOR;
            break;
        case 1:
            borderColor = isInner(dist, BORDER_THICKNESS) ? INNER_COLOR : (AMOUNT_INNER*INNER_COLOR + (1.0f-AMOUNT_INNER)*OUTER_COLOR);
            break;
        case 2:
            borderColor = isInner(dist, BORDER_THICKNESS) ? INNER_COLOR : vec4(INNER_COLOR.xyz,INNER_COLOR.a*AMOUNT_INNER);
            break;
        case 3:
            borderColor = isInner(dist, BORDER_THICKNESS) ? vec4(INNER_COLOR.xyz,INNER_COLOR.a*(1.0f-AMOUNT_INNER)) : INNER_COLOR;
            break;
        default:
            borderColor = INNER_COLOR;
    }


    float fade = 0.0f;
    vec4 fadeColor = borderColor;
    if(DO_FADE){
        fade = calcFade(destructionTimePassed, destructionTime, maxLayer, MID_LAYER, layer);
        fadeColor.a = (1.0f - fade)*fadeColor.a;
    }

    if(borderCase == 0 && isOuter(dist, BORDER_THICKNESS))
        FragColor = fadeColor;
    else
        FragColor = fadeColor*textureColor;
}

float getDist(vec4 textureColor){
        float dist = getColorDist(textureColor.x,redChannel)+getColorDist(textureColor.y,greenChannel)+getColorDist(textureColor.z,blueChannel)+getColorDist(textureColor.a,alphaChannel);
        float distNormalized = dist*maxDistNormal;
        return distNormalized;
}

float getColorDist(float color, float channel){
   return color*maxColorToDistRatio*pow(2,(nrChannels-channel-1)*8); 
}
float calcFade(float destructionTimePassed, float destructionTime, int totalLayers, int midLayer, int layer){
    float fade = 0.0f;
	float timePassed;
	float b;
	float slope;
	if(layer > midLayer){
		timePassed = destructionTimePassed + destructionTime*(totalLayers - 1 - layer);
		b = ABOVE_INTERCEPT;
		slope = ABOVE_SLOPE;

	}
	else{
		timePassed = destructionTimePassed + destructionTime*(midLayer - layer);
		b = BELOW_INTERCEPT;
		slope = BELOW_SLOPE;
	}
    fade = b + slope*timePassed;
    return fade;
}