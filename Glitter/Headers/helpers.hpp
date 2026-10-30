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
#include <thirdparty.hpp>

// #Header Macros -------------------------------------------------------------

// Data sizes

#define BYTESIZE 8
#define SUPER_SMALL_FLOAT 0.00000000000001f
#define SMALL_FLOAT 0.002f
#define NOT_THAT_SMALL_FLOAT 0.01f

// Layer info

#define TOTAL_LAYERS 50
#define SHAPES_IN_BATCH 1
#define MID_LAYER 25
#define TOTAL_EMOTION_LAYERS 6
#define TOTAL_SHAPES (TOTAL_LAYERS*TOTAL_EMOTION_LAYERS*SHAPES_IN_BATCH)
#define BATCHES_SHARE_EMOTION 1 

// Spawn Times

#define SHAPE_INITIAL_SPAWN_TIME 0.15f
#define DESTRUCTION_TIME         0.15f
#define UPDATE_ITERATIONS 10.0f

// Ben's Color Image Generator parameters

#define COLOR_FREQUENCY 0.025f
#define COLOR_WIDTH 255
#define COLOR_HEIGHT 255
#define COLOR_DISPLAY false

// Ben's Simulator Parameters

#define SIM_FREQUENCY 0.1f
#define SIM_CYCLE true
#define SIM_RANDOM false



#define SHAPE_MAX_FADE 1.0f
#define SHAPE_MIN_FADE 0.0f

// TODO: Find out what these do 

#define SHAPE_MAX_FADE_SCALE 1.0f
#define SHAPE_MIN_FADE_SCALE 0.0f

// Set whether certain transitions are allowed

#define DO_SCALE true
#define DO_FADE true
#define DO_VORTEX false

// Transition Specific Scaling

#define DO_VORTEX_SCALE 1.0f
#define DO_SCALE_SCALE 1.0f
#define DO_FADE_SCALE 0.50f

// TODO: Find out what this does 

#define SOFTNESS_SCALE 1.00f

// Random Scale Differences Between Shapes in Same Batch

#define MAX_SCALE 8.0f
#define MIN_SCALE 0.05f
#define AVE_SCALE 0.25f
#define DEV_SCALE 0.225f

// Random Alpha Differences Between Shapes in Same Batch

#define MAX_ALPHA 1.0f
#define MIN_ALPHA 1.0f
#define AVE_ALPHA 0.5f
#define DEV_ALPHA 0.55f

// Sets the amount the coordinates of a shape is randomized or based on emotion information

#define SCATTER_TO_EMOTION_COORD_RATIO 1.0f

// Sets the amount a shape is scattered

#define SCATTER_COORD_SCALE 1.00f

// Sets the amount a shape is positioned based on emotion data

#define EMOTION_COORD_SCALE 1.00f

// Debug: Determines whether certain outputs will occur

#define DEBUG_LINE true
#define DEBUG_BORDER false
#define DEBUG_DISTANCE false
#define DEBUG_FUN false
#define DEBUG_IMAGE true


// Obsolete

#define POINT_LIGHT_COUNT 4
#define SORT_TIME 0.25f
#define SHAPE_SPAWN_TIME         0.01f
#define HACKY_COMPILE_MAKER f
#define MINIMUM_DRAWN_SCALE 0.00f

// #GLFW

/// @brief Get time elapsed since the glfw context was created.
/// @return time elapsed.
float getElapsedTime();


// #Simple Math Operations -------------------------------------------------------------

template <typename NUMBER, typename ITERABLE> NUMBER mean(ITERABLE iterable);
template <typename NUMBER, typename ITERABLE> NUMBER sum(ITERABLE iterable);
float clamp(const float x, const float min, const float max);
float floorWrap(float dividend, float divisor);
float ceilWrap(float dividend, float divisor);
int floorWrap(int dividend, int divisor);
int ceilWrap(int dividend, int divisor);

// #Variable Manipulation

template <typename T> void mySwap(T& t1, T& t2){
    T temp = t1;
    t1 = t2;
    t2 = temp;
}


