#ifndef HELPERS
#define HELPERS
// Local Headers
#include "glitter.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include <iterator>
#include <vector>
#include <random>
#include <tuple>
#include <queue>
#include <stack>
#include <numbers>
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


#include <algorithm>
#include <deque>
#include <json.hpp>
#include <shader.hpp>

// My Headers
#include <thirdparty.hpp>
#include <variablemanipulation.hpp>
#include <debugging.hpp>
#include <shapes.hpp>
#include <color.hpp>
#include <mathoperations.hpp>
#include <randomextensions.hpp>
#include <geometry.hpp>
#include <textures.hpp>

// #Header Macros -------------------------------------------------------------

// Do Test
#define DO_TEST false

// Window Configuration
#define FULL_SCREEN false

// Hardware Specific Configuration
#define ON_PI false

// Default paths
#define SHADER_PATH (std::string("Glitter/Shaders/"))

// Texture Info
#define NUMBER_OF_ATLAS_TEXTURES 16


// Layer info

#define MAXIMUM_IMAGE_POWER 32
#define TOTAL_LAYERS 64
#define SHAPES_IN_BATCH 16
#define MID_LAYER 32
#define TOTAL_EMOTION_LAYERS 16
#define TOTAL_BATCHES (TOTAL_LAYERS*TOTAL_EMOTION_LAYERS)
#define TOTAL_SHAPES (TOTAL_BATCHES*SHAPES_IN_BATCH)
#define SHAPES_DIVIDER 16
#define SHAPES_PER_LAYER (TOTAL_SHAPES/TOTAL_LAYERS)
#define BATCHES_PER_LAYER (TOTAL_SHAPES/(TOTAL_BATCHES))
#define BATCHES_SHARE_EMOTION 1 
#define VEC4PERMAT3 3
#define MAT3PERSHAPE 3
#define VEC4S_PER_SHAPE ((VEC4PERMAT3)*(MAT3PERSHAPE))
#define TOTAL_VEC4_FOR_SHAPES ((VEC4S_PER_SHAPE) * (TOTAL_SHAPES))

// Emotion Data File
#define EMOTION_DATA_FILE_PATH "../data.json"
#define EMOTION_PNG_FILE_PATH "../background.png"

// Spawn Times

#define SHAPE_INITIAL_SPAWN_TIME 0.15f
#define DESTRUCTION_TIME         0.5f
#define UPDATE_ITERATIONS 10.0f

// Pulse

#define PULSE_TIME_RATIO 12.0f
#define PULSE_TIME (DESTRUCTION_TIME*PULSE_TIME_RATIO)
#define PULSE_TIME_INITAL (SHAPE_INITIAL_SPAWN_TIME*PULSE_TIME_RATIO)

#define PULSE_FADE_MIN 0.0f
#define PULSE_FADE_MAX 1.0f
#define PULSE_FADE_SCALE_MIN 0.0f
#define PULSE_FADE_SCALE_MAX 1.0f
#define PULSE_NUMBER 1

// Ben's Color Image Generator parameters

#define COLOR_PATH "../VisualGen_Ben/V1_255x255"
#define COLOR_PROGRAM "python Gener3.py"
#define COLOR_START false
#define COLOR_FREQUENCY 0.015f
#define COLOR_WIDTH 256
#define COLOR_HEIGHT 256
#define COLOR_DISPLAY false

#define USE_BEN_CHANCE 1.0f

// Ben's Simulator Parameters

#define SIM_PATH "../Emotion_Gen_Sim/"
#define SIM_PROGRAM "python emotion_generator_mine.py"
#define SIM_START false
#define SIM_READ_PERIOD 0.001f
#define SIM_INITIAL_READ_PERIOD 0.001f
#define SIM_PERIOD 0.01f
#define SIM_INITIAL_PERIOD 0.01f
#define SIM_CYCLE true
#define SIM_RANDOM false



#define SHAPE_MAX_FADE 1.0f
#define SHAPE_MIN_FADE 0.0f

// TODO: Find out what these do 

#define SHAPE_MAX_FADE_SCALE 1.0f
#define SHAPE_MIN_FADE_SCALE 0.0f

// Set transition to always occur

#define ALWAYS_FADE_T true

// Set whether certain transitions are allowed

#define DO_SCALE_T true
#define DO_FADE_T true
#define DO_MOVE_FORWARD_T true
#define DO_BACK_OUT_T true
#define DO_VORTEX false

