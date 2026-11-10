#version 330 core
precision mediump float;
in vec2 ourTexCoord;
out vec4 FragColor;

const float SCALE_CONST = 1.0f;
const float redChannel_CONST = 0.0f;
const float greenChannel_CONST = 1.0f;
const float blueChannel_CONST = 2.0f;
const float alphaChannel_CONST = 3.0f;
const float nrChannels_CONST = 4.0f;
const float maxColor_CONST = pow(2.0f,8.0f) - 1.0f;
const float maxDistData_CONST = pow(2.0f,8.0f*4.0f)-1.0f;
const float maxColorToDistRatio_CONST = (maxColor_CONST/maxDistData_CONST);
const float maxDistNormal_CONST = sqrt(2.0f);
const int   maxInt_CONST = 2147483647;

uniform sampler2D borderTexture;
uniform int borderCase;
flat in int InstanceID;


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
uniform float BORDER_THICKNESS;
uniform vec4 INNER_COLOR;
uniform vec4 OUTER_COLOR;
uniform vec2 SL_SCALE;
uniform bool DISABLE_ALMOST_EVERYTHING;
uniform sampler2D MODEL_TEXTURE;
in mat4 matrixOut;



float getColorDist(float color, float channel);
float calcFade(float destructionTimePassed, float destructionTime, int totalLayers, int midLayer, int layer);
float distFromCenter(float dist);
bool isOuter(float dist, float thickness);
float getDist(vec4 textureColor);
bool isInner(float dist, float thickness);
float distFromCenter(float dist);
bool isOuter(float dist, float thickness);
bool isInner(float dist, float thickness);
int normalFloatToInt(float nF);
float intToNormalFloat(int i);
int absInt(int x);
float max3(float x[3]);
float min3(float x[3]);
vec3 rGBToHSL(vec3 rgb);
vec3 hSLToRGB(vec3 hsl);





void main(){
    
    vec4 textureColor = texture(ourTexture, ourTexCoord);
    if(!DISABLE_ALMOST_EVERYTHING){
        vec4 borderTextureColor = texture(borderTexture, ourTexCoord);
        
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
        
        vec3 textureColorRGB = textureColor.xyz;
        vec3 textureColorHSL = rGBToHSL(textureColorRGB);
        vec3 textureColorHSLScaled = vec3(textureColorHSL.x, textureColorHSL.yz*SL_SCALE);
        vec3 textureColorRGBScaled = hSLToRGB(textureColorHSLScaled);
        vec4 finalTextureColor = vec4(textureColorRGBScaled, textureColor.a);

        if(borderCase == 0 && isOuter(dist, BORDER_THICKNESS))
            FragColor = fadeColor;
        else
            FragColor = fadeColor*finalTextureColor;
    }
    else{
        //FragColor = INNER_COLOR*textureColor;
        FragColor = vec4(0.5f*matrixOut[InstanceID % 4].xyz + 0.5f*vec3(1.0f), matrixOut[InstanceID % 4].a*0.5f + 0.5f);
    }
}

float getDist(vec4 textureColor){
        float dist = getColorDist(textureColor.x,redChannel_CONST)+getColorDist(textureColor.y,greenChannel_CONST)+getColorDist(textureColor.z,blueChannel_CONST)+getColorDist(textureColor.a,alphaChannel_CONST);
        float distNormalized = dist*maxDistNormal_CONST;
        return distNormalized;
}

float getColorDist(float color, float channel){
   return color*maxColorToDistRatio_CONST*pow(2.0f,(nrChannels_CONST-channel-1.0f)*8.0f); 
}
float calcFade(float destructionTimePassed, float destructionTime, int totalLayers, int midLayer, int layer){
    float fade = 0.0f;
	float timePassed;
	float b;
	float slope;
	if(layer > midLayer){
		timePassed = destructionTimePassed + destructionTime*float(totalLayers - 1 - layer);
		b = ABOVE_INTERCEPT;
		slope = ABOVE_SLOPE;

	}
	else{
		timePassed = destructionTimePassed + destructionTime*float(midLayer - layer);
		b = BELOW_INTERCEPT;
		slope = BELOW_SLOPE;
	}
    fade = b + slope*timePassed;
    return fade;
}
float distFromCenter(float dist){
    return 1.0f - dist;
}

bool isOuter(float dist, float thickness){
    return dist <= thickness;
}

bool isInner(float dist, float thickness){
    return !isOuter(dist,thickness);
}

int normalFloatToInt(float nF){
    int i = int(nF*float(maxInt_CONST));
    return i;
}

float intToNormalFloat(int i){
    float nF = float(i)/float(maxInt_CONST);
    return nF;
}

int absInt(int x){
    if (x >= 0)
        return x;
    else
        return -x;
}

float max3(float x[3]){
    float maxi = x[0];
    for(int i = 1; i < 3; i++){
        float cur = x[i];
        if(cur > maxi)
            maxi = cur;
    }
    return maxi;
}

float min3(float x[3]){
    float mini = x[0];
    for(int i = 1; i < 3; i++){
        float cur = x[i];
        if(cur < mini)
            mini = cur;
    }
    return mini;
}

vec3 rGBToHSL(vec3 rgb){
    float r = rgb.x;
    float g = rgb.y;
    float b = rgb.z;
    float mx = max3(float [3](r,g,b));
    float mn = min3(float [3](r,g,b));
    float c = mx - mn;
    float hPrime;
    if(c == 0.0f) 
        hPrime = -1.0f;
    else if (mx == r)
        hPrime = mod((g-b)/c, 6.0f);
    else if (mx == g)
        hPrime = (b - r)/c + 2.0f;
    else if (mx == b)
        hPrime = (r - g)/c + 4.0f;
    else
        hPrime == -2.0f;
    
    float h;
    if(hPrime >= 0.0f)
        h = 60.0f*hPrime;
    else // Hue is undefined
        h = 0.0f;
    
    float l = (1.0f/2.0f) * (mx + mn);
    
    float sL;
    if (l == 0.0f || l == 1.0f)
        sL = 0.0f;
    else
        sL = c/(1.0f - abs(2.0f*l - 1.0f));
    
    return vec3(h,sL,l);
}

vec3 hSLToRGB(vec3 hsl){
    float h = hsl.x;
    float sL = hsl.y;
    float l = hsl.z;
    float c = (1.0f - abs(2.0f*l - 1.0f))*sL;
    float hPrime = h/60.0f;
    float x = c*(1.0f - abs(mod(hPrime, 2.0f) - 1.0f));
    vec3 rgb1;
    if (hPrime < 0.0f)
        rgb1 = vec3(-1.0f,-1.0f,-1.0f); // Something went wrong!!!
    else if(hPrime < 1.0f)
        rgb1 = vec3(c, x, 0.0f);
    else if(hPrime < 2.0f)
        rgb1 = vec3(x, c, 0.0f);
    else if(hPrime < 3.0f)
        rgb1 = vec3(0.0f, c, x);
    else if(hPrime < 4.0f)
        rgb1 = vec3(0.0f, x, c);
    else if(hPrime < 5.0f)
        rgb1 = vec3(x, 0.0f, c);
    else if(hPrime < 6.0f)
        rgb1 = vec3(c, 0.0f, x);
    else
        rgb1 = vec3(-2.0f, -2.0f, -2.0f); // Something went wrong!!!
    
    float mn = l - c/2.0f;
    vec3 rgb = rgb1 + vec3(mn);
    return rgb;
}