/// @brief Average the columns.
/// @tparam V
/// @param vec Multiple rows.
/// @return The average of each individual column.
template <typename V> std::vector<V> average(std::vector<std::vector<V>> vec);


/// @brief 
/// @tparam T 
/// @param arrayCopyTo array to copy to.
/// @param arrayCopyFrom array to copy from.
/// @param sizeCopyTo size of array to copy to.
/// @param sizeCopyFrom size of array to copy from.
/// @return True if size of arrays are equal, False otherwise.
template <typename T> bool doCopy(T arrayCopyTo[], const T arrayCopyFrom[], const unsigned int sizeCopyTo, const unsigned int sizeCopyFrom){
    for(unsigned int i = 0; i < sizeCopyTo && i < sizeCopyFrom; i++)
        arrayCopyTo[i] = arrayCopyFrom[i];
    return sizeCopyTo == sizeCopyFrom;
}

/// @brief Create a copy of arrayCopyFrom.
/// @tparam T 
/// @param arrayCopyFrom 
/// @param size 
/// @return The copy.
template <typename T> T* copyInitialize(const T arrayCopyFrom[], const unsigned int size){
    T* aCopy = (T*) malloc(sizeof(T)*size);
    doCopy(aCopy, arrayCopyFrom, size, size);  
    return aCopy;
}

// #Classes -------------------------------------------------------------

/// @brief Raw data of a png file, may have been generated procedurally.
///
/// row is the y value of the pixel to be displayed. The y-axis points upward.
///
/// col is the x value of the pixel to be displayed. The x-axis points right.
///
/// gridIndex is the index of a pixel.
///
/// index is the index of a channel of a pixel.
class PngData{
    public:
        /// @brief Allocates memory for png data to fit the space defined by the parameters. nrChannels is set to 4.
        /// @param width
        /// @param height 
        PngData(const int width, const int height);
        /// @brief Allocates memory for png data to fit the space defined by the parameters.
        /// @param width 
        /// @param height 
        /// @param nrChannels color channels
        PngData(const int width, const int height, const int nrChannels);
        /// @brief Either copies the data from data based on the parameters or assigns the objects data to the passed data.
        /// @param width 
        /// @param height 
        /// @param nrChannels color channels
        /// @param data raw uncompressed png data
        /// @param copy if true do copy, otherwise assign this objects data to passed data
        PngData(const int width, const int height, const int nrChannels, unsigned char* data, bool copy=false);
        /// @brief Copies png data.
        /// @param pngData to be copied
        PngData(const PngData& pngData);
        /// @brief Deletes data.
        ~PngData();
        /// @brief Returns the index of the unsigned char at the parameters.
        /// @param row 
        /// @param col 
        /// @param channel 
        /// @return 
        int index(int row, int col, int channel) { return row*width*nrChannels+col*nrChannels+channel; }
        /// @brief Returns the index of unsigned char.
        /// @param gridIndex index ignoring nrChannels.
        /// @param channel
        /// @return 
        int indexFromGrid(int gridIndex, int channel) { return gridIndex*nrChannels + channel; }
        int size() const { return height*width*nrChannels; }
        /// @brief Size ignoring nrChannels.
        /// @return 
        int gridSize() { return height*width; }
        int row(int index) { return floorWrap(index,(width*nrChannels)); }
        int col(int index) { return floorWrap(index - row(index)*width*nrChannels,nrChannels); }
        int rowFromGrid(int gridIndex) { return floorWrap(gridIndex,(width)); }
        int colFromGrid(int gridIndex) { return gridIndex - rowFromGrid(gridIndex)*width; }
        int rFG(int gridIndex) { return rowFromGrid(gridIndex); }
        int cFG(int gridIndex) { return colFromGrid(gridIndex); }
        int gridIndex(int row, int col){ return width*row + col; }
        int gridIndex(int index){ return floorWrap(index,nrChannels); }
        int channel(int index) { return index - row(index)*width*nrChannels - col(index)*nrChannels; }
        void setData(int row, int col, int channel, const unsigned char& byte);
        void setData(int gridIndex, int channel, const unsigned char& byte) { data[indexFromGrid(gridIndex,channel)] = byte; }
        void setData(int index, const unsigned char& byte) { data[index] = byte; }
        /// @brief Set the color of a pixel with RGBA channels.
        /// @param gridIndex 
        /// @param color 
        void setColor(int gridIndex, cdata::ColorRGBA color){
            setData(gridIndex, redChannel, color.red);
            setData(gridIndex, blueChannel, color.blue);
            setData(gridIndex, greenChannel, color.green);
            setData(gridIndex, alphaChannel, color.alpha);
        }
        /// @brief Set the color of a pixel with RGB channels.
        /// @param gridIndex 
        /// @param color 
        void setColor(int gridIndex, cdata::ColorRGB color){
            setData(gridIndex, redChannel, color.red);
            setData(gridIndex, blueChannel, color.blue);
            setData(gridIndex, greenChannel, color.green);
        }
        /// @brief Set the color of a pixel with RGBA channels.
        /// @param row 
        /// @param col 
        /// @param color 
        void setColor(int row, int col, cdata::ColorRGBA color){
            setColor(gridIndex(row,col), color);
        }
        /// @brief Set the color of a pixel with RGB channels.
        /// @param row 
        /// @param col 
        /// @param color 
        void setColor(int row, int col, cdata::ColorRGB color){
            setColor(gridIndex(row,col), color);
        }
        /// @brief The result of a previous = operation.
        /// @return True if this objects width,height, and nrChannels parameters equaled the other objects respective parameters; false otherwise.
        bool isValid(){
            return valid;
        }
        const unsigned char* getData () const { return data; }
        /// @brief Set data of this object to data of pngData. No allocation is done here.
        /// @param pngData 
        /// @return this object.
        PngData& operator=(const PngData& pngData) noexcept {
            valid = (this->width == pngData.width && this->height == pngData.height && this->nrChannels == pngData.nrChannels);
            if(valid)
                doCopy(this->data, pngData.getData(), this->size(), pngData.size());
            return *this;
        }
        