#define TRANSITIONS_NUMBER 2

#define LAYER_TRANSITIONS true
#define LAYER_PULSES true

// Set Pulses to always occur
#define ALWAYS_FADE_P

// Set whether certain pulses are allowed

#define DO_FADE_P   true
#define DO_SCALE_P  true
#define DO_RUMBLE_P true
#define DO_SPINR_P  true
#define DO_SPINL_P  true

#define ONLY_ONE_P true



// Transition Specific Scaling

#define DO_VORTEX_SCALE 1.0f
#define DO_SCALE_SCALE 1.0f
#define DO_FADE_SCALE 0.50f

// TODO: Find out what this does 

#define SOFTNESS_SCALE 1.00f

// Random Scale Differences Between Shapes in Same Batch

#define SCALE_MULTIPLIER 0.5f
#define MAX_SCALE 1.10f*SCALE_MULTIPLIER
#define MIN_SCALE 0.100f*SCALE_MULTIPLIER
#define AVE_SCALE 0.10f*SCALE_MULTIPLIER
#define DEV_SCALE 0.10f*SCALE_MULTIPLIER

// Random Alpha Differences Between Shapes in Same Batch

#define MAX_ALPHA 1.0f
#define MIN_ALPHA 1.0f
#define AVE_ALPHA 1.0f
#define DEV_ALPHA 1.00f

// Sets the amount the coordinates of a shape is randomized or based on emotion information

#define SCATTER_TO_EMOTION_COORD_RATIO 1.0f

// Sets the amount a shape is scattered

#define SCATTER_COORD_SCALE 1.00f

// Sets the amount a shape is positioned based on emotion data

#define EMOTION_COORD_SCALE 1.00f

// Print

#define PRINT_TIME 0.25f

// Debug: Determines whether certain outputs will occur

#define DEBUG_LINE true
#define DEBUG_BORDER false
#define DEBUG_DISTANCE false
#define DEBUG_FUN false
#define DEBUG_IMAGE true
#define DEBUG_RECURSION_FILL false
#define DEBUG_RECURSION true

// Other
#define ProgressMargin 0.0001f

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
        float getWaitTime() const {return waitTime;}
        float getPassedTime() const { return time; }
        float getPassedTimeFrom(float timeStamp) const { return time + timeStamp; }
        float getTimeLeft() const { return waitTime - time; }
        void reset() { time = 0.0f; timer.updateDeltaTime(); }
        bool checkAndUpdate();
        bool check() const { return alarmSounded; }
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
        void draw(Shader& shader);
        void drawBatch(Shader& shader, unsigned int count);
        void drawBatch(Shader& shader, const std::vector<OpenGLTexture>& textures, unsigned int count);
        OpenGLShape(const shapes::Shape& shape);

        // Deprecate this
        OpenGLShape(unsigned int vert_cnt, unsigned int ind_cnt, const float vertices[], const unsigned int indices[], const std::vector<unsigned int>& attributeSizes, unsigned int texture, bool hasTexture);
        // Deprecate this
        OpenGLShape(const shapes::Shape &shape, const std::vector<unsigned int> &textures, const bool hasMultipleTextures);
        // Deprecate this
        OpenGLShape(const shapes::Shape& shape, unsigned int texture, bool hasTexture);
        // Deprecate this
        void draw(Shader& shader, std::vector<unsigned int> textures);
        // Deprecate this
        void drawBatch(Shader& shader, const std::vector<unsigned int>& textures, unsigned int count);
        // Deprecate this
        void setTexture(const unsigned int texture, unsigned const int index=0){ this->textures[index] = texture; }
        // Deprecate this
        void swapTexture(const unsigned int indexA, unsigned const int indexB){ mySwap(textures[indexA], textures[indexB]); }
        // Deprecate this
        void pushTexture(const unsigned int texture){ textures.push_back(texture); }
        // Deprecate this
        void padTextures(unsigned int padding) { for(int i = 0; i < padding; i++) textures.push_back(0); }
        // Deprecate this
        void clearTextures() { textures.clear(); }
        // Deprecate this
        void setTextures(const std::vector<unsigned int> textures) { clearTextures(); for(const auto texture : textures) this->textures.push_back(texture); }
        // Deprecate this
        unsigned int getTexture(const unsigned index) { return textures[index]; }
    private:
        bool hasTextures;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int ind_cnt;
        unsigned int vert_cnt;
        // Deprecate this
        std::vector<unsigned int> textures;
};

