#version 310 es
precision highp float;
precision highp int;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 ourTexCoord;
out mat3 matrixOut;

flat out int InstanceID;


uniform mat4 view;
uniform highp sampler2D TRANS_L_TEXTURE;
uniform highp sampler2D ROTATE_L_TEXTURE;
uniform highp sampler2D SCALE_L_TEXTURE;
const int TEXTURE_ATLASES_CNT = 16;
uniform highp sampler2D SUPER_TEXTURE_VS[TEXTURE_ATLASES_CNT];
uniform highp int SHAPES_ATLAS_SHIFT_LEFT;
uniform highp int SHAPES_PER_ATLAS;
uniform highp int SHAPE_CNT;

mat4 scaleMatrix(mat4 matrix, vec4 scale);
mat4 translateMatrix(mat4 matrix, vec4 translation);
vec2 boundTransformation(vec2 pos, vec2 mini, vec2 maxi, vec2 miniT, vec2 maxiT);
float boundTransformation(float x, float mini, float maxi, float miniT, float maxiT);
mat4 getModel();
vec2 matrixMultiply(mat3 matrix, vec2 pos);
mat4 mat3ToMat4(mat3 matrix3);
ivec2 getTexelCoord(int index);
float getFloatInterpolatedFromTexture   (highp sampler2D floatTexture, vec2 coords);
float getDistFromTexture                (highp sampler2D distTexture, vec2 coords);
mat3 getMat3Model                       (highp sampler2D mat3ModelTexture, int iD);
vec4 getTexel                           (highp sampler2D texture, int index);
ivec4 getTexel                          (highp isampler2D texture, int index);
uvec4 getTexel                          (highp usampler2D texture, int index);

flat out int LAYER_OUT;
flat out int BATCH_OUT;
flat out int TYPE_OUT;
struct SHAPE_PROPS{ mat3 transMat; mat3 rotMat; mat3 scaleMat; int layer; int batch; int type; };
SHAPE_PROPS getShapeProps(int id, highp sampler2D atlases[TEXTURE_ATLASES_CNT], int atlasShiftLeft, int texturesPerAtlas, int stride, int totalDataBlocks);

void main(){
    ourTexCoord = aTexCoord;
    InstanceID = gl_InstanceID;
    SHAPE_PROPS shapeProps = getShapeProps(InstanceID, SUPER_TEXTURE_VS, SHAPES_ATLAS_SHIFT_LEFT, SHAPES_PER_ATLAS, 9, SHAPE_CNT);
    mat3  transS = shapeProps.transMat;
    mat3  scaleS = shapeProps.scaleMat;
    mat3 rotateS= shapeProps.rotMat;
    LAYER_OUT = shapeProps.layer;
    BATCH_OUT = shapeProps.batch;
    TYPE_OUT = shapeProps.type;
    mat3  transL = getMat3Model(TRANS_L_TEXTURE, LAYER_OUT);
    mat3  scaleL = getMat3Model(SCALE_L_TEXTURE, LAYER_OUT);
    mat3 rotateL= getMat3Model(ROTATE_L_TEXTURE, LAYER_OUT);
    mat3 matrix =  (transS+(rotateS*transL))*(rotateS+rotateL)*matrixCompMult(scaleS,scaleL);
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
SHAPE_PROPS getShapeProps(int id, highp sampler2D atlases[TEXTURE_ATLASES_CNT], int atlasShiftLeft, int texturesPerAtlas, int stride, int totalDataBlocks){
    int index = stride*(id - atlasShiftLeft);
    if(index < 0)
        index = stride*totalDataBlocks + index;
    int atlasIndex = index/(texturesPerAtlas*stride);
    int subIndex = index % (texturesPerAtlas*stride);
    vec4 vec4s[32];
    for(int i = 0; i < stride; i++){
        switch(atlasIndex){
            case 0:
                vec4s[i] = texelFetch(atlases[0], ivec2(subIndex + i, 0), 0).rgba;
                break;
            case 1:
                vec4s[i] = texelFetch(atlases[1], ivec2(subIndex + i, 0), 0).rgba;
                break;
            case 2:
                vec4s[i] = texelFetch(atlases[2], ivec2(subIndex + i, 0), 0).rgba;
                break;
            case 3:
                vec4s[i] = texelFetch(atlases[3], ivec2(subIndex + i, 0), 0).rgba;
                break;
            case 4:
                vec4s[i] = texelFetch(atlases[4], ivec2(subIndex + i, 0), 0).rgba;
                break;
            case 5:
                vec4s[i] = texelFetch(atlases[5], ivec2(subIndex + i, 0), 0).rgba;
                break;
            case 6:
                vec4s[i] = texelFetch(atlases[6], ivec2(subIndex + i, 0), 0).rgba;
                break;
            case 7:
                vec4s[i] = texelFetch(atlases[7], ivec2(subIndex + i, 0), 0).rgba;
                break;
            case 8:
                vec4s[i] = texelFetch(atlases[8], ivec2(subIndex + i, 0), 0).rgba;
                break;
            case 9:
                vec4s[i] = texelFetch(atlases[9], ivec2(subIndex + i, 0), 0).rgba;
                break;
            case 10:
                vec4s[i] = texelFetch(atlases[10], ivec2(subIndex + i, 0), 0).rgab;
                break;
            case 11:
                vec4s[i] = texelFetch(atlases[11], ivec2(subIndex + i, 0), 0).rgab;
                break;
            case 12:
                vec4s[i] = texelFetch(atlases[12], ivec2(subIndex + i, 0), 0).rgab;
                break;
            case 13:
                vec4s[i] = texelFetch(atlases[13], ivec2(subIndex + i, 0), 0).rgab;
                break;
            case 14:
                vec4s[i] = texelFetch(atlases[14], ivec2(subIndex + i, 0), 0).rgab;
                break;
            case 15:
                vec4s[i] = texelFetch(atlases[15], ivec2(subIndex + i, 0), 0).rgab;
                break;
        }
    }

    SHAPE_PROPS props;
    props.transMat = mat3(vec4s[0].rgb, vec4s[1].rgb, vec4s[2].rgb);
    props.rotMat =   mat3(vec4s[3].rgb, vec4s[4].rgb, vec4s[5].rgb);
    props.scaleMat = mat3(vec4s[6].rgb, vec4s[7].rgb, vec4s[8].rgb);
    props.batch =     floatBitsToInt(vec4s[0].a);
    props.layer =     floatBitsToInt(vec4s[1].a);
    props.type =      floatBitsToInt(vec4s[2].a);
            
    return props;
}
// struct SHAPE_PROPS{ mat3 transMat; mat3 rotMat; mat3 scaleMat; int layer; int batch; };