        const int width;
        const int height;
        const int nrChannels;
        static const int nrChannelsDefault = 4;
        static const int redChannel = 0;
        static const int greenChannel = 1;
        static const int blueChannel = 2;
        static const int alphaChannel = 3;
        
    private:
        unsigned char* data;
        bool valid = true;
};


/// @brief Returns the time passed since the last time it has been updated. Useful for making sure the speed of objects aren't dependent on the frame rate.
class DeltaTimer{
    public:
        DeltaTimer();
        float getDeltaTime(bool do_update = false);
        float timePassed(float timeStamp){return getElapsedTime() - timeStamp;}
        void  updateDeltaTime();
    private:
        float elapsed_time = 0.0f;
        float lastframe_time = 0.0f;
        float deltatime = 0.0f;
};


/// @brief Sends out a signal at periodic intervals.
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

/// @brief Path consisting of just points.
class StraightPath{
    public:
        StraightPath( std::vector<glm::vec3> p, double prog) : points(p), progress(prog){};
        glm::vec3 getPositionOnPath();
        void setProgress(float prog, bool loop = false);
    private:
        std::vector<glm::vec3> points;
        float progress; // Ranges from 0 to 1
};

/// @brief Contains reference information to an OpenGL shape.
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

// #Enums -------------------------------------------------------------

enum SHAPE_TYPE_3D { CUBE, PYRAMID, OCTAHEDRON};
enum SHAPE_TYPE_2D { TRIANGLE, SQUARE, CIRCLE};
enum EMOTION {HAPPY=0, SAD=1, ANGRY=2, FEAR=3, SURPRISE=4, DISGUST=5 };
/// @brief Determines how a shape goes into view and goes out of view.
enum TRANSITIONS {FADE,SCALE,VORTEX, BACK_OUT, MOVE_FORWARD};



// #Structs -------------------------------------------------------------

struct Material {
    unsigned int diffuse;
    unsigned int specular;
    float shininess;
};

struct ShapeProperties {
    glm::mat4 model;
    glm::vec4 color;
    SHAPE_TYPE_3D type;
    float creationTimeStamp;
    float fadeTime;
    glm::vec3 coords;
};

