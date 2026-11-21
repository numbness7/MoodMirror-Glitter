#include <randomextensions.hpp>
std::random_device rd;
std::mt19937 gen(rd());

float rand_range_uniform(float min, float max){
    std::uniform_real_distribution<> dis(min,max);
    return dis(gen);
}

unsigned int rand_range_uniform(unsigned int min, unsigned int max){
    std::uniform_int_distribution<unsigned int> dis(min,max);
    return dis(gen);
}

float rand_range_normal(float mean, float stddev){
    std::normal_distribution<> dis(mean,stddev);
    return dis(gen);
}

glm::vec3 randRGB(){
    return glm::vec3(rand_range_normal(0.0f,1.0f),rand_range_normal(0.0f,1.0f),rand_range_normal(0.0f,1.0f));
}

glm::vec4 randRGBA(){
    return glm::vec4(rand_range_normal(0.0f,1.0f),rand_range_normal(0.0f,1.0f),rand_range_normal(0.0f,1.0f),rand_range_normal(0.0f,1.0f));
}
glm::vec3 randScale(float min, float max){
    glm::vec3 coords(0.0f);
    for (unsigned int i = 0; i < 3; i++){
        coords[i] = rand_range_uniform(min,max);
    }
    return coords;
}
glm::vec2 randCoords2D(){
    glm::vec2 coords(0.0f);
    for (unsigned int i = 0; i < 2; i++){
        coords[i] = rand_range_uniform(-1.0f,1.0f);
    }
    return coords;
}
glm::vec3 randCoords(){
    glm::vec3 coords(0.0f);
    for (unsigned int i = 0; i < 3; i++){
        coords[i] = rand_range_uniform(-1.0f,1.0f);
    }
    return coords;
}