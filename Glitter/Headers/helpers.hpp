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
#include <vector>
#include <random>
#include <tuple>
#include <queue>
#include <stack>
#include <numbers>
#include <color.hpp>
#include <limits>
#include <format>
#include <color.body.hpp>
#include <thread>
#include <mutex>
// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <stb_image.h>
#include <stb_image_write.h>
#include <stdexcept>

#include <sstream>


#include <deque>
#include <json.hpp>

// Header Macros
#define BYTESIZE 8
#define TOTAL_LAYERS 1000
#define SHAPES_IN_BATCH 1
#define MID_LAYER 100
#define SHAPE_INITIAL_SPAWN_TIME 0.001f
#define BATCHES_SHARE_EMOTION 1 
#define DESTRUCTION_TIME         0.01f

#define SORT_TIME 0.25f
#define SHAPE_SPAWN_TIME         0.01f

#define SHAPE_MAX_FADE 1.0f
#define SHAPE_MIN_FADE 0.0f
#define SHAPE_MAX_FADE_SCALE 1.0f
#define SHAPE_MIN_FADE_SCALE 0.0f

#define SUPER_SMALL_FLOAT 0.00000000000001f
#define SMALL_FLOAT 0.002f
#define NOT_THAT_SMALL_FLOAT 0.01f
#define DO_SCALE true
#define DO_FADE true
#define DO_VORTEX false
#define DO_VORTEX_SCALE 1.0f

#define SOFTNESS_SCALE 1.00f

#define MAX_SCALE 3.0f
#define MIN_SCALE 0.1f
#define AVE_SCALE 0.5f
#define DEV_SCALE 0.125f

#define MAX_ALPHA 1.0f
#define MIN_ALPHA 0.80f
#define AVE_ALPHA 0.5f
#define DEV_ALPHA 0.25f

#define DO_SCALE_SCALE 1.0f
#define DO_FADE_SCALE 1.00f

#define SCATTER_TO_EMOTION_COORD_RATIO 1.0f

#define SCATTER_COORD_SCALE 1.00f
#define EMOTION_COORD_SCALE 1.00f

#define DEBUG_LINE true
#define DEBUG_BORDER false
#define DEBUG_DISTANCE false
#define DEBUG_FUN false
#define HACKY_COMPILE_MAKER f
#define MINIMUM_DRAWN_SCALE 0.00f

#define POINT_LIGHT_COUNT 4

// Header constants
const std::vector<unsigned int> EMOTION_INDEXES = {0,1,2,3,4,5};

struct BenImages{
    std::mutex lock;
    std::deque<unsigned int> images;
};

int floor(int x, int y);
template <typename T> void mySwap(T t1, T t2){
    T temp = t1;
    t1 = t2;
    t2 = temp;
}

// Classes
class PngData{
    public:
        PngData(int width, int height);
        PngData(const int width, const int height, const int nrChannels, const unsigned char* data);
        ~PngData();
        int index(int row, int col, int channel) { return row*width*nrChannels+col*nrChannels+channel; }
        int indexFromGrid(int gridIndex, int channel) { return gridIndex*nrChannels + channel; }
        int size() { return height*width*nrChannels; }
        int gridSize() { return height*width; }
        int row(int index) { return floor(index,(width*nrChannels)); }
        int col(int index) { return floor(index - row(index)*width*nrChannels,nrChannels); }
        int rowFromGrid(int gridIndex) { return floor(gridIndex,(width)); }
        int colFromGrid(int gridIndex) { return gridIndex - rowFromGrid(gridIndex)*width; }
        int rFG(int gridIndex) { return rowFromGrid(gridIndex); }
        int cFG(int gridIndex) { return colFromGrid(gridIndex); }
        int gridIndex(int row, int col){ return width*row + col; }
        int gridIndex(int index){ return floor(index,nrChannels); }
        int channel(int index) { return index - row(index)*width*nrChannels - col(index)*nrChannels; }
        void setData(int row, int col, int channel, const unsigned char& byte);
        void setData(int gridIndex, int channel, const unsigned char& byte) { data[indexFromGrid(gridIndex,channel)] = byte; }
        void setData(int index, const unsigned char& byte) { data[index] = byte; }
        void setColor(int gridIndex, cdata::Color color){
            setData(gridIndex, redChannel, color.red);
            setData(gridIndex, blueChannel, color.blue);
            setData(gridIndex, greenChannel, color.green);
            setData(gridIndex, alphaChannel, color.alpha);
        }
        void setColor(int row, int col, cdata::Color color){
            setColor(gridIndex(row,col), color);
        }
        const unsigned char* getData () const { return data; }
        

        
        static const int redChannel = 0;
        static const int greenChannel = 1;
        static const int blueChannel = 2;
        static const int alphaChannel = 3;
        const int width;
        const int height;
        static const int nrChannelsDefault = 4;
        
