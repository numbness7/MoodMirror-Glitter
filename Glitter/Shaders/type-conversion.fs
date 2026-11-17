#version 310 es
precision highp float;
precision highp int;
out highp vec4 FragColor;
in flat int INSTANCE_ID_OUT;
in flat int COLOR_CHANGE_OUT;

void main(){
    float change = float(INSTANCE_ID_OUT)/16.0f;
    switch (COLOR_CHANGE_OUT){
        case 0:
            FragColor = vec4(change,vec3(1.0f));
            break;
        case 1:
            FragColor = vec4(1.0f,change,vec2(1.0f));
            break;
        case 2:
            FragColor = vec4(1.0f,1.0f,change,1.0f);
            break;
        case 3:
            FragColor = vec4(1.0f,1.0f,1.0f,change);
            break;
    }
}