/// @brief Various information to modify how a shape is drawn.
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
    bool useBenTexture;
    glm::mat4 transition;
    glm::mat4 rotationMatrix;
    glm::mat4 scaleMatrix;
    glm::vec3 direction;
    glm::vec3 rotatedCoords;
};

/// @brief The shader information for drawing shapes.
struct LightingEnvironment {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    Shader object_shader;
    Shader light_shader;
};

/// @brief Multiple shapes that were generated with the same emotion layer.
struct ShapeBatch{
    std::deque<ShapeProperties2D> shapes;
    PngData pngData;
    unsigned int texture;
};

/// @brief Stores emotion data and coloring.
struct EmotionLayer{
    std::vector<float> emotionArray;
    PngData pngData;
};

/// @brief Multiple shapes that are formed by the same set of emotions.
struct ShapeLayer{ 
   std::deque<ShapeBatch> batches;
   std::vector<float> emotionArray;
};

typedef std::deque<ShapeLayer> VisualLayer;


// #CONSTANTS -------------------------------------------------------------

namespace dir{
    const glm::vec3 UP(0.0f,1.0f,0.0f);
    const glm::vec3 DOWN(0.0f,-1.0f,0.0f);
    const glm::vec3 RIGHT(1.0f,0.0f,0.0f);
    const glm::vec3 LEFT(-1.0f,0.0f,0.0f);
    const glm::vec3 UP_RIGHT = glm::normalize(UP + RIGHT);
    const glm::vec3 DOWN_LEFT = glm::normalize(DOWN + LEFT);
}

// #Main Programs -------------------------------------------------------------

/// @brief Draws a 2D display of multiple colored shapes based on emotion data and Ben's visual simulator.
/// @param argc 
/// @param argv 
/// @return 
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


// #Draw -------------------------------------------------------------

/// @brief Draws the shape layers in the rendering loop.
/// @param lightingEnvironment 
/// @param shapeLayers 
/// @param shapes 
/// @param deltaTimer 
/// @param destructionAlarm 
/// @param lock 
/// @param lineAbove 
/// @param lineBelow 
void drawShapeLayers2D(LightingEnvironment& lightingEnvironment, std::deque<ShapeLayer>& shapeLayers, std::map<SHAPE_TYPE_2D, OpenGLShape*> shapes,  DeltaTimer deltaTimer, Alarm destructionAlarm, std::mutex& lock, std::tuple<float, float> lineAbove, std::tuple<float, float> lineBelow);
void drawMultipleTexturedShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int ind_cnt, const std::vector<unsigned int>& textures);
void drawCubeMatrices(LightingEnvironment& lightingEnvironment, std::vector<ShapeProperties> cubeMatrices, std::map<SHAPE_TYPE_3D, OpenGLShape*> shapes,  DeltaTimer deltaTimer);
void setupLightingEnvironmentToDraw(LightingEnvironment& lightingEnvironment, glm::vec3 light_positions[POINT_LIGHT_COUNT], glm::vec3 diffuses[POINT_LIGHT_COUNT], OpenGLShape& lightShape, bool drawLights=true);
void drawShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int ind_cnt);
void drawTexturedShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int ind_cnt, unsigned int texture);
void drawDoubleTexturedShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int vert_cnt, unsigned int texture1, unsigned int texture2);

// #Create -------------------------------------------------------------

OpenGLShape create_circle(unsigned int tri_cnt);
OpenGLShape create_circle(unsigned int tri_cnt, PngData& pngData);
void create_shape(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, 
    const float vert[], unsigned int vert_cnt, const unsigned int ind[], 
    unsigned int ind_cnt);
void create_colored_textured_shape(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, 
    float vert[], unsigned int vert_cnt, unsigned int ind[], 
    unsigned int ind_cnt);
void create_textured_shape(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, 
    const float vert[], unsigned int vert_cnt, const unsigned int ind[], 
    unsigned int ind_cnt);
