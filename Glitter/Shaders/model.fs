#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 view_pos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};


uniform Material material;

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cut_off;
    float outer_cut_off;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    bool is_on;
};

uniform SpotLight spot_light;

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};
#define POINT_LIGHT_COUNT 4
uniform PointLight point_lights[POINT_LIGHT_COUNT];



struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight dir_light;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 view_dir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 view_dir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 view_dir);

void main(){
    vec3 result = vec3(0.0f);
    vec3 view_dir = normalize(view_pos - FragPos);
    result += calcDirLight(dir_light, Normal, view_dir);
    if (spot_light.is_on)
        result += calcSpotLight(spot_light, Normal, view_dir);
    for(int i = 0; i < POINT_LIGHT_COUNT; i += 1){
        result += calcPointLight(point_lights[i], Normal, view_dir);
    }
    
    FragColor = vec4(result, 1.0f);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 view_dir){
     vec3 result;
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light.position - FragPos);
    
    float theta = dot(light.direction, normalize(-light_dir));
    float epsilon = light.cut_off - light.outer_cut_off;
    float intensity = clamp((theta - light.outer_cut_off)/epsilon, 0.0f, 1.0f);
    
    float diff = max(dot(norm,light_dir), 0.0f);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));

    
    vec3 reflect_dir = normalize(reflect(-light_dir,norm));
    
    float spec = pow(max(dot(view_dir,reflect_dir), 0.0f), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.texture_specular1, TexCoords)) * vec3(texture(material.texture_specular2, TexCoords)));
    

    diffuse *= intensity;
    specular *= intensity;
    
    result = ambient + diffuse + specular;
    
    return result;
}   



vec3 calcDirLight(DirLight light, vec3 normal, vec3 view_dir){
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(-light.direction);
    
    float diff = max(dot(norm,light_dir), 0.0f);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));
    
    vec3 reflect_dir = normalize(reflect(-light_dir,norm));
    
    float spec = pow(max(dot(view_dir,reflect_dir), 0.0f), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.texture_specular1, TexCoords)) * vec3(texture(material.texture_specular2, TexCoords)));
    

    
    vec3 result = ambient + diffuse + specular;
    return result;
}


vec3 calcPointLight(PointLight light, vec3 normal, vec3 view_dir){
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light.position - FragPos);
    
    float diff = max(dot(norm,light_dir), 0.0f);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));

    vec3 reflect_dir = normalize(reflect(-light_dir,norm));
    
    float spec = pow(max(dot(view_dir,reflect_dir), 0.0f), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.texture_specular1, TexCoords)) * vec3(texture(material.texture_specular2, TexCoords)));
    
    float dist = length(FragPos - light.position);
    float attenuation = 1.0f/(light.constant + light.linear*dist + light.quadratic*pow(dist,2));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    
    vec3 result = ambient + diffuse + specular;
    return result;
}