// #Enums -------------------------------------------------------------

enum SHAPE_TYPE_3D { CUBE, PYRAMID, OCTAHEDRON};
enum SHAPE_TYPE_2D { TRIANGLE=0, SQUARE=1, CIRCLE=2};
enum EMOTION {HAPPY=0, SAD=1, ANGRY=2, FEAR=3, SURPRISE=4, DISGUST=5 };
/// @brief Determines how a shape goes into view and goes out of view.
enum TRANSITIONS {FADE_T,SCALE_T,VORTEX_T, BACK_OUT_T, MOVE_FORWARD_T};
enum PULSES {SCALE_P, FADE_P, RUMBLE_P, SPINR_P, SPINL_P};



// #Structs -------------------------------------------------------------

struct EmotionIntensities{
    float happiness;
    float sadness;
    float anger;
    float fear;
    float surprise;
    float disgust;
};

/// @brief Various information to modify how a shape is drawn.
struct ShapeProperties2D {
    glm::vec4 innerColor;
    glm::vec4 outerColor;
    SHAPE_TYPE_2D type;
    glm::vec2 coords;
    float borderThickness;
    std::map<TRANSITIONS,bool> transitions;
    int borderCase;
    unsigned int texture;
    bool useBenTexture;
    glm::mat3 transition;
    glm::mat3 rotationMatrix;
    glm::mat3 scaleMatrix;
    glm::vec2 direction;
    glm::vec2 rotatedCoords;
    glm::vec2 rumbleCoords = glm::vec2(0.0f);
    float spin = 0.0f;
    unsigned int layerTag;
    unsigned int batchTag;
};

struct InitialShapeFeatures2D {
    std::vector<unsigned int> layerNumbers;
    std::vector<unsigned int> batchNumbers;
    std::vector<float> borderThickness;
    std::vector<glm::mat3> translationMatrices;
    std::vector<glm::mat3> rotationMatrices;
    std::vector<glm::mat3> scaleMatrices;
    std::vector<unsigned int> types;
    std::vector<unsigned int> layerTags;
    std::vector<unsigned int> batchTags;
};

struct ShapeLayerFeatures2D {
    std::vector<glm::mat3> translationMatrices;
    std::vector<glm::mat3> rotationMatrices;
    std::vector<glm::mat3> scaleMatrices;
    std::vector<glm::vec2> sLSCALEs;
    std::vector<glm::vec4> innerColors;
    std::vector<glm::vec4> outerColors;
    std::vector<float> destructionTimes;
    std::vector<float> destructionTimePasseds;
    std::vector<unsigned int> tags;
};
struct BatchFeatures2D {
    std::vector<unsigned int> tags;
};

/// @brief The shader information for drawing shapes.
struct LightingEnvironment {
    glm::mat4 model;
    glm::mat4 view;
    Shader object_shader;
};

/// @brief Multiple shapes that were generated with the same emotion layer.
struct ShapeBatch{
    std::deque<ShapeProperties2D> shapes;
    PngData pngData;
    std::map<PULSES, bool> pulses;
    unsigned int tag;
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
   Alarm destructionTimer{1.0f};
   Alarm pulseTimer{1.0f};
   int previousLayer = -1;
   glm::vec2 SL_SCALE = glm::vec2(1.0f,1.0f);
   std::map<TRANSITIONS,bool> transitions{};
   std::deque<EmotionLayer> emotionLayers{};
   std::map<PULSES,bool> pulses{};
   glm::vec4 innerColor = glm::vec4(1.0f);
   glm::vec4 outerColor = glm::vec4(1.0f);
   float spin = 0.0f;
   glm::vec2 rumbleCoords = glm::vec2(0.0f);
   unsigned int layerTag;
};

typedef std::deque<ShapeLayer> VisualLayer;

class ShapeLayerStorage{
    public:
       ShapeLayerStorage(); 
       void push_front(ShapeLayer shapeLayer);
       void pop_back();
       ShapeLayer& back() { return shapeLayers.back(); }
       ShapeLayer& front() { return shapeLayers.front(); }
       VisualLayer::iterator begin() { return shapeLayers.begin(); }
       VisualLayer::iterator end()   { return shapeLayers.end(); }
       unsigned int getShapeCnt() { return shapeCnt; }
       unsigned int getBatchCnt() { return batchCnt; }
       unsigned int getLayerCnt() { return layerCnt; }
       unsigned int size() { return getLayerCnt(); }
       bool empty() { return size() == 0; }
    private:
       VisualLayer shapeLayers;
       unsigned int shapeCnt = 0;
       unsigned int batchCnt = 0;
       unsigned int layerCnt = 0;
};

