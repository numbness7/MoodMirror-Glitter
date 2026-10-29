
#include <helpers.hpp>


// Macros

glm::vec3 cameraFront(0.0f,0.0f,-1.0f);
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = (float)mWidth/2.0f;
float lastY = (float)mHeight/2.0f;
float fov = 30.0f;
bool first_mouse = true;
bool flash_light = true;
bool flash_light_pressed = false;
bool debug_pressed = false;
bool mouseCaptured = true;
Camera camera(glm::vec3(0.0f,0.0f,6.2f), glm::vec3(0.0f,1.0f,0.0f), -90, 0);
//Camera camera(glm::vec3(1.0f,1.3f,3.0f), glm::vec3(0.0f,1.0f,0.0f), -100, -20);


struct Material {
    unsigned int diffuse;
    unsigned int specular;
    float shininess;
};


std::random_device rd;
std::mt19937 gen(rd());

float rand_range_uniform(float min, float max){
    std::uniform_real_distribution<> dis(min,max);
    return dis(gen);
}

unsigned int rand_range_uniform(unsigned int min, unsigned int max){
    std::uniform_int_distribution<> dis(min,max);
    return dis(gen);
}

float rand_range_normal(float mean, float stddev){
    std::normal_distribution<> dis(mean,stddev);
    return dis(gen);
}

glm::vec3 randRGB(){
    return glm::vec3(rand_range_normal(0.0f,1.0f),rand_range_normal(0.0f,1.0f),rand_range_normal(0.0f,1.0f));
}

glm::vec4 randRGBA(){
    return glm::vec4(rand_range_normal(0.0f,1.0f),rand_range_normal(0.0f,1.0f),rand_range_normal(0.0f,1.0f),rand_range_normal(0.0f,1.0f));
}



void drawDoubleTexturedShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int vert_cnt, unsigned int texture1, unsigned int texture2){
    shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, vert_cnt, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
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

void releaseCursor(GLFWwindow* mWindow){
    mouseCaptured = false;
    glfwSetInputMode(mWindow, GLFW_CURSOR, mouseCaptured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void captureCursor(GLFWwindow* mWindow){
    mouseCaptured = true;
    glfwSetInputMode(mWindow, GLFW_CURSOR, mouseCaptured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void processInputStatic(GLFWwindow* mWindow){
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(mWindow, true);
}
void processInputViewStatic(GLFWwindow* mWindow, float deltatime){
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(mWindow, true);
}
void processInput(GLFWwindow* mWindow, Camera& camera, float deltatime){

    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(mWindow, true);
    if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltatime);
    if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltatime);
    if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltatime);
    if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltatime);
    if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltatime);
    if (glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltatime);
    if (glfwGetKey(mWindow, GLFW_KEY_F) == GLFW_PRESS)
        flash_light_pressed = true;
    else if(flash_light_pressed){
        flash_light_pressed = false;
        flash_light = !flash_light;
    }
    if(glfwGetKey(mWindow, GLFW_KEY_E) == GLFW_PRESS){
        debug_pressed = true;
    }
    else if(debug_pressed){
        debug_pressed = false;
        mouseCaptured = !mouseCaptured;
        if(mouseCaptured)
            releaseCursor(mWindow);
        else
            captureCursor(mWindow);
    }
}

        
void processInputFreezeGimbal(GLFWwindow* mWindow, Camera& camera, float deltatime){

    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(mWindow, true);
    if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltatime);
    if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltatime);
    if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltatime);
    if (glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltatime);
}
        


