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

class Shader {
    public:
        Shader(const char vertex_shader_filepath[], const char fragment_shader_filepath[]);
        void use() { glUseProgram(shaderProgram); }
        void setUniform(const char name[], const glm::vec4 vec4);
        void setUniform(const char name[], const glm::vec3 vec3);
        void setUniform(const char name[], const unsigned int integer);
        void setUniform(const char name[], const glm::mat4 mat4);
        void setUniform(const char name[], const float floaty);

    
    private:
        unsigned int shaderProgram;
        void compileShader(const char vertex_shader_source[], const char fragment_shader_source[]);


};
#endif