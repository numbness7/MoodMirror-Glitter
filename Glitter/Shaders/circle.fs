#version 330 core
out vec4 FragColor;

in vec3 ourColor;
uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

void main()
{
    vec2 res = vec2(800,600);
    vec2 cent = res/2;
    
    mat4 inv = inverse(model);
    //vec4 inv_FragCoord = inv*gl_FragCoord;
    vec4 inv_FragCoord = gl_FragCoord;
    
    
    float x_diff = inv_FragCoord.x;
    float y_diff = inv_FragCoord.y;
    
    float x_aspect = (cent.x/cent.y)*x_diff;

    float x_norm = x_aspect/(cent.x);
    float y_norm = y_diff/(cent.y);
    
    

    if (1 >= sqrt( pow(x_norm,2) + pow(y_norm,2)))
        FragColor = vec4(ourColor, 1.0f);
    else
        //discard;
        //FragColor = vec4(0.0f);
        FragColor = vec4(vec3(1.0f),1.0f);
}
