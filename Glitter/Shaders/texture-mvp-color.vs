#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 ourTexCoord;



uniform bool USE_MODEL_ARRAY;
uniform float SCALE;
uniform mat4 model;
uniform mat4 view;
uniform int ROWS;
uniform int COLS;
uniform sampler2D POSITION_TEXTURE;

mat4 scaleMatrix(mat4 matrix, vec4 scale);
mat4 translateMatrix(mat4 matrix, vec4 translation);
vec2 boundTransformation(vec2 pos, vec2 mini, vec2 maxi, vec2 miniT, vec2 maxiT);
float boundTransformation(float x, float mini, float maxi, float miniT, float maxiT);
vec2 getPosition(sampler2D position_texture);

void main(){
    if(USE_MODEL_ARRAY){
        vec2 position = getPosition(POSITION_TEXTURE);
        mat4 matrix = mat4(1.0f);
        matrix = translateMatrix(matrix, vec4(position,vec2(0.0f)));
        matrix = scaleMatrix(matrix, vec4(SCALE,SCALE,1.0f,1.0f));
        gl_Position = view * matrix * vec4(aPos,1.0f);
    }
    else
        gl_Position = view * model * vec4(aPos,1.0f);
    ourTexCoord = aTexCoord;
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
vec2 getPosition(sampler2D position_texture){
    int id = gl_InstanceID;
    // CHATGPT START
    ivec2 texelCoord = ivec2(
        id % COLS,
        id / COLS
    );
    vec2 position = texelFetch(
        POSITION_TEXTURE,
        texelCoord,
        0
    ).rg;
    // CHATGPT END
    return position;
}