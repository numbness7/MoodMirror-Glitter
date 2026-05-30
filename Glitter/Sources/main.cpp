// Local Headers
#include "glitter.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_projection.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"
#include <exception>
#include <iterator>
#include <shader.hpp>

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <stb_image.h>
#include <iostream>
#include <stdexcept>

#include <vector>
#include <random>
#include <list>

// Function Prototypes
float blender(float minBound, float maxBound, float ratio);
void worldInit(Shader shader);
float rand_range_uniform(float min=-1.0f, float max=1.0f);
void drawShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int vert_cnt);
void drawTexturedShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int vert_cnt, unsigned int texture);
void drawDoubleTexturedShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int vert_cnt, unsigned int texture1, unsigned int texture2);
void create_shape(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, 
    float vert[], unsigned int vert_cnt, unsigned int ind[], 
    unsigned int ind_cnt, unsigned int dimensions);
void create_colored_textued_shape(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, 
    float vert[], unsigned int vert_cnt, unsigned int ind[], 
    unsigned int ind_cnt);
void processInput(GLFWwindow* window);
void create_textured_shape(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, 
    float vert[], unsigned int vert_cnt, unsigned int ind[], 
    unsigned int ind_cnt);
void create_texture(unsigned int &texture, const char texture_filepath[], std::string filetype);


namespace shapes {
    float r_tri[] = {
        0.0f, 0.5f, 0.0f, // Top
        0.5f, 0.0f, 0.0f, // Right
        0.0f, 0.0f, 0.0f, // Left
    };
    
    unsigned int r_tri_ind[] = {
        0, 1, 2
    };
    
    float acute_tri[] = {
        0.0f, 1.0f, 0.0f, // Top
        1.0f, -1.0f, 0.0f, // Right
        -1.0f, -1.0f, 0.0f, // Left
    };
    
    float rect[] = {
        // position       
        -1.0,  1.0f, 0.0f,//top-left
         1.0,  1.0f, 0.0f, //top-right
         1.0, -1.0f, 0.0f, //bottom-right
        -1.0, -1.0f, 0.0f, //bottom-left
    };
    float rect_c_t[] = {
        // position         color           texture coor
        -0.5,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,//top-left
         0.5,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, //top-right
         0.5, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, //bottom-right
        -0.5, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, //bottom-left
    };
    float rect_t[] = {
        // position           texture coor
        -0.5,  0.5f, 0.0f,  0.0f, 1.0f,//top-left
         0.5,  0.5f, 0.0f,  1.0f, 1.0f, //top-right
         0.5, -0.5f, 0.0f,  1.0f, 0.0f, //bottom-right
        -0.5, -0.5f, 0.0f,  0.0f, 0.0f, //bottom-left
    };
    
    unsigned int rect_ind[] = {
        0, 1, 2,
        0, 2, 3,

    };
    
    float cube[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int cube_ind[] = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        15, 16, 17,
        18, 19, 20,
        21, 22, 23,
        24, 25, 26,
        27, 28, 29,
        30, 31, 32,
        33, 34, 35
    };
    
glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};



};

enum class SHAPE_TYPE {
    CIRCLE, RECTANGLE, TRIANGLE
};

struct AShape{
    glm::vec2 pos;
    glm::vec4 color;
    glm::vec2 scale;
    float rotation;
    SHAPE_TYPE shapeTYPE;
    float creationTime;
};

AShape genRanShape();

enum DIST {NORMAL, UNIFORM};


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