        const int nrChannels;
    private:
        unsigned char* data;
};

unsigned int getTextureFromData(const PngData& pngData);
class PngDataIterator{
    public:
        PngDataIterator(int width, int height, int nrChannels) : width{width},  height{height}, nrChannels{nrChannels} {
            row = 0;
            col = 0;
            channel = 0;
        }
        void iterate(){
            if(channel < nrChannels) channel++;
            else{
                channel = 0;
                if(col < width) col++;
                else {
                    col = 0;
                    if(row < height) row++;
                }
            }
        }
        bool isNotDone(){
            return row < height;
        }
        int index(){
            return row*width*nrChannels+col*nrChannels+channel;
        }
        void setChannelByte(const int channel, const unsigned char& sourceByte, unsigned char& destinationByte){
            if(channel == this->channel) destinationByte = sourceByte;
        }
        static const int redChannel = 0;
        static const int blueChannel = 1;
        static const int greenChannel = 2;
        static const int alphaChannel = 3;
        const int width;
        const int height;
        const int nrChannels;
        bool done = false;
        int row;
        int col;
        int channel;
};

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
        float getPassedTime() const { return time; }
        float getPassedTimeFrom(float timeStamp) const { return time + timeStamp; }
        float getTimeLeft() { return waitTime - time; }
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
        OpenGLShape(const shapes::Shape &shape, const std::vector<unsigned int> &textures, const bool hasMultipleTextures);
        OpenGLShape(const shapes::Shape& shape, unsigned int texture, bool hasTexture);
        void draw(Shader shader);
        void draw(Shader shader, std::vector<unsigned int> textures);
        void setTexture(const unsigned int texture, unsigned const int index=0){ this->textures[index] = texture; }
        void swapTexture(const unsigned int indexA, unsigned const int indexB){ mySwap(textures[indexA], textures[indexB]); }
        void pushTexture(const unsigned int texture){ textures.push_back(texture); }
        void padTextures(unsigned int padding) { for(int i = 0; i < padding; i++) textures.push_back(0); }
        void clearTextures() { textures.clear(); }
        void setTextures(const std::vector<unsigned int> textures) { clearTextures(); for(const auto texture : textures) this->textures.push_back(texture); }
        unsigned int getTexture(const unsigned index) { return textures[index]; }
    private:
        bool hasTextures;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int ind_cnt;
        unsigned int vert_cnt;
        std::vector<unsigned int> textures;
};

// #Enums
enum SHAPE_TYPE_3D { CUBE, PYRAMID, OCTAHEDRON};
enum SHAPE_TYPE_2D { TRIANGLE, SQUARE, CIRCLE};
enum EMOTION {HAPPY=0, SAD=1, ANGRY=2, FEAR=3, SURPRISE=4, DISGUST=5 };
enum TRANSITIONS {FADE,SCALE,VORTEX, BACK_OUT, MOVE_FORWARD};



// #Structs

