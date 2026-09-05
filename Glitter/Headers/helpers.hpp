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

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <stb_image.h>
#include <stdexcept>

#include <sstream>

// Main Programs
int mainLight(int argc, char * argv[]);
int mainCubes(int argc, char * argv[]);
int mainGenerateTexturesCubes(int argc, char * argv[]);
int mainOther(int argc, char * argv[]);
int mainTextureGenerate(int argc, char * argv[]);


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


// Initialization
GLFWwindow* loadGLFWCreateWindow(int width, int height);
GLFWwindow* initOpenGL(int& return_status, int width, int height);
// Input
//  Callback
void scroll_callback(GLFWwindow* mWindow, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* mWindow, double xPos, double yPos);

//  Immediate
void processInput(GLFWwindow* mWindow, glm::vec3& cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);
void processInput(GLFWwindow* mWindow, glm::vec3& cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float deltatime);


// Externs
extern float yaw;
extern float pitch;
extern float lastX;
extern float lastY;
extern bool first_mouse;


class DeltaTimer{
    public:
        DeltaTimer();
        float getDeltaTime(bool do_update = true);
    private:
        void updateDeltaTime();
        float elapsed_time = 0.0f;
        float lastframe_time = 0.0f;
        float deltatime = 0.0f;
};
glm::vec3 cameraDirection(float yaw, float pitch);

#endif