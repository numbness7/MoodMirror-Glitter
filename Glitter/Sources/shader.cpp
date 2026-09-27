#include "glm/gtc/type_ptr.hpp"
#include <fstream>
#include <shader.hpp>
std::string readfile(const char filepath[]){
    std::string s = "";
    char line[5000];
    std::fstream file{filepath, file.in};
    if(!file.is_open()){
        throw std::exception();
    }
    while(file.getline(line,5000,'\n')){
        s += line;        
        s += '\n';
    }
    return s;
}

Shader::Shader(std::string vertex_shader_filepath, std::string fragment_shader_filepath): vertex_shader_filepath(vertex_shader_filepath), fragment_shader_filepath(fragment_shader_filepath){
    std::string vertex_shader_source = readfile(vertex_shader_filepath.c_str());
    std::string fragment_shader_source = readfile(fragment_shader_filepath.c_str());
    compileShader(vertex_shader_source.c_str(), fragment_shader_source.c_str());
}


void Shader::compileShader(const char vertex_shader_source[], const char fragment_shader_source[]){
    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    
    if (!success){
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "ERROR: " << vertex_shader_filepath << ": " << infoLog << std::endl;
    }

    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    
    if (!success){
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        std::cout << "ERROR: " << fragment_shader_filepath << ": " << infoLog << std::endl;
    }
    
    this->shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertex_shader);
    glAttachShader(shaderProgram, fragment_shader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR: " << fragment_shader_filepath << ": " << infoLog << std::endl;
    }
    
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader::setUniform(std::string name, const bool truth){
    unsigned int uniform_loc = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform1i(uniform_loc, truth);
}
void Shader::setUniform(std::string name, const glm::vec3 vec3){
    unsigned int uniform_loc = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform3f(uniform_loc, vec3.x, vec3.y, vec3.z);
}
void Shader::setUniform(std::string name, const unsigned int integer){
    unsigned int uniform_loc = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform1i(uniform_loc, integer);
}
void Shader::setUniform(std::string name, const float floating){
    unsigned int uniform_loc = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform1f(uniform_loc, floating);
}
void Shader::setUniform(std::string name, const glm::mat4 mat4){
    unsigned int uniform_loc = glGetUniformLocation(shaderProgram, name.c_str());
    glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, glm::value_ptr(mat4));
}
void Shader::setVec3(std::string name, const float x,const float y,const float z){
    setUniform(name, glm::vec3(x,y,z));
}


void Shader::setUniform(std::string name, const glm::vec4 vec4){;
    unsigned int uniform_loc = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform4fv(uniform_loc, 1, glm::value_ptr(vec4));
}
void Shader::setUniform(std::string name, const glm::vec4 vec4[], const int size){
    for (int i = 0; i < size; i++){
        std::string partName = name + "[" + std::to_string(i) + "]";
        setUniform(partName, vec4[i]);
    }
}