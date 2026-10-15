#ifndef SHADER
#define SHADER
// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

std::string readfile(const char filepath[]);
class Shader {
    public:
        Shader(std::string vertex_shader_filepath, std::string fragment_shader_filepath) ;
        void use() { glUseProgram(shaderProgram); }
        void setUniformV4(std::string name, const glm::vec4 vec4);
        void setUniformAV4(std::string name, const glm::vec4 vec4[], const int size);
        void setUniformV3(std::string name, const glm::vec3 vec3);
        void setUniformI(std::string name, const unsigned int integer);
        void setUniformF(std::string name, const float floating);
        void setUniformM4(std::string name, const glm::mat4 mat4);
        void setVec3(std::string name, const float x,const float y,const float z);
        void setUniformB(std::string name, const bool truth);
        void setUniformV2(std::string name, const glm::vec2 vec2);

    
    private:
        unsigned int shaderProgram;
        void compileShader(const char vertex_shader_source[], const char fragment_shader_source[]);
        std::string vertex_shader_filepath;
        std::string fragment_shader_filepath;


};
#endif