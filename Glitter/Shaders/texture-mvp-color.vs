#version 310 es
precision highp float;
precision highp int;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 ourTexCoord;
out mat3 matrixOut;

flat out int InstanceID;


uniform bool USE_MODEL_ARRAY;
uniform mat4 model;
uniform mat4 view;
uniform int COLS;
uniform int SIZE;
uniform highp sampler2D TRANS_L_TEXTURE;
uniform highp sampler2D ROTATE_L_TEXTURE;
uniform highp sampler2D SCALE_L_TEXTURE;
uniform highp sampler2D SHAPES_TEXTURE;
uniform int SHAPES_PER_LAYER;
uniform int SHAPES_PER_BATCH;
uniform int TOTAL_BATCHES;
uniform int TOTAL_LAYERS;


mat4 scaleMatrix(mat4 matrix, vec4 scale);
mat4 translateMatrix(mat4 matrix, vec4 translation);
vec2 boundTransformation(vec2 pos, vec2 mini, vec2 maxi, vec2 miniT, vec2 maxiT);
float boundTransformation(float x, float mini, float maxi, float miniT, float maxiT);
mat4 getModel();
vec2 matrixMultiply(mat3 matrix, vec2 pos);
mat4 mat3ToMat4(mat3 matrix3);
uint getLayer();
uint getBatch();
ivec2 getTexelCoord(int index);
float getFloatInterpolatedFromTexture   (highp sampler2D floatTexture, vec2 coords);
float getDistFromTexture                (highp sampler2D distTexture, vec2 coords);
mat3 getMat3Model                       (highp sampler2D mat3ModelTexture, int iD);
vec4 getTexel                           (highp sampler2D texture, int index);
ivec4 getTexel                          (highp isampler2D texture, int index);
uvec4 getTexel                          (highp usampler2D texture, int index);

struct SHAPE_PROPS { mat3 translation; mat3 rotation; mat3 scale; uint type; };
SHAPE_PROPS getShapeProps();

flat out int LAYER_OUT;
flat out int BATCH_OUT;
flat out uint TYPE_OUT;

void main(){
    InstanceID = gl_InstanceID;
    int id = InstanceID;
    ourTexCoord = aTexCoord;
    int layer = int(getLayer());
    LAYER_OUT = layer;
    BATCH_OUT = int(getBatch());
    SHAPE_PROPS props = getShapeProps();
    TYPE_OUT = props.type;
    if(USE_MODEL_ARRAY){
        mat3  transS = props.translation;
        mat3  scaleS = props.scale;
        mat3 rotateS= props.rotation;

        mat3  transL = getMat3Model(TRANS_L_TEXTURE, layer);
        mat3  scaleL = getMat3Model(SCALE_L_TEXTURE, layer);
        mat3 rotateL= getMat3Model(ROTATE_L_TEXTURE, layer);
        //mat3 matrix =  transS*rotateS*rotateL*scaleS*scaleL;
        mat3 matrix =  (transS+(rotateS*transL))*(rotateS+rotateL)*matrixCompMult(scaleS,scaleL);
        //mat3 matrix =  transS*rotateS*scaleS;
        //mat3 matrix =  transL*rotateL*scaleL;
        //mat3 matrix = mat3(1.0f);
        matrixOut = matrix;
        vec2 newPos = matrixMultiply(matrix, aPos.xy);
        //gl_Position = vec4(newPos, 0.0f, 1.0f);
        gl_Position = vec4(newPos, 0.0f, 1.0f);
    }
    else{
        gl_Position = view * model * vec4(aPos,1.0f);
    }
    
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

mat4 getMat4Model(highp sampler2D mat4ModelTexture, int iD){
    // CHATGPT START
    vec4 col0 = texelFetch( mat4ModelTexture, ivec2(4*iD,0), 0);
    vec4 col1 = texelFetch( mat4ModelTexture, ivec2(4*iD+1,0), 0);
    vec4 col2 = texelFetch( mat4ModelTexture, ivec2(4*iD+2,0), 0);
    vec4 col3 = texelFetch( mat4ModelTexture, ivec2(4*iD+3,0), 0);
    mat4 model = mat4(1.0f);
    model[0] = col0;
    model[1] = col1;
    model[2] = col2;
    model[3] = col3;
    // CHATGPT END
    return model;
}

mat4 scaleMatrix(mat4 matrix, vec4 scale){
    for(int i = 0; i < 4; i++){
        matrix[i][i] *= scale[i];
    }
    return matrix;
}
mat4 translateMatrix(mat4 matrix, vec4 translation){
    for(int i = 0; i < 4; i++){
        matrix[3][i] += translation[i];
    }
    return matrix;
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
mat4 mat3ToMat4(mat3 matrix3){
    mat4 matrix4 = mat4(1.0f);
    for(int row = 0; row < 3; row++){
        for(int col = 0; col < 3; col++){
            matrix4[col][row] = matrix3[col][row];
        }
    }
    return matrix4;

}
uint getLayer(){
    return uint(((SHAPES_PER_LAYER*TOTAL_LAYERS) - InstanceID)/SHAPES_PER_LAYER);
}
uint getBatch(){
    return uint(((SHAPES_PER_BATCH*TOTAL_BATCHES) - InstanceID)/SHAPES_PER_BATCH);
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

SHAPE_PROPS getShapeProps(){
    vec4 shapeVec4s[9];
    for(int i = 0; i < 9; i++){
        shapeVec4s[i] = getTexel(SHAPES_TEXTURE, InstanceID*9 + i);
    }
    mat3 translation;
    mat3 rotation;
    mat3 scale;
    uint type;
    translation[0] = vec3(shapeVec4s[0]); 
    translation[1] = vec3(shapeVec4s[1]);
    translation[2] = vec3(shapeVec4s[2]);
    rotation[0]    = vec3(shapeVec4s[3]);
    rotation[1]    = vec3(shapeVec4s[4]);
    rotation[2]    = vec3(shapeVec4s[5]);
    scale[0]       = vec3(shapeVec4s[6]);
    scale[1]       = vec3(shapeVec4s[7]);
    scale[2]       = vec3(shapeVec4s[8]);
    type           = uint(floatBitsToInt(shapeVec4s[0].a));
    return SHAPE_PROPS(translation, rotation, scale, type);
}
//struct SHAPE_PROPS { mat3 translation; mat3 rotation; mat3 scale; uint type; };