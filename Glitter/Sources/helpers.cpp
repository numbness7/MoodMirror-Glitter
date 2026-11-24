#include <helpers.hpp>
unsigned int getGL_TextureX(unsigned int index){
    return GL_TEXTURE0 + index;
}

void drawMultipleTexturedShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int ind_cnt, const std::vector<unsigned int>& textures){
    shader.use();
    for(int i = 0; i < textures.size(); i++){
        unsigned int GL_TEXTUREX = getGL_TextureX(i);
        glActiveTexture(GL_TEXTUREX);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, ind_cnt, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void drawTexturedShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int ind_cnt, unsigned int texture){
    shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, ind_cnt, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
void drawShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int ind_cnt){
    shader.use();
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, ind_cnt, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void processInput(GLFWwindow* mWindow){
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(mWindow, true);
}

void processInputStatic(GLFWwindow* mWindow){
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(mWindow, true);
}
void processInputViewStatic(GLFWwindow* mWindow, float deltatime){
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(mWindow, true);
}

void create_a_shape(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, const unsigned int ind_cnt, const unsigned int vert_cnt, const float vertices[], const unsigned int indices[], const std::vector<unsigned int>& attributeSizes){
    unsigned int totalSize = 0;
    for(unsigned int count : attributeSizes){
        totalSize += count;
    }

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, totalSize*vert_cnt*sizeof(float), vertices, GL_STATIC_DRAW);
    
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind_cnt*sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // Object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int previousCount = 0;
    unsigned int attributeNumber = 0;
    for (unsigned int size : attributeSizes){
        glVertexAttribPointer(attributeNumber, size, GL_FLOAT, GL_FALSE, totalSize * sizeof(float), (void*) + (previousCount * sizeof(float)));
        glEnableVertexAttribArray(attributeNumber);
        attributeNumber++;
        previousCount += size;
    }
}

GLFWwindow* loadGLFWCreateWindow(int width, int height, bool fullscreen){
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    GLFWmonitor* monitor;
    if (count < 1){
        throw std::runtime_error("No monitors exist!\n");
    }
    else if(!USE_PRIMARY_MONITOR && count > 1){
        monitor = monitors[1];
    }
    else{
        monitor = monitors[0];

    }
    if(count <= 1 && !USE_PRIMARY_MONITOR){
        std::cout << "Only primary monitor exists" << std::endl;
    }

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    GLFWwindow* mWindow;
    if(fullscreen)
        mWindow = glfwCreateWindow(mode->width, mode->height, "Glitter", monitor, NULL);
    else
        mWindow = glfwCreateWindow(width, height, "Glitter", monitor, NULL);

    return mWindow;
}

OpenGLWindow* loadGLFWCreateWindow(int width, int height, bool fullscreen,  int renderWidth, int renderHeight){
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    auto mWindow = new OpenGLWindow();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    GLFWmonitor* monitor;
    if (count < 1){
        throw std::runtime_error("No monitors exist!\n");
    }
    else if(!USE_PRIMARY_MONITOR && count > 1){
        monitor = monitors[1];
        mWindow->monitorIndex = 1;
    }
    else{
        monitor = monitors[0];
        mWindow->monitorIndex = 0;

    }
    if(count <= 1 && !USE_PRIMARY_MONITOR){
        std::cout << "Only primary monitor exists" << std::endl;
    }

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if(fullscreen){
        glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
        mWindow->window = glfwCreateWindow(mode->width, mode->height, "Glitter", monitor, NULL);
        width = mode->width;
        height = mode->width;
    }
    else{
        mWindow->window = glfwCreateWindow(width, height, "Glitter", monitor, NULL);
    }
    mWindow->screenWidth = width;
    mWindow->screenHeight = height;
    mWindow->renderWidth = renderWidth;
    mWindow->renderHeight = renderHeight;
    mWindow->monitorCount = count;
    mWindow->monitors = monitors;
    return mWindow;
}
bool initOpenGL(OpenGLWindow*& mWindow, int& return_status, int width, int height, bool fullscreen, int renderWidth, int renderHeight){
    // Load GLFW and Create a Window
    mWindow = loadGLFWCreateWindow(width, height, fullscreen, renderWidth, renderHeight);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return_status = EXIT_FAILURE;
        return false;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow->window);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
    // Source - https://stackoverflow.com/a/1617379
    // Posted by Goz
    // Retrieved 2026-09-26, License - CC BY-SA 2.5
    
    glEnable( GL_BLEND );
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    
    glEnable(GL_DEPTH_TEST);
    return_status = EXIT_SUCCESS;
    return true;
}

int initOpenGL(OpenGLWindow *&mWindow, int width, int height, bool fullscreen, int renderWidth, int renderHeight)
{
    int return_status;
    initOpenGL(mWindow, return_status, width, height, fullscreen, renderWidth, renderHeight);
    return return_status;
}

bool initOpenGL(GLFWwindow*& mWindow, int& return_status, int width, int height){
    // Load GLFW and Create a Window
    mWindow = loadGLFWCreateWindow(width, height);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return_status = EXIT_FAILURE;
        return false;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
    // Source - https://stackoverflow.com/a/1617379
    // Posted by Goz
    // Retrieved 2026-09-26, License - CC BY-SA 2.5
    
    glEnable( GL_BLEND );
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    unsigned int VBO_O, VAO_O, EBO_O;
    
    glEnable(GL_DEPTH_TEST);
    return_status = EXIT_SUCCESS;
    return true;
}

int initOpenGL(GLFWwindow *&mWindow, int width, int height)
{
    int return_status;
    initOpenGL(mWindow, return_status, width, height);
    return return_status;
}

// learnopgl start

glm::vec2 screenToModelCoords(float x, float y, float width, float height){
    return glm::vec2(boundTransformation(x, 0.0f, width, -1.0f, 1.0f),boundTransformation(y, 0.0f, height, -1.0f, 1.0f));
}

nlohmann::json readjsonfile(std::string filepath)
{
    while(true){
        try{
            using json = nlohmann::json;
            std::string json_string = read_file(filepath.c_str());
            json data = json::parse(json_string);
            return data;
        }
        catch(std::exception e){
            // TODO: LOG THIS
            //std::cerr << e.what() << std::endl;
                
        }
    }
}

std::string formatFloat(float f, int digitsBefore, int digitsAfter, bool hasSign){
    std::string floatString = std::to_string(f);
    int decimalPointIndex;
	if(f < 0.0f)
		floatString.erase(floatString.begin());
    for(decimalPointIndex = 0; floatString[decimalPointIndex] != '.'; decimalPointIndex++);
	for(; decimalPointIndex > digitsBefore; decimalPointIndex--)
		floatString.erase(floatString.begin());
    assert(decimalPointIndex <= digitsBefore);
    for(int i = 0; i < digitsBefore - decimalPointIndex; i++){
        floatString = '0' + floatString;
    }
    int digitsAfterPoint = floatString.size() - digitsBefore  - 1;
    for(; digitsAfterPoint > digitsAfter; digitsAfterPoint--) floatString.pop_back();
    for(; digitsAfterPoint < digitsAfter; digitsAfterPoint++) floatString += '0';
	if(hasSign && floatString[0] != '-')
		if(f < 0.0f)
			floatString = '-' + floatString;
		else
			floatString = ' ' + floatString;
    return floatString;
}

EMOTION emotionIntToEnum(int emotion){
    switch(emotion){
        case 0:
            return HAPPY   ;
        case 1:
            return SAD     ;
        case 2:
            return ANGRY   ;
        case 3:
            return FEAR    ;
        case 4:
            return SURPRISE;
        case 5:
            return DISGUST ;
        default:
            throw std::runtime_error("ERROR: Invalid emotion enum: " + std::to_string(emotion));
    }
}

std::string emotionENUMToString(EMOTION emotion){
    switch(emotion){
        case HAPPY:
            return "HAPPY";
        case SAD:
            return "SAD";
        case ANGRY:
            return "ANGRY";
        case FEAR:
            return "FEAR";
        case SURPRISE:
            return "SURPRISE";
        case DISGUST:
            return "DISGUST";
        default:
            throw std::runtime_error("ERROR: Invalid emotion enum: " + std::to_string(emotion));
    }
}

void crappyClearScreen(int lines=1000){
    for(int i = 0; i < lines; i++) std::cout << std::endl;
}

std::string emotionArrayToString(std::vector<float> emotionArray){
    std::ostringstream ss;
    ss << "HAPPY\t\tSAD\t\tANGER\t\tFEAR\t\tSURPRISE\tDISGUST\t\tMAX\n";
    ss << "----------------------------------------------------------------------------------------------------" << std::endl;
    float max = std::numeric_limits<float>::min();
    int maxEmotion = -1;
    for(int col=0; col < emotionArray.size(); col++){
        float current = emotionArray[col];
        ss << formatFloat(current, 1, 7) << "\t";
        if(current > max){
            maxEmotion = col;
            max = current;
        }
    }
    ss << emotionENUMToString(emotionIntToEnum(maxEmotion)) << std::endl;
    ss << std::endl;
    return ss.str();
}

void printEmotionArray(std::vector<float> emotionArray){
    std::cout << "HAPPY\t\tSAD\t\tANGER\t\tFEAR\t\tSURPRISE\tDISGUST\t\tMAX\n";
    std::cout << "----------------------------------------------------------------------------------------------------" << std::endl;
    float max = std::numeric_limits<float>::min();
    int maxEmotion = -1;
    for(int col=0; col < emotionArray.size(); col++){
        float current = emotionArray[col];
        std::cout << formatFloat(current, 1, 7) << "\t";
        if(current > max){
            maxEmotion = col;
            max = current;
        }
    }
    std::cout << emotionENUMToString(emotionIntToEnum(maxEmotion)) << std::endl;
    std::cout << std::endl;
}

std::string emotionLayersToString(std::deque<EmotionLayer> emotionLayers){
    std::ostringstream ss;
    ss << "HAPPY\t\tSAD\t\tANGER\t\tFEAR\t\tSURPRISE\tDISGUST\t\tMAX\n";
    ss << "----------------------------------------------------------------------------------------------------" << std::endl;
    std::vector<std::vector<float>> emotionArrays{};
    for(auto const& emotionLayer : emotionLayers){
        emotionArrays.push_back(emotionLayer.emotionArray);
    }
    for(int row=0; row < emotionArrays.size(); row++){
        float max = -std::numeric_limits<float>::max();
        int maxEmotion = -1;
        for(int col=0; col < emotionArrays[0].size(); col++){
            float current = emotionArrays[row][col];
            ss << formatFloat(current, 1, 7) << "\t";
            if(current > max){
                maxEmotion = col;
                max = current;
            }
        }
        ss << emotionENUMToString(emotionIntToEnum(maxEmotion)) << std::endl;
    }
    ss << std::endl;
    return ss.str();
}

void printEmotionLayers(std::deque<EmotionLayer> emotionLayers){
    std::cout << "HAPPY\t\tSAD\t\tANGER\t\tFEAR\t\tSURPRISE\tDISGUST\t\tMAX\n";
    std::cout << "----------------------------------------------------------------------------------------------------" << std::endl;
    std::vector<std::vector<float>> emotionArrays{};
    for(auto const& emotionLayer : emotionLayers){
        emotionArrays.push_back(emotionLayer.emotionArray);
    }
    for(int row=0; row < emotionArrays.size(); row++){
        float max = -std::numeric_limits<float>::max();
        int maxEmotion = -1;
        for(int col=0; col < emotionArrays[0].size(); col++){
            float current = emotionArrays[row][col];
            std::cout << formatFloat(current, 1, 7) << "\t";
            if(current > max){
                maxEmotion = col;
                max = current;
            }
        }
        std::cout << emotionENUMToString(emotionIntToEnum(maxEmotion)) << std::endl;
    }
    std::cout << std::endl;
}

std::vector<std::vector<float>> readEmotionArray(std::string filepath, bool printIt){
    nlohmann::json data = readjsonfile(filepath);
    std::vector<std::vector<float>> emotionArray =  data["emotion_array"];
    if(printIt){
        std::cout << "HAPPY\t\tSAD\t\tANGER\t\tFEAR\t\tSURPRISE\tDISGUST\t\tMAX\n";
        std::cout << "----------------------------------------------------------------------------------------------------" << std::endl;
        for(int row=0; row < emotionArray.size(); row++){
            float max = std::numeric_limits<float>::min();
            int maxEmotion = -1;
            for(int col=0; col < emotionArray[0].size(); col++){
                float current = emotionArray[row][col];
                std::cout << formatFloat(current, 1, 7) << "\t";
                if(current > max){
                    maxEmotion = col;
                    max = current;
                }
            }
            std::cout << emotionENUMToString(emotionIntToEnum(maxEmotion)) << std::endl;
        }
        std::cout << std::endl;
    }
    return emotionArray;
}

float emotionArrayToLength(std::vector<std::vector<float>> emotionArrays){
    std::vector<float> array0 = emotionArrays[0];
    return array0[0] + 0.1f;
}

float emotionArrayToAlpha(std::vector<float> emotionArray)
{
    float happiness      = emotionArray[HAPPY];
    float sadness     = emotionArray[SAD];
    float anger   = emotionArray[ANGRY];
    float fear   = emotionArray[FEAR];
    float surprise = emotionArray[SURPRISE];
    float disgust  = emotionArray[DISGUST];
    return boundTransformation( (anger + surprise + disgust + happiness - fear - sadness), -2.0f, 4.0f, 0.0f, 1.0f);
}
std::tuple<int,float> emotionArrayToSoftness(std::vector<float> emotionArray, bool noSoft, bool noMiddle, bool noHard)
{
    float happiness = emotionArray[HAPPY];
    float sadness   = emotionArray[SAD];
    float anger     = emotionArray[ANGRY];
    float fear      = emotionArray[FEAR];
    float surprise  = emotionArray[SURPRISE];
    float disgust   = emotionArray[DISGUST];
    float softness = boundTransformation((0.25*sadness + 0.25*happiness + 0.25f*disgust - 0.25f*fear - 0.25f*anger - 0.25f*surprise), -0.75f, 0.75f, 0.0f, 1.0f);
    float hardness = (anger + surprise)/2.0f;

    if(noHard && noSoft && !noMiddle){
        return {4,softness};
    }
    if(noHard && !noSoft && noMiddle){
        return {2,boundTransformation(softness,0.0f,1.0f,0.0f,1.00f)};
    }
    if(!noHard && noSoft && noMiddle){
        return {0,hardness/4.0f};
    }
    if(noHard && noSoft && noMiddle){
        return {4,softness};
    }
    if(noMiddle){
        if(softness > hardness/8.0f)
            return {2,boundTransformation(softness,0.0f,1.0f,0.0f,1.00f)};
        else
            return {0,hardness/4.0f};
    }
    if(noHard){
        if(softness > 0.5f)
            return {2,boundTransformation(softness,0.0f,1.0f,0.0f,1.00f)};
        else
            return {4,softness};
    }
    if(noSoft){
        if(hardness > 0.5f)
            return {0,hardness/4.0f};
        else
            return {4,softness};
    }
    if (softness >= 0.50f){
        return {2,boundTransformation(softness,0.0f,1.0f,0.0f,1.00f)};
    }
    if (softness < 0.50f){
        return {0,hardness/4.0f};
    }
    return {4,softness};

}

float getMinWeightingSum(EmotionIntensities weights){
    float minWeighting = 0.0f;
    minWeighting += weights.disgust     < 0.0f ? weights.disgust : 0.0f;
    minWeighting += weights.anger       < 0.0f ? weights.anger : 0.0f;
    minWeighting += weights.sadness     < 0.0f ? weights.sadness : 0.0f;
    minWeighting += weights.happiness   < 0.0f ? weights.happiness : 0.0f;
    minWeighting += weights.fear        < 0.0f ? weights.fear : 0.0f;
    minWeighting += weights.surprise    < 0.0f ? weights.surprise : 0.0f;
    return minWeighting;
}