struct ShapeProperties {
    glm::mat4 model;
    glm::vec4 color;
    SHAPE_TYPE_3D type;
    float creationTimeStamp;
    float fadeTime;
    glm::vec3 coords;
};

struct ShapeProperties2D {
    glm::mat4 model;
    glm::vec4 innerColor;
    glm::vec4 outerColor;
    SHAPE_TYPE_2D type;
    glm::vec3 coords;
    float rotation;
    glm::vec3 scale;
    float borderThickness;
    std::map<TRANSITIONS,bool> transitions;
    int borderCase;
    unsigned int texture;
    glm::mat4 transition;
    glm::mat4 rotationMatrix;
    glm::mat4 scaleMatrix;
    glm::vec3 direction;
    glm::vec3 rotatedCoords;
};

struct LightingEnvironment {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    Shader object_shader;
    Shader light_shader;
};


// CONSTANTS
namespace dir{
    const glm::vec3 UP(0.0f,1.0f,0.0f);
    const glm::vec3 DOWN(0.0f,-1.0f,0.0f);
    const glm::vec3 RIGHT(1.0f,0.0f,0.0f);
    const glm::vec3 LEFT(-1.0f,0.0f,0.0f);
    const glm::vec3 UP_RIGHT = glm::normalize(UP + RIGHT);
    const glm::vec3 DOWN_LEFT = glm::normalize(DOWN + LEFT);
}

// #Main Programs
int mainCoordsBasic2D(int argc, char * argv[]);
int mainDrawRectangle(int argc, char* argv[]);
int mainDrawCircle(int argc, char* argv[]);
int mainCoords(int argc, char * argv[]);
int mainCoordsBasic(int argc, char * argv[]);
int mainCoordsBasicStream(int argc, char * argv[]);
int mainLight(int argc, char * argv[]);
int mainTurning(int argc, char * argv[]);
int mainCubes(int argc, char * argv[]);
int mainGenerateTexturesCubes(int argc, char * argv[]);
int mainTextureGenerate(int argc, char * argv[]);
int mainPathCube(int argc, char * argv[]);
int mainPathRectangle(int argc, char * argv[]);
int mainSolve(int argc, char * argv[]);


// #Draw
void drawCubeMatrices2DScale(LightingEnvironment& lightingEnvironment, std::deque<ShapeProperties2D>& cubeMatrices, std::map<SHAPE_TYPE_2D, OpenGLShape*> shapes,  DeltaTimer deltaTimer, Alarm destructionAlarm, std::mutex& lock, std::tuple<float, float> lineAbove, std::tuple<float, float> lineBelow);
void drawCubeMatrices2DFade(LightingEnvironment& lightingEnvironment, std::deque<ShapeProperties2D> cubeMatrices, std::map<SHAPE_TYPE_2D, OpenGLShape*> shapes,  DeltaTimer deltaTimer, Alarm destructionAlarm);
void drawCubeMatrices(LightingEnvironment& lightingEnvironment, std::vector<ShapeProperties> cubeMatrices, std::map<SHAPE_TYPE_3D, OpenGLShape*> shapes,  DeltaTimer deltaTimer);
void setupLightingEnvironmentToDraw(LightingEnvironment& lightingEnvironment, glm::vec3 light_positions[POINT_LIGHT_COUNT], glm::vec3 diffuses[POINT_LIGHT_COUNT], OpenGLShape& lightShape, bool drawLights=true);
void drawShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int ind_cnt);
void drawTexturedShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int ind_cnt, unsigned int texture);
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
LightingEnvironment initMPV(std::string vertexShaderPath, std::string fragmentShaderPath);
GLFWwindow* loadGLFWCreateWindow(int width, int height);
bool initOpenGL(GLFWwindow*& mWindow, int& return_status, int width, int height);
int initOpenGL(GLFWwindow*& mWindow, int width, int height);
void initMouse(GLFWwindow* mWindow);
LightingEnvironment initLighting(glm::vec3 light_positions[POINT_LIGHT_COUNT], glm::vec3 diffuses[POINT_LIGHT_COUNT]);
// Start
void startMPVLoop(DeltaTimer& deltaTimer, LightingEnvironment& lightingEnvironment);
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
std::vector<std::vector<float>> readEmotionArray(std::string filepath, bool printIt=true);
std::string formatFloat(float f, int digitsBefore, int digitsAfter);
glm::vec3 emotionArrayToVec3(std::vector<float> emotionArray);
float emotionArrayToLength(std::vector<std::vector<float>> emotionArray);
std::tuple<int,float> emotionArrayToSoftness(std::vector<float> emotionArray, bool forceSoft=false);