void setupSampler2Ds(unsigned int& diffuse_sampler_2d, unsigned int& specular_sampler_2d, std::string diffuse_path, std::string diffuse_file_type, std::string specular_path, std::string specular_file_type);
void create_lamp_and_light_object(unsigned int &VAO_O, unsigned int &VAO_T,unsigned int &VBO, unsigned int& EBO, 
    const float vert[], unsigned int vert_cnt, const unsigned int ind[], 
    unsigned int ind_cnt);
void create_a_shape(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, const unsigned int ind_cnt, const unsigned int vert_cnt, const float vertices[], const unsigned int indices[], const std::vector<unsigned int>& attributeSizes);


// #Initialization -------------------------------------------------------------

LightingEnvironment initMPV(std::string vertexShaderPath, std::string fragmentShaderPath);
GLFWwindow* loadGLFWCreateWindow(int width, int height);
bool initOpenGL(GLFWwindow*& mWindow, int& return_status, int width, int height);
int initOpenGL(GLFWwindow*& mWindow, int width, int height);
void initMouse(GLFWwindow* mWindow);
LightingEnvironment initLighting(glm::vec3 light_positions[POINT_LIGHT_COUNT], glm::vec3 diffuses[POINT_LIGHT_COUNT]);

// #RenderLoop Start -------------------------------------------------------------

void startMPVLoop(DeltaTimer& deltaTimer, LightingEnvironment& lightingEnvironment);
void minimumStartRenderLoop(DeltaTimer& deltaTimer, glm::vec4 backgroundColor = glm::vec4(0.0f,0.0f,0.0f,1.0f));
void startMPVLoop(DeltaTimer& deltaTimer, LightingEnvironment& lightingEnvironment, glm::vec4 backgroundColor);
void startDummyMPVLoop(DeltaTimer& deltaTimer, LightingEnvironment& lightingEnvironment, glm::vec4 backgroundColor);
void setupDummyMPVEnvironmentToDraw(LightingEnvironment& lightingEnvironment);

// #RenderLoop End -------------------------------------------------------------

void endRenderLoop(GLFWwindow*& mWindow, DeltaTimer deltaTimer, Camera& camera);
void endRenderLoopStatic(GLFWwindow*& mWindow);
void endRenderLoopViewStatic(GLFWwindow*& mWindow, DeltaTimer deltaTimer);

// #Input -------------------------------------------------------------

void releaseCursor(GLFWwindow* mWindow);

// #Callback -------------------------------------------------------------

void scroll_callback(GLFWwindow* mWindow, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* mWindow, double xPos, double yPos);
glm::vec3 cameraDirection(float yaw, float pitch);

// #Immediate -------------------------------------------------------------

void processInput(GLFWwindow* mWindow, glm::vec3& cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);
void processInput(GLFWwindow* mWindow, glm::vec3& cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float deltatime);

// #Read -------------------------------------------------------------

nlohmann::json readjsonfile(std::string filepath);
std::vector<std::vector<float>> readEmotionArray(std::string filepath, bool printIt=true);
int emotionIndex(EMOTION emotion);

// #Format -------------------------------------------------------------

std::string formatFloat(float f, int digitsBefore, int digitsAfter);
std::string to_string(std::vector<std::vector<float>> coefficients);
std::string to_string(std::vector<float> coefficients);



// #Externs -------------------------------------------------------------

extern float yaw;
extern float pitch;
extern float lastX;
extern float lastY;
extern bool first_mouse;

// #Random -------------------------------------------------------------

glm::vec3 randScale(float min=0.0f, float max=1.0f);
glm::vec3 randCoords();
float rand_range_uniform(float min, float max);
unsigned int rand_range_uniform(unsigned int min, unsigned int max);
float rand_range_normal(float mean, float stddev);
glm::vec3 randRGB();
glm::vec4 randRGBA();


// #Linear Equation Solver -------------------------------------------------------------

std::vector<float> solve(std::vector<std::vector<float>> coefficients, std::vector<float> constants, const float& smallFloat=SMALL_FLOAT);

// #Conversions -------------------------------------------------------------