void processInput(GLFWwindow* mWindow, glm::vec3& cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float deltatime){
    float movementSpeed = 5.5;
    float cameraSpeed = movementSpeed * deltatime;
    glm::vec3 velocity(0.0f);
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(mWindow, true);
    if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
        velocity += cameraFront;
    if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
        velocity -= cameraFront;
    if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
        velocity += glm::normalize(glm::cross(cameraFront,cameraUp));
    if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
        velocity -= glm::normalize(glm::cross(cameraFront,cameraUp));
    
    if(glm::length(velocity) > SMALL_FLOAT) 
        velocity = glm::normalize(velocity) * cameraSpeed;
    cameraPos += velocity;
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


void create_lamp_and_light_object(unsigned int &VAO_O, unsigned int &VAO_T,unsigned int &VBO, unsigned int& EBO, 
    const float vert[], unsigned int vert_cnt, const unsigned int ind[], 
    unsigned int ind_cnt){
        
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 8*ind_cnt*sizeof(float), vert, GL_STATIC_DRAW);
        
        glGenBuffers(1,&EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind_cnt*sizeof(unsigned int), ind, GL_STATIC_DRAW);
        
        unsigned int dimensions = 3;
        unsigned int normals = 3;
        unsigned int texs = 2;
        unsigned int total = dimensions + normals + texs;
        
        
        // Light
        glGenVertexArrays(1, &VAO_T);
        glBindVertexArray(VAO_T);
        glVertexAttribPointer(0, dimensions, GL_FLOAT, GL_FALSE, total * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Object
        glGenVertexArrays(1, &VAO_O);
        glBindVertexArray(VAO_O);
        glVertexAttribPointer(0, dimensions, GL_FLOAT, GL_FALSE, total * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, normals, GL_FLOAT, GL_FALSE, total * sizeof(float), (void*) + (dimensions * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Texture
        glVertexAttribPointer(2, texs, GL_FLOAT, GL_FALSE, total * sizeof(float), (void*) + ((dimensions + normals) * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

void create_shape(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, 
    const float vert[], unsigned int vert_cnt, const unsigned int ind[], 
    unsigned int ind_cnt){
        
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vert_cnt*sizeof(float), vert, GL_STATIC_DRAW);
        
        glGenBuffers(1,&EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind_cnt*sizeof(unsigned int), ind, GL_STATIC_DRAW);
        
        unsigned int dimensions = 3;
        
        
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glVertexAttribPointer(0, dimensions, GL_FLOAT, GL_FALSE, dimensions * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

GLFWwindow* loadGLFWCreateWindow(int width, int height){
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(width, height, "OpenGL", nullptr, nullptr);
    return mWindow;
}

void create_textured_shape(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, 
    const float vert[], unsigned int vert_cnt, unsigned int const ind[], 
    unsigned int ind_cnt){
    
        
        
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vert_cnt*sizeof(float), vert, GL_STATIC_DRAW);
        
        glGenBuffers(1,&EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind_cnt*sizeof(unsigned int), ind, GL_STATIC_DRAW);
        
        unsigned int dimensions = 3;
        unsigned int tex_dimensions = 2;
        
        unsigned int total = dimensions + tex_dimensions;
        
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glVertexAttribPointer(0, dimensions, GL_FLOAT, GL_FALSE, total * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, tex_dimensions, GL_FLOAT, GL_FALSE, total * sizeof(float), (void*)((total-tex_dimensions)*sizeof(float)));
        glEnableVertexAttribArray(1);
    }
void create_colored_textued_shape(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, 
    float vert[], unsigned int vert_cnt, unsigned int ind[], 
    unsigned int ind_cnt){
    
        
        
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vert_cnt*sizeof(float), vert, GL_STATIC_DRAW);
        
        glGenBuffers(1,&EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind_cnt*sizeof(unsigned int), ind, GL_STATIC_DRAW);
        
        unsigned int dimensions = 3;
        unsigned int color_atts = 3;
        unsigned int tex_dimensions = 2;
        
        unsigned int total = dimensions + color_atts + tex_dimensions;
        
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glVertexAttribPointer(0, dimensions, GL_FLOAT, GL_FALSE, total * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, color_atts, GL_FLOAT, GL_FALSE, total * sizeof(float), (void*)(dimensions*sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, tex_dimensions, GL_FLOAT, GL_FALSE, total * sizeof(float), (void*)((total-tex_dimensions)*sizeof(float)));
        glEnableVertexAttribArray(2);
    }

unsigned int generate_texture(std::string texture_filepath, std::string filetype){
    unsigned int texture;
    generate_texture(texture, texture_filepath, filetype);
    return texture;
}
unsigned char* initializePNGTextureData(int width, int height, int nrChannels){
    unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char)*width*height*nrChannels);
    return data;
}
void freePNGTextureData(unsigned char* data){
    free(data);
}
unsigned int generateTextureSpecified(int width, int height, int nrChannels, std::string filetype){
    unsigned char* data = initializePNGTextureData(width, height, nrChannels);
    generatePNGData(data, width, height, nrChannels);
    unsigned int texture = useTextureDataToCreateTexture(data, width, height, filetype);
    freePNGTextureData(data);
    return texture;
}
bool isOnBorder(int width, int height, int row, int col, int hThickness, int vThickness){
    return col < hThickness || col > width - (hThickness + 1) || row < vThickness || row > height - (vThickness + 1);
}
unsigned char getBorderCheckColor(int width, int height, int row, int col, int hThickness, int vThickness){
    if (isOnBorder(width, height, row, col, hThickness, vThickness)) return cdata::FULL_COLOR_D;
    else return cdata::EMPTY_COLOR_D; 

}
void generatePNGBorder(PngData& pngData, int width, int height,  int hThickness, int vThickness, cdata::ColorRGBA borderColor, cdata::ColorRGBA innerColor, bool ignoreInner, bool ignoreBorder){
    for(int gI = 0; gI < pngData.gridSize(); gI++){
        if (isOnBorder(width, height, pngData.rFG(gI), pngData.cFG(gI), hThickness, vThickness)){
            if(!ignoreBorder) pngData.setColor(gI, borderColor);
        }
        else{
            if(!ignoreInner) pngData.setColor(gI, innerColor);
        }
    }
}
void generatePNGBorder(unsigned char* data, int width, int height, int nrChannels, int hThickness, int vThickness){
    for(PngDataIterator pDI(width, height, nrChannels); pDI.isNotDone(); pDI.iterate()){
        pDI.setChannelByte( PngDataIterator::redChannel, getBorderCheckColor(width, height, pDI.row, pDI.col, hThickness, vThickness), data[pDI.index()]);
        pDI.setChannelByte( PngDataIterator::alphaChannel, getBorderCheckColor(width, height, pDI.row, pDI.col, hThickness, vThickness), data[pDI.index()]);
    }
}

/// @brief 
/// @param width 
/// @param height 
/// @param borderVertices In clockwise order each vertex has x in [-0.5f,0.5f], y in [-0.5f,0.5f]
/// @return 
unsigned int obtainPNGDistanceFromBorder(int width, int height, std::vector<glm::vec2> borderVertices){
    PngData pngData(width, height);
    generatePNGDistanceFromBorder(pngData, borderVertices);
    unsigned int texture = getTextureFromData(pngData);
    return texture;
}

PngData loadPngTexture(std::string texture_filepath)
{
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texture_filepath.c_str(), &width, &height, &nrChannels, 0);
    PngData pngData(width, height, nrChannels, data);
    return pngData;
}
/// @brief
/// @param pngData
/// @param borderVertices In clockwise order each vertex has x in [-0.5f,0.5f], y in [-0.5f,0.5f]
/// @return
unsigned int obtainPNGDistanceFromBorder(PngData& pngData, std::vector<glm::vec2> borderVertices){
    generatePNGDistanceFromBorder(pngData, borderVertices);
    unsigned int texture = getTextureFromData(pngData);
    return texture;
}

float distFromLine(std::tuple<float,float,float> line, glm::vec2 point){
    const auto [A, B, C] = line;
    assert(A != 0 || B != 0);
    return glm::abs(A*point.x + B*point.y + C)/glm::sqrt(glm::pow(A,2) + glm::pow(B,2));
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

void checkPrint(std::string message, bool check, bool debugIt){
    if(check && debugIt)
        std::cout << message;
}
bool isInSquare(float x, float y, float centerX, float centerY, float length){
    float dx = abs(centerX - x);
    float dy = abs(centerY - y);
    return length >= dx && length >= dy;
}

bool isInSquare(glm::vec2 point, glm::vec2 center,  float length){
    return isInSquare(point.x, point.y, center.x, center.y, length);
}


void generatePNGDistanceFromBorder(PngData& pngData, std::vector<glm::vec2> borderVertices){
        std::vector<std::tuple<float,float,float>> borderLines{}; 
        glm::vec2 prevVertex  = borderVertices[borderVertices.size()-1];
        std::vector<glm::vec2> borderVerticesN{};
        glm::vec2 centerN = glm::vec2(
                boundTransformation(0, -0.5f, 0.5f, 0, pngData.width),
                boundTransformation(0, -0.5f, 0.5f, 0, pngData.height));
        for(int borderVertexIndex = 0; borderVertexIndex < borderVertices.size(); borderVertexIndex++){
            glm::vec2 curVertex;
            if(borderVertexIndex < borderVertices.size() - 1)
                curVertex = borderVertices[borderVertexIndex];
            else
                curVertex = borderVertices[borderVertices.size()-1];
            float  preVertexNormX = boundTransformation(prevVertex.x, -0.5f, 0.5f, 0, pngData.width);
            float  preVertexNormY = boundTransformation(prevVertex.y, -0.5f, 0.5f, 0, pngData.height);
            glm::vec2 preVertexNorm(preVertexNormX,preVertexNormY);
            float  curVertexNormX = boundTransformation(curVertex.x, -0.5f, 0.5f, 0, pngData.width);
            float  curVertexNormY = boundTransformation(curVertex.y, -0.5f, 0.5f, 0, pngData.height);
            glm::vec2 curVertexNorm(curVertexNormX,curVertexNormY);
            borderVerticesN.push_back(curVertexNorm);
            auto line = getLineFromVertices(preVertexNorm.x, curVertexNorm.x, preVertexNorm.y, curVertexNorm.y);
            borderLines.push_back(line);
            checkPrint(std::format(      "({:.2f},\t{:.2f})\t-> ",  prevVertex.x,  prevVertex.y), true, DEBUG_LINE);
            checkPrint(std::format(    "\t({:.2f},\t{:.2f})"  ,   curVertex.x,   curVertex.y), true, DEBUG_LINE);
            checkPrint(std::format(  "\t\t\t({:.2f}, {:.2f})\t-> ",  preVertexNorm.x,  preVertexNorm.y), true, DEBUG_LINE);
            checkPrint(std::format(    "\t({:.2f}, {:.2f})\n"  ,   curVertexNorm.x,   curVertexNorm.y), true, DEBUG_LINE);
            prevVertex = curVertex;
        }

        std::vector<std::tuple<float,float,float>> centerlines{}; 

        for(int borderVertexIndexN = 0; borderVertexIndexN < borderVerticesN.size(); borderVertexIndexN++){
            glm::vec2 curVertexN = borderVerticesN[borderVertexIndexN];
            const auto line = getLineFromVertices(centerN.x, curVertexN.x, centerN.y, curVertexN.y);
            centerlines.push_back(line);
        }

    for(int gI = 0; gI < pngData.gridSize(); gI++){
        float row = (float) pngData.rFG(gI);
        float col = (float) pngData.cFG(gI);
        float gridY = row;
        float gridX = col;
        float gridYN = boundTransformation(gridY, 0, pngData.height, -0.5f, 0.5f);
        float gridXN = boundTransformation(gridX, 0, pngData.width, -0.5f, 0.5f);
        glm::vec2 tileCenter(0.5,0.5);
        glm::vec2 gridVertex(gridX + tileCenter.x, gridY + tileCenter.y);
        


        

        

        float minDistance = std::numeric_limits<float>::max();

        int preIndex = borderVertices.size()-1;
        glm::vec2 prevVertexN  = borderVertices[preIndex];
        for(int borderVertexIndexN = 0; borderVertexIndexN < borderVertices.size(); borderVertexIndexN++){
            auto line = borderLines[borderVertexIndexN];
            glm::vec2 curVertexN;
            int curIndex = borderVertexIndexN;
            if(borderVertexIndexN >= borderVertices.size() - 1)
                curIndex = borderVertices.size()-1;
            curVertexN = borderVertices[curIndex];

            float distance = distFromLine(line, gridVertex);
            // bool isOnPreCenterLine = isOnLine(centerlines[preIndex], gridVertex);
            // bool isOnCurCenterLine = isOnLine(centerlines[curIndex], gridVertex);
            if(distance < minDistance){
                minDistance = distance;
            }
            prevVertexN = curVertexN;
        }

        float maximumPossibleDistance = glm::length(glm::vec2(pngData.width,pngData.height))/2.0f;
        static const float multiplier = 1.0f;
        unsigned int distanceFloatValueNorm = boundTransformation((unsigned int) minDistance, 0, (unsigned int) multiplier*(unsigned int) maximumPossibleDistance, 0, std::numeric_limits<unsigned int>::max());
        unsigned int distanceValue = static_cast<unsigned int>(distanceFloatValueNorm);
        cdata::ColorRGBA colorCodedDistance =  pngColorFromValue(distanceValue);
        pngData.setColor(gI, colorCodedDistance);
        

        
        unsigned int maxInt = std::numeric_limits<unsigned int>::max();
        cdata::ColorRGBA colorCodedMaxDistance =  pngColorFromValue(maxInt);
    }

}
unsigned int obtainPNGBorder(int width, int height,  int hThickness, int vThickness, cdata::ColorRGBA borderColor, cdata::ColorRGBA innerColor, bool ignoreInner, bool ignoreBorder){
    PngData pngData(width, height);
    generatePNGBorder(pngData, width, height,  hThickness, vThickness, borderColor, innerColor, ignoreInner, ignoreBorder);
    unsigned texture = getTextureFromData(pngData);
    return texture;
}
unsigned int obtainPNGBlank(PngData& pngData){
    generatePNGBlank(pngData);
    unsigned texture = getTextureFromData(pngData);
    return texture;
}
void generatePNGBlank(PngData& pngData){
    for(int gI = 0; gI < pngData.gridSize(); gI++) {
        for(int channel = 0; channel < pngData.nrChannels; channel++)
            pngData.setData(gI, cdata::FULL_COLOR_D);
    }
}

bool isInRange(float x, float min, float max){
   return min < x && x < max; 
}

bool isOnLine(std::tuple<float, float, float> line, glm::vec2 point)
{
    const auto& [A, B, C] = line;
    float actualValue = A*point.x + B*point.y + C;
    return isInRange(actualValue, -SMALL_FLOAT, SMALL_FLOAT);
}

unsigned int obtainPNGBorder(PngData &pngData, int hThickness, int vThickness, cdata::ColorRGBA borderColor, cdata::ColorRGBA innerColor, bool ignoreInner, bool ignoreBorder)
{
    generatePNGBorder(pngData, pngData.width, pngData.height,  hThickness, vThickness, borderColor, innerColor, ignoreInner, ignoreBorder);
    unsigned texture = getTextureFromData(pngData);
    return texture;
}
cdata::ColorRGBA pngColorFromValue(const unsigned int &value)
{
    const cdata::ColorRGBA color(
        (unsigned char) (value >> ((PngData::nrChannelsDefault - 1 - PngData::redChannel   ) * sizeof(unsigned char)*BYTESIZE)),
        (unsigned char) (value >> ((PngData::nrChannelsDefault - 1 - PngData::greenChannel ) * sizeof(unsigned char)*BYTESIZE)),
        (unsigned char) (value >> ((PngData::nrChannelsDefault - 1 - PngData::blueChannel  ) * sizeof(unsigned char)*BYTESIZE)),
        (unsigned char) (value >> ((PngData::nrChannelsDefault - 1 - PngData::alphaChannel ) * sizeof(unsigned char)*BYTESIZE))
    );

    return color;
}
unsigned int valueFromPngColor(cdata::ColorRGBA color)
{
    unsigned int value = 0;
    value = ((color.red  )   << ((PngData::redChannel   ) *sizeof(unsigned char)*BYTESIZE))  |
            ((color.green)   << ((PngData::greenChannel ) *sizeof(unsigned char)*BYTESIZE))  |
            ((color.blue )   << ((PngData::blueChannel  ) *sizeof(unsigned char)*BYTESIZE))  |
            ((color.alpha)   << ((PngData::alphaChannel ) *sizeof(unsigned char)*BYTESIZE))  ;
    return value;
}
void generatePNGData(unsigned char *data, int width, int height, int nrChannels)
{
    std::cout << "W: " << width << ", H: " << height << ", N: " << nrChannels << std::endl;
    static const unsigned int size = 0xFF;
    std::cout << "S: " << size << std::endl;

    unsigned int stepsToIncrementRed;
    if (height % size != 0)
        stepsToIncrementRed = height/(size) + 1;
    else
        stepsToIncrementRed = height/(size);
    std::cout << "STIR: " << stepsToIncrementRed << std::endl;
    unsigned int stepsToIncrementAlpha;
    if (width % size != 0)
        stepsToIncrementAlpha = width/(size) + 1;
    else
        stepsToIncrementAlpha = width/(size);
    unsigned char red = 0;
    unsigned int other = 0;
    
    for(int row = 0; row < height; row+=1){
        if (row % stepsToIncrementRed == 0) red++;
        if (row % stepsToIncrementRed == 0) other++;
        unsigned char alpha = 0;
        for(int col = 0; col < width*nrChannels; col+=nrChannels){
            if(col % stepsToIncrementAlpha == 0) alpha++;

            for(int channel = 0; channel < nrChannels; channel++){
                if(channel == 0) // Red
                    data[row*width*nrChannels+col+channel] = red;
                if(channel == 1) // Green
                    data[row*width*nrChannels+col+channel] = red;
                if(channel == 2) // Blue
                    data[row*width*nrChannels+col+channel] = red;
                if(channel == 3) // Don't know
                    data[row*width*nrChannels+col+channel] = alpha;
            }
        }
    }
    std::cout << "O: " << other << std::endl;
}
unsigned int useTextureDataToCreateTexture(const unsigned char* data, int width, int height, std::string filetype){
    unsigned int texture;
    glGenTextures(1, &texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, texture);
    
    if (data){
        if(filetype == "png")
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else if (filetype == "pngNA"){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            
        }
        else {
            throw std::runtime_error("Bad file type: " + filetype);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    return texture;
}
void generate_texture(unsigned int &texture, std::string texture_filepath, std::string filetype){
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texture_filepath.c_str(), &width, &height, &nrChannels, 0);
    std::cout << "W: " << width << ", H: " << height << ", N: " << nrChannels << std::endl;
    static const unsigned int size = 0xFF;
    std::cout << "S: " << size << std::endl;

    unsigned int stepsToIncrementRed;
    if (height % size != 0)
        stepsToIncrementRed = height/(size) + 1;
    else
        stepsToIncrementRed = height/(size);
    std::cout << "STIR: " << stepsToIncrementRed << std::endl;
    unsigned int stepsToIncrementAlpha;
    if (width % size != 0)
        stepsToIncrementAlpha = width/(size) + 1;
    else
        stepsToIncrementAlpha = width/(size);
    unsigned char red = 0;
    unsigned int other = 0;
    
    for(int row = 0; row < height; row+=1){
        if (row % stepsToIncrementRed == 0) red++;
        if (row % stepsToIncrementRed == 0) other++;
        unsigned char alpha = 0;
        for(int col = 0; col < width*nrChannels; col+=nrChannels){
            if(col % stepsToIncrementAlpha == 0) alpha++;

            for(int channel = 0; channel < nrChannels; channel++){
                if(channel == 0) // Red
                    data[row*width*nrChannels+col+channel] = red;
                if(channel == 1) // Green
                    data[row*width*nrChannels+col+channel] = red;
                if(channel == 2) // Blue
                    data[row*width*nrChannels+col+channel] = red;
                if(channel == 3) // Don't know
                    data[row*width*nrChannels+col+channel] = alpha;
            }
        }
    }
    std::cout << "O: " << other << std::endl;

    glGenTextures(1, &texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glBindTexture(GL_TEXTURE_2D, texture);
    
    if (data){
        if(filetype == "png")
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else {
            throw std::runtime_error("Bad file type: " + filetype);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        throw std::runtime_error("Failed to load texture: " + std::string(texture_filepath));
    }
    stbi_image_free(data);
}
unsigned int createPngTexture(std::string texture_filepath)
{
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texture_filepath.c_str(), &width, &height, &nrChannels, 0);
    PngData pngData(width, height, nrChannels, data);
    unsigned int texture;
    if(pngData.nrChannels == 4)
        texture = useTextureDataToCreateTexture(pngData.getData(), pngData.width, pngData.height, "png");
    else
        texture = useTextureDataToCreateTexture(pngData.getData(), pngData.width, pngData.height, "pngNA");
    return texture;
}
unsigned int createPngTexture(const PngData* pngData)
{
    unsigned int texture;
    if(pngData->nrChannels == 4)
        texture = useTextureDataToCreateTexture(pngData->getData(), pngData->width, pngData->height, "png");
    else
        texture = useTextureDataToCreateTexture(pngData->getData(), pngData->width, pngData->height, "pngNA");
    return texture;
}
unsigned int createPngTexture(const PngData& pngData)
{
    unsigned int texture;
    if(pngData.nrChannels == 4)
        texture = useTextureDataToCreateTexture(pngData.getData(), pngData.width, pngData.height, "png");
    else
        texture = useTextureDataToCreateTexture(pngData.getData(), pngData.width, pngData.height, "pngNA");
    return texture;
}
unsigned int createTexture(std::string texture_filepath, std::string filetype)
{
    unsigned int texture;
    create_texture(texture, texture_filepath, filetype);
    return texture;
}

void create_texture(unsigned int &texture, std::string texture_filepath, std::string filetype){
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texture_filepath.c_str(), &width, &height, &nrChannels, 0);
    glGenTextures(1, &texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, texture);
    
    if (data){
        if(filetype == "jpg")
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else if(filetype == "png")
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else {
            throw std::runtime_error("Bad file type: " + filetype);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        throw std::runtime_error("Failed to load texture: " + std::string(texture_filepath));
    }
    stbi_image_free(data);
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


// learnopgl start

int mainGenerateTexturesCubes(int argc, char * argv[]){
    
    // Initialize OpenGL
    int return_status = EXIT_SUCCESS;
    GLFWwindow*  mWindow; 
    initOpenGL(mWindow, return_status, mWidth, mHeight);
    if(return_status != EXIT_SUCCESS) return return_status;
    
    DeltaTimer deltaTimer{};
    

    
    unsigned int texture1;
    generate_texture(texture1, "Glitter/Textures/awesomeface.png", "png");



    Shader shaderTextureMVP("Glitter/Shaders/texture-mvp.vs", "Glitter/Shaders/texture-mvp.fs");
    
    unsigned int VBO_T, VAO_T, EBO_T;
    
    
    
    
    create_textured_shape(VAO_T, VBO_T, EBO_T, shapes::textured_cube, 5*36, shapes::cube_ind, 36);
    
    

    shaderTextureMVP.use();
    shaderTextureMVP.setUniformI("ourTexture1",(unsigned int)0);
    shaderTextureMVP.setUniformI("ourTexture2",(unsigned int)1);
    
    glm::mat4 proj;
    proj = glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.1f, 100.0f);

    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));

    glm::mat4 model(1.0f);

    shaderTextureMVP.setUniformM4("projection", proj);
    shaderTextureMVP.setUniformM4("view", view);
    shaderTextureMVP.setUniformM4("model",model);
    
    
    glm::vec3 cameraPos(0.0f,0.0f,3.0f);
    glm::vec3 cameraUp(0.0f,1.0f,0.0f);
    glm::vec3 cameraTarget(0.0f,0.0f,0.0f);
    
    glm::vec3 toCameraDirection(cameraPos - cameraTarget);
    glm::vec3 up(0.0f,1.0f,0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, toCameraDirection));
    
    
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);


    
    
    // #Rendering Loop
    while (!glfwWindowShouldClose(mWindow)) {

        // Background Fill Color
        glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        proj = glm::perspective(glm::radians(camera.Zoom), (float)mWidth / (float)mHeight, 0.1f, 100.0f);
        shaderTextureMVP.setUniformM4("projection",proj);
        

        view = camera.GetViewMatrix();
        shaderTextureMVP.setUniformM4("view",view);


        for(unsigned int i = 0; i < std::size(shapes::cubePositions); i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, shapes::cubePositions[i]);
            model = glm::rotate(model, glm::radians(15.0f)*(float)i, glm::vec3(0.4f,0.95f,0.2f));
            shaderTextureMVP.setUniformM4("model",model);
            drawTexturedShape(VAO_T, EBO_T, shaderTextureMVP, 36, texture1);

        }
        


        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
        processInput(mWindow, camera, deltaTimer.getDeltaTime(false));
        

        
    }   glfwTerminate();
    return EXIT_SUCCESS;
}

glm::vec2 screenToModelCoords(float x, float y, float width, float height){
    return glm::vec2(boundTransformation(x, 0.0f, width, -1.0f, 1.0f),boundTransformation(y, 0.0f, height, -1.0f, 1.0f));
}

int mainCubes(int argc, char * argv[]){
    
    // Initialize OpenGL
    int return_status = EXIT_SUCCESS;
    GLFWwindow*  mWindow; 
initOpenGL(mWindow, return_status, mWidth, mHeight);
    if(return_status != EXIT_SUCCESS) return return_status;
    
    DeltaTimer deltaTimer{};
    

    
    unsigned int texture2;
    create_texture(texture2, "Glitter/Textures/awesomeface.png", "png");


    Shader shaderTextureMVP("Glitter/Shaders/texture-mvp-color.vs", "Glitter/Shaders/texture-mvp-color.fs");
    
    
    
    
    OpenGLShape cube(36,36,shapes::textured_cube,shapes::cube_ind,std::vector<unsigned>{3,2},texture2,true);

    
    
    

    shaderTextureMVP.use();
    shaderTextureMVP.setUniformI("ourTexture1",(unsigned int)0);
    
    glm::mat4 proj;
    proj = glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.1f, 100.0f);

    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));

    glm::mat4 model(1.0f);

    shaderTextureMVP.setUniformM4("projection", proj);
    shaderTextureMVP.setUniformM4("view", view);
    shaderTextureMVP.setUniformM4("model",model);
    shaderTextureMVP.setUniformV4("aColor", glm::vec4(0.6));
    
    
    glm::vec3 cameraPos(0.0f,0.0f,3.0f);
    glm::vec3 cameraUp(0.0f,1.0f,0.0f);
    glm::vec3 cameraTarget(0.0f,0.0f,0.0f);
    
    glm::vec3 toCameraDirection(cameraPos - cameraTarget);
    glm::vec3 up(0.0f,1.0f,0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, toCameraDirection));
    
    
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);


    proj = glm::mat4(1.0f);
    shaderTextureMVP.setUniformM4("projection",proj);
    view = glm::mat4(1.0f);
    shaderTextureMVP.setUniformM4("view",view);
    
    // Rendering Loop
    while (!glfwWindowShouldClose(mWindow)) {

        // Background Fill Color
        glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        deltaTimer.updateDeltaTime();
        

        



        for(unsigned int i = 0; i < std::size(shapes::cubePositions); i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, shapes::cubePositions[i]);
            model = glm::rotate(model, glm::radians(15.0f)*(float)i, glm::vec3(0.4f,0.95f,0.2f));
            shaderTextureMVP.setUniformM4("model",model);
            cube.draw(shaderTextureMVP);
        }
        


        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
        processInput(mWindow, camera, deltaTimer.getDeltaTime());
        

        
    }   glfwTerminate();
    return EXIT_SUCCESS;
}