float getMaxWeightingSum(EmotionIntensities weights){
    float maxWeighting = 0.0f;
    maxWeighting += weights.disgust     > 0.0f ? weights.disgust : 0.0f;
    maxWeighting += weights.anger       > 0.0f ? weights.anger : 0.0f;
    maxWeighting += weights.sadness     > 0.0f ? weights.sadness : 0.0f;
    maxWeighting += weights.happiness   > 0.0f ? weights.happiness : 0.0f;
    maxWeighting += weights.fear        > 0.0f ? weights.fear : 0.0f;
    maxWeighting += weights.surprise    > 0.0f ? weights.surprise : 0.0f;
    return maxWeighting;
}

float getMaxWeight(EmotionIntensities weights){
    std::vector<float> fWeights{weights.disgust,weights.anger, weights.sadness, weights.happiness, weights.fear, weights.surprise};
    float maxi = maxM(fWeights);
    return maxi;
}
float getMinWeight(EmotionIntensities weights){
    std::vector<float> fWeights{weights.disgust,weights.anger, weights.sadness, weights.happiness, weights.fear, weights.surprise};
    float mini = minM(fWeights); 
    return mini;
}

glm::vec2 emotionArrayToSL(const std::vector<float> emotionArray){
    EmotionIntensities eI = emotionArrayToEmotionIntensities(emotionArray);
    std::map<EMOTION, float> mEI = emotionIntensitiesToEmotionMap(eI);
    auto const [mxKey, trash] = maxM(mEI);
    EmotionIntensities wS;
    wS.disgust      = 0.4f;
    wS.sadness      = 0.75f;
    wS.fear         = 0.77f;
    wS.anger        = 1.0f;
    wS.happiness    = 1.0f;
    wS.surprise     = 1.0f;
    std::map<EMOTION, float> mWS = emotionIntensitiesToEmotionMap(wS);
    float saturation = mWS[mxKey];
    EmotionIntensities wL;
    wL.disgust      = 0.5f;
    wL.sadness      = 0.4f;
    wL.fear         = 0.2f;
    wL.anger        = 0.45f;
    wL.happiness    = 0.75f;
    wL.surprise     = 0.66f;
    std::map<EMOTION, float> mWL = emotionIntensitiesToEmotionMap(wL);
    float lightness = mWL[mxKey];
    return glm::vec2(saturation, lightness);
}

glm::vec3 emotionArrayToColor(const std::vector<float> emotionArray){
    
    float happiness = emotionArray[HAPPY];
    float sadness   = emotionArray[SAD];
    float anger     = emotionArray[ANGRY];
    float fear      = emotionArray[FEAR];
    float surprise  = emotionArray[SURPRISE];
    float disgust   = emotionArray[DISGUST];
    
    float redHue =   (0.5*anger + 0.5*fear);
    float greenHue = (0.5*disgust +  0.5*surprise);
    float blueHue =  (0.5*sadness + 0.5*happiness);
    return glm::vec3(redHue,greenHue,blueHue);
    
}
SHAPE_TYPE_2D emotionArrayToShapeType2D(std::vector<float> emotionArray){
    float happiness = emotionArray[HAPPY];
    float sadness   = emotionArray[SAD];
    float anger     = emotionArray[ANGRY];
    float fear      = emotionArray[FEAR];
    float surprise  = emotionArray[SURPRISE];
    float disgust   = emotionArray[DISGUST];
    float tri = anger + surprise;
    float square = disgust + fear;
    float circ = sadness + happiness;
    if(circ > tri && circ > square){
        return CIRCLE;
    }
    else if(tri > circ && tri > square){
        return TRIANGLE;
    }
    else {
        return SQUARE;
    }
}
SHAPE_TYPE_3D emotionArrayToShapeType3D(std::vector<float> emotionArray){
    float happiness = emotionArray[HAPPY];
    float sadness   = emotionArray[SAD];
    float anger     = emotionArray[ANGRY];
    float fear      = emotionArray[FEAR];
    float surprise  = emotionArray[SURPRISE];
    float disgust   = emotionArray[DISGUST];
    float tri = anger + surprise;
    float square = disgust + fear;
    float circ = sadness + happiness;
    if(circ > tri && circ > square){
        return OCTAHEDRON;
    }
    else if(tri > circ && tri > square){
        return PYRAMID;
    }
    else {
        return CUBE;
    }
}
std::string vec2ToString(glm::vec2 vec2)
{
    std::string s;
    s += "(";
    for(int i = 0; i < 2; i++){
        s += std::to_string(vec2[i]);        
        s += ",";
    }
    s.pop_back();
    s+= ")";
    return s;
}
std::string vec3ToString(glm::vec3 vec3)
{
    std::string s;
    s += "(";
    for(int i = 0; i < 3; i++){
        s += std::to_string(vec3[i]);        
        s += ",";
    }
    s.pop_back();
    s+= ")";
    return s;
}
std::string vec4ToString(glm::vec4 vec4)
{
    std::string s;
    s += "(";
    for(int i = 0; i < 4; i++){
        s += std::to_string(vec4[i]);        
        s += ",";
    }
    s.pop_back();
    s+= ")";
    return s;
}



int emotionIndex(EMOTION emotion){
    return emotion;
}

glm::vec3 emotionArrayToVec3(std::vector<float> emotionArray)
{
    float happiness = emotionArray[HAPPY];
    float sadness   = emotionArray[SAD];
    float anger     = emotionArray[ANGRY];
    float fear      = emotionArray[FEAR];
    float surprise  = emotionArray[SURPRISE];
    float disgust   = emotionArray[DISGUST];
    float x = disgust - fear;
    float y = happiness - anger;
    float z = surprise - sadness;
    return glm::vec3(x,y,z);
}
glm::vec2 emotionArrayToVec2(std::vector<float> emotionArray)
{
    float happiness = emotionArray[HAPPY];
    float sadness   = emotionArray[SAD];
    float anger     = emotionArray[ANGRY];
    float fear      = emotionArray[FEAR];
    float surprise  = emotionArray[SURPRISE];
    float disgust   = emotionArray[DISGUST];
    float x = ((surprise + anger + happiness) - (disgust + fear + sadness))/3.0f;
    float y = ((surprise + disgust + happiness) - (anger + fear + sadness))/3.0f;
    return glm::vec2(x,y);
}
float emotionArrayToRotation(std::vector<float> emotionArray)
{
    float happiness = emotionArray[HAPPY];
    float sadness   = emotionArray[SAD];
    float anger     = emotionArray[ANGRY];
    float fear      = emotionArray[FEAR];
    float surprise  = emotionArray[SURPRISE];
    float disgust   = emotionArray[DISGUST];
    
    glm::vec2 happinessDir = dir3D::UP;
    glm::vec2 surpriseDir =  dir3D::UP_RIGHT;
    glm::vec2 angerDir = dir3D::RIGHT;
    glm::vec2 sadnessDir = dir3D::DOWN;
    glm::vec2 disgustDir = dir3D::DOWN_LEFT;
    glm::vec2 fearDir = dir3D::LEFT;
    
    glm::vec2 combineDir = glm::normalize(happiness*happinessDir + surprise* surpriseDir + anger* angerDir + sadness* sadnessDir + disgust * disgustDir + fear * fearDir);
    float rotation = glm::atan(combineDir.y, combineDir.x);
    float pi = std::numbers::pi;
    float rotScale = 2*pi*rotation;
    if(rotScale < pi/4.0f) rotScale = 0.0f;
    else if(rotScale < pi/2.0f) rotScale = pi/4.0f;
    else if(rotScale < 3.0f*pi/4.0f) rotScale = pi/2.0f;
    else if(rotScale < pi) rotScale = 3.0f*pi/4.0f;
    else if(rotScale < pi + pi/4.0f) rotScale = pi;
    else if(rotScale < pi + pi/2.0f) rotScale = pi + pi/4.0f;
    else if(rotScale < pi + 3*pi/4.0f) rotScale = pi + pi/2.0f;
    else if(rotScale < 2*pi) rotScale = pi + 3.0f*pi/4.0f;
    return rotScale;

}
LightingEnvironment initDummyMPV(std::string vertexShaderPath, std::string fragmentShaderPath, bool replaceVersion, std::string version){
    glm::mat4 view(1.0f);
    glm::mat4 model(1.0f);

    Shader object_shader(vertexShaderPath, fragmentShaderPath, replaceVersion, version);
    Shader light_shader(vertexShaderPath, fragmentShaderPath, replaceVersion, version);
    object_shader.use();
    object_shader.setUniformM4("view", view);
    object_shader.setUniformM4("model",model);
    LightingEnvironment lightingEnvironment{model,view,object_shader};
    return lightingEnvironment;
}
void setupDummyMPVEnvironmentToDraw(LightingEnvironment& lightingEnvironment){
    lightingEnvironment.object_shader.use();
    lightingEnvironment.object_shader.setUniformM4("view",lightingEnvironment.view);
}

std::vector<float> vec3ToVector(glm::vec3 a){
    return std::vector<float>{a.x,a.y,a.z};
}

glm::vec3 vectorToVec3(std::vector<float> a){
    return glm::vec3(a[0],a[1],a[2]);
}
InitialShapeFeatures2D loadShapes2D(ShapeLayerStorage& shapeLayers){

    unsigned layerCount = 0;
    unsigned batchCount = 0;
    unsigned shapeCount = 0;
    auto translationMatrices = getFilledVector<glm::mat3>(shapeLayers.getShapeCnt(), glm::mat3());
    auto rotationMatrices = getFilledVector<glm::mat3>(shapeLayers.getShapeCnt(), glm::mat3());
    auto scaleMatrices = getFilledVector<glm::mat3>(shapeLayers.getShapeCnt(), glm::mat3());
    auto BORDER_THICKNESS = getFilledVector<float>(shapeLayers.getBatchCnt(), 0.0f);
    auto batchNumbers = getFilledVector<unsigned int>(shapeLayers.getShapeCnt(), 0u);
    auto layerNumbers = getFilledVector<unsigned int>(shapeLayers.getShapeCnt(),0u);
    auto types = getFilledVector<unsigned int>(shapeLayers.getShapeCnt(), 0u);
    auto layerTags = getFilledVector<unsigned int>(shapeLayers.getShapeCnt(), 0u);
    auto batchTags = getFilledVector<unsigned int>(shapeLayers.getShapeCnt(), 0u);

    
    for(auto& shapeLayer : shapeLayers){
        int layer = shapeLayers.getLayerCnt() - 1 - layerCount;

        auto transitions = shapeLayer.transitions;
        auto pulses = shapeLayer.pulses;
        for(auto& shapeBatch : shapeLayer.batches){
            for(auto& shape : shapeBatch.shapes){
                glm::mat3 translationRotation(1.0f);
                glm::mat3 transition(translationRotation*shape.transition);
                glm::mat3 rotation(shape.rotationMatrix);
                glm::mat3 scale(shape.scaleMatrix);
                assert(layer < TOTAL_LAYERS);
                translationMatrices[shapeCount] = transition;
                rotationMatrices[shapeCount] = rotation;
                scaleMatrices[shapeCount] = scale;
                layerNumbers[shapeCount] = layer;
                types[shapeCount] = shape.type;
                batchNumbers[shapeCount] = batchCount;
                layerTags[shapeCount] = shape.layerTag;
                batchTags[shapeCount] = shape.batchTag;
                shapeCount++;
            }
            BORDER_THICKNESS[batchCount] = shapeBatch.shapes.back().borderThickness;
            batchCount++;
        }
        layerCount++;
    }
    InitialShapeFeatures2D isf;
    isf.layerNumbers = layerNumbers;
    isf.batchNumbers = batchNumbers;
    isf.borderThickness = BORDER_THICKNESS;
    isf.translationMatrices = translationMatrices;
    isf.rotationMatrices = rotationMatrices;
    isf.scaleMatrices = scaleMatrices;
    isf.types = types;
    isf.layerTags = layerTags;
    isf.batchTags = batchTags;

    return isf;
}

BatchFeatures2D loadBatches2D(ShapeLayerStorage& shapeLayers){
    unsigned layerCount = 0;
    auto tags = getFilledVector<unsigned int>(shapeLayers.getBatchCnt(), 0u);
    
    int batchCount = 0;
    for(const auto& shapeLayer : shapeLayers){
        for(const auto& batch : shapeLayer.batches){
            tags[batchCount] = batch.tag;
            batchCount++;
        }
        layerCount++;
    }
    BatchFeatures2D bf;
    bf.tags = tags;
    return bf;
}
ShapeLayerFeatures2D loadShapeLayer2Ds(ShapeLayerStorage& shapeLayers,  std::tuple<float, float> lineAbove, std::tuple<float, float> lineBelow, DeltaTimer deltaTimer){
    unsigned layerCount = 0;
    auto translations = getFilledVector<glm::vec2>(shapeLayers.getLayerCnt(), glm::vec2());
    auto rotations = getFilledVector<float>(shapeLayers.getLayerCnt(), 0.0f);
    auto scales = getFilledVector<float>(shapeLayers.getLayerCnt(), 0.0f);
    auto innerColors = getFilledVector<glm::vec4>(shapeLayers.getLayerCnt(), glm::vec4());
    auto outerColors = getFilledVector<glm::vec4>(shapeLayers.getLayerCnt(), glm::vec4());
    auto destructionTimes = getFilledVector<float>(shapeLayers.getLayerCnt(), 0.0f);
    auto destructionTimePasseds = getFilledVector<float>(shapeLayers.getLayerCnt(), 0.0f);
    auto SL_SCALEs = getFilledVector<glm::vec2>(shapeLayers.getLayerCnt(), glm::vec2());
    auto tags = getFilledVector<unsigned int>(shapeLayers.getLayerCnt(), 0u);
    
    for(auto& shapeLayer : shapeLayers){
        int layer = shapeLayers.getLayerCnt() - 1 - layerCount;
        if(layer != shapeLayer.previousLayer){
            shapeLayer.previousLayer = layer;
            shapeLayer.destructionTimer.reset();
        }
        else{
            shapeLayer.destructionTimer.checkAndUpdate();
        }
        float passedTime = shapeLayer.destructionTimer.getPassedTime();
        float waitTime = shapeLayer.destructionTimer.getWaitTime();
        shapeLayer.pulseTimer.checkAndUpdate();
        float pulseTime = shapeLayer.pulseTimer.getPassedTime();
        float pulseMaxTime = shapeLayer.pulseTimer.getWaitTime();
        auto transitions = shapeLayer.transitions;
        auto pulses = shapeLayer.pulses;
        bool doFade = ALWAYS_FADE_T || transitions[FADE_T];
        glm::vec4 innerColor = shapeLayer.innerColor;    
        glm::vec4 outerColor = shapeLayer.outerColor;    
        if(doFade){
            float fade = calcFade(passedTime,waitTime,TOTAL_LAYERS,MID_LAYER,layer,SHAPE_MIN_FADE,SHAPE_MAX_FADE, false, lineAbove, lineBelow);
            innerColor.a *= (1.0f - fade);
            outerColor.a *= (1.0f - fade);
        }
        if(pulses[FADE_P]){
            float fade = calcFade(pulseTime, pulseMaxTime, 0.0f, 0.5f, 0.5f, 1.0f, PULSE_FADE_MIN, PULSE_FADE_MAX, false);                    
            innerColor.a *= (1.0f - fade);
            outerColor.a *= (1.0f - fade);
        }
        float scaleDown = 0.0f;
        if(transitions[SCALE_T]){
            scaleDown = calcFade(passedTime,waitTime,TOTAL_LAYERS,MID_LAYER,layer,SHAPE_MIN_FADE,SHAPE_MAX_FADE, false, lineAbove, lineBelow);
        }
        float dX = 0.0f;
        if(transitions[BACK_OUT_T] || transitions[MOVE_FORWARD_T]){
            dX = calcDisplacement(passedTime,waitTime,TOTAL_LAYERS,MID_LAYER,layer,SHAPE_MIN_FADE,SHAPE_MAX_FADE, false, lineAbove, lineBelow);
        }
        dX = (transitions[BACK_OUT_T]) ? -dX : dX;
        float scaleDownPulse = 0.0f;
        if(pulses[SCALE_P]){
            scaleDownPulse = calcFade(pulseTime, pulseMaxTime, 0.0f, 0.5f, 0.5f, 1.0f, PULSE_FADE_SCALE_MIN, PULSE_FADE_SCALE_MAX, false);                    
        }
        if(pulses[SPINL_P]){
            static const float speed = 15.0f;
            shapeLayer.spin += speed*deltaTimer.getDeltaTime() / PULSE_TIME_RATIO;
        }
        if(pulses[RUMBLE_P]){
            static const float speed = 1.0f;
            shapeLayer.rumbleCoords += speed * randCoords2D() * deltaTimer.getDeltaTime() / PULSE_TIME_RATIO;
        }
        translations[layer] = dX *dir2D::RIGHT + shapeLayer.rumbleCoords;
        rotations[layer] = shapeLayer.spin;
        scales[layer] = (1.0f - scaleDown)*(1.0f - scaleDownPulse);
        assert(scaleDown  <= 1.0f && scaleDown >= 0.0f);
        innerColors[layer] = innerColor;
        outerColors[layer] = outerColor;
        destructionTimes[layer] = waitTime;
        destructionTimePasseds[layer] = passedTime;
        SL_SCALEs[layer] = shapeLayer.SL_SCALE;
        tags[layer] = shapeLayer.layerTag; // HORRIBLE AWFUL HACK
        layerCount++;
    }
    ShapeLayerFeatures2D slf;
    slf.destructionTimePasseds = destructionTimePasseds;
    slf.destructionTimes  = destructionTimes;
    slf.innerColors = innerColors;
    slf.outerColors = outerColors;
    slf.rotationMatrices = rotations;
    slf.translationMatrices = translations;
    slf.scaleMatrices = scales;
    slf.sLSCALEs = SL_SCALEs;
    slf.tags = tags;
    return slf;
}