glm::vec3 vectorToVec3(std::vector<float> a);
std::vector<float> vec3ToVector(glm::vec3 a);
std::string vec3ToString(glm::vec3 vec3);
std::string vec4ToString(glm::vec4 vec4);
glm::vec2 screenToModelCoords(float x, float y, float width, float height);

// #Front to back comparisons -------------------------------------------------------------

bool compareShapeForwardDistance(const ShapeProperties& a, const ShapeProperties& b);
bool compareZDistance(const ShapeProperties& a, const ShapeProperties& b);


// #Shape Transition Calculations -------------------------------------------------------------

float calcFade(float destructionTimePassed, float destructionTime, int totalLayers, int midLayer, int layer, float shapeMinFade, float shapeMaxFade, bool delay, std::tuple<float, float> lineAbove, std::tuple<float, float> lineBelow);
float calcDisplacement(float destructionTimePassed, float destructionTime, int totalLayers, int midLayer, int layer, float shapeMinFade, float shapeMaxFade, bool delay, std::tuple<float,float> lineAbove, std::tuple<float,float> lineBelow);

// #EmotionArrayTo -------------------------------------------------------------

glm::vec3 emotionArrayToVec3(std::vector<float> emotionArray);
float emotionArrayToLength(std::vector<std::vector<float>> emotionArray);
std::tuple<int,float> emotionArrayToSoftness(std::vector<float> emotionArray, bool noSoft=false, bool noMiddle=false, bool noHard=false);
float emotionArrayToRotation(std::vector<float> array0);
std::map<TRANSITIONS,bool> emotionArrayToTransitions(std::vector<float> emotionArray);
glm::vec3 emotionArrayToColor(std::vector<float> array0);
SHAPE_TYPE_3D emotionArrayToShapeType3D(std::vector<float> array0);
SHAPE_TYPE_2D emotionArrayToShapeType2D(std::vector<float> array0);
float emotionArrayToAlpha(std::vector<float> emotionArray);


// #PNG -------------------------------------------------------------

void generatePNGBlank(PngData& pngData);
/// @brief Generate a OpenGL texture from pngData.
/// @param pngData texture data.
/// @return reference to OpenGL texture.
unsigned int getTextureFromData(const PngData& pngData);
void freePNGTextureData(unsigned char* data);
unsigned int generateTextureSpecified(int width, int height, int nrChannels, std::string filetype);
void generatePNGData(unsigned char* data, int width, int height, int nrChannels);
unsigned int useTextureDataToCreateTexture(const unsigned char* data, int width, int height, std::string filetype);
bool isOnBorder(int width, int height, int row, int col, int hThickness, int vThickness);
unsigned char getBorderCheckColor(int width, int height, int row, int col, int hThickness, int vThickness);
void generatePNGBorder(unsigned char* data, int width, int height, int nrChannels, int hThickness, int vThickness);
void generatePNGBorder(PngData& pngData, const int width, const int height,  const int hThickness, const int vThickness, const cdata::ColorRGBA borderColor, const cdata::ColorRGBA innerColor, const bool ignoreInner, const bool ignoreBorder);
unsigned int obtainPNGBorder(int width, int height,  int hThickness, int vThickness, cdata::ColorRGBA borderColor, cdata::ColorRGBA innerColor, bool ignoreInner, bool ignoreBorder);
unsigned int obtainPNGBorder(PngData& pngData,  int hThickness, int vThickness, cdata::ColorRGBA borderColor, cdata::ColorRGBA innerColor, bool ignoreInner, bool ignoreBorder);
cdata::ColorRGBA pngColorFromValue(const unsigned int& value);
unsigned int valueFromPngColor(cdata::ColorRGBA color);
unsigned int obtainPNGDistanceFromBorder(int width, int height, std::vector<glm::vec2> borderVertices);
PngData loadPngTexture(std::string texture_filepath);
unsigned char* initializePNGTextureData(int width, int height, int nrChannels);
void generatePNGDistanceFromBorder(PngData& pngData, std::vector<glm::vec2> borderVertices);
unsigned int createPngTexture(const PngData* pngData);
void pngDataSavePng(const std::string filepath, const PngData& pngData);


