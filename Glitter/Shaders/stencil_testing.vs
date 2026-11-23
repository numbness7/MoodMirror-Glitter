#version 310 es
precision highp float;
precision highp int;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

flat out int InstanceID;
uniform int N;

void main()
{
    InstanceID = gl_InstanceID;
    TexCoords = aTexCoords;    
    gl_Position = vec4(aPos.xy + vec2((1.0f/float(N))*float(InstanceID)),0.0f, 1.0f);
    //gl_Position = vec4(aPos.xy, 0.0f, 1.0f);
}