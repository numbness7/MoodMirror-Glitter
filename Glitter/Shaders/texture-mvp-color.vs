#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 ourTexCoord;
out mat4 matrixOut;

flat out int InstanceID;


uniform bool USE_MODEL_ARRAY;
uniform mat4 model;
uniform mat4 view;
uniform int ROWS;
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

void main(){
    if(USE_MODEL_ARRAY){
        int id = gl_InstanceID;
        mat4 matrix = getModel();
        matrixOut = matrix;

        gl_Position = view * matrix * vec4(aPos, 1.0f);
    }
    else
        gl_Position = view * model * vec4(aPos,1.0f);
    
    ourTexCoord = aTexCoord;
    InstanceID = gl_InstanceID;
}

mat4 getModel(){
    // CHATGPT START
    vec4 col0 = texelFetch( MODEL_TEXTURE, ivec2(4*gl_InstanceID,0), 0);
    vec4 col1 = texelFetch( MODEL_TEXTURE, ivec2(4*gl_InstanceID+1,0), 0);
    vec4 col2 = texelFetch( MODEL_TEXTURE, ivec2(4*gl_InstanceID+2,0), 0);
    vec4 col3 = texelFetch( MODEL_TEXTURE, ivec2(4*gl_InstanceID+3,0), 0);
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