// #Texture -------------------------------------------------------------

void create_texture(unsigned int &texture, std::string texture_filepath, std::string filetype);
void generate_texture(unsigned int &texture, std::string texture_filepath, std::string filetype);
void generateSampler2Ds(unsigned int& diffuse_sampler_2d, unsigned int& specular_sampler_2d);
unsigned int createPngTexture(std::string texture_filepath);
unsigned int createTexture(std::string texture_filepath, std::string filetype);

// #Linear Operations -------------------------------------------------------------

std::vector<glm::vec2> borderVerticesFromShape(const shapes::Shape& shape);
std::tuple<float,float,float> getLineFromVertices(float xi, float xf, float yi, float yf);
std::vector<glm::vec2> borderVerticesFromShape(const float shape[], const unsigned int vert_cnt, const unsigned int stride);
bool isInSquare(glm::vec2 point, glm::vec2 center,  float length);
bool isInSquare(float x, float y, float centerX, float centerY, float length);
bool isOnLine(std::tuple<float,float,float> line, glm::vec2 point);


// #Linear Transformations -------------------------------------------------------------

float boundTransformation(float x, std::tuple<float,float> line);
std::tuple<float,float> linearTransformation(float min, float max, float minT, float maxT);
float boundTransformation(float x, float slope, float intercept);

/// @brief Convert the range from [min, max] to [minT, maxT].
/// 
/// Useful for normalizing ranges and transforming normalized ranges to other ranges.
/// @param x Number in range [min, max].
/// @param min 
/// @param max 
/// @param minT 
/// @param maxT 
/// @return x that was transformed to be within range [minT, maxT].
float boundTransformation(float x, float min, float max, float minT, float maxT);
/// @brief Obtain a texture that encodes the distance from the border at each pixel.
/// @param pngData stores texture data.
/// @param borderVertices vertices of shape border in clockwise order.
/// @return reference to OpenGL texture.
unsigned int obtainPNGDistanceFromBorder(PngData& pngData, std::vector<glm::vec2> borderVertices);


// #Debug -------------------------------------------------------------

/// @brief Prints a message if a condition is fulfilled and if this print has been enabled.
/// @param message printed.
/// @param check condition to check, true if fulfilled, false otherwise.
/// @param debugIt true if enabled, false otherwise.
void checkPrint(std::string message, bool check, bool debugIt);



// #Threads -------------------------------------------------------------

/// @brief Create ShapeLayers using emotion data and Ben's visual generator.
/// @param shapeLayersBuffer shape layers.
/// @param creationAlarm periodic timer to determine when to create a new layer.
/// @param lock prevent alteration of parameters while another thread is using them.
/// @param notDone indicates when to end the thread.
/// @param shapeAdditions the amount of shapes that have been generated.
void shapeLayerGenerator(std::deque<ShapeLayer>* shapeLayersBuffer, Alarm* creationAlarm,  std::mutex* lock, bool* notDone, int* shapeAdditions);
/// @brief Loads the png file generated by Ben's visual generator.
/// @param datas buffer storing data from multiple loaded png files.
/// @param turnOff terminates the generator.
/// @param lock prevents access to datas.
/// @param filePath path to the png file.
/// @param waitTime amount of time to wait until loading the file again.
void pngLoader(std::deque<PngData>* datas, bool* turnOff, std::mutex* lock, const std::string* filePath, float* waitTime);

// #Start Processes -------------------------------------------------------------

int startBensVisualGenerationProgram(float frequency, int width=255, int height=255, bool doDisplay=false);
int startBensEmotionSimulator(float frequency, bool cycle = true, bool random = false);

// #Unordered -------------------------------------------------------------
ShapeProperties2D generateShape2D(std::map<TRANSITIONS, bool> transitions, glm::vec3 coords, glm::vec3 emotionCoords3D, glm::vec3 rotAxis, float rotScale, glm::vec3 emotionRGB, float alphaScale, bool useBenTexture, SHAPE_TYPE_2D type, float softnessOrThickness, unsigned int borderCase);

#endif