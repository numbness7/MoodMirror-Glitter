#version 310 es
precision highp float;
precision highp int;
out vec4 FragColor;
flat in int InstanceID;
uniform int N;
uniform int MAX_OVERLAPPING_FRAGMENTS;

void main()
{
    vec4 color;
    switch(InstanceID % 4){
        case 0:
            color = vec4(1.00, 0.00, 0.00, 0.2f);
            break;
        case 1:
            color = vec4(0.00, 1.00, 0.00, 0.2f);
            break;
        case 2:
            color = vec4(0.00, 0.00, 1.00, 0.2f);
            break;
        default:
            color = vec4(1.00, 1.00, 1.00, 0.2f);
            break;
    }
    FragColor = color;
}