int main(int argc, char * argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
    


    Shader circleShader("Glitter/Shaders/ellipse.vs","Glitter/Shaders/ellipse.fs");
    worldInit(circleShader);
    Shader rectShader("Glitter/Shaders/mvp-color.vs","Glitter/Shaders/mvp-color.fs");
    worldInit(rectShader);
    glm::mat4 model;
    unsigned int VAO, VBO, EBO;
    unsigned int VAO_T, VBO_T, EBO_T;
    
    create_shape(VAO,VBO,EBO,shapes::rect,4*3,shapes::rect_ind,6,3);
    create_shape(VAO_T,VBO_T,EBO_T,shapes::acute_tri,3*3,shapes::r_tri_ind,3,3);


    
    
    
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

    
    std::list<AShape> ranShapes;
    
    unsigned int batche_size = 4;
    unsigned int batches = 1600;
    
    unsigned int shape_cnt = batche_size*batches;
    
    
    float spawnTimer = 0.0f;
    float spawnTime = 0.015f;
    float preTime = (float)glfwGetTime();
    float lastSpawnTime = preTime;
    float elapsedTime;

    float objectLifespan = (float)shape_cnt*spawnTime/(float)batche_size;
    bool randColor = false;
    glm::vec3 backGroundColorPre;
    glm::vec3 backGroundColorNext;
    glm::vec3 black(0.0f,0.0f,0.0f);
    glm::vec3 white(1.0f,1.0f,1.0f);
    if(randColor){
        backGroundColorPre = (randRGB());
        backGroundColorNext = (randRGB());
    }
    else{
        backGroundColorPre = white;
        backGroundColorNext = black;
    }
    
        Shader* shader;
    
    // Rendering Loop
    while (!glfwWindowShouldClose(mWindow)) {
        
        Shader* shader;
        glClearColor(white.x,white.y,white.z,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);
        elapsedTime = (float)glfwGetTime();
        spawnTimer = elapsedTime - lastSpawnTime;
        if(spawnTimer >= spawnTime){
           if(ranShapes.size()>shape_cnt){
               for(int i = 0; i < batche_size; i++)
                   ranShapes.pop_back();
           }
           for(int i = 0; i < batche_size; i++)
               ranShapes.emplace_front(genRanShape());
           lastSpawnTime = elapsedTime;
        }

        
        for (const auto& item : ranShapes){
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(item.pos,1.0f));
            model = glm::rotate(model, item.rotation, glm::vec3(0.0f,0.0f,1.0f));
            model = glm::scale(model,glm::vec3(item.scale,1.0f));
            if(item.shapeTYPE == SHAPE_TYPE::CIRCLE) shader = &circleShader;
            else shader = &rectShader;
            shader->use();
            shader->setUniform("model",model);
            float timeAlive = elapsedTime - item.creationTime;
            float timeAliveRatio = timeAlive/objectLifespan;
            float alphaPercent = blender(0.25f,0.8f,timeAliveRatio);
            shader->setUniform("aColor",glm::vec4(item.color.x,item.color.y,item.color.z,alphaPercent*item.color.w));
            if (item.shapeTYPE == SHAPE_TYPE::TRIANGLE)  drawShape(VAO_T, EBO_T, *shader, 3);
            else drawShape(VAO,EBO,*shader,6);
        }

        


        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
        processInput(mWindow);
        

        
    }   glfwTerminate();
    return EXIT_SUCCESS;
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
void drawTexturedShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int vert_cnt, unsigned int texture){
    shader.use();
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, vert_cnt, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
void drawShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int vert_cnt){
    shader.use();
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, vert_cnt, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void processInput(GLFWwindow* mWindow){
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(mWindow, true);
}

void create_shape(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, 
    float vert[], unsigned int vert_cnt, unsigned int ind[], 
    unsigned int ind_cnt, unsigned int dimensions){
        
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vert_cnt*sizeof(float), vert, GL_STATIC_DRAW);
        
        glGenBuffers(1,&EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind_cnt*sizeof(unsigned int), ind, GL_STATIC_DRAW);
        
        
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glVertexAttribPointer(0, dimensions, GL_FLOAT, GL_FALSE, dimensions * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

void create_textured_shape(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, 
    float vert[], unsigned int vert_cnt, unsigned int ind[], 
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

void create_texture(unsigned int &texture, const char texture_filepath[], std::string filetype){
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texture_filepath, &width, &height, &nrChannels, 0);
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
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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
unsigned int create_circle(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, unsigned int tri_cnt){


    std::vector<float> circle;
    std::vector<unsigned int> ind;
    unsigned int index = 0;

    for(unsigned int tri = 0; tri < tri_cnt; tri++){
       
        
        for(int vi = 0; vi<3;vi++){

            glm::vec3 vert;
            if(vi > 0){
                glm::mat4 transform(1.0f);
                float angle = glm::radians(((float)tri+vi)*360.0f/(float)tri_cnt);
                transform = glm::rotate(transform, angle, glm::vec3(0.0f,0.0f,1.0f));
                glm::vec3 radius_vec(1.0f,0.0f,0.0f);
                glm::vec4 radius_vec4(radius_vec,1.0f);
                radius_vec4 = transform * radius_vec4;
                vert = glm::vec3(radius_vec4.x,radius_vec4.y,radius_vec4.z);
            }
            else{
                vert = glm::vec3(0.0f,0.0f,0.0f);
            }
            circle.push_back(vert.x);
            circle.push_back(vert.y);
            circle.push_back(vert.z);
            
            for(unsigned int i = 0; i<3; i++){
                ind.push_back(index);
                index++;
            }
        }
    }
    
    create_shape(VAO,VBO,EBO,&circle[0],tri_cnt*3*3,&ind[0],tri_cnt*3*3,3);
    
    return tri_cnt*3;
    

}
AShape genRanShape(){
    AShape ranShape;
    float x = rand_range_uniform();
    float y = rand_range_uniform();
    ranShape.pos = glm::vec2(x,y);
    float r = rand_range_uniform(0.0f,1.0f);
    float g = rand_range_uniform(0.0f,1.0f);
    float b = rand_range_uniform(0.0f,1.0f);
    float a = rand_range_uniform(0.0f,1.0f);
    ranShape.color = glm::vec4(r,g,b,a);
    while((x = rand_range_normal(0.2f,0.05f)) < 0.01f || x > 2.0f);
    y = x;
    ranShape.scale = glm::vec2(x,y);
    unsigned int ranTypeIndex = rand_range_uniform((unsigned int)0,(unsigned int)2);
    SHAPE_TYPE shapeType;
    ranShape.rotation = glm::radians(rand_range_uniform(0.0f,360.0f));
    switch (ranTypeIndex)
        {
        case 0:
            shapeType = SHAPE_TYPE::CIRCLE;
            break;
        case 1:
            shapeType = SHAPE_TYPE::RECTANGLE;
            break;
        case 2:
            shapeType = SHAPE_TYPE::TRIANGLE;
            break;
        }
    ranShape.shapeTYPE = shapeType;
    ranShape.creationTime = (float)glfwGetTime();
    return ranShape;
}

void worldInit(Shader shader){
    shader.use();
    glm::mat4 proj(1.0f);
    float aspectRatio = (float)mWidth/(float)mHeight;
    proj = glm::scale(proj,glm::vec3(1/aspectRatio,1.0f,1.0f));
    glm::mat4 view(1.0f);
    glm::mat4 model(1.0f);
    shader.use();
    shader.setUniform("projection", proj);
    shader.setUniform("view", view);
    shader.setUniform("model",model);
    shader.setUniform("aColor", glm::vec3(1.0f,0.0f,0.0f));
    shader.setUniform("shadeType", (unsigned int)0);
    shader.setUniform("minPercentage", 0.0f);
}

float blender(float minBound, float maxBound, float ratio){
    float blend;
    if (ratio < minBound)
        blend = ratio/minBound;
    else if(ratio < maxBound)
        blend = 1.0f;
    else 
        blend = (1.0f - (ratio-maxBound)/(1.0f-maxBound));
    return blend;
}