//Manage

int findAShape(const ShapeProperties& shapeToDelete, const std::vector<ShapeProperties>& shapesBackToFront);
bool deleteAShape(std::vector<ShapeProperties>& shapesBackToFront, std::deque<ShapeProperties>& cubeMatrices);

glm::vec3 emotionArrayToColor(std::vector<float> array0);
SHAPE_TYPE_3D emotionArrayToShapeType3D(std::vector<float> array0);
SHAPE_TYPE_2D emotionArrayToShapeType2D(std::vector<float> array0);
std::string vec3ToString(glm::vec3 vec3);
std::string vec4ToString(glm::vec4 vec4);

// Externs
extern float yaw;
extern float pitch;
extern float lastX;
extern float lastY;
extern bool first_mouse;

// Random
glm::vec3 randScale(float min=0.0f, float max=1.0f);
glm::vec3 randCoords();


std::string to_string(std::vector<std::vector<float>> coefficients);
std::string to_string(std::vector<float> coefficients);
std::vector<float> solve(std::vector<std::vector<float>> coefficients, std::vector<float> constants, const float& smallFloat=SMALL_FLOAT);
glm::vec3 vectorToVec3(std::vector<float> a);
std::vector<float> vec3ToVector(glm::vec3 a);

bool compareShapeForwardDistance(const ShapeProperties& a, const ShapeProperties& b);
bool compareZDistance(const ShapeProperties& a, const ShapeProperties& b);
void endRenderLoopStatic(GLFWwindow*& mWindow);
void endRenderLoopViewStatic(GLFWwindow*& mWindow, DeltaTimer deltaTimer);
OpenGLShape create_circle(unsigned int tri_cnt);

float clamp(const float x, const float min, const float max);
float calcFade(float destructionTimePassed, float destructionTime, int totalLayers, int midLayer, int layer, float shapeMinFade, float shapeMaxFade, bool delay, std::tuple<float, float> lineAbove, std::tuple<float, float> lineBelow);

float emotionArrayToRotation(std::vector<float> array0);
std::map<TRANSITIONS,bool> emotionArrayToTransitions(std::vector<float> emotionArray);

// PNG
void freePNGTextureData(unsigned char* data);
unsigned int generateTextureSpecified(int width, int height, int nrChannels, std::string filetype);
void generatePNGData(unsigned char* data, int width, int height, int nrChannels);
unsigned int useTextureDataToCreateTexture(const unsigned char* data, int width, int height, std::string filetype);
bool isOnBorder(int width, int height, int row, int col, int hThickness, int vThickness);
unsigned char getBorderCheckColor(int width, int height, int row, int col, int hThickness, int vThickness);
void generatePNGBorder(unsigned char* data, int width, int height, int nrChannels, int hThickness, int vThickness);
void generatePNGBorder(PngData& pngData, int width, int height,  int hThickness, int vThickness, cdata::Color borderColor, cdata::Color innerColor, bool ignoreInner, bool ignoreBorder);
unsigned int obtainPNGBorder(int width, int height,  int hThickness, int vThickness, cdata::Color borderColor, cdata::Color innerColor, bool ignoreInner, bool ignoreBorder);
unsigned int obtainPNGBorder(PngData& pngData,  int hThickness, int vThickness, cdata::Color borderColor, cdata::Color innerColor, bool ignoreInner, bool ignoreBorder);
cdata::Color pngColorFromValue(const unsigned int& value);
unsigned int valueFromPngColor(cdata::Color color);
unsigned int obtainPNGDistanceFromBorder(int width, int height, std::vector<glm::vec2> borderVertices);
PngData loadPngTexture(std::string texture_filepath);