// learnopgl end

glm::vec3 cameraDirection(float yaw, float pitch){
    glm::vec3 direction;
    direction.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    direction.y = glm::sin(glm::radians(pitch));
    direction.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    return direction;
}
void mouse_callback(GLFWwindow* mWindow, double xpos, double ypos){
    if(first_mouse){
        lastX = xpos;
        lastY = ypos;
        first_mouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* mWindow, double xoffset, double yoffset){
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

int mainLight(int argc, char * argv[]){
    
    // Initialize OpenGL
    int return_status = EXIT_SUCCESS;
    GLFWwindow*  mWindow; 
initOpenGL(mWindow, return_status, mWidth, mHeight);
    if(return_status != EXIT_SUCCESS) return return_status;
    
    DeltaTimer deltaTimer{};
    

    


    
    unsigned int VBO_O, VAO_O, EBO_O;
    unsigned int VAO_T;
    unsigned int diffuse_sampler_2d;
    unsigned int specular_sampler_2d;
    create_texture(diffuse_sampler_2d, "Glitter/Textures/container2.png", "png");
    create_texture(specular_sampler_2d, "Glitter/Textures/container2_specular.png", "png");
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_sampler_2d);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specular_sampler_2d);


    
    create_lamp_and_light_object(VAO_O, VAO_T, VBO_O, EBO_O, shapes::normal_textured_triangle_pyramid, 8*36, shapes::normal_textured_triangle_pyramid_ind, 36);
    
    
    
    

    
    glm::mat4 proj;
    proj = glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.1f, 100.0f);

    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));

    glm::mat4 model(1.0f);
    glm::vec3 light_positions[4] = {
        glm::vec3(-10.2f,1.0f,2.0f),
        glm::vec3(0.0f,-10.0f,2.0f),
        glm::vec3(1.2f,0.0f,-10.0f),
        glm::vec3(1.2f,1.0f,2.0f),
    };

    Shader object_shader("Glitter/Shaders/light-object.vs", "Glitter/Shaders/light-object.fs");
    Shader light_shader("Glitter/Shaders/lamp.vs", "Glitter/Shaders/lamp.fs");
    object_shader.use();
    object_shader.setUniformM4("projection", proj);
    object_shader.setUniformM4("view", view);
    object_shader.setUniformM4("model",model);
    
    Material material;
    material.diffuse = 0;
    material.specular = 1;
    material.shininess  = 128.0f*0.25; 

   object_shader.setUniformI("material.diffuse",   material.diffuse);
   object_shader.setUniformI("material.specular",  material.specular);
   object_shader.setUniformF("material.shininess", material.shininess); 
   object_shader.setUniformF("spot_light.cut_off", glm::cos(glm::radians(14.5f)));
   object_shader.setUniformF("spot_light.outer_cut_off", glm::cos(glm::radians(20.0f)));


    object_shader.setUniformV3("dir_light.ambient",  glm::vec3(0.1f));
    object_shader.setUniformV3("dir_light.diffuse",  glm::vec3(0.2f)); // darken diffuse light a bit
    object_shader.setUniformV3("dir_light.specular", glm::vec3(0.2f)); 
    object_shader.setUniformV3("dir_light.direction", glm::vec3(0.0f, 1.0f, 1.0f)); 
    
    object_shader.setUniformV3("spot_light.ambient",  glm::vec3(0.1f));
    object_shader.setUniformV3("spot_light.diffuse",  glm::vec3(1.0f)); // darken diffuse light a bit
    object_shader.setUniformV3("spot_light.specular", glm::vec3(1.0f)); 
    glm::vec3 diffuses[4] = {
        glm::vec3(1.0f,0.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        glm::vec3(0.0f,0.0f,1.0f),
        glm::vec3(0.3f,0.3f,0.3f),
    };

    std::stringstream s ("");
    for(int i = 0; i < 4; i ++){
        s << "point_lights" << "[" << i << "]";
        object_shader.setUniformV3(s.str() + ".ambient",  glm::vec3(0.1f));
        object_shader.setUniformV3(s.str() + ".diffuse",  diffuses[i]); // darken diffuse light a bit
        object_shader.setUniformV3(s.str() + ".specular", diffuses[i]); 
        object_shader.setUniformV3(s.str() + ".position", light_positions[i]);
        object_shader.setUniformF(s.str() + ".constant", 1.0f);
        object_shader.setUniformF(s.str() + ".linear", .09f);
        object_shader.setUniformF(s.str() + ".quadratic", .032f);
        s.str("");
        s.clear();
    }
    
    light_shader.use();
    
    light_shader.setUniformM4("projection", proj);
    light_shader.setUniformM4("view", view);
    light_shader.setUniformM4("model",model);
    light_shader.setUniformV3("aColor",glm::vec3(0.0f,1.0f,0.0f));

    
    
    
    
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);


    
    
    // Rendering Loop
    while (!glfwWindowShouldClose(mWindow)) {

        // Background Fill Color
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        deltaTimer.updateDeltaTime();
        

        proj = glm::perspective(glm::radians(camera.Zoom), (float)mWidth / (float)mHeight, 0.1f, 100.0f);
        

        view = camera.GetViewMatrix();
        
        
        

        light_shader.use();
        light_shader.setUniformM4("projection",proj);
        light_shader.setUniformM4("view",view);

        for(int i = 0; i < 4; i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, light_positions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            light_shader.setUniformM4("model", model);
            light_shader.setUniformV3("aColor", diffuses[i]);
            drawShape(VAO_T, EBO_O, light_shader, 36);
        }

        object_shader.use();
        object_shader.setUniformM4("view",view);
        object_shader.setUniformM4("projection",proj);
        object_shader.setUniformV3("view_pos", camera.Position);
       object_shader.setUniformV3("spot_light.position", camera.Position);
       object_shader.setUniformV3("spot_light.direction", camera.Front);
       object_shader.setUniformB("spot_light.is_on", (bool)flash_light);

        for(unsigned int i = 0; i < std::size(shapes::cubePositions); i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, shapes::cubePositions[i]);
            //model = glm::rotate(model, glm::radians(15.0f)*(float)i, glm::vec3(0.4f,0.95f,0.2f));
            object_shader.setUniformM4("model", model);
            drawTexturedShape(VAO_O, EBO_O, object_shader, 36, diffuse_sampler_2d);
        }
        
        



        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
        processInput(mWindow, camera, deltaTimer.getDeltaTime());
        

        
    }   glfwTerminate();
    return EXIT_SUCCESS;
}

    
int mainTextureGenerate(int argc, char * argv[]){
    
    // Initialize OpenGL
    int return_status = EXIT_SUCCESS;
    GLFWwindow*  mWindow; 
initOpenGL(mWindow, return_status, mWidth, mHeight);
    if(return_status != EXIT_SUCCESS) return return_status;
    
    DeltaTimer deltaTimer{};
    

    


    
    unsigned int VBO_O, VAO_O, EBO_O;
    unsigned int VAO_T;
    unsigned int diffuse_sampler_2d;
    unsigned int specular_sampler_2d;
    generate_texture(diffuse_sampler_2d, "Glitter/Textures/container2.png", "png");
    create_texture(specular_sampler_2d, "Glitter/Textures/container2_specular.png", "png");
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_sampler_2d);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specular_sampler_2d);


    
    create_lamp_and_light_object(VAO_O, VAO_T, VBO_O, EBO_O, shapes::normal_textured_cube, 8*36, shapes::cube_ind, 36);
    
    
    
    

    
    glm::mat4 proj;
    proj = glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.1f, 100.0f);

    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));

    glm::mat4 model(1.0f);
    glm::vec3 light_positions[4] = {
        glm::vec3(-10.2f,1.0f,2.0f),
        glm::vec3(0.0f,-10.0f,2.0f),
        glm::vec3(1.2f,0.0f,-10.0f),
        glm::vec3(1.2f,1.0f,2.0f),
    };

    Shader object_shader("Glitter/Shaders/light-object.vs", "Glitter/Shaders/light-object.fs");
    Shader light_shader("Glitter/Shaders/lamp.vs", "Glitter/Shaders/lamp.fs");
    object_shader.use();
    object_shader.setUniformM4("projection", proj);
    object_shader.setUniformM4("view", view);
    object_shader.setUniformM4("model",model);
    
    Material material;
    material.diffuse = 0;
    material.specular = 1;
    material.shininess  = 128.0f*0.25; 

   object_shader.setUniformI("material.diffuse",   material.diffuse);
   object_shader.setUniformI("material.specular",  material.specular);
   object_shader.setUniformF("material.shininess", material.shininess); 
   object_shader.setUniformF("spot_light.cut_off", glm::cos(glm::radians(14.5f)));
   object_shader.setUniformF("spot_light.outer_cut_off", glm::cos(glm::radians(20.0f)));


    object_shader.setUniformV3("dir_light.ambient",  glm::vec3(0.1f));
    object_shader.setUniformV3("dir_light.diffuse",  glm::vec3(0.2f)); // darken diffuse light a bit
    object_shader.setUniformV3("dir_light.specular", glm::vec3(0.2f)); 
    object_shader.setUniformV3("dir_light.direction", glm::vec3(0.0f, 1.0f, 1.0f)); 
    
    object_shader.setUniformV3("spot_light.ambient",  glm::vec3(0.1f));
    object_shader.setUniformV3("spot_light.diffuse",  glm::vec3(1.0f)); // darken diffuse light a bit
    object_shader.setUniformV3("spot_light.specular", glm::vec3(1.0f)); 
    glm::vec3 diffuses[4] = {
        glm::vec3(1.0f,0.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        glm::vec3(0.0f,0.0f,1.0f),
        glm::vec3(0.3f,0.3f,0.3f),
    };

    std::stringstream s ("");
    for(int i = 0; i < 4; i ++){
        s << "point_lights" << "[" << i << "]";
        object_shader.setUniformV3(s.str() + ".ambient",  glm::vec3(0.1f));
        object_shader.setUniformV3(s.str() + ".diffuse",  diffuses[i]); // darken diffuse light a bit
        object_shader.setUniformV3(s.str() + ".specular", diffuses[i]); 
        object_shader.setUniformV3(s.str() + ".position", light_positions[i]);
        object_shader.setUniformF(s.str() + ".constant", 1.0f);
        object_shader.setUniformF(s.str() + ".linear", .09f);
        object_shader.setUniformF(s.str() + ".quadratic", .032f);
        s.str("");
        s.clear();
    }
    
    light_shader.use();
    
    light_shader.setUniformM4("projection", proj);
    light_shader.setUniformM4("view", view);
    light_shader.setUniformM4("model",model);
    light_shader.setUniformV3("aColor",glm::vec3(0.0f,1.0f,0.0f));

    
    
    
    
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);


    
    
    // Rendering Loop
    while (!glfwWindowShouldClose(mWindow)) {

        // Background Fill Color
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        proj = glm::perspective(glm::radians(camera.Zoom), (float)mWidth / (float)mHeight, 0.1f, 100.0f);
        

        view = camera.GetViewMatrix();
        
        
        

        light_shader.use();
        light_shader.setUniformM4("projection",proj);
        light_shader.setUniformM4("view",view);

        for(int i = 0; i < 4; i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, light_positions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            light_shader.setUniformM4("model", model);
            light_shader.setUniformV3("aColor", diffuses[i]);
            drawShape(VAO_T, EBO_O, light_shader, 36);
        }

        object_shader.use();
        object_shader.setUniformM4("view",view);
        object_shader.setUniformM4("projection",proj);
        object_shader.setUniformV3("view_pos", camera.Position);
       object_shader.setUniformV3("spot_light.position", camera.Position);
       object_shader.setUniformV3("spot_light.direction", camera.Front);
       object_shader.setUniformB("spot_light.is_on", (bool)flash_light);

        for(unsigned int i = 0; i < std::size(shapes::cubePositions); i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, shapes::cubePositions[i]);
            model = glm::rotate(model, glm::radians(15.0f)*(float)i, glm::vec3(0.4f,0.95f,0.2f));
            object_shader.setUniformM4("model", model);
            drawTexturedShape(VAO_O, EBO_O, object_shader, 36, diffuse_sampler_2d);
        }
        
        



        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
        processInput(mWindow, camera, deltaTimer.getDeltaTime());
        

        
    }   glfwTerminate();
    return EXIT_SUCCESS;
}

#define ProgressMargin 0.0001f
    
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

int mainPathCube(int argc, char * argv[]){
    
    // Initialize OpenGL
    int return_status = EXIT_SUCCESS;
    GLFWwindow*  mWindow; 
initOpenGL(mWindow, return_status, mWidth, mHeight);
    if(return_status != EXIT_SUCCESS) return return_status;
    
    DeltaTimer deltaTimer{};
    

    
    unsigned int texture1;
    generate_texture(texture1, "Glitter/Textures/awesomeface.png", "png");



    Shader shaderTextureMVP("Glitter/Shaders/texture-mvp.vs", "Glitter/Shaders/texture-mvp.fs");
    
    unsigned int VBO_T, VAO_T, EBO_T;
    
    
    
    
    create_textured_shape(VAO_T, VBO_T, EBO_T, shapes::textured_triangle_pyramid, 5*36, shapes::triangle_pyramid_ind, 36);
    
    

    shaderTextureMVP.use();
    shaderTextureMVP.setUniformI("ourTexture",(unsigned int)0);
    
    glm::mat4 proj;
    proj = glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.1f, 100.0f);

    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));

    glm::mat4 model(1.0f);

    shaderTextureMVP.setUniformM4("projection", proj);
    shaderTextureMVP.setUniformM4("view", view);
    shaderTextureMVP.setUniformM4("model",model);
    
    
    glm::vec3 cameraPos(0.0f,0.0f,3.0f);
    glm::vec3 cameraUp(0.0f,1.0f,0.0f);
    glm::vec3 cameraTarget(0.0f,0.0f,0.0f);
    
    glm::vec3 toCameraDirection(cameraPos - cameraTarget);
    glm::vec3 up(0.0f,1.0f,0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, toCameraDirection));
    
    
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);

    StraightPath path(std::vector<glm::vec3>{glm::vec3(10,10,0), glm::vec3(0, 10, 0), glm::vec3(0,0,0), glm::vec3(10, 0, 0), glm::vec3(10,10,0)}, 0);

    DeltaTimer delta_timer{};
    float timer = 0.0f;
    
    
    // Rendering Loop
    while (!glfwWindowShouldClose(mWindow)) {

        // Background Fill Color
        glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        proj = glm::perspective(glm::radians(camera.Zoom), (float)mWidth / (float)mHeight, 0.1f, 100.0f);
        shaderTextureMVP.setUniformM4("projection",proj);
        

        view = camera.GetViewMatrix();
        shaderTextureMVP.setUniformM4("view",view);
        timer += delta_timer.getDeltaTime();
        if(timer > 1.0f) {
            timer = timer - 1.0f;
        }
        path.setProgress(timer);

        model = glm::mat4(1.0f);
        model = glm::translate(model, path.getPositionOnPath());
        shaderTextureMVP.setUniformM4("model",model);
        drawTexturedShape(VAO_T, EBO_T, shaderTextureMVP, 36, texture1);
        


        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
        processInput(mWindow, camera, deltaTimer.getDeltaTime());
        

        
    }   glfwTerminate();
    return EXIT_SUCCESS;
}

