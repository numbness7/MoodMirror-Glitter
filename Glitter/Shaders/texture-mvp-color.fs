#version 310 es
precision highp float;
precision highp int;
in vec2 ourTexCoord;
flat in int ATLAS_TAG_OUT;
flat in int LAYER_OUT;
flat in int BATCH_OUT;
out vec4 FragColor;


uniform int borderCase;
flat in int InstanceID;
flat in uint TYPE_OUT;
flat in int SHAPE_TEXTURE_INDEX;


uniform int maxLayer;
uniform float ABOVE_SLOPE;
uniform float BELOW_SLOPE;
uniform float ABOVE_INTERCEPT;
uniform float BELOW_INTERCEPT;
uniform int MID_LAYER;
uniform bool DO_FADE;
const float BORDER_THICKNESS = 1.0f;
uniform bool DISABLE_ALMOST_EVERYTHING;
uniform int  ATLAS_SHIFT_LEFT;
in mat3 matrixOut;
const int ATLAS_TEXTURES_CNT = 16;
uniform int TOTAL_BATCHES;
uniform int TOTAL_LAYERS;


// TEXTURES
uniform highp sampler2D SQUARE_BORDER_TEXTURE;
uniform highp sampler2D CIRCLE_BORDER_TEXTURE;
uniform highp sampler2D TRIANGLE_BORDER_TEXTURE;
uniform highp sampler2D ourTexture;
uniform highp sampler2D ATLAS_TEXTURES[ATLAS_TEXTURES_CNT];
uniform highp sampler2D INNER_COLORS_TEXTURE;
uniform highp sampler2D OUTER_COLORS_TEXTURE;
uniform highp sampler2D SL_SCALES_TEXTURE;

uniform int BATCHES_CNT;
uniform int BATCHES_PER_ATLAS_TEXTURE;


float distFromCenter(float dist);
bool isOuter(float dist, float thickness);
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
float getDistFromTexture(highp sampler2D distTexture, vec2 coords);
ivec2 getTexelCoord(int index);
vec4 getTexel(highp sampler2D texture, int index);
ivec4 getTexel(highp isampler2D texture, int index);
uvec4 getTexel(highp usampler2D texture, int index);
vec4 getBenTexture();


