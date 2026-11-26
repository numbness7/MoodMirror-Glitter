#version 310 es
precision highp float;
precision highp int;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 ourTexCoord;
out mat3 matrixOut;

flat out int InstanceID;


uniform bool USE_MODEL_ARRAY;
uniform int COLS;
uniform int SIZE;
uniform highp sampler2D TRANS_L_TEXTURE;
uniform highp sampler2D ROTATE_L_TEXTURE;
uniform highp sampler2D SCALE_L_TEXTURE;
uniform highp isampler2D LAYER_PULSES_TEXTURE;
uniform highp usampler2D LAYER_TAGS_TEXTURE;
uniform float TIME_PASSED;
const int ATLAS_TEXTURES_CNT = 16;
uniform highp usampler2D BATCH_TAGS_TEXTURE;
uniform highp usampler2D ATLAS_TAGS_TEXTURE;
const int SHAPE_TEXTURES_CNT = 16;
const int STRIDE = 2;
const float RUMBLE_DIST_CONST = 0.1f;
uniform highp sampler2D SHAPES_TEXTURES[16];
uniform int SHAPES_PER_LAYER;
uniform int SHAPES_PER_BATCH;
uniform int TOTAL_BATCHES;
uniform int TOTAL_LAYERS;
uniform int BATCHES_PER_LAYER;
uniform int INSTANCE_ID_OFFSET;
#define PI (3.14159265358979323f)
uniform float TIME_ELAPSED;


mat3 getRotateMatrix( float angle);
mat3 getScaleMatrix( vec2 v2);
mat3 getTranslateMatrix( vec2 v2);
mat3 rotateMatrix(mat3 m3, float angle);
vec2 rotate(vec2 v2, float rads);
mat3 scaleMatrix(mat3  m3, vec2 v2);
mat3 translateMatrix(mat3 m3, vec2 v2);
vec2 boundTransformation(vec2 pos, vec2 mini, vec2 maxi, vec2 miniT, vec2 maxiT);
float boundTransformation(float x, float mini, float maxi, float miniT, float maxiT);
vec2 matrixMultiply(mat3 matrix, vec2 pos);
uint getLayer(uint layerTag);
uint getBatch(uint batchTag);
uint getCatch(uint catchTag, highp usampler2D CATCH_TAGS_TEXTURE, uint TOTAL_CATCHES);
ivec2 getTexelCoord(int index);
mat3 getMat3Model                       (highp sampler2D mat3ModelTexture, int iD);
vec4 getTexel                           (highp sampler2D texture, int index);
ivec4 getTexel                          (highp isampler2D texture, int index);
uvec4 getTexel                          (highp usampler2D texture, int index);



float snoise(vec2 v);
float cnoise(vec2 P);

struct SHAPE_PROPS { vec2 translation; float rotation; float scale; uint type; uint layerTag; uint batchTag; float seed; };
SHAPE_PROPS getShapeProps();

flat out int LAYER_OUT;
flat out int BATCH_OUT;
flat out uint TYPE_OUT;
flat out int SHAPE_TEXTURE_INDEX;
flat out int ATLAS_TAG_OUT;
/*
    pulsesL[layer] |= (pulses[RUMBLE_P])? (1 << 0) : 0;
    pulsesL[layer] |= (pulses[SPINL_P]) ? (1 << 1) : 0;
    pulsesL[layer] |= (pulses[SPINR_P]) ? (1 << 2) : 0;
    pulsesL[layer] |= (pulses[SCALE_P]) ? (1 << 3) : 0;
    pulsesL[layer] |= (pulses[FADE_P])  ? (1 << 4) : 0;
*/

