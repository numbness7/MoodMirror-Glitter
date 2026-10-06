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
#define MAXIMUM_BATCHES 100
#define SHAPES_IN_BATCH 100
#define SHAPE_INITIAL_SPAWN_TIME 0.0166666f
#define SHAPE_SPAWN_TIME 0.01666666f
#define SHAPE_MAX_FADE 0.99f

// Classes
class DeltaTimer{
    public:
        DeltaTimer();
        float getDeltaTime(bool do_update = false);
        float getElapsedTime(bool do_update = false) { if (do_update) updateDeltaTime(); return elapsed_time; }
        float timePassed(float timeStamp){return getElapsedTime() - timeStamp;}
        void  updateDeltaTime();
    private:
        float elapsed_time = 0.0f;
        float lastframe_time = 0.0f;
        float deltatime = 0.0f;
};

class Alarm{
    public:
        Alarm( float waitTime);
        void setWaitTime(float waitTime);
        float getWaitTime(){return waitTime;}
        bool checkAndUpdate();
        bool check(){ return alarmSounded; }
    private:
        float waitTime;
        float time;
        bool alarmSounded;
        DeltaTimer timer{};
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

class OpenGLShape {
    public:
        OpenGLShape(unsigned int vert_cnt, unsigned int ind_cnt, const float vertices[], const unsigned int indices[], const std::vector<unsigned int>& attributeSizes, unsigned int texture, bool hasTexture);
        void draw(Shader shader);
    private:
        bool hasTexture;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int ind_cnt;
        unsigned int vert_cnt;
        unsigned int texture;
};

// Enums
enum SHAPE_TYPE { CUBE, PYRAMID};

// Structs

struct ShapeProperties {
    glm::mat4 model;
    glm::vec4 color;
    SHAPE_TYPE type;
    float creationTimeStamp;
    float fadeTime;
    glm::vec3 coords;
};

struct LightingEnvironment {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    Shader object_shader;
    Shader light_shader;
};

// Main Programs
int mainCoords(int argc, char * argv[]);
int mainLight(int argc, char * argv[]);
int mainTurning(int argc, char * argv[]);
int mainCubes(int argc, char * argv[]);
int mainGenerateTexturesCubes(int argc, char * argv[]);
int mainTextureGenerate(int argc, char * argv[]);
int mainPathCube(int argc, char * argv[]);
int mainPathRectangle(int argc, char * argv[]);


// Draw
void drawCubeMatrices(LightingEnvironment lightingEnvironment, std::deque<ShapeProperties> cubeMatrices, OpenGLShape cubeShape, OpenGLShape pyramidShape, DeltaTimer deltaTimer);
void setupLightingEnvironmentToDraw(LightingEnvironment& lightingEnvironment, glm::vec3 light_positions[POINT_LIGHT_COUNT], glm::vec3 diffuses[POINT_LIGHT_COUNT], OpenGLShape& lightShape, bool drawLights=true);
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
void generateSampler2Ds(unsigned int& diffuse_sampler_2d, unsigned int& specular_sampler_2d);
void setupSampler2Ds(unsigned int& diffuse_sampler_2d, unsigned int& specular_sampler_2d, std::string diffuse_path, std::string diffuse_file_type, std::string specular_path, std::string specular_file_type);
void create_lamp_and_light_object(unsigned int &VAO_O, unsigned int &VAO_T,unsigned int &VBO, unsigned int& EBO, 
    const float vert[], unsigned int vert_cnt, const unsigned int ind[], 
    unsigned int ind_cnt);
void create_a_shape(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, const unsigned int ind_cnt, const unsigned int vert_cnt, const float vertices[], const unsigned int indices[], const std::vector<unsigned int>& attributeSizes);


// Initialization
GLFWwindow* loadGLFWCreateWindow(int width, int height);
bool initOpenGL(GLFWwindow*& mWindow, int& return_status, int width, int height);
int initOpenGL(GLFWwindow*& mWindow, int width, int height);
void initMouse(GLFWwindow* mWindow);
LightingEnvironment initLighting(glm::vec3 light_positions[POINT_LIGHT_COUNT], glm::vec3 diffuses[POINT_LIGHT_COUNT]);
// End
void endRenderLoop(GLFWwindow*& mWindow, DeltaTimer deltaTimer, Camera& camera);
// Input
void releaseCursor(GLFWwindow* mWindow);
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
glm::vec3 emotionArrayToVec3(std::vector<float> emotionArray);
float emotionArrayToLength(std::vector<std::vector<float>> emotionArray);

//Manage

int findAShape(const ShapeProperties& shapeToDelete, const std::vector<ShapeProperties>& shapesBackToFront);
bool deleteAShape(std::vector<ShapeProperties>& shapesBackToFront, std::deque<ShapeProperties>& cubeMatrices);

glm::vec3 emotionArrayToColor(std::vector<float> array0);
std::string vec3ToString(glm::vec3 vec3);
std::string vec4ToString(glm::vec4 vec4);

// Externs
extern float yaw;
extern float pitch;
extern float lastX;
extern float lastY;
extern bool first_mouse;

// Random
glm::vec3 randCoords();


#endif