void drawShapeLayers2D(LightingEnvironment& lightingEnvironment, OpenGLShape& openGLShape,  std::vector<OpenGLTexture>& openGLTextures, int shapeCount, FrameBuffer& fB){
    openGLShape.drawBatch(lightingEnvironment.object_shader, openGLTextures, shapeCount, fB); // TAG
}
void processInputView2D(GLFWwindow* mWindow, float deltatime, glm::vec2& position, float& zoom){
    static const float maxZoom = 500.0f;
    static const float minZoom = 1.0f;
    static const float midZoom = 1.0f;
    float zoomChange;
    if(zoom > midZoom){
        zoomChange = zoom*deltatime;
    }
    else if(zoom > minZoom){
        zoomChange = deltatime/zoom; 
    }
    else{
        zoomChange = deltatime/minZoom;
    }
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(mWindow, true);
    if (glfwGetKey(mWindow,   GLFW_KEY_A) == GLFW_PRESS)
        position.x += deltatime;
    if (glfwGetKey(mWindow,   GLFW_KEY_D) == GLFW_PRESS)
        position.x -= deltatime;
    if (glfwGetKey(mWindow,   GLFW_KEY_S) == GLFW_PRESS)
        position.y += deltatime;
    if (glfwGetKey(mWindow,   GLFW_KEY_W) == GLFW_PRESS)
        position.y -= deltatime;
    if (glfwGetKey(mWindow,   GLFW_KEY_SPACE) == GLFW_PRESS){
        zoom += zoomChange;
        zoom = (zoom < maxZoom) ? zoom : maxZoom;
    }
    if (glfwGetKey(mWindow,   GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        zoom -= zoomChange;
        zoom = (zoom > minZoom) ? zoom : minZoom;
    }
}
void endRenderLoopView2D(GLFWwindow*& mWindow, DeltaTimer& deltaTimer, glm::vec2& position, float& zoom){
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
    processInputView2D(mWindow, deltaTimer.getDeltaTime(), position, zoom);
}
void endRenderLoopViewStatic(GLFWwindow*& mWindow, DeltaTimer deltaTimer){
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
    processInputViewStatic(mWindow, deltaTimer.getDeltaTime());
}
void endRenderLoopStatic(GLFWwindow*& mWindow){
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
    processInputStatic(mWindow);
}

std::string to_string(std::vector<std::vector<float>> coefficients){
	std::string s = "[\n";
	for(int row = 0; row < coefficients.size(); row++){
		s+= "[";

		for (int col = 0; col < coefficients[0].size(); col++){
			s += std::to_string(coefficients[row][col]) + ", ";
		}
		s.pop_back();
		s += "]\n";
	}
	s += "]";
	return s;
}
std::string to_string(std::vector<float> coefficients){
	std::string s = "[";
		for (int col = 0; col < coefficients.size(); col++){
			s += std::to_string(coefficients[col]) + ", ";
		}
		s.pop_back();
		s += "]";
	return s;
}

void swap(float& a, float&b){
	float temp = a;
	a = b;
	b = temp;
}


std::vector<float> solve(std::vector<std::vector<float>> coefficients, std::vector<float> constants, const float& smallFloat){
	int rows = coefficients.size();
	int columns = coefficients[0].size();
	assert(constants.size() == rows);
	assert(rows == columns);
	assert(rows > 0);

	for(int rowTop = 0; rowTop < rows; rowTop++){
		int colLeft = rowTop;
		int rowNotZeroIndex = -1;
		for(int row = rowTop; row < rows; row++){
			if (coefficients[row][colLeft] != 0){
				rowNotZeroIndex = row;
				break;
			}
		}


		if(rowNotZeroIndex >= 0){ // Only bother with column with one of its values are not zero
			coefficients[rowTop].swap(coefficients[rowNotZeroIndex]);
			swap(constants[rowTop], constants[rowNotZeroIndex]);
			for(int row = rowTop; row < rows; row++){ // Set leftmost column to 1
				float divisor = coefficients[row][colLeft];
				if(divisor > smallFloat || divisor < -smallFloat){
					constants[row] /= divisor;
					for(int col = colLeft; col < columns; col++){
						coefficients[row][col] /= divisor;
					}
				}
			}
			if(rowTop < rows - 1){ // Set rows below row top leftmost column to 0
				for(int row = rowTop+1; row < rows; row++){
					if(coefficients[row][colLeft] > smallFloat || coefficients[row][colLeft] < -smallFloat){ // Don't subtract row already in correct form
						constants[row] -= constants[rowTop];
						for(int col = colLeft; col < columns; col++){
							coefficients[row][col] -= coefficients[rowTop][col];
						}
					}
				}
			}
		}
	}
	for(int rowBottom = 1; rowBottom < rows; rowBottom++){
		int colLeft = rowBottom;
		for(int row = rowBottom - 1; row >= 0; row--){
			float multiplicand = coefficients[row][colLeft];
			constants[row] -= multiplicand*constants[rowBottom];
			for(int col = colLeft; col < columns; col++){
				coefficients[row][col] -= multiplicand*coefficients[rowBottom][col];
			}
		}
	}
	return constants;
}

void shapeLayerGenerator(std::deque<ShapeLayer>* shapeLayersBuffer, Alarm* creationAlarm, Alarm* simAlarm,  std::mutex* shapeLock, std::mutex* timeLock, std::counting_semaphore<TOTAL_LAYERS>* shapesSemaphore, bool* notDone, int* shapeAdditions){
    bool alarmSounded;
    Alarm printAlarm{1.0f};
    long int iterations = 0;
    PngData blankPNGData(COLOR_WIDTH,COLOR_HEIGHT, 4);
    PngData borderPNGData(COLOR_WIDTH,COLOR_HEIGHT, 4);
    generatePNGBorder(borderPNGData, borderPNGData.width, borderPNGData.height, 10, 10, cdata::BLACK_D_RGBA, cdata::WHITE_D_RGBA, false, false);
    generatePNGBlank(blankPNGData);
    std::deque<EmotionLayer> emotionLayers{};
    for(int i = 0; i < TOTAL_EMOTION_LAYERS; i++)
        emotionLayers.push_front({std::vector<float>{1,1,1,1,1,1},borderPNGData});

    std::string filepath = "../background.png";

    PngData pD(COLOR_WIDTH,COLOR_HEIGHT,3);
    ShapeLayer shapeLayer{};
    while(*notDone){
        bool printAlarmSounded = printAlarm.checkAndUpdate();
        bool simAlarmSounded;
        {
            std::lock_guard<std::mutex> guard(*timeLock);
            simAlarmSounded = (*simAlarm).checkAndUpdate();
            alarmSounded  = creationAlarm->checkAndUpdate();
        }
        if(simAlarmSounded){
            do{
                pD = loadPngTexture(filepath);
                std::this_thread::sleep_for(std::chrono::nanoseconds(1));
            } while((!pD.isValid()));
            emotionLayers.push_front({readEmotionArray("../data.json",false)[0],pD});
            emotionLayers.pop_back();
        }

        
        bool useBenTexture = rand_range_uniform(0.0f,1.0f) < USE_BEN_CHANCE;
        
        // #Emotion Extraction
        // Use emotion array to set properties of shapes
        if(alarmSounded){
            std::vector<std::vector<float>> emotionArrays{};
            shapeLayer = {};
            unsigned int shapeLayerTag = rand_range_uniform(0, std::numeric_limits<unsigned int>::max());
            for(auto const& emotionLayer : emotionLayers){
                auto const& emotionArray = emotionLayer.emotionArray;
                emotionArrays.push_back(emotionArray);
                glm::vec3 emotionRGB;

                emotionRGB = emotionArrayToColor(emotionArray);
                glm::vec2 emotionCoords2D = emotionArrayToVec2(emotionArray);
                glm::vec3 emotionCoords3D = glm::vec3(emotionCoords2D.x, emotionCoords2D.y, 0.0f);
                glm::vec3 coords;
                glm::vec3 rotAxis = glm::vec3(0.0f,0.0f,-1.0f);
                float rotScale = emotionArrayToRotation(emotionArray);
                //float rotScale = 0.0f;

                SHAPE_TYPE_2D type = emotionArrayToShapeType2D(emotionArray);
                auto transitions = emotionArrayToTransitions(emotionArray);
                auto pulses = emotionArrayToPulses(emotionArray);
                const auto [borderCase, softnessOrThickness] = emotionArrayToSoftness(emotionArray,false, true, false);
                transitions[VORTEX_T] = rand_range_uniform(0.0f, 1.0f) <= 0.1f;

                float alphaScale = 1.0f;
                alphaScale = clamp(rand_range_normal(AVE_ALPHA, DEV_ALPHA), MIN_ALPHA, MAX_ALPHA);
                auto const& [count, size] = std::tuple<unsigned int, float>{(unsigned int)1,(float)1.0f};
                unsigned int batchTag = rand_range_uniform(0, std::numeric_limits<unsigned int>::max());

                ShapeBatch shapeBatch{std::deque<ShapeProperties2D>{}, emotionLayer.pngData, pulses, batchTag};

                for (int shapeIndex = 0; shapeIndex < SHAPES_IN_BATCH*count; shapeIndex++){
                    auto shape = generateShape2D(transitions, coords, emotionCoords3D, rotAxis, rotScale, emotionRGB, alphaScale, useBenTexture, type, 1.0f, 2, size, shapeLayerTag, batchTag);
                    shapeBatch.shapes.push_front(shape);
                    shapeAdditions++;
                }
                shapeLayer.batches.push_front(shapeBatch);
            }
            shapeLayer.emotionArray = average(emotionArrays);
            shapeLayer.SL_SCALE = emotionArrayToSL(shapeLayer.emotionArray);
            shapeLayer.transitions = emotionArrayToTransitions(shapeLayer.emotionArray);
            shapeLayer.emotionLayers = emotionLayers;
            shapeLayer.pulses = emotionArrayToPulses(shapeLayer.emotionArray);
            shapeLayer.layerTag = shapeLayerTag;
            {
                shapesSemaphore->acquire();
                std::lock_guard<std::mutex> guard(*shapeLock);
                shapeLayersBuffer->push_front(shapeLayer);
            }
        iterations++;
        }

    }

}

int startBensEmotionSimulator(float frequency, bool cycle, bool random){
    assert(cycle || random);
    char simulationType;
    if (cycle)
        simulationType = 'c';

    if (random)
        simulationType = 'r';

    return runCommand("(cd ../Emotion_Gen_Sim && source venv/bin/activate && echo -e \"" + std::to_string(frequency) + "\n" + simulationType +"\" | nohup python emotion_generator.py)");
}
int startMineBensEmotionSimulator(float frequency, bool cycle, bool random){
    assert(cycle || random);
    char simulationType;
    if (cycle)
        simulationType = 'c';

    if (random)
        simulationType = 'r';

    return runCommand("(cd " + std::string(SIM_PATH) + " && source venv/bin/activate && echo -e \"" + std::to_string(frequency) + "\n" + simulationType +"\" | nohup " + std::string(SIM_PROGRAM) + ")");
}

int startBensVisualGenerationProgram(float frequency, int width, int height, bool doDisplay){

    char display;
    if (doDisplay)
        display = 'y';
    else
        display = 'n';
    
    return runCommand("(cd " + std::string(COLOR_PATH) + " && source venv/bin/activate && echo -e \"" + std::to_string(width) + "\n" 
        + std::to_string(height) + "\n" + std::to_string(frequency) + "\n" + display + "\" | nohup " + COLOR_PROGRAM + ")");
}

std::tuple<shapes::Shape, std::vector<glm::vec2>> createCircleShapeBorder(unsigned int tri_cnt){
    std::vector<float> circleVertices{};
    std::vector<unsigned> ind{};
    unsigned int index = 0;
    std::vector<glm::vec2> border;
    for(unsigned int tri = 0; tri < tri_cnt; tri++){
        for(int vi = 0; vi<3;vi++){
            glm::vec3 vert;
            if(vi > 0){
                glm::mat4 transform(1.0f);
                float angle = glm::radians(((float)tri+vi)*360.0f/(float)tri_cnt);
                transform = glm::rotate(transform, angle, glm::vec3(0.0f,0.0f,1.0f));
                glm::vec3 radius_vec(0.5f,0.0f,0.0f);
                glm::vec4 radius_vec4(radius_vec,1.0f);
                radius_vec4 = transform * radius_vec4;
                vert = glm::vec3(radius_vec4.x,radius_vec4.y,radius_vec4.z);
                assert(vert.x != 0.0f || vert.y != 0.0f);
                if(vi == 1) // Vetice in middle of right turn path
                    border.push_back(glm::vec2(vert.x,vert.y));
            }
            else{
                vert = glm::vec3(0.0f,0.0f,0.0f); // Center Vertex
            }
            circleVertices.push_back(vert.x);
            circleVertices.push_back(vert.y);
            circleVertices.push_back(vert.z);
            circleVertices.push_back(boundTransformation(vert.x, -0.5f, 0.5f, 0.0f, 1.0f)); // Texture x coordinate (Have to normalize it to [0,1])
            circleVertices.push_back(boundTransformation(vert.y, -0.5f, 0.5f, 0.0f, 1.0f)); // Texture x coordinate (Have to normalize it to [0,1])
            
            for(unsigned int i = 0; i<3; i++){
                ind.push_back(index);
                index++;
            }
        }
    }
    shapes::Shape circleShape(circleVertices.data(), (unsigned) (tri_cnt*3), std::vector<unsigned>{3, 2}, ind.data(), (unsigned) (tri_cnt)*3);
    return {circleShape, border};
}

float calcFade(float destructionTimePassed, float maxDestructionTime, float changePointStart, float changePointBefore, float changePointAfter, float changePointEnd, float shapeMinFade, float shapeMaxFade, bool delay){
	assert(maxDestructionTime != 0);
	float timePassed;
    if(destructionTimePassed < changePointStart*maxDestructionTime){
        return shapeMaxFade;
    }
    else if (destructionTimePassed < changePointBefore*maxDestructionTime){
        timePassed = boundTransformation(destructionTimePassed, changePointStart*maxDestructionTime, changePointBefore*maxDestructionTime, shapeMinFade, shapeMaxFade);
        return shapeMaxFade - timePassed;
    }
    else if (destructionTimePassed < changePointAfter*maxDestructionTime){
        return shapeMinFade;
    }
    else if (destructionTimePassed < changePointEnd*maxDestructionTime){
        timePassed = boundTransformation(destructionTimePassed, changePointAfter*maxDestructionTime, changePointEnd*maxDestructionTime, shapeMinFade, shapeMaxFade);
        return shapeMinFade + timePassed;
    }
    else{
        return shapeMaxFade;
    }
}

float calcFade(float destructionTimePassed, float destructionTime, int totalLayers, int midLayer, int layer, float shapeMinFade, float shapeMaxFade, bool delay, std::tuple<float, float> lineAbove, std::tuple<float, float> lineBelow){
    float fade = 0.0f;
	assert(totalLayers != 0);
	assert(midLayer != 0);
	assert(destructionTime != 0);
	float timePassed;
	float b;
	float slope;
    
    if(delay){
        if(layer > midLayer) return 0;
        totalLayers = midLayer + 1;
        midLayer = 1*midLayer/4;
    }

	if(layer > midLayer){
		timePassed = destructionTimePassed + destructionTime*(totalLayers - 1 - layer);
		if(timePassed < 0) 
			std::cout << "<0 above" << std::endl;
		auto const [slopeT, interceptT] = lineAbove;
		b = interceptT;
		slope = slopeT;

	}
	else{
		timePassed = destructionTimePassed + destructionTime*(midLayer - layer);
		if(timePassed < 0) 
			std::cout << "<0 below" << std::endl;
		auto const [slopeT, interceptT] = lineBelow;
		b = interceptT;
		slope = slopeT;
	}
    fade = b + slope*timePassed;
    return fade;
}

void minimumStartRenderLoop(DeltaTimer& deltaTimer, glm::vec4 backgroundColor){
    deltaTimer.updateDeltaTime();
    // Background Fill Color
    glClearColor(backgroundColor.x,backgroundColor.y,backgroundColor.z,backgroundColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
void startDummyMPVLoop(DeltaTimer& deltaTimer, LightingEnvironment& lightingEnvironment, glm::vec4 backgroundColor){
    minimumStartRenderLoop(deltaTimer, backgroundColor);
    setupDummyMPVEnvironmentToDraw(lightingEnvironment);
}

// #Classes -------------------------------------------------------------
DeltaTimer::DeltaTimer(){
    updateDeltaTime();
}
float DeltaTimer::getDeltaTime(bool do_update){
    if(do_update) 
        updateDeltaTime();
    return deltatime;
}
void DeltaTimer::updateDeltaTime(){
    elapsed_time = glfwGetTime();
    deltatime = elapsed_time - lastframe_time;
    lastframe_time = elapsed_time;
}
Alarm::Alarm(float waitTime) : waitTime(waitTime)
{
}

void Alarm::setWaitTime(float waitTime)
{
    this->waitTime = waitTime;
    this->time = 0.0f;
    this->timer.updateDeltaTime();
}

bool Alarm::checkAndUpdate()
{
    time += timer.getDeltaTime(true);
    if (time >= waitTime) {
        time = 0.0f;
        alarmSounded = true;
    }
    else alarmSounded = false;
    return alarmSounded;
}

void StraightPath::setProgress(float prog, bool loop) { 
    if (loop){
        if( prog < 0){
            progress = 1.0f;
        }
        else if( prog > 1){
            progress = 0.0f;
        }
        else{
            progress = prog; 
        }
    }
    else if (prog >= 0.0f && prog <= 1.0f + ProgressMargin )
        progress = prog; 
    else{
        throw std::runtime_error("Error: Invalid progress: " + std::to_string(prog) + ", should be between 0 and 1!");
    }
}

glm::vec3 StraightPath::getPositionOnPath(){
    int numPoints = points.size();
    // Find start point
    float point_progress = float(progress*(float)(numPoints-1));
    int start_point_index = (int)point_progress;
    if(start_point_index >= numPoints - 1){
        return points.back();
    }
    int end_point_index = start_point_index + 1;
    glm::vec3 start_point = points[start_point_index];
    glm::vec3 end_point = points[end_point_index];
    glm::vec3 direction = glm::normalize(end_point-start_point);
    float distance = glm::distance(end_point,start_point);
    float left_over_point_progress = point_progress - (float) start_point_index;
    glm::vec3 change_vector = direction * distance * left_over_point_progress;
    glm::vec3 point_on_path = start_point + change_vector;
    return start_point + change_vector;
}
OpenGLShape::OpenGLShape(unsigned int vert_cnt, unsigned int ind_cnt, const float vertices[], const unsigned int indices[], const std::vector<unsigned int>& attributeSizes, unsigned int texture, bool hasTexture){
    this->ind_cnt = ind_cnt;
    this->vert_cnt = vert_cnt;
    this->textures = std::vector<unsigned int>{texture};
    this->hasTextures = hasTexture;
    create_a_shape(this->VAO, this->VBO, this->EBO, this->ind_cnt, this->vert_cnt, vertices, indices, attributeSizes);
}
OpenGLShape::OpenGLShape(const shapes::Shape &shape, unsigned int texture, bool hasTexture) : OpenGLShape(shape.vertCnt, shape.indCnt, shape.vertices, shape.indices, shape.attributeSizes, texture, hasTexture)
{
}
OpenGLShape::OpenGLShape(const shapes::Shape &shape, const std::vector<unsigned int> &textures, const bool hasMultipleTextures)
{
    this->ind_cnt = shape.indCnt;
    this->vert_cnt = shape.vertCnt;
    this->hasTextures = hasMultipleTextures;
    setTextures(textures);
    create_a_shape(this->VAO, this->VBO, this->EBO, this->ind_cnt, this->vert_cnt, shape.vertices, shape.indices, shape.attributeSizes);
}
void OpenGLShape::draw(Shader& shader)
{
    if(hasTextures)
        drawMultipleTexturedShape(VAO, EBO, shader, ind_cnt, textures);
    else
        drawShape(VAO, EBO, shader, ind_cnt);
}

void OpenGLShape::draw(Shader& shader, std::vector<unsigned int> textures)
{
    drawMultipleTexturedShape(VAO, EBO, shader, ind_cnt, textures);
}

void drawMultipleTexturedShapeBatch(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int ind_cnt, const std::vector<unsigned int>& textures, unsigned int count){
    shader.use();
    for(int i = 0; i < textures.size(); i++){
        unsigned int GL_TEXTUREX = getGL_TextureX(i);
        glActiveTexture(GL_TEXTUREX);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElementsInstanced(GL_TRIANGLES, ind_cnt, GL_UNSIGNED_INT, 0, count);
    glBindVertexArray(0);
}
void OpenGLShape::drawBatch(Shader& shader, const std::vector<unsigned int> &textures, unsigned int count)
{
    drawMultipleTexturedShapeBatch(VAO, EBO, shader, ind_cnt, textures, count);
}

void OpenGLShape::drawBatch(Shader& shader, unsigned int count)
{
    drawMultipleTexturedShapeBatch(VAO, EBO, shader, ind_cnt, textures, count);
}

//void drawMultipleTexturedShapeBatch(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int ind_cnt, const std::vector<unsigned int>& textures, unsigned int count, FrameBuffer& fB);
//void drawMultipleTexturedShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int ind_cnt, const std::vector<unsigned int>& textures);
void OpenGLShape::drawBatch(Shader &shader, const std::vector<OpenGLTexture> &textures, unsigned int count, FrameBuffer &fB)
{
    shader.use();
    for(int texturesIndex = 0; texturesIndex < textures.size(); texturesIndex++){
        static const unsigned int textureColorBufferOffset = 1;
        unsigned int GL_TEXTUREX = getGL_TextureX(texturesIndex + textureColorBufferOffset);
        glActiveTexture(GL_TEXTUREX);
        glBindTexture(GL_TEXTURE_2D, textures[texturesIndex].address);
        if (textures[texturesIndex].name != "")
            shader.setUniformI(textures[texturesIndex].name, texturesIndex + textureColorBufferOffset);
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElementsInstanced(GL_TRIANGLES, ind_cnt, GL_UNSIGNED_INT, 0, count);
    glBindVertexArray(0);
}

// #Main Programs -------------------------------------------------------------

int mainCoordsBasic2D(int argc, char * argv[]){
    // Initialize OpenGL
    int return_status;
    OpenGLWindow*  mWindow; 
    if((return_status = initOpenGL(mWindow, mWidth, mHeight, FULL_SCREEN, mRWidth, mRHeight)) != EXIT_SUCCESS) return return_status;


    LightingEnvironment lE = initDummyMPV("Glitter/Shaders/texture-mvp-color.vs", "Glitter/Shaders/texture-mvp-color.fs");
    // ====================================================================================================
    DeltaTimer deltaTimer{};
    Alarm creationAlarm(SHAPE_INITIAL_SPAWN_TIME);
    float currentDestructionTime = creationAlarm.getWaitTime();
    bool shapesFull = false;
    PngData blankPNGData(COLOR_WIDTH,COLOR_HEIGHT, 3);
    const unsigned int blankTexture = obtainPNGBlank(blankPNGData);
    PngData pngDataSquare(COLOR_WIDTH, COLOR_HEIGHT);
    std::vector<glm::vec2> squareBorder = borderVerticesFromShape(shapes::rectT);
    std::vector<float> distances; 
    distances.reserve(COLOR_HEIGHT*COLOR_WIDTH);
    generateDistancesFromBorder(distances, COLOR_WIDTH, COLOR_HEIGHT, squareBorder);
    unsigned int squareBorderTexture = useFloatsToCreateTexture2D(distances.data(), COLOR_WIDTH, COLOR_HEIGHT);

    std::vector<glm::vec2> triangleBorder = borderVerticesFromShape(shapes::eTriT);
    std::vector<float> tridistances; 
    tridistances.reserve(COLOR_HEIGHT*COLOR_WIDTH);
    generateDistancesFromBorder(tridistances, COLOR_WIDTH, COLOR_HEIGHT, triangleBorder);
    unsigned int triangleBorderTexture = useFloatsToCreateTexture2D(tridistances.data(), COLOR_WIDTH, COLOR_HEIGHT);
    
    auto [trash1, circleBorder] = createCircleShapeBorder(16);
    std::vector<float> circle_distances; 
    circle_distances.reserve(COLOR_HEIGHT*COLOR_WIDTH);
    generateDistancesFromBorder(circle_distances, COLOR_WIDTH, COLOR_HEIGHT, circleBorder);
    unsigned int circleBorderTexture = useFloatsToCreateTexture2D(circle_distances.data(), COLOR_WIDTH, COLOR_HEIGHT);

    OpenGLShape square(shapes::rectT);
    

    lE.view = glm::scale(lE.view, glm::vec3(1.2f,1.2f,0.0f));
    glm::mat4 model(1.0f);
    lE.object_shader.use();
    lE.object_shader.setUniformI("ourTexture", 0);
    lE.object_shader.setUniformI("borderTexture", 1);
    lE.object_shader.setUniformI("borderCase", 2);
    float destructionTime = DESTRUCTION_TIME;
    float totalLayers = TOTAL_LAYERS;
    float midLayer = MID_LAYER;
    float shapeMaxFade = SHAPE_MAX_FADE;
    float shapeMinFade = SHAPE_MIN_FADE;
    std::tuple<float,float> lineAbove = linearTransformation(0.0f, destructionTime*(totalLayers - 1 - midLayer), shapeMaxFade, shapeMinFade);
    auto const [slopeAT, interceptAT] = lineAbove;
    std::tuple<float,float> lineBelow = linearTransformation(0.0f, destructionTime*(midLayer + 1), shapeMinFade, shapeMaxFade);
    auto const [slopeBT, interceptBT] = lineBelow;
    lE.object_shader.setUniformF("destructionTime", destructionTime);
    lE.object_shader.setUniformI("MID_LAYER", midLayer);
    lE.object_shader.setUniformF("ABOVE_SLOPE", slopeAT);
    lE.object_shader.setUniformF("ABOVE_INTERCEPT", interceptAT);
    lE.object_shader.setUniformF("BELOW_SLOPE", slopeBT);
    lE.object_shader.setUniformF("BELOW_INTERCEPT", interceptBT);
    lE.object_shader.setUniformF("DISABLE_ALMOST_EVERYTHING", false);
    lE.object_shader.setUniformF("USE_MODEL_ARRAY", true);
    lE.object_shader.setUniformI("BATCHES_CNT", TOTAL_LAYERS*TOTAL_EMOTION_LAYERS);

    std::deque<ShapeLayer> shapeLayersBuffer{};
    ShapeLayerStorage shapeLayersStorage{};
    ShapeLayerStorage shapeLayersTransfer{};
    glm::vec4 backgroundColor(0.0f);
    std::mutex shapeLock{};
    std::mutex timeLock{};
    std::counting_semaphore<TOTAL_LAYERS> shapesSemaphore{0};
    bool notDone = true;
    int additions = 0;
    int simPID, colPID, killPID;

    if(SIM_START){
        simPID = startMineBensEmotionSimulator(SIM_INITIAL_PERIOD, SIM_CYCLE, SIM_RANDOM);
    }
    if(COLOR_START){
        colPID = startBensVisualGenerationProgram(COLOR_FREQUENCY, COLOR_WIDTH, COLOR_HEIGHT, COLOR_DISPLAY);
    }
    Alarm otherDestructionAlarm{SHAPE_INITIAL_SPAWN_TIME};
    Alarm simAlarm{SIM_INITIAL_READ_PERIOD};
    std::thread creationThread(shapeLayerGenerator,&shapeLayersBuffer,&creationAlarm, &simAlarm, &shapeLock, &timeLock, &shapesSemaphore, &notDone, &additions);
    bool midLayerMet = false;
    float requiredLayersFloat = 0.0f;
    float currentPulseMaxTime = PULSE_TIME_INITAL;
    Alarm printAlarm(PRINT_TIME);
    debug::RunawayPrinter runawayPrinter{1.3f};
    debug::StopWatch stopWatch{};


    PngData pd(COLOR_WIDTH,COLOR_HEIGHT,3);
    OpenGLTextureOptions options = ttm::png3Map(COLOR_WIDTH, COLOR_HEIGHT);
    TextureAtlas textureAtlas(options, TOTAL_BATCHES,  NUMBER_OF_ATLAS_TEXTURES, "ATLAS_TEXTURES");
    unsigned int trashInt;
    OpenGLTextureOptions tagOptions = ttm::map2D(&trashInt, 1,1);
    TextureAtlas tagAtlas(tagOptions, TOTAL_BATCHES,  1, "ATLAS_TAGS_TEXTURE");
    runawayPrinter.pause(3.0f);
    std::deque<OpenGLTexture> shapeTextures;
    PseudoVector<glm::vec4> shapeVec4s(TOTAL_SHAPES/SHAPES_DIVIDER,VEC4S_PER_SHAPE,glm::vec4(0.0f));
    PseudoVector<glm::vec4> layerVec4s(TOTAL_LAYERS,VEC4S_PER_SHAPE,glm::vec4(0.0f));
    debug::FrequencyWatch fpsWatch{60};

    
    // ====================================================================================================
    std::vector<FrameBuffer> fBs = {};
    for(const auto& distribution : distributions)
        fBs.push_back(initFrameBuffer(mWindow->renderWidth/std::get<1>(distribution), mWindow->renderHeight/std::get<2>(distribution), mWindow->screenWidth, mWindow->screenHeight));
    std::cout << "ScreenWidth: " << mWindow->screenWidth << std::endl;
    std::cout << "ScreenHeight: " << mWindow->screenHeight << std::endl;
    std::cout << "RenderWidth: " << mWindow->renderWidth << std::endl;
    std::cout << "RenderHeight: " << mWindow->renderHeight << std::endl;
    // ====================================================================================================
    // #Render 2D Loop
    while (!glfwWindowShouldClose(mWindow->window)) {
        fpsWatch.step();

        startDummyMPVLoop(deltaTimer, lE, backgroundColor);
        

        glEnable(GL_BLEND);

        glBlendFuncSeparate(
            GL_SRC_ALPHA,
            GL_ONE_MINUS_SRC_ALPHA,
            GL_ONE,
            GL_ONE_MINUS_SRC_ALPHA
        );
    // ====================================================================================================
        //{
            requiredLayersFloat += deltaTimer.getDeltaTime()/currentDestructionTime;
            unsigned int requiredLayers = (unsigned int) floorWrap(requiredLayersFloat);
            std::tuple<unsigned int, unsigned int, unsigned int> transfers;
            std::deque<PngData> pngDataDeque;

            if(requiredLayers > 0)
            {
                requiredLayersFloat -= (float) requiredLayers;
                transfers = transferShapeLayers(shapesSemaphore, shapeLock, requiredLayers, shapeLayersBuffer, shapeLayersTransfer, currentDestructionTime, currentPulseMaxTime,pngDataDeque);
            }
            

            for(const auto batch : shapeLayersTransfer.getBatches()){
                textureAtlas.replaceTexture(batch->pngData.getData());
                tagAtlas.replaceTexture(&batch->tag);
            }
            if (std::get<0>(transfers) > 0){
                for(const auto shape : shapeLayersTransfer.getShapes()){
                    assert(shapeVec4s.getSize() % VEC4S_PER_SHAPE == 0);
                    shapeVec4s.push_back( glm::vec4(glm::vec2(shape->coords), glm::angle(shape->direction) , shape->scaleMatrix[0][0]));
                    shapeVec4s.push_back( glm::vec4(*reinterpret_cast<const float*>(&shape->type    ),*reinterpret_cast<const float*>(&shape->layerTag    ),*reinterpret_cast<const float*>(&shape->batchTag    ), 0.0f));
                    if (shapeVec4s.isFull()){
                        if(shapeTextures.size() >= SHAPES_DIVIDER)
                            shapeTextures.pop_back();
                        shapeTextures.push_front({"", useTextureDataToCreateTexture(shapeVec4s.getVector(), shapeVec4s.getSize(), 1)});
                        int i = 0;
                        for(auto& texture : shapeTextures){
                            texture.name = "SHAPES_TEXTURES[" + std::to_string(i) + "]";
                            i++;
                        }
                    }
                }
            }

            while(!shapeLayersTransfer.empty()){
                shapeLayersStorage.push_front(shapeLayersTransfer.back());
                shapeLayersTransfer.pop_back();
            }

            bool shapeDeleted = false;
            // Delete cubes
            while (shapeLayersStorage.size() > TOTAL_LAYERS){
                auto const shapeLayer = shapeLayersStorage.back();
                shapeDeleted = true;
                shapeLayersStorage.pop_back();
            }
            if(shapeLayersStorage.size() >= (MID_LAYER - 0) && !midLayerMet){
                midLayerMet = true;
                std::cout << "The middle layer was met!!!" << std::endl;

            }
            if(shapeDeleted && !shapesFull){
                std::lock_guard guard(timeLock);
                creationAlarm.setWaitTime(DESTRUCTION_TIME);
                otherDestructionAlarm.setWaitTime(DESTRUCTION_TIME);
                simAlarm.setWaitTime(SIM_READ_PERIOD);
                if(SIM_START){
                    killCommand(simPID);
                    simPID = startMineBensEmotionSimulator(SIM_PERIOD, true, false);
                }
                currentDestructionTime = creationAlarm.getWaitTime();
                currentPulseMaxTime = PULSE_TIME;
                shapesFull = true;
                unsigned layerCount = 0;
                for(auto& shapeLayer : shapeLayersStorage){
                    int layer = TOTAL_LAYERS - 1 - layerCount;
                    shapeLayer.destructionTimer.setWaitTime(currentDestructionTime);
                    shapeLayer.pulseTimer.setWaitTime(currentPulseMaxTime);
                    shapeLayer.previousLayer = -1;
                    layerCount++;
                }
                std::cout << "Shapes loaded!" << std::endl;
            }
            // Draw cubes
            glDisable(GL_DEPTH_TEST);
            auto slf = loadShapeLayer2Ds(shapeLayersStorage, lineAbove, lineBelow, deltaTimer);
            auto bf = loadBatches2D(shapeLayersStorage);
            std::vector<OpenGLTexture> openGLTextures{
                {"ourTexture",blankTexture},
                {"SQUARE_BORDER_TEXTURE",squareBorderTexture},
                {"CIRCLE_BORDER_TEXTURE",circleBorderTexture},
                {"TRIANGLE_BORDER_TEXTURE",triangleBorderTexture},
                {"TRANS_L_TEXTURE",useTextureDataToCreateTexture(slf.translationMatrices, shapeLayersStorage.getLayerCnt(), 1)},
                {"ROTATE_L_TEXTURE",useTextureDataToCreateTexture(slf.rotationMatrices, shapeLayersStorage.getLayerCnt(), 1)},
                {"SCALE_L_TEXTURE",useTextureDataToCreateTexture(slf.scaleMatrices, shapeLayersStorage.getLayerCnt(), 1)},
                {"INNER_COLORS_TEXTURE",useTextureDataToCreateTexture(slf.innerColors, shapeLayersStorage.getLayerCnt(), 1)},
                {"OUTER_COLORS_TEXTURE",useTextureDataToCreateTexture(slf.outerColors, shapeLayersStorage.getLayerCnt(), 1)},
                {"SL_SCALES_TEXTURE",useTextureDataToCreateTexture(slf.sLSCALEs, shapeLayersStorage.getLayerCnt(), 1)},
                {"LAYER_TAGS_TEXTURE",useTextureDataToCreateTexture(slf.tags, shapeLayersStorage.getLayerCnt(), 1)},
                {"BATCH_TAGS_TEXTURE",useTextureDataToCreateTexture(bf.tags, shapeLayersStorage.getBatchCnt(), 1)}
            };
            for(const auto& texture : textureAtlas.getTextures())
                openGLTextures.push_back(texture);
            for(const auto& texture : tagAtlas.getTextures())
                openGLTextures.push_back(texture);
            for(const auto& texture : shapeTextures)
                openGLTextures.push_back(texture);


            lE.object_shader.setUniformI("maxLayer", TOTAL_LAYERS); // TAG
            lE.object_shader.setUniformB("DO_FADE", false); // TAG
            lE.object_shader.setUniformI("borderCase", 2); // TAG
            lE.object_shader.setUniformI("ATLAS_SHIFT_LEFT", textureAtlas.getShiftLeft()); // TAG
            lE.object_shader.setUniformI("BATCHES_PER_ATLAS_TEXTURE", textureAtlas.getTexturesPerAtlas()); // TAG
            lE.object_shader.setUniformI("SHAPES_PER_LAYER", SHAPES_PER_LAYER); // TAG
            lE.object_shader.setUniformI("SHAPES_PER_BATCH", SHAPES_IN_BATCH); // TAG
            lE.object_shader.setUniformI("BATCHES_PER_LAYER", BATCHES_PER_LAYER); // TAG
            lE.object_shader.setUniformI("TOTAL_LAYERS", TOTAL_LAYERS); // TAG
            lE.object_shader.setUniformI("TOTAL_BATCHES", TOTAL_BATCHES); // TAG
            
    // ====================================================================================================
            glDisable(GL_DEPTH_TEST);
        if(!shapeLayersStorage.empty()){
            int offset = 0;
            for(int i = 0; i < fBs.size(); i++){ // Render Frame buffers
                int shapeBatchCount = (int) ceilWrap(std::get<0>(distributions[i])*((float)shapeLayersStorage.getShapeCnt()));
                auto& fBCur = fBs[i];
                glBindFramebuffer(GL_FRAMEBUFFER, fBCur.framebuffer);
                glClearColor(0, 0, 0, 0);
                glClear(GL_COLOR_BUFFER_BIT);

                glViewport(0,0, fBCur.RND_WIDTH, fBCur.RND_HEIGHT);
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                fBCur.screenShader.use();
                fBCur.screenShader.setUniformI("screenTexture", 1);
                if(i > 0){
                    auto& fBPrev = fBs[i-1];
                    glBindVertexArray(fBPrev.quadVAO);
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, fBPrev.textureColorBuffer);	// use the color attachment texture as the texture of the quad plane
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                }

                // Straight alpha input, premultiplied alpha output.
                glBlendFuncSeparate(
                    GL_SRC_ALPHA,
                    GL_ONE_MINUS_SRC_ALPHA,
                    GL_ONE,
                    GL_ONE_MINUS_SRC_ALPHA
                );
                lE.object_shader.use();
                lE.object_shader.setUniformI("INSTANCE_ID_OFFSET", offset);
                if(i == fBs.size() - 1){
                    int leftOver = shapeLayersStorage.getShapeCnt() - offset;
                    square.drawBatch(lE.object_shader, openGLTextures, leftOver, fBs[i]);
                }
                else{
                    square.drawBatch(lE.object_shader, openGLTextures, shapeBatchCount, fBs[i]);
                }
                offset += shapeBatchCount;
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClearColor(0, 0, 0, 0);
            auto& fBFinal = fBs[fBs.size() - 1];
            fBFinal.screenShader.use();
            fBFinal.screenShader.setUniformI("screenTexture", 1);
            glViewport(0,0, fBFinal.SCR_WIDTH, fBFinal.SCR_HEIGHT);
            glBindVertexArray(fBFinal.quadVAO);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, fBFinal.textureColorBuffer);	// use the color attachment texture as the texture of the quad plane
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        if(printAlarm.checkAndUpdate() && !shapeLayersStorage.empty()){

            runawayPrinter.pushMessage(emotionLayersToString(shapeLayersStorage.back().emotionLayers));
            runawayPrinter.pushMessage(emotionArrayToString(shapeLayersStorage.back().emotionArray));
            runawayPrinter.pushMessage("FPS: " + std::to_string(fpsWatch.getFrequency()));
        }
        for(auto& texture : openGLTextures){
            if(!contains(texture.name, std::string("SHAPES_TEXTURES")) && !contains(texture.name, std::string("BORDER")) &&!contains(texture.name, std::string("ATLAS")) && !contains(texture.name, std::string("borderTexture")) && !contains(texture.name, std::string("ourTexture"))){   
                glDeleteTextures(1, &texture.address);
            }
        }
        //}
        
    // ====================================================================================================
        endRenderLoopStatic(mWindow->window);
    // ====================================================================================================
    }   

    {
        std::lock_guard<std::mutex> guard(shapeLock);
        notDone = false;
    }
    creationThread.join();
    if(SIM_START){
        killCommand(simPID);
        waitpid(simPID, nullptr, 0);
        int killPPID = killProgram(SIM_PROGRAM);
        waitpid(killPPID, nullptr, 0);
    }
    if(COLOR_START){
        killCommand(colPID);
        waitpid(colPID, nullptr, 0);
        int killPPID = killProgram(COLOR_PROGRAM);
        waitpid(killPPID, nullptr, 0);
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}

float getFPS(DeltaTimer deltaTimer){
    return 1.0f/deltaTimer.getDeltaTime();
}

int mainDrawRectangle(int argc, char* argv[]){
    int return_status;
    GLFWwindow*  window; 
    if((return_status = initOpenGL(window, mWidth, mHeight)) != EXIT_SUCCESS) return return_status;

    LightingEnvironment lE = initDummyMPV(SHADER_PATH + "texture-color.vs", SHADER_PATH + "texture-color.fs");


    PngData blankPngData(16,16);
    PngData borderPngData(16,16);
    std::vector<float> distances;
    generateDistancesFromBorder(distances, 256, 256, borderVerticesFromShape(shapes::rectT));
    unsigned int borderTexture = useTextureDataToCreateTexture(distances.data(), ttm::map2D(distances, 256, 256));
    unsigned int blankTexture = obtainPNGBlank(blankPngData);
    OpenGLShape rectangle(shapes::rectT, {blankTexture, borderTexture}, true);
    Shader colorShader("Glitter/Shaders/texture-color");
    pngDataSavePng("testSSR.png", borderPngData);


    lE.object_shader.use();
    lE.object_shader.setUniformV4("aColor", glm::vec4(1.0f,1.0f,1.0f,1.0f));
    lE.object_shader.setUniformI("ourTexture", 0);
    lE.object_shader.setUniformI("borderTexture", 1);

    
    OpenGLShape square(shapes::rectT);
    FrameBuffer fB = initFrameBuffer(mRWidth, mRHeight, mWidth, mHeight);
    DeltaTimer deltaTimer;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        startDummyMPVLoop(deltaTimer, lE, glm::vec4(0.0f));
        startFrameBuffer(fB);
        //lE.object_shader.use();
        square.drawBatch(lE.object_shader, {{"ourTexture", blankTexture}, {"borderTexture",borderTexture}}, 1, fB);
        endFrameBuffer(fB);
        endRenderLoopStatic(window);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    deleteFrameBuffer(fB);
    
    
    return EXIT_SUCCESS;
    
}
int mainDrawCircle(int argc, char *argv[])
{
    int return_status;
    GLFWwindow*  mWindow; 
    if((return_status = initOpenGL(mWindow, mWidth, mHeight)) != EXIT_SUCCESS) return return_status;
    Shader colorShader("Glitter/Shaders/texture-color.vs", "Glitter/Shaders/texture-color.fs");
    colorShader.use();
    colorShader.setUniformV4("aColor", glm::vec4(1.0f,1.0f,1.0f,1.0f));
    unsigned int texture5 = createTexture("Glitter/Textures/container2.png", "png");
    int width, height, nrChannels;
    std::string texture_filepath = "Glitter/Textures/container2.png";
    std::vector<glm::vec2> borderVertices = borderVerticesFromShape(shapes::rectT.vertices, shapes::rectT.vertCnt, shapes::rectT.getStride());
    unsigned int blankTexture = obtainPNGBorder(255,255,50,50, cdata::ColorRGBA(255,255,255,255),cdata::ColorRGBA(255,255,255,255), false, false);
    colorShader.setUniformI("ourTexture", 0);
    DeltaTimer deltaTimer{};
    std::vector<float> distances;
    generateDistancesFromBorder(distances, 256, 256, std::get<std::vector<glm::vec2>>(createCircleShapeBorder(60)));
    unsigned int borderTexture = useTextureDataToCreateTexture(distances.data(), ttm::map2D(distances, 256, 256));
    PngData pngData(255, 255);
    OpenGLShape circle(shapes::rectT, {blankTexture, borderTexture}, true);
    circle.setTextures({blankTexture,circle.getTexture(0)});
    pngDataSavePng("test.png", pngData);
    colorShader.setUniformI("borderCase", 2);
    colorShader.setUniformI("ourTexture", 0);
    colorShader.setUniformI("borderTexture", 1);
    while(!glfwWindowShouldClose(mWindow)){
        minimumStartRenderLoop(deltaTimer);
        glDepthMask(false);
        colorShader.setUniformV4("aColor", glm::vec4(1.0f,1.0f,1.0f,1.0f));
        colorShader.setUniformI("borderCase", 2);
        circle.draw(colorShader);
        glDepthMask(true);
        endRenderLoopViewStatic(mWindow, deltaTimer);
    } glfwTerminate();
    
    return EXIT_SUCCESS;
}

int mainSolve(int argc, char *argv[])
{
    float min = 1.0f/2.0f;
    float max = 5.0f/4.0f;
    float minT = -1.0f;
    float maxT = 1.0f;
    //std::vector<float> solution = solve(std::vector<std::vector<float>> {{min, 1},{max,1}}, std::vector<float> {minT, maxT});
    //float slope = (minT - maxT)/(min - max);
    //float intercept = (min*maxT - minT*max)/(min - max);
    std::tuple<float,float> line = linearTransformation(min, max, minT, maxT);
    const auto [slope, intercept] = line;
    std::cout << "S1: " << slope << std::endl;
    std::cout << "S2: " << intercept << std::endl;
    std::cout << "max: " << max << std::endl;
    std::cout << "min: " << min << std::endl;
    float mid = (max+min)/2.0f;
    std::cout << "mid: " << mid << std::endl;
    std::cout << "maxT: " << maxT << std::endl;
    std::cout << "minT: " << minT << std::endl;
    float midT = (maxT+minT)/2.0f;
    std::cout << "midT: " <<  midT << std::endl;
    
    std::cout << "maxS: " << slope*max + intercept << std::endl;
    std::cout << "minS: " << slope*min + intercept << std::endl;
    std::cout << "midS: " << slope*mid + intercept << std::endl;
    float x = 0.33f;
    std::cout << "x: " << x << std::endl;
    std::cout << "xS: " << boundTransformation(x, line)  << std::endl;

    return 0;
}


std::map<PULSES,bool> emotionArrayToPulses(std::vector<float> emotionArray){
    float happiness = emotionArray[HAPPY];
    float sadness   = emotionArray[SAD];
    float anger     = emotionArray[ANGRY];
    float fear      = emotionArray[FEAR];
    float surprise  = emotionArray[SURPRISE];
    float disgust   = emotionArray[DISGUST];
    std::map<std::string, float> potentials {{"scale",disgust}, {"fade", (fear + sadness)/1.75f}, {"rumble", anger}, 
        {"spinr", (happiness)}, {"spinl", surprise}};
    std::vector<std::string> pulsesStr{};
    for(int i = 0; i < PULSE_NUMBER; i++){
        auto [key, trash] = maxM(potentials);
        pulsesStr.push_back(key);
        potentials.erase(key);
    }
    std::map<PULSES,bool> pulses{}; // Assuming when a key is used to access a value when the key is not in the map then the value returned is false
    for(const auto& pulse : pulsesStr){
        pulses[FADE_P  ] = (pulses[FADE_P  ] || pulse == "fade"  ) && DO_FADE_P  ;
        pulses[SCALE_P ] = (pulses[SCALE_P ] || pulse == "scale" ) && DO_SCALE_P ;
        pulses[RUMBLE_P] = (pulses[RUMBLE_P] || pulse == "rumble") && DO_RUMBLE_P;
        pulses[SPINR_P ] = (pulses[SPINR_P ] || pulse == "spinr" ) && DO_SPINR_P ;
        pulses[SPINL_P ] = (pulses[SPINL_P ] || pulse == "spinl" ) && DO_SPINL_P ;
    }
    return pulses;
}

std::map<TRANSITIONS,bool> emotionArrayToTransitions(std::vector<float> emotionArray){
    float happiness = emotionArray[HAPPY];
    float sadness   = emotionArray[SAD];
    float anger     = emotionArray[ANGRY];
    float fear      = emotionArray[FEAR];
    float surprise  = emotionArray[SURPRISE];
    float disgust   = emotionArray[DISGUST];
    float scalePotential  = (DO_SCALE_T         )  ?    (anger + surprise           )/2.0f              : 0.0f;
    float fadePotential   = (DO_FADE_T        )  ?    (fear + sadness             )/2.0f              : 0.0f;
    float rightPotential  = (DO_MOVE_FORWARD_T )  ?    (happiness + anger          )/2.0f              : 0.0f;
    float leftPotential   = (DO_BACK_OUT_T     )  ?    (disgust + fear             )/2.0f              : 0.0f;
    std::map<TRANSITIONS,bool> transitionsMap{}; // Assuming when a key is used to access a value when the key is not in the map then the value returned is false
    std::map<TRANSITIONS,float> transitionPotentials{{FADE_T,fadePotential},{SCALE_T,scalePotential},{MOVE_FORWARD_T,rightPotential},{BACK_OUT_T,leftPotential}};
    for(int i = 0; i < TRANSITIONS_NUMBER; i++){
        auto [key, trash] = maxM(transitionPotentials);
        transitionsMap[key] = true;
        transitionPotentials.erase(key);
    }
    return transitionsMap;

    
}
float calcDisplacement(float destructionTimePassed, float destructionTime, int totalLayers, int midLayer, int layer, float shapeMinFade, float shapeMaxFade, bool delay, std::tuple<float,float> lineAbove, std::tuple<float,float> lineBelow){
        float dX = 0.0f;
        dX = calcFade(destructionTimePassed, destructionTime, totalLayers, midLayer, layer, shapeMinFade, shapeMaxFade, false, lineAbove, lineBelow);
        if(delay){
            if(layer > MID_LAYER) dX = 0.0f;
        }
        else{
            if(layer <= MID_LAYER)
                dX = (1.0f + dX)/2.0f;
            else
                dX = (1.0f - dX)/2.0f;
        }
        return dX;
}

float getElapsedTime(){
    return glfwGetTime();
}

// #Unordered -------------------------------------------------------------

ShapeProperties2D generateShape2D(std::map<TRANSITIONS, bool> transitions, glm::vec3 coords, const glm::vec3 emotionCoords3D, const glm::vec3 rotAxis, const float rotScale, const glm::vec3 emotionRGB, const float alphaScale, const bool useBenTexture, const SHAPE_TYPE_2D type, const float softnessOrThickness, const unsigned int borderCase, const float emotionScale, unsigned int tag, unsigned int batchTag){
    
    float maxScale = MAX_SCALE;
    float minScale = MIN_SCALE;
    float aveScale = AVE_SCALE;
    float devScale = DEV_SCALE;
    auto scale = clamp(rand_range_normal(aveScale,devScale), minScale, maxScale);
    //auto scale = 0.001f;
    if(transitions[SCALE_T] && transitions[FADE_T]) scale *= DO_SCALE_SCALE;
    else if(transitions[SCALE_T]) scale *= DO_SCALE_SCALE;
    else if (transitions[FADE_T]) scale *= DO_FADE_SCALE;
    
    if(transitions[VORTEX_T]) scale *= DO_VORTEX_SCALE;

    
    coords = SCATTER_TO_EMOTION_COORD_RATIO* SCATTER_COORD_SCALE * randCoords()+(1-SCATTER_TO_EMOTION_COORD_RATIO)*EMOTION_COORD_SCALE*emotionCoords3D;
    coords.z = 0.0f;
    
    glm::mat4 model = glm::mat4(1.0f);
    float xCoordScale = 1.0f;
    float yCoordScale = 1.0f;
    glm::vec3 scaledCoordinates(1.0f);
    scaledCoordinates.x = coords.x*xCoordScale;
    scaledCoordinates.y = coords.y*yCoordScale;
    scaledCoordinates.z = 0.0f;

    
    // Create and delete cubes
    glm::vec4 shapeColor = glm::vec4(emotionRGB,alphaScale);
    //glm::vec4 invertedShapeColor = glm::vec4(glm::vec3(1.0f) - emotionRGB, alphaScale);
    glm::vec4 invertedShapeColor;
    if(useBenTexture){
        invertedShapeColor = glm::vec4(emotionRGB, alphaScale);
        shapeColor = cdata::WHITE_V4;
    }
    else{
        invertedShapeColor = glm::vec4(0.8f*emotionRGB, alphaScale);
    }
    ShapeProperties2D shape = ShapeProperties2D{shapeColor,invertedShapeColor, type, scaledCoordinates, softnessOrThickness, transitions, borderCase, 0, useBenTexture};
    shape.scaleMatrix = glm::scale(glm::mat3(1.0f), scale*glm::vec2(1.0f));
    shape.rotationMatrix = glm::rotate(glm::mat3(1.0f), rotScale);
    shape.transition = glm::translate(glm::mat3(1.0f), shape.coords);
    shape.direction = dir3D::UP;
    shape.direction  = shape.rotationMatrix * shape.direction;
    shape.direction = glm::normalize(shape.direction);
    shape.rotatedCoords = glm::mat3(shape.rotationMatrix) * glm::vec3(shape.coords.x, 0.0f, 0.0f);
    shape.layerTag = tag;
    shape.batchTag = batchTag;
    return shape;
}

float emotionIntensitiesWeighting(EmotionIntensities eI, EmotionIntensities weights, float minT, float maxT){
    float weighting = 
        eI.disgust*weights.disgust +
        eI.anger*weights.anger +
        eI.sadness*weights.sadness +
        eI.surprise*weights.surprise +
        eI.happiness*weights.happiness +
        eI.fear*weights.fear;
    float minWeighting = getMinWeightingSum(weights);
    float maxWeighting = getMaxWeightingSum(weights);
    return boundTransformation(weighting, minWeighting, maxWeighting, minT, maxT);
}
float emotionIntensitiesWeighting(EmotionIntensities eI, EmotionIntensities weights){
    return emotionIntensitiesWeighting(eI, weights, getMinWeight(weights), getMaxWeight(weights));
}

std::tuple<unsigned int, float> emotionArrayToShapeCountSize(std::vector<float> emotionArray){
    EmotionIntensities eI = emotionArrayToEmotionIntensities(emotionArray);
    EmotionIntensities scaleWeightings{};
    scaleWeightings.anger       = 1.0f;
    scaleWeightings.happiness   = 0.75f;
    scaleWeightings.sadness     = 0.5f;
    scaleWeightings.surprise    = 0.5f;
    scaleWeightings.fear        = 0.25f;
    scaleWeightings.disgust     = 0.25f;
    EmotionIntensities countWeightings{};
    countWeightings.anger       = 0.25f;
    countWeightings.happiness   = 0.5f;
    countWeightings.sadness     = 0.75f;
    countWeightings.surprise    = 0.75f;
    countWeightings.fear        = 1.0f;
    countWeightings.disgust     = 1.0f;

    float powerScale = emotionIntensitiesWeighting(eI, scaleWeightings, -4.0f, 0.0f);
    float powerCount = emotionIntensitiesWeighting(eI, countWeightings, 0.0f, 3.25f);
        
    float scale = glm::pow(2.5f, powerScale);
    float countFloat = glm::pow(2.5f, powerCount);
    unsigned int count = (unsigned int) floorWrap(countFloat);

    return {count, scale};

}

EmotionIntensities emotionArrayToEmotionIntensities(std::vector<float> emotionArray){
    float happiness = emotionArray[HAPPY];
    float sadness   = emotionArray[SAD];
    float anger     = emotionArray[ANGRY];
    float fear      = emotionArray[FEAR];
    float surprise  = emotionArray[SURPRISE];
    float disgust   = emotionArray[DISGUST];
    return EmotionIntensities(happiness, sadness, anger, fear, surprise, disgust);
}

std::map<EMOTION, float> emotionIntensitiesToEmotionMap(EmotionIntensities eI){
    std::map<EMOTION, float> eIMap{};
    eIMap[HAPPY] = eI.happiness;
    eIMap[SAD] = eI.sadness;
    eIMap[FEAR] = eI.fear;
    eIMap[ANGRY] = eI.anger;
    eIMap[SURPRISE] = eI.surprise;
    eIMap[DISGUST] = eI.disgust;
    return eIMap;
}

EmotionIntensities EmotionMapToEmotionIntensities(std::map<EMOTION, float> eIMap)
{
    EmotionIntensities eI;
    eI.happiness = eIMap[HAPPY]    ; 
    eI.sadness   = eIMap[SAD]      ; 
    eI.fear      = eIMap[FEAR]     ; 
    eI.anger     = eIMap[ANGRY]    ; 
    eI.surprise  = eIMap[SURPRISE] ; 
    eI.disgust   = eIMap[DISGUST]  ; 
    return eI;
}

std::map<EMOTION, float> emotionArrayToEmotionMap(std::vector<float> emotionArray)
{
    return emotionIntensitiesToEmotionMap(emotionArrayToEmotionIntensities(emotionArray));
}

int killProgram(std::string programName){
    int killPID = runCommand("ps -eo pid,args | grep '" + programName + "' | awk '{print $1}' | xargs -r kill -9;");
    waitpid(killPID, nullptr, 0);
    return killPID;
}

void drawRecursiveSquareHoles(GLFWwindow* window, OpenGLShape& square, OpenGLShape& squareHole, LightingEnvironment& lE, unsigned int iterations, glm::vec2 position, float size){
    lE.object_shader.use();
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(position,0.0f));
    model = glm::scale(model, glm::vec3(glm::vec2(size),1.0f));
    lE.object_shader.setUniformM4("model", model);
    square.draw(lE.object_shader);
    drawRecursiveSquareHolesHelper(window, square, squareHole, lE, iterations, position, size);
}
void drawRecursiveSquareHolesHelper(GLFWwindow* window, OpenGLShape& square, OpenGLShape& squareHole, LightingEnvironment& lE, unsigned int iterations, glm::vec2 position, float size){
    if(iterations > 0){
        iterations--;
        lE.object_shader.use();
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(position,0.0f));
        size = size/3.0f;
        model = glm::scale(model, glm::vec3(glm::vec2(size),1.0f));
        lE.object_shader.setUniformM4("model", model);
        squareHole.draw(lE.object_shader);
        for(int row = -1; row <= 1; row++){
            for(int col = -1; col <= 1; col++){
                if(row != 0 || col != 0)
                    drawRecursiveSquareHolesHelper(window, square, squareHole, lE, iterations, position + size*glm::vec2(row,col), size);
            }
        }
    }
}

void getRecursiveShapeHolesHelper(unsigned int maxIterations, unsigned int iterations, std::vector<std::tuple<std::vector<glm::vec2>,SHAPE_TYPE_2D>> shapes, glm::vec2 position, float rotation, float size, std::vector<std::vector<std::tuple<glm::vec2, float, float, SHAPE_TYPE_2D>>>& shapeHoles){
    if(iterations < maxIterations - 1){
        iterations++;
        float preSize = size;
        size = size/3.0f;

        auto const& [preShapeVec2s, preShapeType] = shapes[iterations - 1];
        auto const& [postShapeVec2s, shapeType] = shapes[iterations];
        auto preVec2 = preShapeVec2s[preShapeVec2s.size() - 1];
        shapeHoles[iterations].push_back(std::tuple<glm::vec2, float, float, SHAPE_TYPE_2D>(position, size, rotation, shapeType));
        debug::checkPrint(std::format("iterations: {}, position: {}, rotation: {}*PI\n", iterations, vec2ToString(position), formatFloat(rotation/mm_math::PI, 1, 4)),true, DEBUG_RECURSION);
        getRecursiveShapeHolesHelper(maxIterations, iterations, shapes,  position, rotation, size, shapeHoles);

        for(int i = 0; i < preShapeVec2s.size(); i++){
            // TODO: calculate this stuff outside getRecursiveSquareHolesHelper
            // Do calutations ignoring changes to position and size
            const glm::vec2 vec2 = preShapeVec2s[i];
            const glm::vec2 ray = vec2 - preVec2;
            const float rayAngle = glm::atan(ray.y, ray.x); // Assumes shape is initially pointing to the right, this will have to be corrected later since the
                                                      // shapes in shapes.hpp point upward initially.
            const float perpRayAngle = rayAngle - mm_math::PI/2.0f;
            const glm::vec2 newRay = glm::normalize(glm::vec2(glm::rotate(glm::mat4(1.0f), perpRayAngle, dir3D::FORWARD) * glm::vec4(1.0f,0.0f,0.0f,0.0f)));
            const glm::vec2 perpRayFromPos = newRay;
            glm::vec2 intersection;
            tryGetIntersection(intersection, { glm::vec2(0.0f), perpRayFromPos}, {preVec2, vec2}, true);
            const glm::vec2 newNormalPosition = 2.0f * intersection;

            // Now onto iteration specific calculations
            const float newAngle = perpRayAngle + rotation;
            const glm::vec2 offset = size*newNormalPosition;
            const glm::vec2 newPosition = glm::vec2(offset.x + position.x, offset.y + position.y);
            shapeHoles[iterations].push_back(std::tuple<glm::vec2, float, float, SHAPE_TYPE_2D>(newPosition, size, newAngle, shapeType));
            getRecursiveShapeHolesHelper(maxIterations, iterations, shapes,  newPosition, newAngle, size, shapeHoles);
            debug::checkPrint(std::format("iterations: {}, {} -> {},  newPosit: {}, newAngle: {}*PI, intersection: {}\n", iterations, vec2ToString(preVec2), vec2ToString(vec2), vec2ToString(newPosition), formatFloat(newAngle/mm_math::PI,1,4), vec2ToString(intersection)),true, DEBUG_RECURSION);

            preVec2 = vec2;
        }
    }
}
std::vector<std::vector<std::tuple<glm::vec2, float, float, SHAPE_TYPE_2D>>> getRecursiveSquareHoles(unsigned int iterations, std::vector<std::tuple<std::vector<glm::vec2>,SHAPE_TYPE_2D>> shapes, glm::vec2 position, float size){
    std::vector<std::vector<std::tuple<glm::vec2, float, float, SHAPE_TYPE_2D>>> squareHoles(iterations);
    getRecursiveShapeHolesHelper(iterations, iterations, shapes, position, 0.0f, size, squareHoles);
    return squareHoles;
}


std::vector<std::vector<std::tuple<glm::mat4, SHAPE_TYPE_2D>>> getRecursiveSquareHolesModels(unsigned int maxIterations, std::vector<std::tuple<std::vector<glm::vec2>,SHAPE_TYPE_2D>> shapes, glm::vec2 position, float size){
    std::vector<std::vector<std::tuple<glm::mat4, SHAPE_TYPE_2D>>> models{};
    std::vector<std::vector<std::tuple<glm::vec2, float, float, SHAPE_TYPE_2D>>> squareHoleLayers(maxIterations);
    float firstAngle = 0.0f;
    squareHoleLayers[0].push_back(std::tuple<glm::vec2, float, float, SHAPE_TYPE_2D>(position, size, firstAngle, std::get<SHAPE_TYPE_2D>(shapes[0])));
    getRecursiveShapeHolesHelper(maxIterations, 0, shapes, position, firstAngle, size, squareHoleLayers);
    for(const auto& holeLayer : squareHoleLayers){
        models.push_back(std::vector<std::tuple<glm::mat4,SHAPE_TYPE_2D>>{});
        for(const auto& [position, size, rotation, shapeType] : holeLayer){
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(position,0.0f));
            model = glm::rotate(model, rotation, dir3D::FORWARD);
            model = glm::scale(model, glm::vec3(glm::vec2(size),1.0f));
            models.back().push_back({model,shapeType});
        }
    }
    return models;
}

void drawModelPositions(const std::vector<std::vector<std::tuple<glm::mat4,SHAPE_TYPE_2D>>>& modelLayers, std::map<SHAPE_TYPE_2D,OpenGLShape*>& shapes, LightingEnvironment& lE, std::deque<glm::vec4> colors){
    lE.object_shader.use();
    for(auto& modelLayer : modelLayers){
        const glm::vec4 color = colors.front();
        assert(!colors.empty());
        colors.pop_front();
        lE.object_shader.setUniformV4("INNER_COLOR", color);
        for(auto const& [model, shapeType] : modelLayer){
            lE.object_shader.setUniformM4("model", model);
            auto& shape = shapes[shapeType];
            shape->draw(lE.object_shader);
        }
    }
}
int mainRecursion(int argc, char * argv[]){
    int returnVal;
    GLFWwindow* mWindow;
    if((returnVal = initOpenGL(mWindow, mWidth, mHeight)) != EXIT_SUCCESS) return returnVal;
    LightingEnvironment lE = initDummyMPV("Glitter/Shaders/texture-mvp-color.vs", "Glitter/Shaders/texture-mvp-color.fs");
    unsigned int borderTextureRed = obtainPNGBorder(255, 255, 2, 2, cdata::BLACK_D_RGBA, cdata::WHITE_D_RGBA, false, false);
    OpenGLShape square(shapes::rectT, {borderTextureRed, borderTextureRed}, true);
    unsigned int borderTextureBlue = obtainPNGBorder(255, 255, 0, 0, cdata::BLACK_D_RGBA, cdata::WHITE_D_RGBA, false, false);
    OpenGLShape squareHole(shapes::rectT, {borderTextureBlue, borderTextureBlue}, true);
    OpenGLShape triangle(shapes::eTriTRight, {borderTextureRed, borderTextureRed}, true);
    OpenGLShape triangleHole(shapes::eTriTRight, {borderTextureBlue, borderTextureBlue}, true);

    DeltaTimer deltaTimer{};
    
    glDisable(GL_DEPTH_TEST);
    lE.object_shader.use();
    lE.view = glm::mat4(1.0f);

    lE.object_shader.setUniformB("DISABLE_ALMOST_EVERYTHING", true);
    lE.object_shader.setUniformV4("INNER_COLOR", cdata::WHITE_V4);
    glm::mat4 model(1.0f);
    model = glm::scale(model, glm::vec3(glm::vec2(2.0f),1.0f));
    auto squareBorder = std::tuple<std::vector<glm::vec2>,SHAPE_TYPE_2D>{borderVerticesFromShape(shapes::rectT), SQUARE};
    auto triangleBorder = std::tuple<std::vector<glm::vec2>,SHAPE_TYPE_2D>{borderVerticesFromShape(shapes::eTriTRight), TRIANGLE};
    std::vector<std::tuple<std::vector<glm::vec2>,SHAPE_TYPE_2D>> shapes{triangleBorder, triangleBorder, triangleBorder};
    
    lE.object_shader.setUniformM4("model", model);
    auto models = getRecursiveSquareHolesModels(2, shapes, glm::vec2(0.0f), 2.0f);
    glm::vec2 position(0.0f);
    float zoom = 1.0f;
    std::map<SHAPE_TYPE_2D, OpenGLShape*> shapeMap{{SQUARE, &squareHole},{TRIANGLE, &triangleHole}};
    while(!glfwWindowShouldClose(mWindow)){
        startDummyMPVLoop(deltaTimer, lE);
        lE.view = glm::mat4(1.0f);
        lE.view = glm::scale(lE.view, glm::vec3(glm::vec2(zoom),0.0f));
        lE.view = glm::translate(lE.view, glm::vec3(position,0.0f));
        drawModelPositions(models, shapeMap, lE, std::deque<glm::vec4>{cdata::BLUE_V4, cdata::RED_V4, cdata::GREEN_V4, cdata::YELLOW_V4, cdata::PURPLE_V4, cdata::DARK_BLUE_V4, cdata::TURQUOISE_V4, cdata::DARK_TURQUOISE_V4, cdata::BLACK_V4, cdata::WHITE_V4});
        endRenderLoopView2D(mWindow, deltaTimer, position, zoom);
    } glfwTerminate();
    
    return EXIT_SUCCESS;

}

void fillSquare(PngData& pngData, const float size, const glm::vec2 position, const cdata::ColorRGBA& color, const int iterations){
    float offset = size/2.0f;
    glm::vec2 bottomLeft = position - offset;
    glm::vec2 topRight = position + offset;
    assert(topRight.x >= bottomLeft.x && topRight.y >= bottomLeft.y);
    glm::vec2 difference = (topRight - bottomLeft);
    if(glm::length(difference) >= 1.0f){
        int bottomLeftX = glm::round(bottomLeft.x);
        int bottomLeftY = glm::round(bottomLeft.y);
        int topRightX = glm::round(topRight.x);
        int topRightY = glm::round(topRight.y);
        for (int y = bottomLeftY; y <  topRightY; y++){
            for (int x = bottomLeftX; x < topRightX; x++){
                debug::checkPrint(std::format("==========\nx: {}, y: {}, \nblx: {}, bly: {}, \ntrx: {}, try: {}\nbl: {}\ntr: {}\n", x, y, bottomLeftX, bottomLeftY, topRightX, topRightY, vec2str(bottomLeft,2), vec2str( topRight,2)), iterations == 6, DEBUG_RECURSION_FILL);
                pngData.setColor(y, x, color);
            }
        }
    }
    
}

void getRecursiveSquareHolesHelper(const int maxIterations, const int preIterations, const glm::vec2 position, const float preWidth, PngData& pngData, const std::vector<cdata::ColorRGBA>& colors){
    if(preIterations < maxIterations){
        float width = preWidth/3.0f;
        int iterations = preIterations + 1;
        auto const color = colors[iterations];
        debug::checkPrint(std::format("=========================\n{}\n",col2str(color)), iterations==6, DEBUG_RECURSION);
        fillSquare(pngData, width, position, color, iterations);
        for(int row = -1; row <= 1; row++){
            for(int col = -1; col <= 1; col++){
                if(abs(row) + abs(col) != 2)
                    getRecursiveSquareHolesHelper(maxIterations, iterations, position + width*glm::vec2(row,col), width, pngData, colors);
            }
        }
    }
}

void getRecursiveSquareHoles(unsigned int iterations, PngData& pngData, std::vector<cdata::ColorRGBA> colors, glm::vec2 origin=glm::vec2(0.5f,0.5f)){
    assert(pngData.width >= glm::pow(3,iterations) && pngData.height >= glm::pow(3,iterations));
    std::vector<std::vector<std::tuple<glm::vec2, float>>> squareHoles(iterations);
    generatePNGBlank(pngData, colors[0]);
    float width = pngData.width;
    glm::vec2 position = boundTransformation(origin, glm::vec2(0.0f), glm::vec2(1.0f), glm::vec2(0.0f), glm::vec2(pngData.width, pngData.height));
    getRecursiveSquareHolesHelper(iterations, 0, position, width, pngData, colors);
}

std::tuple<std::vector<glm::vec4>, std::vector<glm::vec4>, std::vector<glm::vec4>, std::vector<glm::vec4>> modelsToCols(std::vector<glm::mat4> models){
    std::vector<glm::vec4> modelCols0;
    std::vector<glm::vec4> modelCols1;
    std::vector<glm::vec4> modelCols2;
    std::vector<glm::vec4> modelCols3;
    modelCols0.reserve(models.size());
    modelCols1.reserve(models.size());
    modelCols2.reserve(models.size());
    modelCols3.reserve(models.size());
    for(int i = 0; i < models.size(); i++){
        modelCols0.push_back(models[i][0]);
        modelCols1.push_back(models[i][1]);
        modelCols2.push_back(models[i][2]);
        modelCols3.push_back(models[i][3]);
    }
    return std::tuple<std::vector<glm::vec4>, std::vector<glm::vec4>, std::vector<glm::vec4>, std::vector<glm::vec4>>{modelCols0, modelCols1, modelCols2, modelCols3};
}


int mainDrawBatch(int argc, char* argv[]){
    int return_status;
    GLFWwindow*  mWindow; 
    if((return_status = initOpenGL(mWindow, mWidth, mHeight)) != EXIT_SUCCESS) return return_status;
    LightingEnvironment lE = initDummyMPV("Glitter/Shaders/texture-mvp-color.vs", "Glitter/Shaders/texture-mvp-color.fs");
    lE.object_shader.use();
    lE.object_shader.setUniformV4("INNER_COLOR", glm::vec4(1.0f,0.0f,1.0f,1.0f));
    lE.object_shader.setUniformV4("OUTER_COLOR", glm::vec4(0.0f,1.0f,0.0f,1.0f));
    lE.object_shader.setUniformI("borderCase", 1);
    lE.object_shader.setUniformI("ourTexture", 0);
    lE.object_shader.setUniformI("borderTexture", 1);
    lE.object_shader.setUniformB("DISABLE_ALMOST_EVERYTHING", true);
    lE.object_shader.setUniformV2("SL_SCALE", glm::vec2(1.0f,1.0f));
    lE.object_shader.setUniformF("BORDER_THICKNESS", 0.5f);
    lE.object_shader.setUniformB("DO_FADE", false);
    lE.object_shader.setUniformB("USE_MODEL_ARRAY", true);
    DeltaTimer deltaTimer{};
    static const unsigned int WIDTH = COLOR_WIDTH;
    static const unsigned int HEIGHT = COLOR_HEIGHT;
    PngData blankPngData(WIDTH,HEIGHT);
    PngData borderPngData(WIDTH,HEIGHT);
    std::vector<float> distances;
    generateDistancesFromBorder(distances, 256, 256, borderVerticesFromShape(shapes::rectT));
    unsigned int borderTexture = useTextureDataToCreateTexture(distances.data(), ttm::map2D(distances, 256, 256));
    unsigned int blankTexture = obtainPNGBlank(blankPngData);

    static const unsigned int ROWS = 15;
    static const unsigned int TOTAL_BATCHES_TEST = 20;
    static const unsigned int COLS = ROWS;
    static const unsigned int SIZE = COLS*ROWS;
    glm::mat3* models = (glm::mat3 *) malloc(sizeof(glm::mat3)*SIZE);
    std::vector<unsigned int> batchNumbers;
    batchNumbers.reserve(SIZE);
    unsigned int divisor = SIZE/TOTAL_BATCHES_TEST;

    for(unsigned int row = 1;     row <= ROWS; row++){
        for(unsigned int col = 1; col <= COLS; col++){
            auto pos = boundTransformation(
                glm::vec2(col, row),
                glm::vec2(0.0f),
                glm::vec2(COLS+1.0f),
                glm::vec2(-1.0f),
                glm::vec2(1.0f)
            );
            glm::mat3 model(1.0f);
            model = glm::translate(model,pos);
            model = glm::scale(model, glm::vec2(0.1f, 0.1f));
            unsigned int index = (row-1)*COLS + (col-1);
            models[index] = model;
            unsigned int batchNumber = index/divisor;
            batchNumbers.push_back( batchNumber);
        }
    }



    std::vector<OpenGLTexture> textures;
    TextureAtlas textureAtlasData(defaults::options, TOTAL_BATCHES_TEST, 1, "ATLAS_TEXTURES");
    PngData benTexture(WIDTH, HEIGHT, 3);
    for(int i = 0; i < TOTAL_BATCHES_TEST; i++){
            do{
                benTexture = loadPngTexture(EMOTION_PNG_FILE_PATH);
                std::this_thread::sleep_for(std::chrono::nanoseconds(1));
            } while(!benTexture.isValid());
            textureAtlasData.replaceTexture(benTexture.getData());
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    unsigned int textureAtlus = textureAtlasData.getTexture().address;
    textures.push_back({"ourTexture",blankTexture});
    textures.push_back({"borderTexture",borderTexture});
    unsigned int modelsTexture = useTextureDataToCreateTexture(models, SIZE*3, 1, GL_TEXTURE_2D, 0, GL_RGB32F, GL_RGB, GL_FLOAT);
    textures.push_back({"MODEL_TEXTURE",modelsTexture});
    unsigned int batchNumbersTexture = useTextureDataToCreateTexture(batchNumbers.data(), SIZE, 1, GL_TEXTURE_2D, 0, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT);
    textures.push_back({"ONE_BIG_BEAUTIFUL_TEXTURE", textureAtlus});
    textures.push_back({"BATCH_NUMBER_TEXTURE", batchNumbersTexture});
    OpenGLShape rectangle(shapes::rectT, {blankTexture, borderTexture, 
        modelsTexture,
    }, true);


    glm::mat4 model(1.0f);

    lE.object_shader.setUniformI("SIZE", SIZE);
    lE.object_shader.setUniformI("COLS", COLS);
    lE.object_shader.setUniformI("BATCHES_CNT", TOTAL_BATCHES_TEST);
    
    glm::vec2 pos = glm::vec2(0.0f);
    float zoom = 1.0f;
    
    while(!glfwWindowShouldClose(mWindow)){
        startDummyMPVLoop(deltaTimer, lE);
        rectangle.drawBatch(lE.object_shader, textures, SIZE);
        endRenderLoopView2D(mWindow, deltaTimer, pos, zoom);
    } glfwTerminate();
    
    return EXIT_SUCCESS;
    
}


ShapeLayerStorage::ShapeLayerStorage()
{
}

void ShapeLayerStorage::push_front(ShapeLayer shapeLayer)
{
    shapeLayers.push_front(shapeLayer);
    for(auto& shapeBatch : this->shapeLayers.front().batches){
        this->batches.push_front(&shapeBatch);
        for(auto& shape : shapeBatch.shapes){
            this->shapes.push_back(&shape);
        }
    }
}

void ShapeLayerStorage::pop_back()
{
    auto& shapeLayer = shapeLayers.back();
    for(const auto& shapeBatch : shapeLayer.batches){
        for(const auto& shape : shapeBatch.shapes){
            shapes.pop_back();
        }
        batches.pop_back();
    }
    shapeLayers.pop_back();
}

std::tuple<unsigned int, unsigned int, unsigned int> transferShapeLayers(std::counting_semaphore<TOTAL_LAYERS>& shapesSemaphore, std::mutex& shapeLock, const unsigned int& requiredLayers, std::deque<ShapeLayer>& shapeLayersBuffer, ShapeLayerStorage& shapeLayersStorage, float currentDestructionTime, float currentPulseMaxTime, std::deque<PngData>& pngDataDeque)
    {
    std::tuple<unsigned int, unsigned int, unsigned int> transfers{0,0,0};
    shapesSemaphore.release(requiredLayers);
    std::lock_guard guard(shapeLock);
    while(!shapeLayersBuffer.empty()){
        auto shapeLayerB = shapeLayersBuffer.back();
        ShapeLayer shapeLayer = shapeLayerB;
        for(auto& shapeBatch : shapeLayer.batches){
            for(auto& shape : shapeBatch.shapes)
                std::get<2>(transfers)++;
            std::get<1>(transfers)++;
            pngDataDeque.push_front(shapeBatch.pngData);
        }

        shapeLayer.destructionTimer.setWaitTime(currentDestructionTime);
        shapeLayer.pulseTimer.setWaitTime(currentPulseMaxTime);
        shapeLayersStorage.push_front(shapeLayer);
        shapeLayersBuffer.pop_back();
        std::get<0>(transfers)++;
    }
    return transfers;
}

int mainDrawFloats(int argc, char* argv[]){
    int return_status;
    GLFWwindow*  mWindow; 
    if((return_status = initOpenGL(mWindow, mWidth, mHeight)) != EXIT_SUCCESS) return return_status;
    DeltaTimer deltaTimer;
    Shader floatsS(SHADER_PATH + "floats");
    floatsS.use();
    std::vector<float> floats;

    floats.reserve(255*255);
    for(int row = 0; row < 255; row++){
        for(int col = 0; col < 255; col++){
            float f = (float)(row*255 + col)/(float)(255*255 + 255);
            floats.push_back(f);
        }
    }

    unsigned int floatsTexture = useTextureDataToCreateTexture(&floats[0], 255, 255, GL_TEXTURE_2D, 0, GL_R32F, GL_RED, GL_FLOAT);
    floatsS.setUniformI("TEXTURE", 0);
    OpenGLShape square(shapes::rectT, {floatsTexture}, true);

    glm::vec2 pos = glm::vec2(0.0f);
    float zoom = 1.0f;
    

    while(!glfwWindowShouldClose(mWindow)){
        minimumStartRenderLoop(deltaTimer);
        square.draw(floatsS);
        endRenderLoopViewStatic(mWindow, deltaTimer);
    } glfwTerminate();
    
    return EXIT_SUCCESS;
}
    
int mainDrawDistances(int argc, char* argv[]){
    int return_status;
    GLFWwindow*  mWindow; 
    if((return_status = initOpenGL(mWindow, mWidth, mHeight)) != EXIT_SUCCESS) return return_status;
    DeltaTimer deltaTimer;
    Shader floatsS(SHADER_PATH + "floats");
    floatsS.use();
    std::vector<float> floats;
    auto [circleShape, circleBorder] = createCircleShapeBorder(60);
    static const int width = 255;
    static const int height = 255;

    floats.reserve(width*height);
    generateDistancesFromBorder(floats, width, height, circleBorder);
    unsigned int floatsTexture = useFloatsToCreateTexture2D(floats.data(), width, height);
    floatsS.setUniformI("TEXTURE", 0);
    OpenGLShape square(shapes::rectT, {floatsTexture}, true);

    glm::vec2 pos = glm::vec2(0.0f);
    float zoom = 1.0f;
    

    while(!glfwWindowShouldClose(mWindow)){
        minimumStartRenderLoop(deltaTimer);
        square.draw(floatsS);
        endRenderLoopViewStatic(mWindow, deltaTimer);
    } glfwTerminate();
    
    return EXIT_SUCCESS;

}

unsigned int useFloatsToCreateTexture2D(float* floats, unsigned int width, unsigned int height){
    unsigned int floatsTexture = useTextureDataToCreateTexture(floats, ttm::map2D(floats, width, height));
    return floatsTexture;
}
    
void OpenGLShape::drawBatch(Shader& shader, const std::vector<OpenGLTexture>& textures, unsigned int count){
    shader.use();
    for(int i = 0; i < textures.size(); i++){
        unsigned int GL_TEXTUREX = getGL_TextureX(i);
        glActiveTexture(GL_TEXTUREX);
        glBindTexture(GL_TEXTURE_2D, textures[i].address);
        if (textures[i].name != "")
            shader.setUniformI(textures[i].name, i);
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElementsInstanced(GL_TRIANGLES, ind_cnt, GL_UNSIGNED_INT, 0, count);
    glBindVertexArray(0);
}

OpenGLShape::OpenGLShape(const shapes::Shape &shape)
{
    this->ind_cnt = shape.indCnt;
    this->vert_cnt = shape.vertCnt;
    create_a_shape(this->VAO, this->VBO, this->EBO, this->ind_cnt, this->vert_cnt, shape.vertices, shape.indices, shape.attributeSizes);
}




int mainDrawSubTextures(int argc, char* argv[]){
    int return_status;
    GLFWwindow*  mWindow; 
    if((return_status = initOpenGL(mWindow, mWidth, mHeight)) != EXIT_SUCCESS) return return_status;
    DeltaTimer deltaTimer{};
    const int WIDTH = 256;
    const int HEIGHT = WIDTH;
    const int CHANNELS = 3;
    const int BATCHES = 4;
    OpenGLTextureOptions options = defaults::options;
    options.width = WIDTH;
    options.height = HEIGHT;

    TextureAtlas textureAtlas(options, BATCHES, 1, "ONE_BIG_BEAUTIFUL_TEXTURE");
    PngData benTexture(WIDTH,HEIGHT,CHANNELS);
    generatePNGBlank(benTexture);
    for(int i = 0; i < BATCHES; i++){
        textureAtlas.replaceTexture(benTexture.getData());
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    for (int i = 0; i < BATCHES; i++) {
        loadPngTexture("../background.png", benTexture);
        textureAtlas.replaceTexture(benTexture.getData());
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    OpenGLShape rectangle(shapes::rectT, textureAtlas.getTexture().address, true);
    Shader texS("Glitter/Shaders/texture");

    
    while(!glfwWindowShouldClose(mWindow)){
        minimumStartRenderLoop(deltaTimer);
        rectangle.draw(texS);
        endRenderLoopStatic(mWindow);
    } glfwTerminate();
    
    return EXIT_SUCCESS;
}

std::vector<glm::vec2> borderVerticesFromShape(const shapes::Shape& shape){
    return borderVerticesFromShape(shape.vertices, shape.vertCnt, shape.getStride());
}

std::vector<glm::vec2> borderVerticesFromShape(const float shape[], const unsigned int vert_cnt, const unsigned int stride){
    std::vector<glm::vec2> borderVertices{};
    for(int i = 0; i < vert_cnt; i+=stride){
        borderVertices.push_back(glm::vec2(shape[i],shape[i+1]));
    }
    return borderVertices;
}



int mainFrameBufferTesting(int argc, char *argv[])
{
    int returnStatus;
    GLFWwindow* window;
    if ((returnStatus = initOpenGL(window, mWidth, mHeight)) == EXIT_FAILURE) return returnStatus;
    LightingEnvironment lE = initDummyMPV(SHADER_PATH + "framebuffers.vs", SHADER_PATH + "framebuffers.fs");



    // load textures
    // -------------
    auto [ cubeTexture, cOptions] = loadTexture("Glitter/Textures/container2.png", "texture1");

    lE.object_shader.setUniformI("texture1", 0);

    
    OpenGLShape square(shapes::rectT);
    FrameBuffer fB = initFrameBuffer(mRWidth, mRHeight, mWidth, mHeight);
    DeltaTimer deltaTimer;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        startDummyMPVLoop(deltaTimer, lE, glm::vec4(0.0f));
        startFrameBuffer(fB);
        //lE.object_shader.use();
        square.drawBatch(lE.object_shader, {cubeTexture}, 1);
        endFrameBuffer(fB);
        endRenderLoopStatic(window);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    deleteFrameBuffer(fB);

    glfwTerminate();
    return 0;
}

void deleteFrameBuffer(FrameBuffer& fB){
    glDeleteVertexArrays(1, &fB.quadVAO);
    glDeleteBuffers(1, &fB.quadVBO);
    glDeleteRenderbuffers(1, &fB.rbo);
    glDeleteFramebuffers(1, &fB.framebuffer);
}

int mainStencilTesting(int argc, char* argv[]){
    GLFWwindow* window;
    int returnStatus;
    if((returnStatus = initOpenGL(window, mWidth, mHeight)) == EXIT_FAILURE) return EXIT_FAILURE;



    // build and compile shaders
    // -------------------------
    LightingEnvironment lE = initDummyMPV("Glitter/Shaders/stencil_testing.vs","Glitter/Shaders/stencil_single_color.fs", false);
    DeltaTimer deltaTimer{};
    OpenGLShape rectangle(shapes::rectT);

    while (!glfwWindowShouldClose(window)){

        // render
        // ------
        //glClearColor(0.0f,0.0f,0.0f,0.0f);
        startDummyMPVLoop(deltaTimer, lE, glm::vec4(0.0f));




        static const int MAX_OVERLAPPING_FRAGMENTS = 3;
        static const int N = 5;
        lE.object_shader.setUniformI("N", N);
        lE.object_shader.setUniformI("MAX_OVERLAPPING_FRAGMENTS", MAX_OVERLAPPING_FRAGMENTS);

        // Clear stencil to zero
        glDisable(GL_DEPTH_TEST);
        glStencilMask(0xFF);
        glClear(GL_STENCIL_BUFFER_BIT);
        glEnable(GL_STENCIL_TEST);
        glClearStencil(0);

        // Only allow stencil values less than 255
        glStencilFunc(GL_GEQUAL, MAX_OVERLAPPING_FRAGMENTS - 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);


        rectangle.drawBatch(lE.object_shader,  N);
        glm::vec2 pos;
        float zoom;
        std::cout << "FPS: " << std::to_string(1.0f/deltaTimer.getDeltaTime()) << std::endl;

        endRenderLoopView2D(window, deltaTimer, pos, zoom);
    }


    glfwTerminate();
    return returnStatus;
}

void drawFrameBuffers(std::vector<FrameBuffer>& fBs){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    // clear all relevant buffers
    for(auto& fB : fBs){
        glViewport(0,0, fB.SCR_WIDTH, fB.SCR_HEIGHT);
        fB.screenShader.use();
        glBindVertexArray(fB.quadVAO);
        glBindTexture(GL_TEXTURE_2D, fB.textureColorBuffer);	// use the color attachment texture as the texture of the quad plane
    }
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawFrameBuffer(FrameBuffer& fB){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    // clear all relevant buffers
    glViewport(0,0, fB.SCR_WIDTH, fB.SCR_HEIGHT);
    fB.screenShader.use();
    glBindVertexArray(fB.quadVAO);
    glBindTexture(GL_TEXTURE_2D, fB.textureColorBuffer);	// use the color attachment texture as the texture of the quad plane
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void endFrameBuffer(FrameBuffer& fB){
    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

}
void renderToFrameBuffer(FrameBuffer& fB){
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0,0, fB.RND_WIDTH, fB.RND_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, fB.framebuffer);
}


FrameBuffer initFrameBuffer(unsigned int RND_WIDTH, unsigned int RND_HEIGHT, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT){
    FrameBuffer fB{};
    Shader screenShader(SHADER_PATH + "framebuffers.vs",SHADER_PATH +  "screen.fs", false);
    screenShader.use();
    screenShader.setUniformI("screenTexture", 0);
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    

    // framebuffer configuration
    // -------------------------
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, RND_WIDTH, RND_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, RND_WIDTH, RND_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    fB.quadVAO = quadVAO;
    fB.quadVBO = quadVBO;
    fB.framebuffer = framebuffer;
    fB.rbo = rbo;
    fB.RND_HEIGHT = RND_HEIGHT;
    fB.RND_WIDTH = RND_WIDTH;
    fB.textureColorBuffer = textureColorbuffer;
    fB.screenShader = screenShader;
    fB.SCR_HEIGHT = SCR_HEIGHT;
    fB.SCR_WIDTH = SCR_WIDTH;
    return fB;

}

void beginFramebuffer(FrameBuffer& fB)
{
    glViewport(0,0, fB.RND_WIDTH, fB.RND_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, fB.framebuffer);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);

    // Straight alpha input, premultiplied alpha output.
    glBlendFuncSeparate(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA,
        GL_ONE,
        GL_ONE_MINUS_SRC_ALPHA
    );
}
void startFrameBuffer(FrameBuffer& fB){
    glClearColor(0.0f,0.0f,0.0f,0.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}