void main(){
    InstanceID = gl_InstanceID + INSTANCE_ID_OFFSET;
    int id = InstanceID;
    ourTexCoord = aTexCoord;
    SHAPE_PROPS props = getShapeProps();
    int layer = int(getLayer(props.layerTag));
    LAYER_OUT = layer;
    BATCH_OUT = int(getBatch(props.batchTag));
    TYPE_OUT = props.type;
    ATLAS_TAG_OUT = int(getCatch(props.batchTag, ATLAS_TAGS_TEXTURE, uint(TOTAL_BATCHES)));
    vec2  transS = props.translation;
    float  scaleS = props.scale;
    float rotateS= props.rotation;

    mat3 matrix = mat3(1.0f);
    vec2  transL = getTexel(TRANS_L_TEXTURE, layer).xy;
    float  scaleL = getTexel(SCALE_L_TEXTURE, layer).x;
    //float rotateL=  getTexel(ROTATE_L_TEXTURE, layer).x;
    int pulsesL = getTexel(LAYER_PULSES_TEXTURE, layer).x;
    vec2 rumbleOffset = vec2(0.0f);
    vec2 surpriseOffset = vec2(0.0f);
    float spinR = 0.0f;
    float spinL = 0.0f;
    
/*
    pulsesL[layer] |= (pulses[RUMBLE_P])? (1 << 0) : 0;
    pulsesL[layer] |= (pulses[SPINL_P]) ? (1 << 1) : 0;
    pulsesL[layer] |= (pulses[SPINR_P]) ? (1 << 2) : 0;
    pulsesL[layer] |= (pulses[SCALE_P]) ? (1 << 3) : 0;
    pulsesL[layer] |= (pulses[FADE_P])  ? (1 << 4) : 0;
*/
    if((pulsesL & (1 << 0)) > 0){
        float o_x = snoise(vec2(20.0f*TIME_ELAPSED,props.seed*2.0f));
        float o_y = snoise(vec2(20.0f*TIME_ELAPSED,props.seed));
        rumbleOffset = 0.05f*vec2(o_x, o_y);
    }
    if((pulsesL & (1 << 1)) > 0){
        //float o_x = abs(sin(cnoise(vec2(0.1f*1.0f*TIME_ELAPSED,props.seed*4.0f))));
        //float o_x = cnoise(vec2(0.1f*1.0f*TIME_ELAPSED,props.seed*4.0f));
        //float o_y = cnoise(vec2(1.0f*TIME_ELAPSED,props.seed*3.0f));
        //surpriseOffset = 2.0f*vec2(o_x, o_y);
        spinL = 2.0f*PI*TIME_ELAPSED*0.1f;
    }
    if((pulsesL & (1 << 2)) > 0){
        spinR = -2.0f*PI*TIME_ELAPSED*0.1f;
    }
        
    matrix = scaleMatrix(matrix, vec2(scaleL*scaleS));
    matrix = rotateMatrix(matrix, rotateS + spinR + spinL);
    matrix = translateMatrix(matrix, rotate(transL, rotateS) + transS + rumbleOffset + surpriseOffset);
    //matrix = rotateMatrix(matrix, spinL);
    matrixOut = matrix;
    vec2 newPos = matrixMultiply(matrix, aPos.xy);
    gl_Position = vec4(newPos, 0.0f, 1.0f);
    
}

vec2 matrixMultiply(mat3 matrix, vec2 pos){
    vec3 v3 = vec3(pos,1.0f);
    vec2 nV2 = vec2(0.0f);
    for(int row = 0; row < 2; row++){
        for(int col = 0; col < 3; col++){
            nV2[row] += matrix[col][row] * v3[col];
        }
    }
    return nV2;
}

mat3 getMat3Model(highp sampler2D mat3ModelTexture, int iD){
    // CHATGPT START
    vec3 col0 = texelFetch( mat3ModelTexture, ivec2(3*iD,0), 0).rgb;
    vec3 col1 = texelFetch( mat3ModelTexture, ivec2(3*iD+1,0), 0).rgb;
    vec3 col2 = texelFetch( mat3ModelTexture, ivec2(3*iD+2,0), 0).rgb;
    mat3 model = mat3(1.0f);
    model[0] = col0;
    model[1] = col1;
    model[2] = col2;
    // CHATGPT END
    return model;
}