void main(){
    vec4 finalFragColor = vec4(1.0f);
    vec4 BenTextureColor = getBenTexture();
    finalFragColor = BenTextureColor;

    if(!DISABLE_ALMOST_EVERYTHING){
        int layer = int(getLayer()); 
        int batch = int(getBatch());
        vec4 INNER_COLOR = getTexel(INNER_COLORS_TEXTURE, layer);
        vec4 OUTER_COLOR = getTexel(OUTER_COLORS_TEXTURE, layer);
        vec2 SL_SCALE = getTexel(SL_SCALES_TEXTURE, layer).rg;
        
        float dist;
        switch(TYPE_OUT){
            case 0u:
                dist = getDistFromTexture(TRIANGLE_BORDER_TEXTURE, ourTexCoord);
                break;
            case 1u:
                dist = getDistFromTexture(SQUARE_BORDER_TEXTURE, ourTexCoord);
                break;
            case 2u:
                dist = getDistFromTexture(CIRCLE_BORDER_TEXTURE, ourTexCoord);
                break;
        }
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
    if(LAYER_OUT == -1)
        FragColor = vec4(1.0f,0.0f,1.0f,1.0f);
    // switch(LAYER_OUT % 16){
    //     case 0:
    //         FragColor *= vec4(1.0f,1.0f,1.0f,1.0f);
    //         break;
    //     case 1:
    //         FragColor *= vec4(0.0f,0.0f,0.2f,1.0f);
    //         break;
    //     case 2:
    //         FragColor *= vec4(0.0f,0.0f,0.3f,1.0f);
    //         break;
    //     case 3:
    //         FragColor *= vec4(0.0f,0.0f,0.4f,1.0f);
    //         break;
    //     case 4:
    //         FragColor *= vec4(0.0f,0.0f,0.5f,1.0f);
    //         break;
    //     case 5:
    //         FragColor *= vec4(0.0f,0.0f,0.6f,1.0f);
    //         break;
    //     case 6:
    //         FragColor *= vec4(0.0f,0.0f,0.7f,1.0f);
    //         break;
    //     case 7:
    //         FragColor *= vec4(0.0f,0.0f,0.8f,1.0f);
    //         break;
    //     case 8:
    //         FragColor *= vec4(0.0f,0.0f,0.9f,1.0f);
    //         break;
    //     case 9:
    //         FragColor *= vec4(0.0f,0.0f,1.0f,1.0f);
    //         break;
    //     case 10:
    //         FragColor *= vec4(0.0f,0.1f,0.0f,1.0f);
    //         break;
    //     case 11:
    //         FragColor *= vec4(0.0f,0.2f,0.0f,1.0f);
    //         break;
    //     case 12:
    //         FragColor *= vec4(0.0f,0.3f,0.0f,1.0f);
    //         break;
    //     case 13:
    //         FragColor *= vec4(0.0f,0.4f,0.0f,1.0f);
    //         break;
    //     case 14:
    //         FragColor *= vec4(0.0f,0.5f,0.0f,1.0f);
    //         break;
    //     case 15:
    //         FragColor *= vec4(0.0f,0.6f,0.0f,1.0f);
    //         break;
    //     case 16:
    //         FragColor *= vec4(0.0f,0.7f,0.0f,1.0f);
    //         break;
    //     case 17:
    //         FragColor *= vec4(0.0f,0.8f,0.0f,1.0f);
    //         break;
    //     case 18:
    //         FragColor *= vec4(0.0f,0.9f,0.0f,1.0f);
    //         break;
    //     case 19:
    //         FragColor *= vec4(0.0f,1.0f,0.0f,1.0f);
    //         break;
    //     case 20:
    //         FragColor *= vec4(0.1f,0.0f,0.0f,1.0f);
    //         break;
    //     case 21:
    //         FragColor *= vec4(0.2f,0.0f,0.0f,1.0f);
    //         break;
    //     case 22:
    //         FragColor *= vec4(0.3f,0.0f,0.0f,1.0f);
    //         break;
    //     case 23:
    //         FragColor *= vec4(0.4f,0.0f,0.0f,1.0f);
    //         break;
    //     case 24:
    //         FragColor *= vec4(0.5f,0.0f,0.0f,1.0f);
    //         break;
    //     case 25:
    //         FragColor *= vec4(0.6f,0.0f,0.0f,1.0f);
    //         break;
    //     case 26:
    //         FragColor *= vec4(0.7f,0.0f,0.0f,1.0f);
    //         break;
    //     case 27:
    //         FragColor *= vec4(0.8f,0.0f,0.0f,1.0f);
    //         break;
    //     case 28:
    //         FragColor *= vec4(0.9f,0.0f,0.0f,1.0f);
    //         break;
    //     case 29:
    //         FragColor *= vec4(1.0f,0.0f,0.0f,1.0f);
    //         break;
    //     case 30:
    //         FragColor *= vec4(0.1f,0.1f,0.1f,1.0f);
    //         break;
    //     case 31:
    //         FragColor *= vec4(0.2f,0.2f,0.2f,1.0f);
    //         break;
    //     case 32:
    //         FragColor *= vec4(0.2f,0.2f,0.2f,1.0f);
    //         break;
    // }
}

ivec2 getTexelCoord(int index){
    return ivec2(index,0);
}



vec4 getTexel(highp sampler2D texture, int index){
    ivec2 texelCoord = getTexelCoord(index);
    return texelFetch(texture, texelCoord, 0);
}
ivec4 getTexel(highp isampler2D texture, int index){
    ivec2 texelCoord = getTexelCoord(index);
    return texelFetch(texture, texelCoord, 0);
}
uvec4 getTexel(highp usampler2D texture, int index){
    ivec2 texelCoord = getTexelCoord(index);
    return texelFetch(texture, texelCoord, 0);
}


float getFloatInterpolatedFromTexture(highp sampler2D floatTexture, vec2 coords){
    float f = texture(floatTexture, coords).r;
    return f;
}
float getDistFromTexture(highp sampler2D distTexture, vec2 coords){
    return getFloatInterpolatedFromTexture(distTexture, coords);
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
    return uint(LAYER_OUT);
}
uint getBatch(){
    return uint(BATCH_OUT);
}

vec4 getBenTexture(){
    int batchNumber = int(getBatch());
    int index = ATLAS_TAG_OUT;
    if(index < 0)
        index = BATCHES_CNT + index;
    int atlasIndex = index/BATCHES_PER_ATLAS_TEXTURE;
    int subIndex = index % BATCHES_PER_ATLAS_TEXTURE;
    vec2 coords = vec2(ourTexCoord.x/float(BATCHES_PER_ATLAS_TEXTURE) + float(int(subIndex)%int(BATCHES_PER_ATLAS_TEXTURE))/float(BATCHES_PER_ATLAS_TEXTURE), ourTexCoord.y);
    //vec2 coords = vec2(ourTexCoord.x, ourTexCoord.y);
    vec4 BenTextureColor;
    BenTextureColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    switch(atlasIndex){
        case 0:
            BenTextureColor = texture(ATLAS_TEXTURES[0], coords);
            break;
        case 1:
            BenTextureColor = texture(ATLAS_TEXTURES[1], coords);
            break;
        case 2:
            BenTextureColor = texture(ATLAS_TEXTURES[2], coords);
            break;
        case 3:
            BenTextureColor = texture(ATLAS_TEXTURES[3], coords);
            break;
        case 4:
            BenTextureColor = texture(ATLAS_TEXTURES[4], coords);
            break;
        case 5:
            BenTextureColor = texture(ATLAS_TEXTURES[5], coords);
            break;
        case 6:
            BenTextureColor = texture(ATLAS_TEXTURES[6], coords);
            break;
        case 7:
            BenTextureColor = texture(ATLAS_TEXTURES[7], coords);
            break;
        case 8:
            BenTextureColor = texture(ATLAS_TEXTURES[8], coords);
            break;
        case 9:
            BenTextureColor = texture(ATLAS_TEXTURES[9], coords);
            break;
        case 10:
            BenTextureColor = texture(ATLAS_TEXTURES[10], coords);
            break;
        case 11:
            BenTextureColor = texture(ATLAS_TEXTURES[11], coords);
            break;
        case 12:
            BenTextureColor = texture(ATLAS_TEXTURES[12], coords);
            break;
        case 13:
            BenTextureColor = texture(ATLAS_TEXTURES[13], coords);
            break;
        case 14:
            BenTextureColor = texture(ATLAS_TEXTURES[14], coords);
            break;
        case 15:
            BenTextureColor = texture(ATLAS_TEXTURES[15], coords);
            break;
    }
    return BenTextureColor;
}