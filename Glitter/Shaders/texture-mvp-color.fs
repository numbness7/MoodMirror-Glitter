#version 330 core
precision mediump float;
in vec2 ourTexCoord;
out vec4 FragColor;


uniform int borderCase;
flat in int InstanceID;


uniform int maxLayer;
uniform float ABOVE_SLOPE;
uniform float BELOW_SLOPE;
uniform float ABOVE_INTERCEPT;
uniform float BELOW_INTERCEPT;
uniform int MID_LAYER;
uniform bool DO_FADE;
const float BORDER_THICKNESS = 0.5f;
uniform bool DISABLE_ALMOST_EVERYTHING;
uniform int  ATLAS_SHIFT_LEFT;
in mat3 matrixOut;


// TEXTURES
uniform sampler2D borderTexture;
uniform sampler2D ourTexture;
uniform sampler2D ONE_BIG_BEAUTIFUL_TEXTURE;
uniform isampler2D BATCH_NUMBER_TEXTURE;
uniform isampler2D LAYER_NUMBER_TEXTURE;
uniform sampler2D INNER_COLORS_TEXTURE;
uniform sampler2D OUTER_COLORS_TEXTURE;
uniform sampler2D SL_SCALES_TEXTURE;
uniform sampler2D DESTRUCTION_TIMES_TEXTURE;
uniform sampler2D DESTRUCTION_TIME_PASSEDS_TEXTURE;

uniform int BATCHES_CNT;


float distFromCenter(float dist);
bool isOuter(float dist, float thickness);
float getDist();
bool isInner(float dist, float thickness);
float distFromCenter(float dist);
bool isOuter(float dist, float thickness);
bool isInner(float dist, float thickness);
int absInt(int x);
float max3(float x[3]);
float min3(float x[3]);
vec3 rGBToHSL(vec3 rgb);
vec3 hSLToRGB(vec3 hsl);
uint getLayer();
uint getBatch();
float getFloatInterpolatedFromTexture(sampler2D floatTexture, vec2 coords);
float getDistFromTexture(sampler2D distTexture, vec2 coords);
ivec2 getTexelCoord(int index);
vec4 getTexel(sampler2D texture, int index);
ivec4 getTexel(isampler2D texture, int index);
uvec4 getTexel(usampler2D texture, int index);


void main(){
    
    vec4 finalFragColor = vec4(1.0f);
    int batchNumber = int(getBatch());
    int index = batchNumber - ATLAS_SHIFT_LEFT;
    if(index < 0)
        index = BATCHES_CNT + index;
    vec4 BenTextureColor = texture(ONE_BIG_BEAUTIFUL_TEXTURE, vec2(ourTexCoord.x/float(BATCHES_CNT) + float(int(index)%int(BATCHES_CNT))/float(BATCHES_CNT), ourTexCoord.y));
    finalFragColor = BenTextureColor;

    if(!DISABLE_ALMOST_EVERYTHING){
        vec4 borderTextureColor = texture(borderTexture, ourTexCoord);
        int layer = int(getLayer()); 
        int batch = int(getBatch());
        vec4 INNER_COLOR = getTexel(INNER_COLORS_TEXTURE, layer);
        vec4 OUTER_COLOR = getTexel(OUTER_COLORS_TEXTURE, layer);
        vec2 SL_SCALE = getTexel(SL_SCALES_TEXTURE, layer).rg;
        float destructionTime = getTexel(DESTRUCTION_TIMES_TEXTURE, layer).r;
        float destructionTimePassed = getTexel(DESTRUCTION_TIME_PASSEDS_TEXTURE, layer).r;
        
        float dist = getDist();
        float AMOUNT_INNER = 0.0f;
        if(dist < 0.0f)
            AMOUNT_INNER = 0.0f;
        else
            AMOUNT_INNER = dist/BORDER_THICKNESS;
        
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
        finalFragColor = BenTextureColor*borderColor;


        vec3 textureColorRGB = BenTextureColor.xyz;
        vec3 textureColorHSL = rGBToHSL(textureColorRGB);
        vec3 textureColorHSLScaled = vec3(textureColorHSL.x, textureColorHSL.yz*SL_SCALE);
        vec3 textureColorRGBScaled = hSLToRGB(textureColorHSLScaled);
        vec4 finalTextureColor = vec4(textureColorRGBScaled, BenTextureColor.a);
        finalFragColor = finalTextureColor*borderColor;
    }
    FragColor = finalFragColor;
}

ivec2 getTexelCoord(int index){
    return ivec2(index,0);
}



vec4 getTexel(sampler2D texture, int index){
    ivec2 texelCoord = getTexelCoord(index);
    return texelFetch(texture, texelCoord, 0);
}
ivec4 getTexel(isampler2D texture, int index){
    ivec2 texelCoord = getTexelCoord(index);
    return texelFetch(texture, texelCoord, 0);
}
uvec4 getTexel(usampler2D texture, int index){
    ivec2 texelCoord = getTexelCoord(index);
    return texelFetch(texture, texelCoord, 0);
}


float getFloatInterpolatedFromTexture(sampler2D floatTexture, vec2 coords){
    float f = texture(floatTexture, coords).r;
    return f;
}
float getDistFromTexture(sampler2D distTexture, vec2 coords){
    return getFloatInterpolatedFromTexture(distTexture, coords);
}

float getDist(){
    return getDistFromTexture(borderTexture, ourTexCoord);
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

uint getLayer(){
    return uint(texelFetch(LAYER_NUMBER_TEXTURE, ivec2(InstanceID, 0), 0).r);
}
uint getBatch(){
    return uint(texelFetch(BATCH_NUMBER_TEXTURE, ivec2(InstanceID, 0), 0).r);
}