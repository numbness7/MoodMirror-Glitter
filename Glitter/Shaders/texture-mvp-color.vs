#version 330 core
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
uniform sampler2D MODEL_TEXTURE;


mat4 scaleMatrix(mat4 matrix, vec4 scale);
mat4 translateMatrix(mat4 matrix, vec4 translation);
vec2 boundTransformation(vec2 pos, vec2 mini, vec2 maxi, vec2 miniT, vec2 maxiT);
float boundTransformation(float x, float mini, float maxi, float miniT, float maxiT);
vec2 getPosition(sampler2D position_texture);
mat4 getModel();
vec2 getPosition(int id, sampler2D position_texture);
mat3 getMat3Model(sampler2D mat3ModelTexture, int iD);
vec2 matrixMultiply(mat3 matrix, vec2 pos);
mat4 mat3ToMat4(mat3 matrix3);

void main(){
    if(USE_MODEL_ARRAY){
        int id = gl_InstanceID;
        mat3 matrix = getMat3Model(MODEL_TEXTURE, id);
        matrixOut = matrix;
        vec2 newPos = matrixMultiply(matrix, aPos.xy);
        //gl_Position = vec4(newPos, 0.0f, 1.0f);
        gl_Position = vec4(newPos, 0.0f, 1.0f);
    }
    else{
        gl_Position = view * model * vec4(aPos,1.0f);
    }
    
    ourTexCoord = aTexCoord;
    InstanceID = gl_InstanceID;
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

mat3 getMat3Model(sampler2D mat3ModelTexture, int iD){
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

mat4 getMat4Model(sampler2D mat4ModelTexture, int iD){
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
vec2 getPosition(int id, sampler2D position_texture){
    // CHATGPT START
    ivec2 texelCoord = ivec2(
        id % COLS,
        id / COLS
    );
    vec2 position = texelFetch(
        position_texture,
        texelCoord,
        0
    ).rg;
    // CHATGPT END
    return position;
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