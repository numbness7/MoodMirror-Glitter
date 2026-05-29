#version 330 core
out vec4 FragColor;

in vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 aColor;
uniform int shadeType;
uniform float minPercentage;

void main()
{
    
    
    

    
    vec3 center = vec3(model*vec4(vec3(0.0),1.0));
    vec3 up = vec3(model*vec4(0.0,1.0,0.0,1.0));
    vec3 right = vec3(model*vec4(1.0,0.0,0.0,1.0));
    float a_max = distance(right,center);
    float b_max = distance(up,center);
    vec3 diff = FragPos - center;
    vec3 base_diff = right - center;
    float angle = (atan(diff.y,diff.x)-atan(base_diff.y,base_diff.x));
    float max_distance = (a_max*b_max)/sqrt(pow(b_max*cos(angle),2) + pow(a_max*sin(angle),2));
    float cur_distance = distance(FragPos,center);
    
    

    if (max_distance >= cur_distance && max_distance*minPercentage <= cur_distance){
        if(shadeType == 0)
            FragColor = aColor;
        else if(shadeType == 1)
            FragColor = vec4((1-cur_distance/max_distance)*aColor.xyz,aColor.w);
        else if(shadeType == 2)
            FragColor = vec4((cur_distance/max_distance)*aColor.xyz,aColor.w);
        else
            FragColor = vec4(diff, 1.0f);
    }
    else
        discard;
        //FragColor = vec4(0.0f);
}