// Texture
unsigned int createPngTexture(std::string texture_filepath);
unsigned int createTexture(std::string texture_filepath, std::string filetype);



float boundTransformation(float x, std::tuple<float,float> line);
std::tuple<float,float> linearTransformation(float min, float max, float minT, float maxT);
float boundTransformation(float x, float slope, float intercept);
float boundTransformation(float x, float min, float max, float minT, float maxT);
unsigned char* initializePNGTextureData(int width, int height, int nrChannels);

void generatePNGDistanceFromBorder(PngData& pngData, std::vector<glm::vec2> borderVertices);
std::tuple<float,float,float> getLineFromVertices(float xi, float xf, float yi, float yf);
std::vector<glm::vec2> borderVerticesFromShape(const float shape[], const unsigned int vert_cnt, const unsigned int stride);

void checkPrint(std::string message, bool check, bool debugIt);
bool isInSquare(glm::vec2 point, glm::vec2 center,  float length);
bool isInSquare(float x, float y, float centerX, float centerY, float length);
OpenGLShape create_circle(unsigned int tri_cnt, PngData& pngData);
void pngDataSavePng(const std::string filepath, const PngData& pngData);
unsigned int obtainPNGDistanceFromBorder(PngData& pngData, std::vector<glm::vec2> borderVertices);
std::vector<glm::vec2> borderVerticesFromShape(const shapes::Shape& shape);
void drawMultipleTexturedShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int ind_cnt, const std::vector<unsigned int>& textures);
void generatePNGBlank(PngData& pngData);
bool isOnLine(std::tuple<float,float,float> line, glm::vec2 point);
void minimumStartRenderLoop(DeltaTimer& deltaTimer, glm::vec4 backgroundColor = glm::vec4(0.0f,0.0f,0.0f,1.0f));
float emotionArrayToAlpha(std::vector<float> emotionArray);
void startMPVLoop(DeltaTimer& deltaTimer, LightingEnvironment& lightingEnvironment, glm::vec4 backgroundColor);
int emotionIndex(EMOTION emotion);
glm::vec2 screenToModelCoords(float x, float y, float width, float height);
void startDummyMPVLoop(DeltaTimer& deltaTimer, LightingEnvironment& lightingEnvironment, glm::vec4 backgroundColor);
void setupDummyMPVEnvironmentToDraw(LightingEnvironment& lightingEnvironment);
float calcDisplacement(float destructionTimePassed, float destructionTime, int totalLayers, int midLayer, int layer, float shapeMinFade, float shapeMaxFade, bool delay, std::tuple<float,float> lineAbove, std::tuple<float,float> lineBelow);
template <typename NUMBER, typename ITERABLE> NUMBER mean(ITERABLE iterable);
template <typename NUMBER, typename ITERABLE> NUMBER sum(ITERABLE iterable);
void keepGettingImages(BenImages* benImages, GLFWwindow* mWindow, bool* notDone);
void something(std::deque<ShapeProperties2D>* cubeMatrices, const unsigned int* blankTexture, Alarm* creationAlarm, GLFWwindow* mWindow, std::mutex* lock, bool* notDone, int* additions);
PngData* loadPngTextureNew(std::string texture_filepath);
unsigned int createPngTexture(const PngData* pngData);
void pngLoader(std::deque<PngData*>* datas, bool* turnOff, std::mutex* lock, const std::string* filePath);
#endif