template <typename T> class PseudoVector{
    public:
        int getSize() const { return size; }
        int getStrides() const { return strides; }
        int getStride() const { return stride; }
        bool isFull () const { return index == getSize(); }
        PseudoVector(int strides, int stride, T element);
        bool push_back(T element){
            if (isFull()){
                throw std::runtime_error("TOO BIG!\n");
            }
            else{
                data[index] = element;
                index++;
                return isFull();
            }
        }
        std::vector<T> getVector();
    private:
        std::vector<T> data;
        int index;
        int stride;
        int strides;
        int size;
        T element;
        void reset();
};

// #Main Programs -------------------------------------------------------------

/// @brief Draws a 2D display of multiple colored shapes based on emotion data and Ben's visual simulator.
/// @param argc 
/// @param argv 
/// @return 
int mainCoordsBasic2D(int argc, char * argv[]);
int mainDrawFloats(int argc, char* argv[]);
int mainDrawRectangle(int argc, char* argv[]);
int mainDrawCircle(int argc, char* argv[]);
int mainDrawDistances(int argc, char* argv[]);
int mainDrawBatch(int argc, char* argv[]);
int mainSolve(int argc, char * argv[]);
int mainRecursion(int argc, char * argv[]);
int mainDrawSubTextures(int argc, char* argv[]);

// #Draw -------------------------------------------------------------

/// @brief Draws the shape layers in the rendering loop.
/// @param lightingEnvironment 
/// @param shapeLayers 
/// @param shapes 
/// @param timePassed 
/// @param waitTime 
/// @param lock 
/// @param lineAbove 
/// @param lineBelow 
void drawShapeLayers2D(LightingEnvironment& lightingEnvironment, OpenGLShape& openGLShape,  std::vector<OpenGLTexture>& openGLTextures, int shapeCount);
void drawMultipleTexturedShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int ind_cnt, const std::vector<unsigned int>& textures);
void drawShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int ind_cnt);
void drawTexturedShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int ind_cnt, unsigned int texture);

// #Create -------------------------------------------------------------
void create_a_shape(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, const unsigned int ind_cnt, const unsigned int vert_cnt, const float vertices[], const unsigned int indices[], const std::vector<unsigned int>& attributeSizes);

// #Initialization -------------------------------------------------------------

LightingEnvironment initDummyMPV(std::string vertexShaderPath, std::string fragmentShaderPath, bool replaceVersion = false, std::string version = "330 core");
GLFWwindow* loadGLFWCreateWindow(int width, int height, bool fullscreen=FULL_SCREEN);
bool initOpenGL(GLFWwindow*& mWindow, int& return_status, int width, int height);
int initOpenGL(GLFWwindow*& mWindow, int width, int height);
void initMouse(GLFWwindow* mWindow);

// #RenderLoop Start -------------------------------------------------------------

void minimumStartRenderLoop(DeltaTimer& deltaTimer, glm::vec4 backgroundColor = glm::vec4(0.0f,0.0f,0.0f,1.0f));
void startDummyMPVLoop(DeltaTimer& deltaTimer, LightingEnvironment& lightingEnvironment, glm::vec4 backgroundColor=glm::vec4(1.0f));
void setupDummyMPVEnvironmentToDraw(LightingEnvironment& lightingEnvironment);

// #RenderLoop End -------------------------------------------------------------

void endRenderLoopStatic(GLFWwindow*& mWindow);
void endRenderLoopViewStatic(GLFWwindow*& mWindow, DeltaTimer deltaTimer);
void endRenderLoopView2D(GLFWwindow*& mWindow, DeltaTimer& deltaTimer, glm::vec2& position, float& zoom);

// #Input -------------------------------------------------------------

void releaseCursor(GLFWwindow* mWindow);

// #Callback -------------------------------------------------------------

// #Immediate -------------------------------------------------------------


// #Read -------------------------------------------------------------

nlohmann::json readjsonfile(std::string filepath);
std::vector<std::vector<float>> readEmotionArray(std::string filepath, bool printIt=true);
int emotionIndex(EMOTION emotion);

// #Format -------------------------------------------------------------

std::string formatFloat(float f, int digitsBefore, int digitsAfter, bool hasSign=true);
std::string to_string(std::vector<std::vector<float>> coefficients);
std::string to_string(std::vector<float> coefficients);