void generateSampler2Ds(unsigned int& diffuse_sampler_2d, unsigned int& specular_sampler_2d){
    
    generate_texture(diffuse_sampler_2d, "Glitter/Textures/container2.png", "png");
    generate_texture(specular_sampler_2d, "Glitter/Textures/container2_specular.png", "png");
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_sampler_2d);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specular_sampler_2d);
}
void setupSampler2Ds(unsigned int& diffuse_sampler_2d, unsigned int& specular_sampler_2d, std::string diffuse_path, std::string diffuse_file_type, std::string specular_path, std::string specular_file_type){
    
    create_texture(diffuse_sampler_2d, diffuse_path, diffuse_file_type);
    create_texture(specular_sampler_2d, specular_path, specular_file_type);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_sampler_2d);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specular_sampler_2d);
}

int mainTurning(int argc, char * argv[]){
    // Initialize OpenGL
    int return_status = EXIT_SUCCESS;
    GLFWwindow*  mWindow; 
    initOpenGL(mWindow, return_status, mWidth, mHeight);
    if(return_status != EXIT_SUCCESS) return return_status;
    
    DeltaTimer deltaTimer{};
    
    unsigned int diffuse_sampler_2d;
    unsigned int specular_sampler_2d;
    generateSampler2Ds(diffuse_sampler_2d, specular_sampler_2d);


    
    
    std::vector<unsigned int> lightSizes{};
    lightSizes.push_back(3);
    OpenGLShape lightShape((unsigned int) 36, (unsigned int)36, shapes::cube, shapes::cube_ind, lightSizes, (unsigned int)0, false);
    std::vector<unsigned int> objectSizes{3,3,2};
    OpenGLShape objectShape((unsigned int) 36, (unsigned int)36, shapes::normal_textured_cube, shapes::cube_ind, objectSizes, diffuse_sampler_2d, true);
    
    
    glm::vec3 light_positions[POINT_LIGHT_COUNT] = {
        glm::vec3(-10.2f,1.0f,2.0f),
        glm::vec3(0.0f,-10.0f,2.0f),
        glm::vec3(1.2f,0.0f,-10.0f),
        glm::vec3(1.2f,1.0f,2.0f),
    };

    glm::vec3 diffuses[POINT_LIGHT_COUNT] = {
        glm::vec3(1.0f,0.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        glm::vec3(0.0f,0.0f,1.0f),
        glm::vec3(0.3f,0.3f,0.3f),
    };
    
    LightingEnvironment lightingEnvironment = initLighting(light_positions, diffuses);

    
    
    
    
    initMouse(mWindow);


    
    std::deque<ShapeProperties> cubeMatrices{};
    

    glm::mat4 rotatedModel(1.0f);
    float length = 3.0f;

    
    
    // Rendering Loop
    while (!glfwWindowShouldClose(mWindow)) {

        // Background Fill Color
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        setupLightingEnvironmentToDraw(lightingEnvironment, light_positions, diffuses, lightShape);



        std::vector<std::vector<float>> emotionArrays = readEmotionArray("../data.json");
        
        // Use emotion array to set properties of shapes
        glm::vec3 emotionRGB = emotionArrayToColor(emotionArrays[0]);
        glm::vec3 rotationAxis = glm::vec3(sin(getElapsedTime()*1.0f + M_1_PI),-sin(getElapsedTime()*1.0f/2.7865432f + M_1_PI/2),sin(getElapsedTime()*1.0f/3.125105f + M_1_PI/3));
        rotatedModel = glm::rotate(glm::mat4(1.0f), (float)(1.0f*((float)(M_PI))), rotationAxis);

        
        // Stationary Cube
        lightingEnvironment.model = glm::mat4(1.0f);
        lightingEnvironment.model = glm::scale(lightingEnvironment.model, glm::vec3(0.2f));
        
        // Rotating Rod
        glm::vec4 color = glm::vec4(1.0f,1.0f,1.0f,1.0f);
        lightingEnvironment.object_shader.setUniformM4("model",lightingEnvironment.model);
        lightingEnvironment.object_shader.setUniformV4("aColor", color);
        objectShape.draw(lightingEnvironment.object_shader);
        lightingEnvironment.model = glm::mat4(rotatedModel);
        lightingEnvironment.model = glm::translate(lightingEnvironment.model, glm::vec3(length/2,0.0f,0.0f));
        lightingEnvironment.model = glm::scale(lightingEnvironment.model, glm::vec3(length,0.1f,0.1f));
        lightingEnvironment.object_shader.setUniformM4("model",lightingEnvironment.model);
        lightingEnvironment.object_shader.setUniformV4("aColor", color);
        objectShape.draw(lightingEnvironment.object_shader);
        
        // Create cube at location rod is pointing to
        lightingEnvironment.model = glm::mat4(rotatedModel);
        lightingEnvironment.model = glm::translate(lightingEnvironment.model, glm::vec3(length,0.0f,0.0f));
        lightingEnvironment.model = glm::scale(lightingEnvironment.model, glm::vec3(0.10f));

        cubeMatrices.push_front(ShapeProperties{lightingEnvironment.model,glm::vec4(emotionRGB,0.5f)});
        
        // Delete a cube
        if (cubeMatrices.size() > 10000){
            cubeMatrices.pop_back();
        }

        // Draw cubes
        for( ShapeProperties cubePair : cubeMatrices){
            lightingEnvironment.object_shader.setUniformM4("model",cubePair.model);
            lightingEnvironment.object_shader.setUniformV4("aColor", cubePair.color);
            objectShape.draw(lightingEnvironment.object_shader);
        }
        



        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
        std::cout << "FPS: " << 1/deltaTimer.getDeltaTime() << std::endl;
        processInput(mWindow, camera, deltaTimer.getDeltaTime());
        deltaTimer.updateDeltaTime();

        
    }   glfwTerminate();
    return EXIT_SUCCESS;
    
    
    
}

int mainPathRectangle(int argc, char * argv[]){
    
    // Initialize OpenGL
    int return_status = EXIT_SUCCESS;
    GLFWwindow*  mWindow; 
initOpenGL(mWindow, return_status, mWidth, mHeight);
    if(return_status != EXIT_SUCCESS) return return_status;
    
    DeltaTimer deltaTimer{};
    

    
    unsigned int texture1;
    generate_texture(texture1, "Glitter/Textures/awesomeface.png", "png");



    Shader shaderTextureMVP("Glitter/Shaders/texture-mvp.vs", "Glitter/Shaders/texture-mvp.fs");
    
    unsigned int VBO_T, VAO_T, EBO_T;
    
    
    
    
    create_textured_shape(VAO_T, VBO_T, EBO_T, shapes::rect_t, 5*6, shapes::rect_ind, 6);
    
    

    shaderTextureMVP.use();
    shaderTextureMVP.setUniformI("ourTexture",(unsigned int)0);
    
    glm::mat4 proj;
    proj = glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.1f, 100.0f);

    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));

    glm::mat4 model(1.0f);

    shaderTextureMVP.setUniformM4("projection", proj);
    shaderTextureMVP.setUniformM4("view", view);
    shaderTextureMVP.setUniformM4("model",model);
    
    
    glm::vec3 cameraPos(0.0f,0.0f,3.0f);
    glm::vec3 cameraUp(0.0f,1.0f,0.0f);
    glm::vec3 cameraTarget(0.0f,0.0f,0.0f);
    
    glm::vec3 toCameraDirection(cameraPos - cameraTarget);
    glm::vec3 up(0.0f,1.0f,0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, toCameraDirection));
    
    
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);

    StraightPath path(std::vector<glm::vec3>{glm::vec3(10,10,0), glm::vec3(0, 10, 0), glm::vec3(0,0,0), glm::vec3(10, 0, 0), glm::vec3(10,10,0)}, 0);

    DeltaTimer delta_timer{};
    float timer = 0.0f;
    
    
    // Rendering Loop
    while (!glfwWindowShouldClose(mWindow)) {

        // Background Fill Color
        glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        proj = glm::perspective(glm::radians(camera.Zoom), (float)mWidth / (float)mHeight, 0.1f, 100.0f);
        shaderTextureMVP.setUniformM4("projection",proj);
        

        view = camera.GetViewMatrix();
        shaderTextureMVP.setUniformM4("view",view);
        timer += delta_timer.getDeltaTime();
        if(timer > 1.0f) {
            timer = timer - 1.0f;
        }
        std::cout << "timer: " << timer << std::endl;
        path.setProgress(timer);

        model = glm::mat4(1.0f);
        model = glm::translate(model, path.getPositionOnPath());
        shaderTextureMVP.setUniformM4("model",model);
        drawTexturedShape(VAO_T, EBO_T, shaderTextureMVP, 36, texture1);
        


        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
        processInput(mWindow, camera, deltaTimer.getDeltaTime());
        

        
    }   glfwTerminate();
    return EXIT_SUCCESS;
}
unsigned int boundTransformation(unsigned int x, unsigned int min, unsigned int max, unsigned int minT, unsigned int maxT){
    unsigned int slope = (maxT - minT)/(max - min);
    unsigned int intercept = minT - slope*min;
    return slope*x + intercept;
}
float boundTransformation(float x, float min, float max, float minT, float maxT){
    float slope = (maxT - minT)/(max - min);
    float intercept = minT - slope*min;
    return slope*x + intercept;
}
std::tuple<float,float> linearTransformation(float xf, float xi, float yf, float yi){
    if(xf - xi == 0){
        throw(std::runtime_error("Error: No horizontal distance"));
    }
    float slope = (yf - yi)/(xf - xi);
    float intercept = yi - slope*xi;
    return std::tuple<float,float>(slope,intercept);
}
std::tuple<float,float,float> getLineFromVertices(float xi, float xf, float yi, float yf){
    float A = yf - yi;
    float B = xi - xf;
    float C = xf*yi - xi*yf;
    return {A,B,C};
    
}
float boundTransformation(float x, std::tuple<float,float> line){
    return x*std::get<0>(line)+ std::get<1>(line);
}
float boundTransformation(float x, float slope, float intercept){
    return x*slope + intercept;
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
nlohmann::json readjsonfile(std::string filepath)
{

    while(true){
        try{
            using json = nlohmann::json;
            std::string json_string = readfile(filepath.c_str());
            json data = json::parse(json_string);
            return data;
        }
        catch(std::exception e){
            // TODO: LOG THIS
            //std::cerr << e.what() << std::endl;
                
        }
    }
}

std::string formatFloat(float f, int digitsBefore, int digitsAfter){
    std::string floatString = std::to_string(f);
    int decimalPointIndex;
    for(decimalPointIndex = 0; floatString[decimalPointIndex] != '.'; decimalPointIndex++);
    assert(decimalPointIndex <= digitsBefore);
    for(int i = 0; i < decimalPointIndex - digitsBefore; i++){
        floatString = '0' + floatString;
    }
    int digitsAfterPoint = floatString.size() - decimalPointIndex - 1;
    assert(digitsAfterPoint <= digitsAfter);
    for(; digitsAfterPoint < digitsAfter; digitsAfterPoint++) floatString += '0';
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
    float hue;

    if(redHue > greenHue && redHue > blueHue)
        if(greenHue > blueHue) hue = (greenHue - blueHue)/(redHue - blueHue);
        else hue = (greenHue - blueHue)/(redHue - greenHue);
    else if(greenHue > redHue && greenHue > blueHue)
        if(redHue > blueHue) hue = 2.0f + (blueHue - redHue)/(greenHue - blueHue);
        else hue = 2.0f + (blueHue - redHue)/(greenHue - redHue);
    else
        if(redHue > greenHue) hue = 4.0f + (redHue - greenHue)/(blueHue - greenHue);
        else hue = 4.0f + (redHue - greenHue)/(blueHue - redHue);
    hue *= 60.0f;
    if(hue < 0.0f) hue += 360.0f;
    
    float saturation = 100.0f*(0.4f*disgust + 0.75f*sadness + 0.77f*fear + anger + happiness + surprise)/(6.0f);
    float lightness = 100.0f*(0.2f*fear + 0.4f*sadness + 0.45f*anger + 0.5f*disgust + 0.66f*surprise + 0.75f*happiness)/(6.0f);
    color::hsl<float> colorHSL({hue,saturation,lightness});
    color::rgb<float> colorRGB;
    colorRGB = colorHSL;
    
    float red = color::get::red(colorRGB);
    float green = color::get::green(colorRGB);
    float blue = color::get::blue(colorRGB);
    
    
    
    
    return glm::vec3(red,green,blue);
    
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
template <typename NUMBER, typename ITERABLE> NUMBER sum(ITERABLE iterable){
    NUMBER summation = 0;
    for(auto const number : iterable) summation += number;
    return summation;
}
template <typename NUMBER, typename ITERABLE> NUMBER mean(ITERABLE iterable){
    NUMBER summation = ssum<NUMBER>(iterable);
    return summation/iterable.size();
}
float emotionArrayToRotation(std::vector<float> emotionArray)
{
    float happiness = emotionArray[HAPPY];
    float sadness   = emotionArray[SAD];
    float anger     = emotionArray[ANGRY];
    float fear      = emotionArray[FEAR];
    float surprise  = emotionArray[SURPRISE];
    float disgust   = emotionArray[DISGUST];
    
    glm::vec2 happinessDir = dir::UP;
    glm::vec2 surpriseDir =  dir::UP_RIGHT;
    glm::vec2 angerDir = dir::RIGHT;
    glm::vec2 sadnessDir = dir::DOWN;
    glm::vec2 disgustDir = dir::DOWN_LEFT;
    glm::vec2 fearDir = dir::LEFT;
    
    glm::vec2 combineDir = glm::normalize(happiness*happinessDir + surprise* surpriseDir + anger* angerDir + sadness* sadnessDir + disgust * disgustDir + fear * fearDir);
    float rotation = glm::atan(combineDir.y, combineDir.x);
    return rotation;

}
void initMouse(GLFWwindow* mWindow){
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);
}
LightingEnvironment initMPV(std::string vertexShaderPath, std::string fragmentShaderPath){
    glm::mat4 proj;
    proj = glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.1f, 100000.0f);
    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));
    glm::mat4 model(1.0f);

    Shader object_shader(vertexShaderPath, fragmentShaderPath);
    Shader light_shader(vertexShaderPath, fragmentShaderPath);
    object_shader.use();
    object_shader.setUniformM4("projection", proj);
    object_shader.setUniformM4("view", view);
    object_shader.setUniformM4("model",model);
    LightingEnvironment lightingEnvironment{model,view,proj,object_shader,light_shader};
    return lightingEnvironment;
}
LightingEnvironment initLighting(glm::vec3 light_positions[POINT_LIGHT_COUNT], glm::vec3 diffuses[POINT_LIGHT_COUNT]){
    glm::mat4 proj;
    proj = glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.1f, 100.0f);

    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));

    glm::mat4 model(1.0f);

    Shader object_shader("Glitter/Shaders/light-object-color.vs", "Glitter/Shaders/light-object-color.fs");
    Shader light_shader("Glitter/Shaders/lamp.vs", "Glitter/Shaders/lamp.fs");
    object_shader.use();
    object_shader.setUniformM4("projection", proj);
    object_shader.setUniformM4("view", view);
    object_shader.setUniformM4("model",model);
    
    Material material;
    material.diffuse = 0;
    material.specular = 1;
    material.shininess  = 128.0f*0.25; 

   object_shader.setUniformI("material.diffuse",   material.diffuse);
   object_shader.setUniformI("material.specular",  material.specular);
   object_shader.setUniformF("material.shininess", material.shininess); 
   object_shader.setUniformF("spot_light.cut_off", glm::cos(glm::radians(100.5f)));
   object_shader.setUniformF("spot_light.outer_cut_off", glm::cos(glm::radians(200.0f)));


    object_shader.setUniformV3("dir_light.ambient",  glm::vec3(0.1f));
    object_shader.setUniformV3("dir_light.diffuse",  glm::vec3(0.1f)); // darken diffuse light a bit
    object_shader.setUniformV3("dir_light.specular", glm::vec3(0.1f)); 
    object_shader.setUniformV3("dir_light.direction", glm::vec3(0.0f, 1.0f, 1.0f)); 
    
    object_shader.setUniformV3("spot_light.ambient",  glm::vec3(0.1f));
    object_shader.setUniformV3("spot_light.diffuse",  glm::vec3(1.0f)); // darken diffuse light a bit
    object_shader.setUniformV3("spot_light.specular", glm::vec3(1.0f)); 

    std::stringstream s ("");
    for(int i = 0; i < POINT_LIGHT_COUNT; i ++){
        s << "point_lights" << "[" << i << "]";
        object_shader.setUniformV3(s.str() + ".ambient",  glm::vec3(0.1f));
        object_shader.setUniformV3(s.str() + ".diffuse",  diffuses[i]); // darken diffuse light a bit
        object_shader.setUniformV3(s.str() + ".specular", diffuses[i]); 
        object_shader.setUniformV3(s.str() + ".position", light_positions[i]);
        object_shader.setUniformF(s.str() + ".constant", 1.0f);
        object_shader.setUniformF(s.str() + ".linear", .09f);
        object_shader.setUniformF(s.str() + ".quadratic", .032f);
        s.str("");
        s.clear();
    }
    
    light_shader.use();
    
    light_shader.setUniformM4("projection", proj);
    light_shader.setUniformM4("view", view);
    light_shader.setUniformM4("model",model);
    light_shader.setUniformV3("aColor",glm::vec3(0.0f,1.0f,0.0f));
    return LightingEnvironment{model,view,proj,object_shader,light_shader};
}
void setupDummyMPVEnvironmentToDraw(LightingEnvironment& lightingEnvironment){
    lightingEnvironment.object_shader.use();
    lightingEnvironment.view = glm::mat4(1.0f);
    lightingEnvironment.projection = glm::mat4(1.0f);
    lightingEnvironment.object_shader.setUniformM4("view",lightingEnvironment.view);
    lightingEnvironment.object_shader.setUniformM4("projection",lightingEnvironment.projection);

}
void setupMPVEnvironmentToDraw(LightingEnvironment& lightingEnvironment){

    lightingEnvironment.projection = glm::perspective(glm::radians(camera.Zoom), (float)mWidth / (float)mHeight, 0.1f, 100000.0f);
    lightingEnvironment.view = camera.GetViewMatrix();

    lightingEnvironment.object_shader.use();
    lightingEnvironment.object_shader.setUniformM4("view",lightingEnvironment.view);
    lightingEnvironment.object_shader.setUniformM4("projection",lightingEnvironment.projection);
}
void setupLightingEnvironmentToDraw(LightingEnvironment& lightingEnvironment, glm::vec3 light_positions[POINT_LIGHT_COUNT], glm::vec3 diffuses[POINT_LIGHT_COUNT], OpenGLShape& lightShape, bool drawLights){

    lightingEnvironment.projection = glm::perspective(glm::radians(camera.Zoom), (float)mWidth / (float)mHeight, 0.1f, 100.0f);
    lightingEnvironment.view = camera.GetViewMatrix();
    
    
    

    lightingEnvironment.light_shader.use();
    lightingEnvironment.light_shader.setUniformM4("projection",lightingEnvironment.projection);
    lightingEnvironment.light_shader.setUniformM4("view",lightingEnvironment.view);

    if (drawLights){
        for(int i = 0; i < POINT_LIGHT_COUNT; i++){
            lightingEnvironment.model = glm::mat4(1.0f);
            lightingEnvironment.model = glm::translate(lightingEnvironment.model, light_positions[i]);
            lightingEnvironment.model = glm::scale(lightingEnvironment.model, glm::vec3(0.2f));
            lightingEnvironment.light_shader.setUniformM4("model", lightingEnvironment.model);
            lightingEnvironment.light_shader.setUniformV3("aColor", diffuses[i]);
            lightShape.draw(lightingEnvironment.light_shader);
        }
    }

    lightingEnvironment.object_shader.use();
    lightingEnvironment.object_shader.setUniformM4("view",lightingEnvironment.view);
    lightingEnvironment.object_shader.setUniformM4("projection",lightingEnvironment.projection);
    lightingEnvironment.object_shader.setUniformV3("view_pos", camera.Position);
   lightingEnvironment.object_shader.setUniformV3("spot_light.position", camera.Position);
   lightingEnvironment.object_shader.setUniformV3("spot_light.direction", camera.Front);
   lightingEnvironment.object_shader.setUniformB("spot_light.is_on", (bool)flash_light);
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
void OpenGLShape::draw(Shader shader)
{
    if(hasTextures)
        drawMultipleTexturedShape(VAO, EBO, shader, ind_cnt, textures);
    else
        drawShape(VAO, EBO, shader, ind_cnt);
}

void OpenGLShape::draw(Shader shader, std::vector<unsigned int> textures)
{
    drawMultipleTexturedShape(VAO, EBO, shader, ind_cnt, textures);
}

bool cmpShape(const ShapeProperties& a, const ShapeProperties& b){
    return a.coords.z < b.coords.z;
}

int mainCoords(int argc, char * argv[]){
    // Initialize OpenGL
    int return_status;
    GLFWwindow*  mWindow; 
    if((return_status = initOpenGL(mWindow, mWidth, mHeight)) != EXIT_SUCCESS) return return_status;
    
    DeltaTimer deltaTimer{};
    Alarm alarm(SHAPE_SPAWN_TIME);
    bool shapesFull = false;
    
    unsigned int diffuse_sampler_2d;
    unsigned int specular_sampler_2d;
    generateSampler2Ds(diffuse_sampler_2d, specular_sampler_2d);


    
    
    std::vector<unsigned int> lightSizes{};
    lightSizes.push_back(3);
    OpenGLShape lightShape((unsigned int) 36, (unsigned int)36, shapes::cube, shapes::cube_ind, lightSizes, (unsigned int)0, false);
    std::vector<unsigned int> objectSizes{3,3,2};
    OpenGLShape cubeShape((unsigned int) 36, (unsigned int)36, shapes::normal_textured_cube, shapes::cube_ind, objectSizes, diffuse_sampler_2d, true);
    OpenGLShape pyramidShape((unsigned int) 12, (unsigned int)12, shapes::normal_textured_triangle_pyramid, shapes::triangle_pyramid_ind, objectSizes, diffuse_sampler_2d, true);
    OpenGLShape octahedronShape((unsigned int) 24, (unsigned int)24, shapes::octahedron, shapes::octahedron_ind, objectSizes, diffuse_sampler_2d, false);
    
    
    std::map<SHAPE_TYPE_3D, OpenGLShape*> shapeTypes{{CUBE , &cubeShape}, {PYRAMID, &pyramidShape}, {OCTAHEDRON, &octahedronShape}};
    
    
    glm::vec3 light_positions[POINT_LIGHT_COUNT] = {
        glm::vec3(4.0f,0.0f,0.0f),
        glm::vec3(0.0f,0.0f,4.0f),
        glm::vec3(0.0f,0.0f,-4.0f),
        glm::vec3(-4.0f,0.0f,0.0f),
    };

    glm::vec3 diffuses[POINT_LIGHT_COUNT] = {
        glm::vec3(0.5f,0.5f,0.5f),
        glm::vec3(0.5f,0.5f,0.5f),
        glm::vec3(0.5f,0.5f,0.5f),
        glm::vec3(0.5f,0.5f,0.5f),
    };
    
    LightingEnvironment lightingEnvironment = initLighting(light_positions, diffuses);

    lightingEnvironment.object_shader.use();
    lightingEnvironment.object_shader.setUniformF("SHAPE_SPAWN_TIME", (float) SHAPE_SPAWN_TIME);
    lightingEnvironment.object_shader.setUniformF("MAXIMUM_BATCHES", (float) TOTAL_LAYERS);
    lightingEnvironment.object_shader.setUniformF("SHAPE_MAX_FADE", (float) SHAPE_MAX_FADE);
    glm::mat4 model(1.0f);
    
    
    
    
    initMouse(mWindow);


    
    std::deque<ShapeProperties> cubeMatrices{};
    
    std::vector<ShapeProperties> frontToBack{};


    
    
    // Rendering Loop
    while (!glfwWindowShouldClose(mWindow)) {
        bool cubesChanged = false;
        alarm.checkAndUpdate();

        deltaTimer.updateDeltaTime();
        // Background Fill Color
        glClearColor(0.50f, 0.50f, 0.50f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        setupLightingEnvironmentToDraw(lightingEnvironment, light_positions, diffuses, lightShape, false);



        std::vector<std::vector<float>> emotionArray = readEmotionArray("../data.json");
        
        // Use emotion array to set properties of shapes
        for(unsigned int i = 0; i < TOTAL_EMOTIONS; i++){
            std::vector<float> array = emotionArray[i];
            glm::vec3 emotionRGB = emotionArrayToColor(array);
            for (int j = 0; j < SHAPES_IN_BATCH; j++){

                glm::vec3 coords = randCoords();
                float maxScale = 10.0f;
                glm::vec3 rotAxis = randCoords();
                float rotScale = rand_range_uniform(0.0f, 1.0f);


                // glm::vec3 scale = randScale(1.0f,maxScale);
                glm::vec3 scale = rand_range_uniform(1.0f, maxScale)*glm::vec3(1.0f);
                unsigned int typeNum = rand_range_uniform((unsigned) 0, (unsigned) 2);
                SHAPE_TYPE_3D type;
                switch (typeNum){
                    case 0:
                        type = CUBE;
                        break;
                    case 1:
                        type = PYRAMID;
                        break;
                    case 2:
                        type = OCTAHEDRON;
                        break;
                }

                
                model = glm::mat4(1.0f);
                float xCoordScale = 50.0f;
                float yCoordScale = 50.0f;
                glm::vec3 scaledCoordinates(1.0f);
                float zCoordScale = 50.0f;
                float afterZScale = 4.0f;
                float zCoordOffset = -6.0f; //- 8.0f*(scale.x*scale.y*scale.z)/(glm::pow(maxScale,2.0f));
                scaledCoordinates.x = coords.x*xCoordScale;
                scaledCoordinates.y = coords.y*yCoordScale;
                scaledCoordinates.z = coords.z*zCoordScale;
                scaledCoordinates.z += -zCoordScale+zCoordOffset-maxScale;
                // model = glm::rotate(model, rotScale, rotAxis);
                model = glm::translate(model, scaledCoordinates);
                model = glm::scale(model, scale);
                

                
                // Create and delete cubes
                if(alarm.check()){
                        cubesChanged = true;
                        ShapeProperties shape = ShapeProperties{model,glm::vec4(emotionRGB,1.0f),type,getElapsedTime(),0.0f, scaledCoordinates};
                        cubeMatrices.push_front(shape);
                    if(!shapesFull) {
                        shapesFull = true;
                        alarm.setWaitTime(SHAPE_SPAWN_TIME);
                    }
                }

            }
        }
        // Delete cubes
        while (cubeMatrices.size() > TOTAL_LAYERS*SHAPES_IN_BATCH*TOTAL_EMOTIONS){
            cubeMatrices.pop_back();
        }

        if (cubesChanged){
            frontToBack = {};
            for (ShapeProperties shape : cubeMatrices){
                frontToBack.push_back(shape);
            }
            std::sort(frontToBack.begin(),frontToBack.end(),compareShapeForwardDistance);


        }
        // Draw cubes
        drawCubeMatrices(lightingEnvironment, frontToBack, shapeTypes, deltaTimer);        



        endRenderLoop(mWindow,deltaTimer,camera);

        
    }   glfwTerminate();
    return EXIT_SUCCESS;
    
    
}

glm::vec3 randScale(float min, float max){
    glm::vec3 coords(0.0f);
    for (unsigned int i = 0; i < 3; i++){
        coords[i] = rand_range_uniform(min,max);
    }
    return coords;
}
glm::vec3 randCoords(){
    glm::vec3 coords(0.0f);
    for (unsigned int i = 0; i < 3; i++){
        coords[i] = rand_range_uniform(-1.0f,1.0f);
    }
    return coords;
}

Alarm::Alarm(float waitTime) : waitTime(waitTime), prevTimesSounded(0), alarmSounded(false)
{
    startTime = getElapsedTime();
}

Alarm::Alarm(float waitTime, float startTime) : waitTime(waitTime), startTime(startTime), prevTimesSounded(0), alarmSounded(false)
{
}


void Alarm::setWaitTime(float waitTime)
{
    this->waitTime = waitTime;
}

unsigned int Alarm::sync(unsigned int timesSounded)
{
    unsigned int prevTimesSounded = getPrevTimesSounded();
    this->prevTimesSounded = timesSounded;
    return timesSounded - prevTimesSounded;
}

bool Alarm::checkAndUpdate()
{
    int timesSounded = getTimesSounded();
    int prevTimesSounded = getPrevTimesSounded();
    assert(prevTimesSounded <= timesSounded);
    if(prevTimesSounded < timesSounded){
        alarmSounded = true;
        prevTimesSounded = timesSounded;
    }
    else{
        alarmSounded = false;
    }
    return alarmSounded;
}


std::vector<float> vec3ToVector(glm::vec3 a){
    return std::vector<float>{a.x,a.y,a.z};
}

glm::vec3 vectorToVec3(std::vector<float> a){
    return glm::vec3(a[0],a[1],a[2]);
}


glm::vec3 getViewCoordinates(const glm::vec3 coords){
    glm::vec3 f = camera.Front;
    glm::vec3 r = camera.Right;
    glm::vec3 u = camera.Up;
    glm::vec3 p = camera.Position;
    std::vector<std::vector<float>> coefficients {
        std::vector{ r.x, u.x, f.x,},
        std::vector{ r.y, u.y, f.y,},
        std::vector{ r.z, u.z, f.z,},
    };
    std::vector<float> constants = vec3ToVector(coords - p);
    return vectorToVec3(solve(coefficients, constants));
}

float forwardDistance(const glm::vec3& coords){

    glm::vec3 f = camera.Front;
    glm::vec3 r = camera.Right;
    glm::vec3 u = camera.Up;
    glm::vec3 p = camera.Position;
    glm::vec3 o = coords - p;

    return -(o.x*r.y*u.z - o.x*r.z*u.y - o.y*r.x*u.z + o.y*r.z*u.x + o.z*r.x*u.y - o.z*r.y*u.x)/(f.x*r.y*u.z - f.x*r.z*u.y - f.y*r.x*u.z + f.y*r.z*u.x + f.z*r.x*u.y - f.z*r.y*u.x);
}

bool compareZDistance(const ShapeProperties& a, const ShapeProperties& b){
    return a.coords.z - camera.Position.z < b.coords.z - camera.Position.z;
}

bool compareVec3ForwardDistance(const glm::vec3& a, const glm::vec3& b){
    return getViewCoordinates(a).z < getViewCoordinates(b).z;
}

bool compareShapeForwardDistance(const ShapeProperties& a, const ShapeProperties& b){
    return forwardDistance(a.coords) < forwardDistance(b.coords);
}

void drawCubeMatrices2DScale(LightingEnvironment& lightingEnvironment, std::deque<ShapeLayer>& cubeMatrices, std::map<SHAPE_TYPE_2D, OpenGLShape*> shapes,  DeltaTimer deltaTimer, Alarm destructionAlarm, std::mutex& lock, std::tuple<float, float> lineAbove, std::tuple<float, float> lineBelow){

    glDepthMask(false);
    float passedTime;
    float waitTime;
    {
        passedTime = destructionAlarm.getPassedTime();
        waitTime = destructionAlarm.getWaitTime();
    }
    lightingEnvironment.object_shader.use();
    lightingEnvironment.object_shader.setUniformF("destructionTime", waitTime);
    lightingEnvironment.object_shader.setUniformF("destructionTimePassed", passedTime);
    unsigned layerCount = 0;
    lightingEnvironment.object_shader.setUniformI("maxLayer", TOTAL_LAYERS);
    // Draw cubes
    const float speed = 4.0f;
    for(auto const& shapeLayer : cubeMatrices){
        unsigned int layer = TOTAL_LAYERS - 1 - layerCount;
        unsigned int batchCount = 0;
        for(auto const& shapeBatch : shapeLayer){
            for(auto const& shape : shapeBatch.shapes){
                glm::mat4 model(1.0f);
                glm::mat4 transition(shape.transition);
                glm::mat4 rotation(shape.rotationMatrix);
                glm::mat4 scale(shape.scaleMatrix);
                lightingEnvironment.object_shader.setUniformM4("model", shape.model);
                //lightingEnvironment.object_shader.setUniformB("DO_FADE", (DO_FADE && shape.transitions[FADE]));
                lightingEnvironment.object_shader.setUniformB("DO_FADE", shape.transitions.at(FADE));
                lightingEnvironment.object_shader.setUniformI("borderCase", shape.borderCase);

                lightingEnvironment.object_shader.setUniformI("layer", layer);
                




                if(shape.transitions.at(BACK_OUT)){
                    float dX = calcDisplacement(passedTime,waitTime,TOTAL_LAYERS,MID_LAYER,layer,SHAPE_MIN_FADE,SHAPE_MAX_FADE, true, lineAbove, lineBelow);
                    transition = glm::mat4(1.0f);
                    transition = glm::translate(transition, 2.0f*shape.direction - shape.rotatedCoords);
                    transition = glm::translate(transition, -speed*dX*shape.direction);
                }
                else if(shape.transitions.at(MOVE_FORWARD)){
                    float dX = calcDisplacement(passedTime,waitTime,TOTAL_LAYERS,MID_LAYER,layer,SHAPE_MIN_FADE,SHAPE_MAX_FADE, true, lineAbove, lineBelow);
                    transition = glm::mat4(1.0f);
                    transition = glm::translate(transition, -2.0f*shape.direction - shape.rotatedCoords);
                    transition = glm::translate(transition, speed*dX*shape.direction);
                }
                
                if(shape.transitions.at(SCALE)){
                    float scaleDown = calcFade(passedTime,waitTime,TOTAL_LAYERS,MID_LAYER,layer,SHAPE_MIN_FADE,SHAPE_MAX_FADE, false, lineAbove, lineBelow);
                    scale = glm::scale(scale, glm::vec3(glm::vec2(1.0f - scaleDown),1.0f));
                }
                
                
                assert(layer < TOTAL_LAYERS);
                
                
                //model = scale*rotation*transition;
                model = transition*rotation*scale;


                lightingEnvironment.object_shader.setUniformM4("model", model);
                lightingEnvironment.object_shader.setUniformV4("INNER_COLOR", shape.innerColor);
                lightingEnvironment.object_shader.setUniformV4("OUTER_COLOR", shape.outerColor);
                lightingEnvironment.object_shader.setUniformF("BORDER_THICKNESS", shape.borderThickness);


                shapes[shape.type]->draw(lightingEnvironment.object_shader, {shapeBatch.texture, shapes[shape.type]->getTexture(1)});
            }
            batchCount++;
        }
        layerCount++;
    }
    glDepthMask(true);
}
void drawCubeMatrices(LightingEnvironment& lightingEnvironment, std::vector<ShapeProperties> cubeMatrices, std::map<SHAPE_TYPE_3D, OpenGLShape*> shapes,  DeltaTimer deltaTimer){

    lightingEnvironment.object_shader.setUniformF("elapsedTime", (float)getElapsedTime());
    // Draw cubes
    glDepthMask(false);
    for( ShapeProperties shape : cubeMatrices){
        lightingEnvironment.object_shader.setUniformF("creationDate", (float)shape.creationTimeStamp);
        lightingEnvironment.object_shader.setUniformM4("model", shape.model);
        lightingEnvironment.object_shader.setUniformV4("aColor", shape.color);
        shapes[shape.type]->draw(lightingEnvironment.object_shader);
    }
    glDepthMask(true);
}
void endRenderLoop(GLFWwindow*& mWindow, DeltaTimer deltaTimer, Camera& camera){
    static int loopCount = 1;
    static std::vector<float> deltaTimes{};
    // Flip Buffers and Draw
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
    processInput(mWindow, camera, deltaTimer.getDeltaTime());
    deltaTimes.push_back(deltaTimer.getDeltaTime());
    if(loopCount >= 60){
        float sum = 0.0f;
        for(float time : deltaTimes){
            sum+=time;
        }
        std::cout << "FPS: " << ((float)loopCount)/((float)sum) << std::endl;
        deltaTimes.clear();
        loopCount = 1;
    }
    else {
        loopCount++;
    }
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

int mainCoordsBasic(int argc, char * argv[]){
    // Initialize OpenGL
    int return_status;
    GLFWwindow*  mWindow; 
    if((return_status = initOpenGL(mWindow, mWidth, mHeight)) != EXIT_SUCCESS) return return_status;
    
    DeltaTimer deltaTimer{};
    Alarm alarm(SHAPE_SPAWN_TIME);
    bool shapesFull = false;
    
    unsigned int diffuse_sampler_2d;
    //create_texture(diffuse_sampler_2d, "Glitter/Textures/container2.png", "png");
    generate_texture(diffuse_sampler_2d, "Glitter/Textures/container2.png", "png");


    
    
    std::vector<unsigned int> objectSizes{3, 2};
    OpenGLShape cubeShape((unsigned int) 36, (unsigned int)36, shapes::textured_cube, shapes::cube_ind, objectSizes, diffuse_sampler_2d, true);
    OpenGLShape pyramidShape((unsigned int) 12, (unsigned int)12, shapes::textured_triangle_pyramid, shapes::triangle_pyramid_ind, objectSizes, diffuse_sampler_2d, true);
    OpenGLShape octahedronShape((unsigned int) 24, (unsigned int)24, shapes::textured_octahedron, shapes::octahedron_ind, objectSizes, diffuse_sampler_2d, true);
    
    std::map<SHAPE_TYPE_3D, OpenGLShape*> shapeTypes{{CUBE , &cubeShape}, {PYRAMID, &pyramidShape}, {OCTAHEDRON, &octahedronShape}};
    
    
    
    
    
    
    LightingEnvironment lightingEnvironment = initMPV("Glitter/Shaders/texture-mvp-color.vs", "Glitter/Shaders/texture-mvp-color.fs");

    lightingEnvironment.object_shader.use();
    lightingEnvironment.object_shader.setUniformF("SHAPE_SPAWN_TIME", (float) SHAPE_SPAWN_TIME);
    lightingEnvironment.object_shader.setUniformF("MAXIMUM_BATCHES", (float) TOTAL_LAYERS);
    lightingEnvironment.object_shader.setUniformF("SHAPE_MAX_FADE", (float) SHAPE_MAX_FADE);
    lightingEnvironment.object_shader.setUniformI("ourTexture1", (unsigned) 0);
    glm::mat4 model(1.0f);
    
    
    
    
    initMouse(mWindow);


    
    std::deque<ShapeProperties> cubeMatrices{};
    
    std::vector<ShapeProperties> frontToBack{};

    Alarm sortAlarm(SORT_TIME);

    
    
    // Rendering Loop
    while (!glfwWindowShouldClose(mWindow)) {
        bool cubesChanged = false;
        alarm.checkAndUpdate();
        sortAlarm.checkAndUpdate();

        deltaTimer.updateDeltaTime();
        // Background Fill Color
        glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        setupMPVEnvironmentToDraw(lightingEnvironment);



        std::vector<std::vector<float>> emotionArray = readEmotionArray("../data.json");
        
        // Use emotion array to set properties of shapes
        for(int i = 0; i < emotionArray.size(); i++){
            std::vector<float> array = emotionArray[i];
            glm::vec3 emotionRGB = emotionArrayToColor(array);
            glm::vec3 coords = emotionArrayToVec3(array);
            glm::vec3 rotAxis = emotionArrayToVec3(array);
            float rotScale = rand_range_uniform(0.0f, 1.0f);
            SHAPE_TYPE_3D type = emotionArrayToShapeType3D(array);
            for (int j = 0; j < SHAPES_IN_BATCH; j++){

                float maxScale = 2.0f;
                coords += 0.1f*randCoords();
                glm::vec3 scale = rand_range_uniform(1.0f,maxScale) * glm::vec3(1.0f);
                scale = maxScale * glm::vec3(1.0f);


                
                model = glm::mat4(1.0f);
                float xCoordScale = 50.0f;
                float yCoordScale = 50.0f;
                glm::vec3 scaledCoordinates(1.0f);
                float zCoordScale = 50.0f;
                float afterZScale = 4.0f;
                float zCoordOffset = -6.0f; //- 8.0f*(scale.x*scale.y*scale.z)/(glm::pow(maxScale,2.0f));
                scaledCoordinates.x = coords.x*xCoordScale;
                scaledCoordinates.y = coords.y*yCoordScale;
                scaledCoordinates.z = coords.z*zCoordScale;
                scaledCoordinates.z += -zCoordScale+zCoordOffset-maxScale;
                model = glm::translate(model, scaledCoordinates);
                model = glm::scale(model, scale);
                model = glm::rotate(model, rotScale, rotAxis);
                

                
                // Create and delete cubes
                if(alarm.check()){
                        cubesChanged = true;
                        ShapeProperties shape = ShapeProperties{model,glm::vec4(emotionRGB,1.0f),type,getElapsedTime(),0.0f, scaledCoordinates};
                        cubeMatrices.push_front(shape);
                        frontToBack.push_back(shape);
                    if(!shapesFull) {
                        shapesFull = true;
                        alarm.setWaitTime(SHAPE_SPAWN_TIME);
                    }
                }

            }
        }
        // Delete cubes
        while (cubeMatrices.size() > TOTAL_LAYERS*SHAPES_IN_BATCH*TOTAL_EMOTIONS){
            cubeMatrices.pop_back();
        }

        if (cubesChanged){
            frontToBack = {};
            for (ShapeProperties shape : cubeMatrices){
                frontToBack.push_back(shape);
            }
            std::sort(frontToBack.begin(),frontToBack.end(),compareShapeForwardDistance);
        }
        // Draw cubes
        drawCubeMatrices(lightingEnvironment, frontToBack, shapeTypes, deltaTimer);        



        endRenderLoop(mWindow,deltaTimer,camera);

        
    }   glfwTerminate();
    return EXIT_SUCCESS;
    
    
}
int mainCoordsBasicStream(int argc, char * argv[]){
    // Initialize OpenGL
    int return_status;
    GLFWwindow*  mWindow; 
    if((return_status = initOpenGL(mWindow, mWidth, mHeight)) != EXIT_SUCCESS) return return_status;
    
    DeltaTimer deltaTimer{};
    Alarm alarm(SHAPE_SPAWN_TIME);
    bool shapesFull = false;
    
    unsigned int diffuse_sampler_2d;
    //create_texture(diffuse_sampler_2d, "Glitter/Textures/container2.png", "png");
    generate_texture(diffuse_sampler_2d, "Glitter/Textures/container2.png", "png");


    
    
    std::vector<unsigned int> objectSizes{3, 2};
    OpenGLShape cubeShape((unsigned int) 36, (unsigned int)36, shapes::textured_cube, shapes::cube_ind, objectSizes, diffuse_sampler_2d, true);
    OpenGLShape pyramidShape((unsigned int) 12, (unsigned int)12, shapes::textured_triangle_pyramid, shapes::triangle_pyramid_ind, objectSizes, diffuse_sampler_2d, true);
    OpenGLShape octahedronShape((unsigned int) 24, (unsigned int)24, shapes::textured_octahedron, shapes::octahedron_ind, objectSizes, diffuse_sampler_2d, true);
    
    std::map<SHAPE_TYPE_3D, OpenGLShape*> shapeTypes{{CUBE , &cubeShape}, {PYRAMID, &pyramidShape}, {OCTAHEDRON, &octahedronShape}};
    
    
    
    
    
    
    LightingEnvironment lightingEnvironment = initMPV("Glitter/Shaders/texture-mvp-color.vs", "Glitter/Shaders/texture-mvp-color.fs");

    lightingEnvironment.object_shader.use();
    lightingEnvironment.object_shader.setUniformF("SHAPE_SPAWN_TIME", (float) SHAPE_SPAWN_TIME);
    lightingEnvironment.object_shader.setUniformF("MAXIMUM_BATCHES", (float) TOTAL_LAYERS);
    lightingEnvironment.object_shader.setUniformF("SHAPE_MAX_FADE", (float) SHAPE_MAX_FADE);
    lightingEnvironment.object_shader.setUniformI("ourTexture1", (unsigned) 0);
    glm::mat4 model(1.0f);
    
    
    
    
    initMouse(mWindow);


    
    std::deque<ShapeProperties> cubeMatrices{};
    
    std::vector<ShapeProperties> frontToBack{};


    
    
    // Rendering Loop
    while (!glfwWindowShouldClose(mWindow)) {
        bool cubesChanged = false;
        alarm.checkAndUpdate();

        deltaTimer.updateDeltaTime();
        // Background Fill Color
        glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        setupMPVEnvironmentToDraw(lightingEnvironment);



        std::vector<std::vector<float>> emotionArray = readEmotionArray("../data.json");
        
        // Use emotion array to set properties of shapes
        for(int i = 0; i < emotionArray.size(); i++){
            std::vector<float> array = emotionArray[i];
            glm::vec3 emotionRGB = emotionArrayToColor(array);
            glm::vec3 coords = emotionArrayToVec3(array);
            glm::vec3 rotAxis = emotionArrayToVec3(array);
            for (int j = 0; j < SHAPES_IN_BATCH; j++){

                float maxScale = 10.0f;
                float rotScale = rand_range_uniform(0.0f, 1.0f);


                // glm::vec3 scale = randScale(1.0f,maxScale);
                //scale = glm::vec3(1.0f);
                unsigned int typeNum = rand_range_uniform((unsigned) 0, (unsigned) 2);
                SHAPE_TYPE_3D type;
                switch (typeNum){
                    case 0:
                        type = CUBE;
                        break;
                    case 1:
                        type = PYRAMID;
                        break;
                    case 2:
                        type = OCTAHEDRON;
                        break;
                }

                
                model = glm::mat4(1.0f);
                float xCoordScale = 50.0f;
                float yCoordScale = 50.0f;
                glm::vec3 scaledCoordinates(1.0f);
                float zCoordScale = 50.0f;
                float afterZScale = 4.0f;
                float zCoordOffset = -6.0f; //- 8.0f*(scale.x*scale.y*scale.z)/(glm::pow(maxScale,2.0f));
                scaledCoordinates.x = coords.x*xCoordScale;
                scaledCoordinates.y = coords.y*yCoordScale;
                scaledCoordinates.z = coords.z*zCoordScale;
                scaledCoordinates.z += -zCoordScale+zCoordOffset-maxScale;
                model = glm::rotate(model, rotScale, rotAxis);
                model = glm::translate(model, scaledCoordinates);
                

                
                // Create and delete cubes
                if(alarm.check()){
                        cubesChanged = true;
                        ShapeProperties shape = ShapeProperties{model,glm::vec4(emotionRGB,1.0f),type,getElapsedTime(),0.0f, scaledCoordinates};
                        cubeMatrices.push_front(shape);
                    if(!shapesFull) {
                        shapesFull = true;
                        alarm.setWaitTime(SHAPE_SPAWN_TIME);
                    }
                }

            }
        }
        // Delete cubes
        while (cubeMatrices.size() > TOTAL_LAYERS*SHAPES_IN_BATCH*TOTAL_EMOTIONS){
            cubeMatrices.pop_back();
        }

        if (cubesChanged || true){
            frontToBack = {};
            for (ShapeProperties shape : cubeMatrices){
                frontToBack.push_back(shape);
            }
            std::sort(frontToBack.begin(),frontToBack.end(),compareShapeForwardDistance);
        }
        // Draw cubes
        drawCubeMatrices(lightingEnvironment, frontToBack, shapeTypes, deltaTimer);        



        endRenderLoop(mWindow,deltaTimer,camera);

        
    }   glfwTerminate();
    return EXIT_SUCCESS;
    
}


<<<<<<< Updated upstream
void LayerGenerator(std::deque<ShapeProperties2D>* cubeBuffer, Alarm* creationAlarm, GLFWwindow* mWindow, std::mutex* lock, bool* notDone, int* additions){
=======
void something(std::deque<ShapeLayer>* shapeLayersBuffer, Alarm* creationAlarm,  std::mutex* lock, bool* notDone, int* additions){
>>>>>>> Stashed changes

    std::deque<EmotionLayer> emotionArrays{};
    bool alarmSounded;
    Alarm printAlarm{1.0f};
    long int iterations = 0;
    PngData blankPNGData(255,255, 3);
    generatePNGBlank(blankPNGData);
    std::deque<EmotionLayer> emotionLayers{};
    for(int i = 0; i < TOTAL_EMOTIONS; i++)
        emotionLayers.push_front({std::vector<float>{1,1,1,1,1,1},blankPNGData});
    float timePassed;

    float waitTime;
    {
        std::lock_guard guard(*lock);
        waitTime = creationAlarm->getWaitTime();
    }
    float imageWaitTime = waitTime;
	std::deque<PngData> pngDataBuffer{};
	std::deque<PngData> pngDataStorage{};
	bool turnOff = false;
	std::mutex lockImage{};
    std::string filepath = "../background.png";
	std::thread imageThread(pngLoader, &pngDataBuffer, &turnOff, &lockImage, &filepath, &imageWaitTime);

    while(*notDone){
        bool printAlarmSounded = printAlarm.checkAndUpdate();
        if(printAlarmSounded){
            std::cout << "iterations per second: " << iterations << std::endl;
            iterations = 0;
        }
        {
            std::lock_guard<std::mutex> guard(*lock);
            alarmSounded = true;
            creationAlarm->checkAndUpdate();
            timePassed = creationAlarm->getPassedTime();
            waitTime = creationAlarm->getWaitTime();
        }
        {
            std::lock_guard<std::mutex> guard(lockImage);
            imageWaitTime = waitTime;
        }
        {
            while(pngDataStorage.size() < TOTAL_EMOTIONS){
                std::this_thread::sleep_for(std::chrono::nanoseconds((int64_t)((100.0f))));
                {
                    std::lock_guard<std::mutex> guard(lockImage);
                    while(!pngDataBuffer.empty()){
                        pngDataStorage.push_front(pngDataBuffer.back());
                        pngDataBuffer.pop_back();
                    }
                }
            }
        }
        emotionLayers.push_front({readEmotionArray("../data.json",false)[0],pngDataStorage.back()});
        emotionLayers.pop_back();
        pngDataStorage.pop_back();

        
        bool useBenTexture = rand_range_uniform(0.0f,1.0f) < 0.50f;
        
        // #Emotion Extraction
        // Use emotion array to set properties of shapes
        if(alarmSounded){
            ShapeLayer shapeLayer{};
            for(auto const& emotionLayer : emotionLayers){
                auto const& emotionArray = emotionLayer.emotionArray;

                glm::vec3 emotionRGB;

                emotionRGB = emotionArrayToColor(emotionArray);
                glm::vec2 emotionCoords2D = emotionArrayToVec2(emotionArray);
                glm::vec3 emotionCoords3D = glm::vec3(emotionCoords2D.x, emotionCoords2D.y, 0.0f);
                glm::vec3 coords;
                glm::vec3 rotAxis = glm::vec3(0.0f,0.0f,-1.0f);
                float pi = std::numbers::pi;
                float rotScale = 2*pi*emotionArrayToRotation(emotionArray);
                if(rotScale < pi/4.0f) rotScale = 0.0f;
                else if(rotScale < pi/2.0f) rotScale = pi/4.0f;
                else if(rotScale < 3.0f*pi/4.0f) rotScale = pi/2.0f;
                else if(rotScale < pi) rotScale = 3.0f*pi/4.0f;
                else if(rotScale < pi + pi/4.0f) rotScale = pi;
                else if(rotScale < pi + pi/2.0f) rotScale = pi + pi/4.0f;
                else if(rotScale < pi + 3*pi/4.0f) rotScale = pi + pi/2.0f;
                else if(rotScale < 2*pi) rotScale = pi + 3.0f*pi/4.0f;

                SHAPE_TYPE_2D type = emotionArrayToShapeType2D(emotionArray);
                auto transitions = emotionArrayToTransitions(emotionArray);
                const auto [borderCase, softnessOrThickness] = emotionArrayToSoftness(emotionArray,false, true, false);
                transitions[VORTEX] = rand_range_uniform(0.0f, 1.0f) <= 0.1f;
                //const auto emotionAlpha = emotionArrayToAlpha(emotionArray);

                float alphaScale = 1.0f;
                alphaScale = clamp(rand_range_normal(AVE_ALPHA, DEV_ALPHA), MIN_ALPHA, MAX_ALPHA);
                //alphaScale = (1.0f - 0.0)*alphaScale + (0.0)*emotionAlpha;

                ShapeBatch shapeBatch{std::deque<ShapeProperties2D>{}, emotionLayer.pngData, 0};

                for (int j = 0; j < SHAPES_IN_BATCH; j++){
                    float maxScale = MAX_SCALE;
                    float minScale = MIN_SCALE;
                    float aveScale = AVE_SCALE;
                    float devScale = DEV_SCALE;
                    glm::vec3 scale = clamp(rand_range_normal(aveScale,devScale), minScale, maxScale) * glm::vec3(1.0f);
                    if(transitions[SCALE] && transitions[FADE]) scale *= DO_SCALE_SCALE;
                    else if(transitions[SCALE]) scale *= DO_SCALE_SCALE;
                    else if (transitions[FADE]) scale *= DO_FADE_SCALE;
                    
                    if(transitions[VORTEX]) scale *= DO_VORTEX_SCALE;

                    
                    coords = SCATTER_TO_EMOTION_COORD_RATIO* SCATTER_COORD_SCALE * randCoords()+(1-SCATTER_TO_EMOTION_COORD_RATIO)*EMOTION_COORD_SCALE*emotionCoords3D;
                    coords.z = 0.0f;


                    
                    glm::mat4 model = glm::mat4(1.0f);
                    float xCoordScale = 1.0f;
                    float yCoordScale = 1.0f;
                    glm::vec3 scaledCoordinates(1.0f);
                    scaledCoordinates.x = coords.x*xCoordScale;
                    scaledCoordinates.y = coords.y*yCoordScale;
                    scaledCoordinates.z = 0.0f;
                    model = glm::translate(model, scaledCoordinates);
                    model = glm::scale(model, scale);
                    model = glm::rotate(model, rotScale, rotAxis);
                    

                    
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
                    ShapeProperties2D shape = ShapeProperties2D{model,shapeColor,invertedShapeColor, type, scaledCoordinates, rotScale, scale, softnessOrThickness, transitions, borderCase, 0, useBenTexture};
                    shape.scaleMatrix = glm::scale(glm::mat4(1.0f), shape.scale);
                    shape.rotationMatrix = glm::rotate(glm::mat4(1.0f), shape.rotation, glm::vec3(0.0f,0.0f,-1.0f));
                    shape.transition = glm::translate(glm::mat4(1.0f), shape.coords);
                    shape.direction = dir::UP;
                    shape.direction  = glm::mat3(shape.rotationMatrix) * shape.direction;
                    shape.direction = glm::normalize(shape.direction);
                    shape.rotatedCoords = glm::mat3(shape.rotationMatrix) * glm::vec3(shape.coords.x, 0.0f, 0.0f);
                    shapeBatch.shapes.push_front(shape);
                    additions++;
                }
                shapeLayer.push_front(shapeBatch);
            }
            {
                std::lock_guard<std::mutex> guard(*lock);
                shapeLayersBuffer->push_front(shapeLayer);
            }
        }

    iterations++;
    }
    {
        std::lock_guard<std::mutex> guard(lockImage);
        turnOff = true;
    }
    imageThread.join();

}
void pngLoader(std::deque<PngData>* datas, bool* turnOff, std::mutex* lock, const std::string* filePath, float* waitTime){
    static const unsigned int bufferSizeMult = 1;
    auto& dataBuffer = *datas;
    PngData pD(255,255,3);
	while(!*turnOff){

        bool full;
        float tempWaitTime;
        {
            std::lock_guard guard(*lock);
<<<<<<< Updated upstream
            full = dataBuffer.size()>=TOTAL_LAYERS*TOTAL_EMOTION_LAYERS*bufferSizeMult;
=======
            full = dataBuffer.size()>=TOTAL_EMOTIONS*bufferSizeMult;
>>>>>>> Stashed changes
            tempWaitTime = *waitTime;
        }

        if(full){
            std::this_thread::sleep_for(std::chrono::nanoseconds((unsigned int)(tempWaitTime*100.0f)));
        }
        else{
            pD = loadPngTexture(*filePath);
            if(pD.isValid()){
                std::this_thread::sleep_for(std::chrono::nanoseconds((unsigned int)(tempWaitTime*1000000000.0f/(TOTAL_EMOTIONS*bufferSizeMult))));
                std::lock_guard guard(*lock);
                dataBuffer.push_front(pD);
            }
            else{
                std::this_thread::sleep_for(std::chrono::nanoseconds(1));
                checkPrint("ERROR: loading image\n", false, DEBUG_IMAGE);
            }
        }
	}
	
}

int mainCoordsBasic2D(int argc, char * argv[]){
    // Initialize OpenGL
    int return_status;
    GLFWwindow*  mWindow; 
    if((return_status = initOpenGL(mWindow, mWidth, mHeight)) != EXIT_SUCCESS) return return_status;
    
    DeltaTimer deltaTimer{};
    Alarm creationAlarm(SHAPE_INITIAL_SPAWN_TIME);
    Alarm otherAlarm(SHAPE_INITIAL_SPAWN_TIME);
    bool shapesFull = false;
    

    PngData blankPNGData(255,255, 3);
    const unsigned int blankTexture = obtainPNGBlank(blankPNGData);

    std::cout << "Generate triangle" << std::endl;
    PngData pngDataTriangle(255, 255);
    std::vector<glm::vec2> triangleBorder = borderVerticesFromShape(shapes::eTriT);
    unsigned int triangleBorderTexture = obtainPNGDistanceFromBorder(pngDataTriangle, triangleBorder);
    OpenGLShape triangle(shapes::eTriT, {blankTexture,triangleBorderTexture}, true);
    
    

    std::cout << "Generate square" << std::endl;
    PngData pngDataSquare(255, 255);
    std::vector<glm::vec2> squareBorder = borderVerticesFromShape(shapes::rectT);
    unsigned int squareBorderTexture = obtainPNGDistanceFromBorder(pngDataSquare, squareBorder);
    OpenGLShape square(shapes::rectT, {blankTexture,squareBorderTexture}, true);
    

    

    std::cout << "Generate circle" << std::endl;
    PngData pngDataCircle(255, 255);
    OpenGLShape circle = create_circle(16, pngDataCircle);
    circle.setTextures({blankTexture, circle.getTexture(0)});
    
    std::map<SHAPE_TYPE_2D, OpenGLShape*> shapeTypes{{SQUARE , &square}, {TRIANGLE, &triangle}, {CIRCLE, &circle}};
    
    
    
    
    
    
    LightingEnvironment lightingEnvironment = initMPV("Glitter/Shaders/texture-mvp-color.vs", "Glitter/Shaders/texture-mvp-color.fs");

    glm::mat4 model(1.0f);
    lightingEnvironment.object_shader.use();
    lightingEnvironment.object_shader.setUniformI("ourTexture", 0);
    lightingEnvironment.object_shader.setUniformI("borderTexture", 1);
    lightingEnvironment.object_shader.setUniformI("borderCase", 4);
    float destructionTime = DESTRUCTION_TIME;
    float totalLayers = TOTAL_LAYERS;
    float midLayer = MID_LAYER;
    float shapeMaxFade = SHAPE_MAX_FADE;
    float shapeMinFade = SHAPE_MIN_FADE;
    std::tuple<float,float> lineAbove = linearTransformation(0.0f, destructionTime*(totalLayers - 1 - midLayer), shapeMaxFade, shapeMinFade);
    auto const [slopeAT, interceptAT] = lineAbove;
    std::tuple<float,float> lineBelow = linearTransformation(0.0f, destructionTime*(midLayer + 1), shapeMinFade, shapeMaxFade);
    auto const [slopeBT, interceptBT] = lineBelow;
    lightingEnvironment.object_shader.setUniformF("destructionTime", destructionTime);
    lightingEnvironment.object_shader.setUniformI("MID_LAYER", midLayer);
    lightingEnvironment.object_shader.setUniformF("ABOVE_SLOPE", slopeAT);
    lightingEnvironment.object_shader.setUniformF("ABOVE_INTERCEPT", interceptAT);
    lightingEnvironment.object_shader.setUniformF("BELOW_SLOPE", slopeBT);
    lightingEnvironment.object_shader.setUniformF("BELOW_INTERCEPT", interceptBT);
    
    
    // initMouse(mWindow);


    
    std::deque<ShapeLayer> shapeLayersBuffer{};
    std::deque<ShapeLayer> shapeLayersStorage{};
    std::deque<ShapeLayer> shapeLayersDraw{};


    
    glm::vec4 backgroundColor(1.0f,1.0f,1.0f,1.0f);
    std::mutex shapeLock{};

    bool notDone = true;
    int additions = 0;
<<<<<<< Updated upstream
    float waitTime = creationAlarm.getWaitTime();
    std::thread creationThread(LayerGenerator,&shapesBuffer,&creationAlarm,mWindow,&lock, &notDone, &additions);
=======
    std::thread creationThread(something,&shapeLayersBuffer,&creationAlarm,&shapeLock, &notDone, &additions);
>>>>>>> Stashed changes

    bool midLayerMet = false;
    
    // #Render 2D Loop
    while (!glfwWindowShouldClose(mWindow)) {
        startDummyMPVLoop(deltaTimer, lightingEnvironment, backgroundColor);

        if (otherAlarm.checkAndUpdate()){
            std::lock_guard guard(shapeLock);
            while(!shapeLayersBuffer.empty()){
                auto shapeLayerB = shapeLayersBuffer.back();
                ShapeLayer shapeLayer = shapeLayerB;
                for(auto& shapeBatch : shapeLayer){
                    shapeBatch.texture = createPngTexture(shapeBatch.pngData);
                }

                shapeLayersStorage.push_front(shapeLayer);
                shapeLayersBuffer.pop_back();
            }
            
        }
        bool shapeDeleted = false;
        // Delete cubes
        while (shapeLayersStorage.size() > TOTAL_LAYERS){
            auto const shapeLayer = shapeLayersStorage.back();
            for(auto const& shapeBatch : shapeLayer){
                if(shapeBatch.texture != blankTexture)
                    glDeleteTextures(1, &shapeBatch.texture);
            }
            shapeDeleted = true;
            shapeLayersStorage.pop_back();
        }
<<<<<<< Updated upstream
        while(pngDataStorage.size() > TOTAL_LAYERS*TOTAL_EMOTION_LAYERS)
            pngDataStorage.pop_back();
        if(shapesStorage.size() >= (MID_LAYER - 0) && !midLayerMet){
=======
        if(shapeLayersStorage.size() >= (MID_LAYER - 0) && !midLayerMet){
>>>>>>> Stashed changes
            midLayerMet = true;
            std::cout << "The middle layer was met!!!" << std::endl;

        }
        if(shapeDeleted && !shapesFull){
            std::lock_guard guard(shapeLock);
            creationAlarm.setWaitTime(DESTRUCTION_TIME);
            otherAlarm.setWaitTime(DESTRUCTION_TIME);
            shapesFull = true;
            std::cout << "Shapes loaded!" << std::endl;
        }
        // Draw cubes
        drawCubeMatrices2DScale(lightingEnvironment, shapeLayersStorage, shapeTypes, deltaTimer, otherAlarm, shapeLock, lineAbove, lineBelow);



        endRenderLoop(mWindow,deltaTimer,camera);

        
    }   

    {
        std::lock_guard<std::mutex> guard(shapeLock);
        notDone = false;
    }
    creationThread.join();

    glfwTerminate();

    return EXIT_SUCCESS;
    
    
}

int mainDrawRectangle(int argc, char* argv[]){
    
    int return_status;
    GLFWwindow*  mWindow; 
    if((return_status = initOpenGL(mWindow, mWidth, mHeight)) != EXIT_SUCCESS) return return_status;
    Shader colorShader("Glitter/Shaders/texture-color.vs", "Glitter/Shaders/texture-color.fs");
    colorShader.use();
    colorShader.setUniformV4("aColor", glm::vec4(1.0f,1.0f,1.0f,1.0f));
    colorShader.setUniformI("borderCase", 3);
    colorShader.setUniformI("ourTexture", 0);
    colorShader.setUniformI("borderTexture", 1);
    DeltaTimer deltaTimer{};
    PngData blankPngData(16,16);
    PngData borderPngData(16,16);
    unsigned int borderTexture = obtainPNGDistanceFromBorder(borderPngData, borderVerticesFromShape(shapes::rectT));
    unsigned int blankTexture = obtainPNGBlank(blankPngData);
    OpenGLShape rectangle(shapes::rectT, {blankTexture, borderTexture}, true);
    pngDataSavePng("testSSR.png", borderPngData);
    

    
    
    while(!glfwWindowShouldClose(mWindow)){
        minimumStartRenderLoop(deltaTimer, glm::vec4(0.0f,0.1f,0.0f,1.0f));
        rectangle.draw(colorShader);
        endRenderLoopViewStatic(mWindow, deltaTimer);
    } glfwTerminate();
    
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
    
    PngData pngData(255, 255);
    OpenGLShape circle = create_circle(3, pngData);
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
OpenGLShape create_circle(unsigned int tri_cnt, PngData& pngData){

    std::vector<float> circle{};
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
                if(vi == 1)
                    border.push_back(glm::vec2(vert.x,vert.y));
            }
            else{
                vert = glm::vec3(0.0f,0.0f,0.0f); // Center Vertex
            }
            circle.push_back(vert.x);
            circle.push_back(vert.y);
            circle.push_back(vert.z);
            circle.push_back(boundTransformation(vert.x, -0.5f, 0.5f, 0.0f, 1.0f)); // Texture x coordinate (Have to normalize it to [0,1])
            circle.push_back(boundTransformation(vert.y, -0.5f, 0.5f, 0.0f, 1.0f)); // Texture x coordinate (Have to normalize it to [0,1])
            
            for(unsigned int i = 0; i<3; i++){
                ind.push_back(index);
                index++;
            }
        }
    }
    
    unsigned int texture = obtainPNGDistanceFromBorder(pngData, border);
    
    return OpenGLShape((unsigned) (tri_cnt*3), (unsigned) (tri_cnt*3), &circle[0], &ind[0], std::vector<unsigned>{ 3, 2}, texture, true);
}
void pngDataSavePng(const std::string filepath, const PngData& pngData){
    
    stbi_write_png(filepath.c_str(), pngData.width, pngData.height, pngData.nrChannels, pngData.getData(), pngData.nrChannels * pngData.width);
}
OpenGLShape create_circle(unsigned int tri_cnt)
{

    std::vector<float> circle{};
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
                if(vi == 1)
                    border.push_back(glm::vec2(vert.x,vert.y));
            }
            else{
                vert = glm::vec3(0.0f,0.0f,0.0f); // Center Vertex
            }
            circle.push_back(vert.x);
            circle.push_back(vert.y);
            circle.push_back(vert.z);
            circle.push_back(boundTransformation(vert.x, -0.5f, 0.5f, 0.0f, 1.0f)); // Texture x coordinate (Have to normalize it to [0,1])
            circle.push_back(boundTransformation(vert.y, -0.5f, 0.5f, 0.0f, 1.0f)); // Texture x coordinate (Have to normalize it to [0,1])
            
            for(unsigned int i = 0; i<3; i++){
                ind.push_back(index);
                index++;
            }
        }
    }
    
    unsigned int texture = obtainPNGDistanceFromBorder(255,255, border);
    
    return OpenGLShape((unsigned) (tri_cnt*3), (unsigned) (tri_cnt*3), &circle[0], &ind[0], std::vector<unsigned>{ 3, 2}, texture, true);
}
float clamp(const float x, const float min, const float max){
    if(x < min) return min;
    if(x > max) return max;
    return x;
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void startDummyMPVLoop(DeltaTimer& deltaTimer, LightingEnvironment& lightingEnvironment, glm::vec4 backgroundColor){
    minimumStartRenderLoop(deltaTimer, backgroundColor);
    setupDummyMPVEnvironmentToDraw(lightingEnvironment);
}
void startMPVLoop(DeltaTimer& deltaTimer, LightingEnvironment& lightingEnvironment, glm::vec4 backgroundColor){
    minimumStartRenderLoop(deltaTimer, backgroundColor);
    setupMPVEnvironmentToDraw(lightingEnvironment);
}
void startMPVLoop(DeltaTimer& deltaTimer, LightingEnvironment& lightingEnvironment){
    minimumStartRenderLoop(deltaTimer, glm::vec4(1.0f,1.0f,1.0f,1.0f));
    setupMPVEnvironmentToDraw(lightingEnvironment);
}
PngData::PngData(const int width, const int height, const int nrChannels): width{width},  height{height}, nrChannels{nrChannels}{
    this->data = initializePNGTextureData(width, height, nrChannels);
}
PngData::PngData(int width, int height) : width{width},  height{height}, nrChannels{PngData::nrChannelsDefault}{
    this->data = initializePNGTextureData(width, height, nrChannels);
}
PngData::PngData(const int width, const int height, const int nrChannels, unsigned char *data, bool copy) : width{width},  height{height}, nrChannels{nrChannels}
{
    if(copy){
        this->data = copyInitialize(data, width*height*nrChannels);
    }
    else this->data = data;
}
PngData::PngData(const PngData &pngData) : width{pngData.width}, height{pngData.height}, nrChannels{pngData.nrChannels}
{
    this->data = copyInitialize(pngData.getData(), pngData.size());
}
PngData::~PngData()
{
    freePNGTextureData(this->data);
}

void PngData::setData(int row, int col, int channel, const unsigned char& byte){
    this->data[index(row, col, channel)] = byte;
}

float floorWrap(float x, float y)
{
    return floor(x/y);
}

float ceilWrap(float x, float y)
{
    return ceil(x/y);
}

int floorWrap(int x, int y) {
    int mod = x % y;
    if(mod == 0)
        return x/y;
    else
        return (x-mod)/y;
}

int ceilWrap(int x, int y)
{
    int mod = x % y;
    if(mod == 0)
        return x/y;
    else
        return (x-mod)/y + 1;
}

unsigned int getTextureFromData(const PngData &pngData)
{
    if(pngData.nrChannels == 4)
        return useTextureDataToCreateTexture(pngData.getData(), pngData.width, pngData.height, "png");
    else
        return useTextureDataToCreateTexture(pngData.getData(), pngData.width, pngData.height, "pngNA");
}

void generatePNGLineToBordersFromPoint(PngData& pngData, std::vector<glm::vec2> borderVertices, int pointRow, int pointCol, int thickness, cdata::ColorRGBA color){
    float gridY = pointRow;
    float gridX = pointCol;
    float gridYN = boundTransformation(gridY, 0, pngData.height, -0.5f, 0.5f);
    float gridXN = boundTransformation(gridX, 0, pngData.width, -0.5f, 0.5f);
    bool combine = false;
    glm::vec2 gridVertexN(gridXN, gridYN);
    //bool isInRange = (colN >= -0.01f && colN <= 0.01f && rowN >= -0.01f && rowN <= 0.01f);
    glm::vec2 gridVertex(gridX, gridY);

    glm::vec2 prevVertex  = borderVertices[borderVertices.size()-1];
    for(int i = 0; i < borderVertices.size(); i++){
        glm::vec2 curVertex;
        if(i < borderVertices.size() - 1)
            curVertex = borderVertices[i];
        else
            curVertex = borderVertices[borderVertices.size()-1];
        // Normalize
        float  preVertexNormX = boundTransformation(prevVertex.x, -0.5f, 0.5f, 0, pngData.width);
        float  preVertexNormY = boundTransformation(prevVertex.y, -0.5f, 0.5f, 0, pngData.height);
        glm::vec2 preVertexNorm(preVertexNormX,preVertexNormY);
        float  curVertexNormX = boundTransformation(curVertex.x, -0.5f, 0.5f, 0, pngData.width);
        float  curVertexNormY = boundTransformation(curVertex.y, -0.5f, 0.5f, 0, pngData.height);
        glm::vec2 curVertexNorm(curVertexNormX,curVertexNormY);

        auto line = getLineFromVertices(preVertexNorm.x, curVertexNorm.x, preVertexNorm.y, curVertexNorm.y);
        float distance = distFromLine(line, gridVertex);
        // checkPrint("NORM=============\n",  combine, DEBUG_BORDER);
        checkPrint(std::format("({},{}) -> ",  preVertexNorm.x,  preVertexNorm.y), combine, DEBUG_BORDER);
        checkPrint(std::format("({},{})\n"  ,   curVertexNorm.x,   curVertexNorm.y), combine, DEBUG_BORDER);
        // checkPrint("NORM=============\n",  combine, DEBUG_BORDER);
        prevVertex = curVertex;
    }

}
std::map<TRANSITIONS,bool> emotionArrayToTransitions(std::vector<float> emotionArray){
    float happiness = emotionArray[HAPPY];
    float sadness   = emotionArray[SAD];
    float anger     = emotionArray[ANGRY];
    float fear      = emotionArray[FEAR];
    float surprise  = emotionArray[SURPRISE];
    float disgust   = emotionArray[DISGUST];
    float scalePotential = (anger + surprise)/2.0f;
    float fadePotential = (fear + sadness)/2.0f;
    float rightPotential = (happiness + anger - sadness)/2.0f;
    float leftPotential = (disgust + fear - sadness)/2.0f;
    std::map<TRANSITIONS,bool> transitions{}; // Assuming when a key is used to access a value when the key is not in the map then the value returned is false
    transitions[FADE] = fadePotential >= 0.5f && DO_FADE;
    transitions[SCALE] = scalePotential >= 0.5f && DO_SCALE;
    transitions[MOVE_FORWARD] = rightPotential >= 0.85f;
    transitions[BACK_OUT] = leftPotential >= 0.85f;
    if(transitions[MOVE_FORWARD] && transitions[BACK_OUT]){
        if(leftPotential > rightPotential) transitions[BACK_OUT] = false;
        else transitions[MOVE_FORWARD] = false;
    }
    bool isEmpty = true;
    for(auto const value : transitions){
        if(value.second){
            isEmpty = false;
            break;
        }
        
    }
    if(isEmpty) transitions[FADE] = true;
    return transitions;

    
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