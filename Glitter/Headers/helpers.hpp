#ifndef HELPERS
#define HELPERS
// Local Headers
#include "glitter.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include <iterator>
#include <shader.hpp>
#include <camera.hpp>
#include <shapes.hpp>
#include <model.hpp>
#include <vector>
#include <random>

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <stb_image.h>
#include <stdexcept>

#include <sstream>


#include <deque>
#include <json.hpp>

// Header Macros
#define POINT_LIGHT_COUNT 4

// Structs

struct LightingEnvironment {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    Shader object_shader;
    Shader light_shader;
};

// Main Programs
int mainLight(int argc, char * argv[]);
int mainTurning(int argc, char * argv[]);
int mainCubes(int argc, char * argv[]);
int mainGenerateTexturesCubes(int argc, char * argv[]);
int mainTextureGenerate(int argc, char * argv[]);
int mainPathCube(int argc, char * argv[]);
int mainPathRectangle(int argc, char * argv[]);


// Draw
void drawShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int vert_cnt);
void drawTexturedShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int vert_cnt, unsigned int texture);
void drawDoubleTexturedShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int vert_cnt, unsigned int texture1, unsigned int texture2);


// Create
void create_shape(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, 
    const float vert[], unsigned int vert_cnt, const unsigned int ind[], 
    unsigned int ind_cnt);
void create_colored_textued_shape(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, 
    float vert[], unsigned int vert_cnt, unsigned int ind[], 
    unsigned int ind_cnt);
void create_textured_shape(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, 
    const float vert[], unsigned int vert_cnt, const unsigned int ind[], 
    unsigned int ind_cnt);
void create_texture(unsigned int &texture, std::string texture_filepath, std::string filetype);
void generate_texture(unsigned int &texture, std::string texture_filepath, std::string filetype);
void create_lamp_and_light_object(unsigned int &VAO_O, unsigned int &VAO_T,unsigned int &VBO, unsigned int& EBO, 
    const float vert[], unsigned int vert_cnt, const unsigned int ind[], 
    unsigned int ind_cnt);
void create_a_shape(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, const unsigned int ind_cnt, const float vertices[], const unsigned int indices[], const std::vector<unsigned int>& attributeSizes);


// Initialization
GLFWwindow* loadGLFWCreateWindow(int width, int height);
GLFWwindow* initOpenGL(int& return_status, int width, int height);
void initMouse(GLFWwindow* mWindow);
LightingEnvironment initLighting(glm::vec3 light_positions[POINT_LIGHT_COUNT], glm::vec3 diffuses[POINT_LIGHT_COUNT]);
// Input
//  Callback
void scroll_callback(GLFWwindow* mWindow, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* mWindow, double xPos, double yPos);
glm::vec3 cameraDirection(float yaw, float pitch);

//  Immediate
void processInput(GLFWwindow* mWindow, glm::vec3& cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);
void processInput(GLFWwindow* mWindow, glm::vec3& cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float deltatime);

// Read
nlohmann::json readjsonfile(std::string filepath);
std::vector<std::vector<float>> readEmotionArray(std::string filepath);
glm::vec3 emotionArrayToRotationAxis(std::vector<std::vector<float>> emotionArray);
float emotionArrayToLength(std::vector<std::vector<float>> emotionArray);


glm::vec3 emotionArrayToColor(std::vector<std::vector<float>> emotionArray);

// Externs
extern float yaw;
extern float pitch;
extern float lastX;
extern float lastY;
extern bool first_mouse;


// Classes
class DeltaTimer{
    public:
        DeltaTimer();
        float getDeltaTime(bool do_update = false);
        float getElapsedTime(bool do_update = false) { if (do_update) updateDeltaTime(); return elapsed_time; }
        void updateDeltaTime();
    private:
        float elapsed_time = 0.0f;
        float lastframe_time = 0.0f;
        float deltatime = 0.0f;
};


// Path consisting of just points
class StraightPath{
    public:
        StraightPath( std::vector<glm::vec3> p, double prog) : points(p), progress(prog){};
        glm::vec3 getPositionOnPath();
        void setProgress(float prog, bool loop = false);
    private:
        std::vector<glm::vec3> points;
        float progress; // Ranges from 0 to 1
};

class Shape {
    public:
        Shape(unsigned int ind_cnt, const float vertices[], const unsigned int indices[], const std::vector<unsigned int>& attributeSizes, unsigned int texture, bool hasTexture){
            this->ind_cnt = ind_cnt;
            this->texture = texture;
            this->hasTexture = hasTexture;
            create_a_shape(this->VAO, this->VBO, this->EBO, this->ind_cnt, &vertices[0], &indices[0], attributeSizes);
        }
        void draw(Shader shader){
            if(hasTexture)
                drawTexturedShape(VAO, EBO, shader, ind_cnt, texture);
            else
                drawShape(VAO, EBO, shader, ind_cnt);
        }
    private:
        bool hasTexture;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int ind_cnt;
        unsigned int texture;
};

#endif