// #Externs -------------------------------------------------------------

extern float yaw;
extern float pitch;
extern float lastX;
extern float lastY;
extern bool first_mouse;


// #Linear Equation Solver -------------------------------------------------------------

std::vector<float> solve(std::vector<std::vector<float>> coefficients, std::vector<float> constants, const float& smallFloat=SMALL_FLOAT);

// #Conversions -------------------------------------------------------------

glm::vec3 vectorToVec3(std::vector<float> a);
std::vector<float> vec3ToVector(glm::vec3 a);
std::string vec3ToString(glm::vec3 vec3);
std::string vec4ToString(glm::vec4 vec4);
glm::vec2 screenToModelCoords(float x, float y, float width, float height);
std::map<EMOTION, float> emotionIntensitiesToEmotionMap(EmotionIntensities eI);
EmotionIntensities EmotionMapToEmotionIntensities(std::map<EMOTION, float> eIMap);
std::map<EMOTION, float> emotionArrayToEmotionMap(std::vector<float> emotionArray);

// #Shape Transition Calculations -------------------------------------------------------------

float calcFade(float destructionTimePassed, float destructionTime, int totalLayers, int midLayer, int layer, float shapeMinFade, float shapeMaxFade, bool delay, std::tuple<float, float> lineAbove, std::tuple<float, float> lineBelow);
float calcFade(float destructionTimePassed, float maxDestructionTime, float changePointStart, float changePointBefore, float changePointAfter, float changePointEnd, float shapeMinFade, float shapeMaxFade, bool delay);
float calcDisplacement(float destructionTimePassed, float destructionTime, int totalLayers, int midLayer, int layer, float shapeMinFade, float shapeMaxFade, bool delay, std::tuple<float,float> lineAbove, std::tuple<float,float> lineBelow);

// #EmotionArrayTo -------------------------------------------------------------

std::tuple<unsigned int, float> emotionArrayToShapeCountSize(std::vector<float> emotionArray);
EmotionIntensities emotionArrayToEmotionIntensities(std::vector<float> emotionArray);
glm::vec3 emotionArrayToVec3(std::vector<float> emotionArray);
float emotionArrayToLength(std::vector<std::vector<float>> emotionArray);
std::tuple<int,float> emotionArrayToSoftness(std::vector<float> emotionArray, bool noSoft=false, bool noMiddle=false, bool noHard=false);
float emotionArrayToRotation(std::vector<float> array0);
std::map<TRANSITIONS,bool> emotionArrayToTransitions(std::vector<float> emotionArray);
glm::vec3 emotionArrayToColor(std::vector<float> array0);
SHAPE_TYPE_3D emotionArrayToShapeType3D(std::vector<float> array0);
SHAPE_TYPE_2D emotionArrayToShapeType2D(std::vector<float> array0);
float emotionArrayToAlpha(std::vector<float> emotionArray);
glm::vec2 emotionArrayToSL(const std::vector<float> emotionArray);

// #Linear Operations -------------------------------------------------------------

std::vector<glm::vec2> borderVerticesFromShape(const shapes::Shape& shape);
std::vector<glm::vec2> borderVerticesFromShape(const float shape[], const unsigned int vert_cnt, const unsigned int stride);
std::vector<glm::vec2> borderVerticesFromShape(const float shape[], const unsigned int vert_cnt, const unsigned int stride);

// #Threads -------------------------------------------------------------

/// @brief Create ShapeLayers using emotion data and Ben's visual generator.
/// @param shapeLayersBuffer shape layers.
/// @param creationAlarm periodic timer to determine when to create a new layer.
/// @param shapeLock prevent alteration of parameters while another thread is using them.
/// @param timeLock prevent alteration of parameters while another thread is using them.
/// @param notDone indicates when to end the thread.
/// @param shapeAdditions the amount of shapes that have been generated.
void shapeLayerGenerator(std::deque<ShapeLayer>* shapeLayersBuffer, Alarm* creationAlarm, Alarm* simAlarm,  std::mutex* shapeLock, std::mutex* timeLock, std::counting_semaphore<TOTAL_LAYERS>* shapesSemaphore, bool* notDone, int* shapeAdditions);
/// @brief Loads the png file generated by Ben's visual generator.
/// @param datas buffer storing data from multiple loaded png files.
/// @param turnOff terminates the generator.
/// @param lock prevents access to datas.
/// @param filePath path to the png file.
/// @param waitTime amount of time to wait until loading the file again.