vec2 boundTransformation(vec2 pos, vec2 mini, vec2 maxi, vec2 miniT, vec2 maxiT){
    float xT = boundTransformation(pos.x, mini.x, maxi.x, miniT.x, maxiT.x);
    float yT = boundTransformation(pos.y, mini.y, maxi.y, miniT.y, maxiT.y);
    return vec2(xT, yT);
}
float boundTransformation(float x, float mini, float maxi, float miniT, float maxiT){
    float slope = (maxiT - miniT)/(maxi - mini);
    float intercept = miniT - slope*mini;
    return slope*x + intercept;
}
uint getLayer(uint layerTag){
    return getCatch(layerTag, LAYER_TAGS_TEXTURE, uint(TOTAL_LAYERS));
}
uint getBatch(uint batchTag){
    return getCatch(batchTag, BATCH_TAGS_TEXTURE, uint(TOTAL_BATCHES));
}
uint getCatch(uint catchTag, highp usampler2D CATCH_TAGS_TEXTURE, uint TOTAL_CATCHES){
    uint catchOut;
    for(catchOut = 0u; catchTag != uint(getTexel(CATCH_TAGS_TEXTURE, int(catchOut)).r) && catchOut <= uint(TOTAL_CATCHES); catchOut++){
    }
    if (catchOut == uint(TOTAL_CATCHES + 1u))
        return uint(TOTAL_CATCHES + 1u); // SomethlayerOutng went horrlayerOutbly wrong
    return catchOut;
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


SHAPE_PROPS getShapeProps(){
    vec4 shapeVec4s[STRIDE];
    int TOTAL_SHAPES = SHAPES_PER_LAYER*TOTAL_LAYERS;
    SHAPE_TEXTURE_INDEX = (InstanceID*SHAPE_TEXTURES_CNT)/TOTAL_SHAPES;
    int subIndex =     STRIDE * ((InstanceID) % (TOTAL_SHAPES/SHAPE_TEXTURES_CNT));
    for(int i = 0; i < STRIDE; i++){
        switch (SHAPE_TEXTURE_INDEX){
            case 0:
                shapeVec4s[i] = getTexel(SHAPES_TEXTURES[0], subIndex + i);
                break;
            case 1:
                shapeVec4s[i] = getTexel(SHAPES_TEXTURES[1], subIndex + i);
                break;
            case 2:
                shapeVec4s[i] = getTexel(SHAPES_TEXTURES[2], subIndex + i);
                break;
            case 3:
                shapeVec4s[i] = getTexel(SHAPES_TEXTURES[3], subIndex + i);
                break;
            case 4:
                shapeVec4s[i] = getTexel(SHAPES_TEXTURES[4], subIndex + i);
                break;
            case 5:
                shapeVec4s[i] = getTexel(SHAPES_TEXTURES[5], subIndex + i);
                break;
            case 6:
                shapeVec4s[i] = getTexel(SHAPES_TEXTURES[6], subIndex + i);
                break;
            case 7:
                shapeVec4s[i] = getTexel(SHAPES_TEXTURES[7], subIndex + i);
                break;
            case 8:
                shapeVec4s[i] = getTexel(SHAPES_TEXTURES[8], subIndex + i);
                break;
            case 9:
                shapeVec4s[i] = getTexel(SHAPES_TEXTURES[9], subIndex + i);
                break;
            case 10:
                shapeVec4s[i] = getTexel(SHAPES_TEXTURES[10], subIndex + i);
                break;
            case 11:
                shapeVec4s[i] = getTexel(SHAPES_TEXTURES[11], subIndex + i);
                break;
            case 12:
                shapeVec4s[i] = getTexel(SHAPES_TEXTURES[12], subIndex + i);
                break;
            case 13:
                shapeVec4s[i] = getTexel(SHAPES_TEXTURES[13], subIndex + i);
                break;
            case 14:
                shapeVec4s[i] = getTexel(SHAPES_TEXTURES[14], subIndex + i);
                break;
            case 15:
                shapeVec4s[i] = getTexel(SHAPES_TEXTURES[15], subIndex + i);
                break;
        }
    }
    vec2 translation = shapeVec4s[0].xy;
    float rotation   = shapeVec4s[0].z;
    float scale      = shapeVec4s[0].w;
    uint type        = uint(floatBitsToInt(shapeVec4s[1].x));
    uint layerTag    = floatBitsToUint(shapeVec4s[1].y);
    uint batchTag    = floatBitsToUint(shapeVec4s[1].z);
    float seed       = shapeVec4s[1].w;
    return SHAPE_PROPS(translation, rotation, scale, type, layerTag, batchTag, seed);
}
// struct SHAPE_PROPS { vec2 translation; float rotation; float scale; uint type; uint layerTag; uint batchTag; };

mat3 rotateMatrix(mat3 m3, float angle)
{
    mat3 rotm3 = mat3(0.0f);
    rotm3[0][0] = cos(angle);
    rotm3[1][0] = -sin(angle);
    rotm3[0][1] = sin(angle);
    rotm3[1][1] = cos(angle);
    rotm3[2][2] = 1.0f;
    return rotm3 * m3;
}

mat3 scaleMatrix(mat3 m3, vec2 v2)
{
    for(int i = 0; i < 2; i++){
        m3[i][i] *= v2[i];
    }
    return m3;
}

mat3 translateMatrix(mat3 m3, vec2 v2)
{
    for(int i = 0; i < 2; i++){
        m3[2][i] += v2[i];
    }
    return m3;
}

mat3 getRotateMatrix(float angle)
{
    mat3 rotm3 = mat3(0.0f);
    rotm3[0][0] = cos(angle);
    rotm3[1][0] = -sin(angle);
    rotm3[0][1] = sin(angle);
    rotm3[1][1] = cos(angle);
    rotm3[2][2] = 1.0f;
    return rotm3;
}

mat3 getScaleMatrix(vec2 v2)
{
    mat3 m3 = mat3(1.0f);
    for(int i = 0; i < 2; i++){
        m3[i][i] = v2[i];
    }
    return m3;
}

mat3 getTranslateMatrix(vec2 v2)
{
    mat3 m3 = mat3(1.0f);
    for(int i = 0; i < 2; i++){
        m3[2][i] = v2[i];
    }
    return m3;
}
vec2 rotate(vec2 v2, float rads){
    float originalAngle = atan(v2.y,v2.x);
    vec2 rotatedN = vec2(cos(originalAngle +  rads), sin( originalAngle + rads));
    return length(v2) * rotatedN;
}
// GLSL textureless classic 2D noise "cnoise",
// with an RSL-style periodic variant "pnoise".
// Author:  Stefan Gustavson (stefan.gustavson@gmail.com)
// Version: 2024-11-07
//
// Many thanks to Ian McEwan of Ashima Arts for the
// ideas for permutation and gradient selection.
//
// Copyright (c) 2011 Stefan Gustavson. All rights reserved.
// Distributed under the MIT license. See LICENSE file.
// https://github.com/stegu/webgl-noise
//

vec4 mod289(vec4 x)
{
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x)
{
  return mod289(((x*34.0)+10.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

vec2 fade(vec2 t) {
  return t*t*t*(t*(t*6.0-15.0)+10.0);
}

// Classic Perlin noise
float cnoise(vec2 P)
{
  vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
  vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
  Pi = mod289(Pi); // To avoid truncation effects in permutation
  vec4 ix = Pi.xzxz;
  vec4 iy = Pi.yyww;
  vec4 fx = Pf.xzxz;
  vec4 fy = Pf.yyww;

  vec4 i = permute(permute(ix) + iy);

  vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0 ;
  vec4 gy = abs(gx) - 0.5 ;
  vec4 tx = floor(gx + 0.5);
  gx = gx - tx;

  vec2 g00 = vec2(gx.x,gy.x);
  vec2 g10 = vec2(gx.y,gy.y);
  vec2 g01 = vec2(gx.z,gy.z);
  vec2 g11 = vec2(gx.w,gy.w);

  vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));

  float n00 = norm.x * dot(g00, vec2(fx.x, fy.x));
  float n01 = norm.y * dot(g01, vec2(fx.z, fy.z));
  float n10 = norm.z * dot(g10, vec2(fx.y, fy.y));
  float n11 = norm.w * dot(g11, vec2(fx.w, fy.w));

  vec2 fade_xy = fade(Pf.xy);
  vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
  float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
  return 2.3 * n_xy;
}

// Classic Perlin noise, periodic variant
float pnoise(vec2 P, vec2 rep)
{
  vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
  vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
  Pi = mod(Pi, rep.xyxy); // To create noise with explicit period
  Pi = mod289(Pi);        // To avoid truncation effects in permutation
  vec4 ix = Pi.xzxz;
  vec4 iy = Pi.yyww;
  vec4 fx = Pf.xzxz;
  vec4 fy = Pf.yyww;

  vec4 i = permute(permute(ix) + iy);

  vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0 ;
  vec4 gy = abs(gx) - 0.5 ;
  vec4 tx = floor(gx + 0.5);
  gx = gx - tx;

  vec2 g00 = vec2(gx.x,gy.x);
  vec2 g10 = vec2(gx.y,gy.y);
  vec2 g01 = vec2(gx.z,gy.z);
  vec2 g11 = vec2(gx.w,gy.w);

  vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));

  float n00 = norm.x * dot(g00, vec2(fx.x, fy.x));
  float n01 = norm.y * dot(g01, vec2(fx.z, fy.z));
  float n10 = norm.z * dot(g10, vec2(fx.y, fy.y));
  float n11 = norm.w * dot(g11, vec2(fx.w, fy.w));

  vec2 fade_xy = fade(Pf.xy);
  vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
  float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
  return 2.3 * n_xy;
}
//
// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : stegu
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
//               https://github.com/stegu/webgl-noise
// 

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+10.0)*x);
}

float snoise(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}