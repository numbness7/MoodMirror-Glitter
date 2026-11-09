#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

const int INSTANCES_CONST = 100;
const bool USE_MODEL_ARRAY = true;
out vec2 ourTexCoord;



uniform mat4 models[INSTANCES_CONST];
uniform mat4 model;
uniform mat4 view;

void main(){
    if(USE_MODEL_ARRAY)
        gl_Position = view * models[gl_InstanceID] * vec4(aPos,1.0f);
    else
        gl_Position = view * model * vec4(aPos,1.0f);
    ourTexCoord = aTexCoord;
}