// #Start Processes -------------------------------------------------------------

int startBensVisualGenerationProgram(float frequency, int width=255, int height=255, bool doDisplay=false);
int startBensEmotionSimulator(float frequency, bool cycle = true, bool random = false);
int startMineBensEmotionSimulator(float frequency, bool cycle, bool random);

// #Unordered -------------------------------------------------------------
ShapeProperties2D generateShape2D(std::map<TRANSITIONS, bool> transitions, glm::vec3 coords, const glm::vec3 emotionCoords3D, const glm::vec3 rotAxis, const float rotScale, const glm::vec3 emotionRGB, const float alphaScale, const bool useBenTexture, const SHAPE_TYPE_2D type, const float softnessOrThickness, const unsigned int borderCase, const float emotionScale, unsigned int tag, unsigned int batchTag);
float emotionIntensitiesWeighting(EmotionIntensities eI, EmotionIntensities weights, float minT, float maxT);
void printEmotionLayers(std::deque<EmotionLayer> emotionLayers);
std::map<PULSES,bool> emotionArrayToPulses(std::vector<float> emotionArray);
float getMaxWeightingSum(EmotionIntensities weights);
float getMinWeightingSum(EmotionIntensities weights);
float emotionIntensitiesWeighting(EmotionIntensities eI, EmotionIntensities weights);
void printEmotionArray(std::vector<float> emotionArray);

int killProgram(std::string programName);
void drawRecursiveSquareHolesHelper(GLFWwindow* window, OpenGLShape& square, OpenGLShape& squareHole, LightingEnvironment& lE, unsigned int iterations, glm::vec2 position=glm::vec2(0.0f,0.0f), float size=1.0f);
void drawRecursiveSquareHoles(GLFWwindow* window, OpenGLShape& square, OpenGLShape& squareHole, LightingEnvironment& lE, unsigned int iterations, glm::vec2 position=glm::vec2(0.0f,0.0f), float size=1.0f);
void generatePNGBlank(PngData& pngData, cdata::ColorRGBA color);
void generatePNGBlank(PngData& pngData, cdata::ColorRGB color);
template<typename T> std::string vec2str(T vec, unsigned int size);
std::string vec2ToString(glm::vec2 vec2);

float getFPS(DeltaTimer deltaTimer);

std::tuple<unsigned int, unsigned int, unsigned int> transferShapeLayers(std::counting_semaphore<TOTAL_LAYERS>& shapesSemaphore, std::mutex& shapeLock, const unsigned int& requiredLayers, std::deque<ShapeLayer>& shapeLayersBuffer, ShapeLayerStorage& shapeLayersStorage, float currentDestructionTime, float currentPulseMaxTime, std::deque<PngData>& pngDataDeque);
std::tuple<std::vector<glm::vec4>, std::vector<glm::vec4>, std::vector<glm::vec4>, std::vector<glm::vec4>> modelsToCols(std::vector<glm::mat4> models);
unsigned int useFloatsToCreateTexture2D(float* floats, unsigned int width, unsigned int height);
unsigned int getGL_TextureX(unsigned int index);
void fillSquare(PngData& pngData, const float size, const glm::vec2 position, const cdata::ColorRGBA& color, const int iterations=0);
InitialShapeFeatures2D loadShapes2D(ShapeLayerStorage& shapeLayers);
ShapeLayerFeatures2D loadShapeLayer2Ds(ShapeLayerStorage& shapeLayers,  std::tuple<float, float> lineAbove, std::tuple<float, float> lineBelow, DeltaTimer deltaTimer);

template <typename T>
inline PseudoVector<T>::PseudoVector(int strides, int stride, T element)
{ 
    this->element = element;
    this->size = strides * stride;
    this->stride = stride;
    reset();
}

template <typename T>
inline std::vector<T> PseudoVector<T>::getVector()
{
    if (!isFull()){
        throw std::runtime_error("TOO SMALL!\n");
    }
    std::vector<T> dataCopy = getFilledVector(size, element);
    for(int i = 0; i < dataCopy.size(); i++){
        dataCopy[i] = data[i];
    }
    reset();
    return dataCopy;
}

template <typename T>
inline void PseudoVector<T>::reset()
{
    this->data.clear();
    this->data.reserve(size);
    for(int i = 0; i < size; i++){
        this->data.push_back(element);
    }
    index = 0;
}

#endif