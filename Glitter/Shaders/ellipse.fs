#version 330 core
out vec4 FragColor;

in vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 aColor;

void main()
{
    
    
    

    
    vec3 center = vec3(model*vec4(vec3(0.0),1.0));
    vec3 up = vec3(model*vec4(0.0,1.0,0.0,1.0));
    vec3 right = vec3(model*vec4(1.0,0.0,0.0,1.0));
    float a = distance(right,center);
    float b = distance(up,center);
    vec3 diff = FragPos - center;
    vec3 base_diff = right - center;
    float angle = (atan(diff.y,diff.x)-atan(base_diff.y,base_diff.x));
    float max_distance = (a*b)/sqrt(pow(b*cos(angle),2) + pow(a*sin(angle),2));
    
    

    if (max_distance >= distance(FragPos,center))
        FragColor = vec4(aColor, 1.0f);
    else
        //discard;
        //FragColor = vec4(0.0f);
        FragColor = vec4(vec3(1.0f),1.0f);
}
