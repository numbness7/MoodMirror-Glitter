#version 310 es
precision highp float;
precision highp int;
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
const int TEXTURE_ATLASES_CNT = 16;
uniform highp sampler2D SUPER_TEXTURE_VS[TEXTURE_ATLASES_CNT];
uniform highp int ATLAS_SHIFT_LEFT;
uniform highp int TEXTURES_PER_ATLAS;
uniform highp int SHAPE_CNT;


out flat int INSTANCE_ID_OUT;
out flat int COLOR_CHANGE_OUT;


struct SHAPE_PROPS{
    vec2 pos;
    int colorChange;
    float rotation;
};

SHAPE_PROPS getPosition(int id, highp sampler2D atlases[TEXTURE_ATLASES_CNT], int atlasShiftLeft, int texturesPerAtlas, int stride, int totalDataBlocks){
    int index = stride*(id - atlasShiftLeft);
    if(index < 0)
        index = stride*totalDataBlocks + index;
    int atlasIndex = index/(texturesPerAtlas*stride);
    int subIndex = index % (texturesPerAtlas*stride);
    vec2 position;
    float floats[4];
    for(int i = 0; i < stride; i++){
        switch(atlasIndex){
            case 0:
                floats[i] = texelFetch(atlases[0], ivec2(subIndex + i, 0), 0).r;
                break;
            case 1:
                floats[i] = texelFetch(atlases[1], ivec2(subIndex + i, 0), 0).r;
                break;
            case 2:
                floats[i] = texelFetch(atlases[2], ivec2(subIndex + i, 0), 0).r;
                break;
            case 3:
                floats[i] = texelFetch(atlases[3], ivec2(subIndex + i, 0), 0).r;
                break;
            case 4:
                floats[i] = texelFetch(atlases[4], ivec2(subIndex + i, 0), 0).r;
                break;
            case 5:
                floats[i] = texelFetch(atlases[5], ivec2(subIndex + i, 0), 0).r;
                break;
            case 6:
                floats[i] = texelFetch(atlases[6], ivec2(subIndex + i, 0), 0).r;
                break;
            case 7:
                floats[i] = texelFetch(atlases[7], ivec2(subIndex + i, 0), 0).r;
                break;
            case 8:
                floats[i] = texelFetch(atlases[8], ivec2(subIndex + i, 0), 0).r;
                break;
            case 9:
                floats[i] = texelFetch(atlases[9], ivec2(subIndex + i, 0), 0).r;
                break;
            case 10:
                floats[i] = texelFetch(atlases[10], ivec2(subIndex + i, 0), 0).r;
                break;
            case 11:
                floats[i] = texelFetch(atlases[11], ivec2(subIndex + i, 0), 0).r;
                break;
            case 12:
                floats[i] = texelFetch(atlases[12], ivec2(subIndex + i, 0), 0).r;
                break;
            case 13:
                floats[i] = texelFetch(atlases[13], ivec2(subIndex + i, 0), 0).r;
                break;
            case 14:
                floats[i] = texelFetch(atlases[14], ivec2(subIndex + i, 0), 0).r;
                break;
            case 15:
                floats[i] = texelFetch(atlases[15], ivec2(subIndex + i, 0), 0).r;
                break;
        }
    }
    return SHAPE_PROPS(vec2(floats[0],floats[1]),  floatBitsToInt(floats[2]), floats[3]);
}

void main(){
    INSTANCE_ID_OUT = gl_InstanceID;
    SHAPE_PROPS props = getPosition(INSTANCE_ID_OUT, SUPER_TEXTURE_VS, ATLAS_SHIFT_LEFT, TEXTURES_PER_ATLAS, 4, SHAPE_CNT);
    float oAngle = atan(aPos.y, aPos.x);
    vec2 rotN = vec2(cos(oAngle + props.rotation), sin(oAngle + props.rotation));
    vec2 newAPos = length(aPos)*rotN;
    gl_Position = vec4((newAPos.xy*(1.0f/16.0f)) + props.pos, 0.0f, 1.0f);
    COLOR_CHANGE_OUT = props.colorChange;
}