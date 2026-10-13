
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
Camera camera(glm::vec3(0.0f,0.0f,9.0f), glm::vec3(0.0f,1.0f,0.0f), -90, 0);
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

void drawTexturedShape(unsigned int &VAO, unsigned int &EBO, Shader shader, unsigned int vert_cnt, unsigned int texture){
    shader.use();
    glActiveTexture(GL_TEXTURE0);
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

void releaseCursor(GLFWwindow* mWindow){
    mouseCaptured = false;
    glfwSetInputMode(mWindow, GLFW_CURSOR, mouseCaptured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void captureCursor(GLFWwindow* mWindow){
    mouseCaptured = true;
    glfwSetInputMode(mWindow, GLFW_CURSOR, mouseCaptured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
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

void generate_texture(unsigned int &texture, std::string texture_filepath, std::string filetype){
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texture_filepath.c_str(), &width, &height, &nrChannels, 0);
    if(filetype=="jpg"){
        for(int i = 0; i < height; i+=1){
            for(int j = 0; j < width*3; j+=1){
                for(int color = 0; color < 3; color++){
                    if(color == 0)
                        data[i*width*3+j+color] = 0xFF;
                    if(color == 1)
                        data[i*width*3+j+color] = 0xFF;
                    if(color == 2)
                        data[i*width*3+j+color] = 0xFF;
                }
            }
        }
    }
    else if(filetype=="png"){
        for(int i = 0; i < height; i+=1){
            for(int j = 0; j < width*4; j+=4){
                for(int color = 0; color < 4; color++){
                    if(color == 0) // Red
                        data[i*width*4+j+color] = 0xFF;
                    if(color == 1) // Green
                        data[i*width*4+j+color] = 0xFF;
                    if(color == 2) // Blue
                        data[i*width*4+j+color] = 0xFF;
                    if(color == 3) // Don't know
                        data[i*width*4+j+color] = 0xFF;
                }
            }
        }
    }

    glGenTextures(1, &texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
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
    shaderTextureMVP.setUniform("ourTexture1",(unsigned int)0);
    shaderTextureMVP.setUniform("ourTexture2",(unsigned int)1);
    
    glm::mat4 proj;
    proj = glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.1f, 100.0f);

    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));

    glm::mat4 model(1.0f);

    shaderTextureMVP.setUniform("projection", proj);
    shaderTextureMVP.setUniform("view", view);
    shaderTextureMVP.setUniform("model",model);
    
    
    glm::vec3 cameraPos(0.0f,0.0f,3.0f);
    glm::vec3 cameraUp(0.0f,1.0f,0.0f);
    glm::vec3 cameraTarget(0.0f,0.0f,0.0f);
    
    glm::vec3 toCameraDirection(cameraPos - cameraTarget);
    glm::vec3 up(0.0f,1.0f,0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, toCameraDirection));
    
    
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);


    
    
    // Rendering Loop
    while (!glfwWindowShouldClose(mWindow)) {

        // Background Fill Color
        glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        proj = glm::perspective(glm::radians(camera.Zoom), (float)mWidth / (float)mHeight, 0.1f, 100.0f);
        shaderTextureMVP.setUniform("projection",proj);
        

        view = camera.GetViewMatrix();
        shaderTextureMVP.setUniform("view",view);


        for(unsigned int i = 0; i < std::size(shapes::cubePositions); i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, shapes::cubePositions[i]);
            model = glm::rotate(model, glm::radians(15.0f)*(float)i, glm::vec3(0.4f,0.95f,0.2f));
            shaderTextureMVP.setUniform("model",model);
            drawTexturedShape(VAO_T, EBO_T, shaderTextureMVP, 36, texture1);

        }
        


        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
        processInput(mWindow, camera, deltaTimer.getDeltaTime(false));
        

        
    }   glfwTerminate();
    return EXIT_SUCCESS;
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
    shaderTextureMVP.setUniform("ourTexture1",(unsigned int)0);
    
    glm::mat4 proj;
    proj = glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.1f, 100.0f);

    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));

    glm::mat4 model(1.0f);

    shaderTextureMVP.setUniform("projection", proj);
    shaderTextureMVP.setUniform("view", view);
    shaderTextureMVP.setUniform("model",model);
    shaderTextureMVP.setUniform("aColor", glm::vec4(0.6));
    
    
    glm::vec3 cameraPos(0.0f,0.0f,3.0f);
    glm::vec3 cameraUp(0.0f,1.0f,0.0f);
    glm::vec3 cameraTarget(0.0f,0.0f,0.0f);
    
    glm::vec3 toCameraDirection(cameraPos - cameraTarget);
    glm::vec3 up(0.0f,1.0f,0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, toCameraDirection));
    
    
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);


    
    
    // Rendering Loop
    while (!glfwWindowShouldClose(mWindow)) {

        // Background Fill Color
        glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        deltaTimer.updateDeltaTime();
        

        proj = glm::perspective(glm::radians(camera.Zoom), (float)mWidth / (float)mHeight, 0.1f, 100.0f);
        shaderTextureMVP.setUniform("projection",proj);
        

        view = camera.GetViewMatrix();
        shaderTextureMVP.setUniform("view",view);


        for(unsigned int i = 0; i < std::size(shapes::cubePositions); i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, shapes::cubePositions[i]);
            model = glm::rotate(model, glm::radians(15.0f)*(float)i, glm::vec3(0.4f,0.95f,0.2f));
            shaderTextureMVP.setUniform("model",model);
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
    object_shader.setUniform("projection", proj);
    object_shader.setUniform("view", view);
    object_shader.setUniform("model",model);
    
    Material material;
    material.diffuse = 0;
    material.specular = 1;
    material.shininess  = 128.0f*0.25; 

   object_shader.setUniform("material.diffuse",   material.diffuse);
   object_shader.setUniform("material.specular",  material.specular);
   object_shader.setUniform("material.shininess", material.shininess); 
   object_shader.setUniform("spot_light.cut_off", glm::cos(glm::radians(14.5f)));
   object_shader.setUniform("spot_light.outer_cut_off", glm::cos(glm::radians(20.0f)));


    object_shader.setUniform("dir_light.ambient",  glm::vec3(0.1f));
    object_shader.setUniform("dir_light.diffuse",  glm::vec3(0.2f)); // darken diffuse light a bit
    object_shader.setUniform("dir_light.specular", glm::vec3(0.2f)); 
    object_shader.setUniform("dir_light.direction", glm::vec3(0.0f, 1.0f, 1.0f)); 
    
    object_shader.setUniform("spot_light.ambient",  glm::vec3(0.1f));
    object_shader.setUniform("spot_light.diffuse",  glm::vec3(1.0f)); // darken diffuse light a bit
    object_shader.setUniform("spot_light.specular", glm::vec3(1.0f)); 
    glm::vec3 diffuses[4] = {
        glm::vec3(1.0f,0.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        glm::vec3(0.0f,0.0f,1.0f),
        glm::vec3(0.3f,0.3f,0.3f),
    };

    std::stringstream s ("");
    for(int i = 0; i < 4; i ++){
        s << "point_lights" << "[" << i << "]";
        object_shader.setUniform(s.str() + ".ambient",  glm::vec3(0.1f));
        object_shader.setUniform(s.str() + ".diffuse",  diffuses[i]); // darken diffuse light a bit
        object_shader.setUniform(s.str() + ".specular", diffuses[i]); 
        object_shader.setUniform(s.str() + ".position", light_positions[i]);
        object_shader.setUniform(s.str() + ".constant", 1.0f);
        object_shader.setUniform(s.str() + ".linear", .09f);
        object_shader.setUniform(s.str() + ".quadratic", .032f);
        s.str("");
        s.clear();
    }
    
    light_shader.use();
    
    light_shader.setUniform("projection", proj);
    light_shader.setUniform("view", view);
    light_shader.setUniform("model",model);
    light_shader.setUniform("aColor",glm::vec3(0.0f,1.0f,0.0f));

    
    
    
    
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
        light_shader.setUniform("projection",proj);
        light_shader.setUniform("view",view);

        for(int i = 0; i < 4; i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, light_positions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            light_shader.setUniform("model", model);
            light_shader.setUniform("aColor", diffuses[i]);
            drawShape(VAO_T, EBO_O, light_shader, 36);
        }

        object_shader.use();
        object_shader.setUniform("view",view);
        object_shader.setUniform("projection",proj);
        object_shader.setUniform("view_pos", camera.Position);
       object_shader.setUniform("spot_light.position", camera.Position);
       object_shader.setUniform("spot_light.direction", camera.Front);
       object_shader.setUniform("spot_light.is_on", (bool)flash_light);

        for(unsigned int i = 0; i < std::size(shapes::cubePositions); i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, shapes::cubePositions[i]);
            //model = glm::rotate(model, glm::radians(15.0f)*(float)i, glm::vec3(0.4f,0.95f,0.2f));
            object_shader.setUniform("model", model);
            drawTexturedShape(VAO_O, EBO_O, object_shader, 36, diffuse_sampler_2d);
        }
        
        



        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
        processInput(mWindow, camera, deltaTimer.getDeltaTime());
        

        
    }   glfwTerminate();
    return EXIT_SUCCESS;
}
int mainModel(int argc, char * argv[]){
    
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
    create_texture(diffuse_sampler_2d, "Glitter/Textures/container2.png", "png");
    create_texture(specular_sampler_2d, "Glitter/Textures/container2_specular.png", "png");
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_sampler_2d);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specular_sampler_2d);


    Model backpack("Glitter/Models/backpack/backpack.obj");
    
    
    
    
    

    
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

    Shader object_shader("Glitter/Shaders/mesh.vs", "Glitter/Shaders/mesh.fs");
    Shader light_shader("Glitter/Shaders/lamp.vs", "Glitter/Shaders/lamp.fs");
    object_shader.use();
    object_shader.setUniform("projection", proj);
    object_shader.setUniform("view", view);
    object_shader.setUniform("model",model);
    
    Material material;
    material.diffuse = 0;
    material.specular = 1;
    material.shininess  = 128.0f*0.25; 

   object_shader.setUniform("material.diffuse",   material.diffuse);
   object_shader.setUniform("material.specular",  material.specular);
   object_shader.setUniform("material.shininess", material.shininess); 
   object_shader.setUniform("spot_light.cut_off", glm::cos(glm::radians(14.5f)));
   object_shader.setUniform("spot_light.outer_cut_off", glm::cos(glm::radians(20.0f)));


    object_shader.setUniform("dir_light.ambient",  glm::vec3(0.1f));
    object_shader.setUniform("dir_light.diffuse",  glm::vec3(0.2f)); // darken diffuse light a bit
    object_shader.setUniform("dir_light.specular", glm::vec3(0.2f)); 
    object_shader.setUniform("dir_light.direction", glm::vec3(0.0f, 1.0f, 1.0f)); 
    
    object_shader.setUniform("spot_light.ambient",  glm::vec3(0.1f));
    object_shader.setUniform("spot_light.diffuse",  glm::vec3(1.0f)); // darken diffuse light a bit
    object_shader.setUniform("spot_light.specular", glm::vec3(1.0f)); 
    glm::vec3 diffuses[4] = {
        glm::vec3(1.0f,0.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        glm::vec3(0.0f,0.0f,1.0f),
        glm::vec3(0.3f,0.3f,0.3f),
    };

    std::stringstream s ("");
    for(int i = 0; i < 4; i ++){
        s << "point_lights" << "[" << i << "]";
        object_shader.setUniform(s.str() + ".ambient",  glm::vec3(0.1f));
        object_shader.setUniform(s.str() + ".diffuse",  diffuses[i]); // darken diffuse light a bit
        object_shader.setUniform(s.str() + ".specular", diffuses[i]); 
        object_shader.setUniform(s.str() + ".position", light_positions[i]);
        object_shader.setUniform(s.str() + ".constant", 1.0f);
        object_shader.setUniform(s.str() + ".linear", .09f);
        object_shader.setUniform(s.str() + ".quadratic", .032f);
        s.str("");
        s.clear();
    }
    
    light_shader.use();
    
    light_shader.setUniform("projection", proj);
    light_shader.setUniform("view", view);
    light_shader.setUniform("model",model);
    light_shader.setUniform("aColor",glm::vec3(0.0f,1.0f,0.0f));

    
    
    
    
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
        light_shader.setUniform("projection",proj);
        light_shader.setUniform("view",view);

        for(int i = 0; i < 4; i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, light_positions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            light_shader.setUniform("model", model);
            light_shader.setUniform("aColor", diffuses[i]);
            drawShape(VAO_T, EBO_O, light_shader, 36);
        }

        object_shader.use();
        object_shader.setUniform("view",view);
        object_shader.setUniform("projection",proj);
        object_shader.setUniform("view_pos", camera.Position);
       object_shader.setUniform("spot_light.position", camera.Position);
       object_shader.setUniform("spot_light.direction", camera.Front);
       object_shader.setUniform("spot_light.is_on", (bool)flash_light);

        
       backpack.Draw(object_shader);
        



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
    object_shader.setUniform("projection", proj);
    object_shader.setUniform("view", view);
    object_shader.setUniform("model",model);
    
    Material material;
    material.diffuse = 0;
    material.specular = 1;
    material.shininess  = 128.0f*0.25; 

   object_shader.setUniform("material.diffuse",   material.diffuse);
   object_shader.setUniform("material.specular",  material.specular);
   object_shader.setUniform("material.shininess", material.shininess); 
   object_shader.setUniform("spot_light.cut_off", glm::cos(glm::radians(14.5f)));
   object_shader.setUniform("spot_light.outer_cut_off", glm::cos(glm::radians(20.0f)));


    object_shader.setUniform("dir_light.ambient",  glm::vec3(0.1f));
    object_shader.setUniform("dir_light.diffuse",  glm::vec3(0.2f)); // darken diffuse light a bit
    object_shader.setUniform("dir_light.specular", glm::vec3(0.2f)); 
    object_shader.setUniform("dir_light.direction", glm::vec3(0.0f, 1.0f, 1.0f)); 
    
    object_shader.setUniform("spot_light.ambient",  glm::vec3(0.1f));
    object_shader.setUniform("spot_light.diffuse",  glm::vec3(1.0f)); // darken diffuse light a bit
    object_shader.setUniform("spot_light.specular", glm::vec3(1.0f)); 
    glm::vec3 diffuses[4] = {
        glm::vec3(1.0f,0.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        glm::vec3(0.0f,0.0f,1.0f),
        glm::vec3(0.3f,0.3f,0.3f),
    };

    std::stringstream s ("");
    for(int i = 0; i < 4; i ++){
        s << "point_lights" << "[" << i << "]";
        object_shader.setUniform(s.str() + ".ambient",  glm::vec3(0.1f));
        object_shader.setUniform(s.str() + ".diffuse",  diffuses[i]); // darken diffuse light a bit
        object_shader.setUniform(s.str() + ".specular", diffuses[i]); 
        object_shader.setUniform(s.str() + ".position", light_positions[i]);
        object_shader.setUniform(s.str() + ".constant", 1.0f);
        object_shader.setUniform(s.str() + ".linear", .09f);
        object_shader.setUniform(s.str() + ".quadratic", .032f);
        s.str("");
        s.clear();
    }
    
    light_shader.use();
    
    light_shader.setUniform("projection", proj);
    light_shader.setUniform("view", view);
    light_shader.setUniform("model",model);
    light_shader.setUniform("aColor",glm::vec3(0.0f,1.0f,0.0f));

    
    
    
    
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
        light_shader.setUniform("projection",proj);
        light_shader.setUniform("view",view);

        for(int i = 0; i < 4; i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, light_positions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            light_shader.setUniform("model", model);
            light_shader.setUniform("aColor", diffuses[i]);
            drawShape(VAO_T, EBO_O, light_shader, 36);
        }

        object_shader.use();
        object_shader.setUniform("view",view);
        object_shader.setUniform("projection",proj);
        object_shader.setUniform("view_pos", camera.Position);
       object_shader.setUniform("spot_light.position", camera.Position);
       object_shader.setUniform("spot_light.direction", camera.Front);
       object_shader.setUniform("spot_light.is_on", (bool)flash_light);

        for(unsigned int i = 0; i < std::size(shapes::cubePositions); i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, shapes::cubePositions[i]);
            model = glm::rotate(model, glm::radians(15.0f)*(float)i, glm::vec3(0.4f,0.95f,0.2f));
            object_shader.setUniform("model", model);
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
    shaderTextureMVP.setUniform("ourTexture",(unsigned int)0);
    
    glm::mat4 proj;
    proj = glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.1f, 100.0f);

    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));

    glm::mat4 model(1.0f);

    shaderTextureMVP.setUniform("projection", proj);
    shaderTextureMVP.setUniform("view", view);
    shaderTextureMVP.setUniform("model",model);
    
    
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
        shaderTextureMVP.setUniform("projection",proj);
        

        view = camera.GetViewMatrix();
        shaderTextureMVP.setUniform("view",view);
        timer += delta_timer.getDeltaTime();
        if(timer > 1.0f) {
            timer = timer - 1.0f;
        }
        path.setProgress(timer);

        model = glm::mat4(1.0f);
        model = glm::translate(model, path.getPositionOnPath());
        shaderTextureMVP.setUniform("model",model);
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



        std::vector<std::vector<float>> emotionArray = readEmotionArray("../data.json");
        
        // Use emotion array to set properties of shapes
        glm::vec3 emotionRGB = emotionArrayToColor(emotionArray[0]);
        glm::vec3 rotationAxis = glm::vec3(sin(deltaTimer.getElapsedTime()*1.0f + M_1_PI),-sin(deltaTimer.getElapsedTime()*1.0f/2.7865432f + M_1_PI/2),sin(deltaTimer.getElapsedTime()*1.0f/3.125105f + M_1_PI/3));
        rotatedModel = glm::rotate(glm::mat4(1.0f), (float)(1.0f*((float)(M_PI))), rotationAxis);

        
        // Stationary Cube
        lightingEnvironment.model = glm::mat4(1.0f);
        lightingEnvironment.model = glm::scale(lightingEnvironment.model, glm::vec3(0.2f));
        
        // Rotating Rod
        glm::vec4 color = glm::vec4(1.0f,1.0f,1.0f,1.0f);
        lightingEnvironment.object_shader.setUniform("model",lightingEnvironment.model);
        lightingEnvironment.object_shader.setUniform("aColor", color);
        objectShape.draw(lightingEnvironment.object_shader);
        lightingEnvironment.model = glm::mat4(rotatedModel);
        lightingEnvironment.model = glm::translate(lightingEnvironment.model, glm::vec3(length/2,0.0f,0.0f));
        lightingEnvironment.model = glm::scale(lightingEnvironment.model, glm::vec3(length,0.1f,0.1f));
        lightingEnvironment.object_shader.setUniform("model",lightingEnvironment.model);
        lightingEnvironment.object_shader.setUniform("aColor", color);
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
            lightingEnvironment.object_shader.setUniform("model",cubePair.model);
            lightingEnvironment.object_shader.setUniform("aColor", cubePair.color);
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
    shaderTextureMVP.setUniform("ourTexture",(unsigned int)0);
    
    glm::mat4 proj;
    proj = glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.1f, 100.0f);

    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));

    glm::mat4 model(1.0f);

    shaderTextureMVP.setUniform("projection", proj);
    shaderTextureMVP.setUniform("view", view);
    shaderTextureMVP.setUniform("model",model);
    
    
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
        shaderTextureMVP.setUniform("projection",proj);
        

        view = camera.GetViewMatrix();
        shaderTextureMVP.setUniform("view",view);
        timer += delta_timer.getDeltaTime();
        if(timer > 1.0f) {
            timer = timer - 1.0f;
        }
        std::cout << "timer: " << timer << std::endl;
        path.setProgress(timer);

        model = glm::mat4(1.0f);
        model = glm::translate(model, path.getPositionOnPath());
        shaderTextureMVP.setUniform("model",model);
        drawTexturedShape(VAO_T, EBO_T, shaderTextureMVP, 36, texture1);
        


        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
        processInput(mWindow, camera, deltaTimer.getDeltaTime());
        

        
    }   glfwTerminate();
    return EXIT_SUCCESS;
}
nlohmann::json readjsonfile(std::string filepath){

    while(true){
        try{
            using json = nlohmann::json;
            std::string json_string = readfile(filepath.c_str());
            json data = json::parse(json_string);
            return data;
        }
        catch(std::exception e){
            std::cout << e.what() << std::endl;
                
        }
    }
}

std::vector<std::vector<float>> readEmotionArray(std::string filepath){
    nlohmann::json data = readjsonfile(filepath);
    return data["emotion_array"];
}

float emotionArrayToLength(std::vector<std::vector<float>> emotionArray){
    std::vector<float> array0 = emotionArray[0];
    return array0[0] + 0.1f;
}

glm::vec3 emotionArrayToColor(std::vector<float> array0){
    float fear      = array0[0];
    float anger     = array0[1];
    float sadness   = array0[2];
    float disgust   = array0[3];
    float happiness = array0[4];
    float surprise  = array0[5];
    float red   =   0.5f + (disgust - fear)/2;
    float green =   0.5f + (happiness - anger)/2;
    float blue  =   0.5f + (surprise - sadness)/2;
    return glm::vec3(red,green,blue);
}
SHAPE_TYPE emotionArrayToShapeType(std::vector<float> array0){
    float fear      = array0[0];
    float anger     = array0[1];
    float sadness   = array0[2];
    float disgust   = array0[3];
    float happiness = array0[4];
    float surprise  = array0[5];
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
glm::vec3 emotionArrayToVec3(std::vector<float> array0)
{
    float fear      = array0[0];
    float anger     = array0[1];
    float sadness   = array0[2];
    float disgust   = array0[3];
    float happiness = array0[4];
    float surprise  = array0[5];
    float x = disgust - fear;
    float y = happiness - anger;
    float z = surprise - sadness;
    return glm::vec3(x,y,z);
}
void initMouse(GLFWwindow* mWindow){
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);
}
LightingEnvironment initMPV(std::string vertexShaderPath, std::string fragmentShaderPath){
    glm::mat4 proj;
    proj = glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.1f, 100.0f);
    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));
    glm::mat4 model(1.0f);

    Shader object_shader(vertexShaderPath, fragmentShaderPath);
    Shader light_shader(vertexShaderPath, fragmentShaderPath);
    object_shader.use();
    object_shader.setUniform("projection", proj);
    object_shader.setUniform("view", view);
    object_shader.setUniform("model",model);
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
    object_shader.setUniform("projection", proj);
    object_shader.setUniform("view", view);
    object_shader.setUniform("model",model);
    
    Material material;
    material.diffuse = 0;
    material.specular = 1;
    material.shininess  = 128.0f*0.25; 

   object_shader.setUniform("material.diffuse",   material.diffuse);
   object_shader.setUniform("material.specular",  material.specular);
   object_shader.setUniform("material.shininess", material.shininess); 
   object_shader.setUniform("spot_light.cut_off", glm::cos(glm::radians(100.5f)));
   object_shader.setUniform("spot_light.outer_cut_off", glm::cos(glm::radians(200.0f)));


    object_shader.setUniform("dir_light.ambient",  glm::vec3(0.1f));
    object_shader.setUniform("dir_light.diffuse",  glm::vec3(0.1f)); // darken diffuse light a bit
    object_shader.setUniform("dir_light.specular", glm::vec3(0.1f)); 
    object_shader.setUniform("dir_light.direction", glm::vec3(0.0f, 1.0f, 1.0f)); 
    
    object_shader.setUniform("spot_light.ambient",  glm::vec3(0.1f));
    object_shader.setUniform("spot_light.diffuse",  glm::vec3(1.0f)); // darken diffuse light a bit
    object_shader.setUniform("spot_light.specular", glm::vec3(1.0f)); 

    std::stringstream s ("");
    for(int i = 0; i < POINT_LIGHT_COUNT; i ++){
        s << "point_lights" << "[" << i << "]";
        object_shader.setUniform(s.str() + ".ambient",  glm::vec3(0.1f));
        object_shader.setUniform(s.str() + ".diffuse",  diffuses[i]); // darken diffuse light a bit
        object_shader.setUniform(s.str() + ".specular", diffuses[i]); 
        object_shader.setUniform(s.str() + ".position", light_positions[i]);
        object_shader.setUniform(s.str() + ".constant", 1.0f);
        object_shader.setUniform(s.str() + ".linear", .09f);
        object_shader.setUniform(s.str() + ".quadratic", .032f);
        s.str("");
        s.clear();
    }
    
    light_shader.use();
    
    light_shader.setUniform("projection", proj);
    light_shader.setUniform("view", view);
    light_shader.setUniform("model",model);
    light_shader.setUniform("aColor",glm::vec3(0.0f,1.0f,0.0f));
    return LightingEnvironment{model,view,proj,object_shader,light_shader};
}
void setupMPVEnvironmentToDraw(LightingEnvironment& lightingEnvironment){

    lightingEnvironment.projection = glm::perspective(glm::radians(camera.Zoom), (float)mWidth / (float)mHeight, 0.1f, 100.0f);
    lightingEnvironment.view = camera.GetViewMatrix();

    lightingEnvironment.object_shader.use();
    lightingEnvironment.object_shader.setUniform("view",lightingEnvironment.view);
    lightingEnvironment.object_shader.setUniform("projection",lightingEnvironment.projection);
}
void setupLightingEnvironmentToDraw(LightingEnvironment& lightingEnvironment, glm::vec3 light_positions[POINT_LIGHT_COUNT], glm::vec3 diffuses[POINT_LIGHT_COUNT], OpenGLShape& lightShape, bool drawLights){

    lightingEnvironment.projection = glm::perspective(glm::radians(camera.Zoom), (float)mWidth / (float)mHeight, 0.1f, 100.0f);
    lightingEnvironment.view = camera.GetViewMatrix();
    
    
    

    lightingEnvironment.light_shader.use();
    lightingEnvironment.light_shader.setUniform("projection",lightingEnvironment.projection);
    lightingEnvironment.light_shader.setUniform("view",lightingEnvironment.view);

    if (drawLights){
        for(int i = 0; i < POINT_LIGHT_COUNT; i++){
            lightingEnvironment.model = glm::mat4(1.0f);
            lightingEnvironment.model = glm::translate(lightingEnvironment.model, light_positions[i]);
            lightingEnvironment.model = glm::scale(lightingEnvironment.model, glm::vec3(0.2f));
            lightingEnvironment.light_shader.setUniform("model", lightingEnvironment.model);
            lightingEnvironment.light_shader.setUniform("aColor", diffuses[i]);
            lightShape.draw(lightingEnvironment.light_shader);
        }
    }

    lightingEnvironment.object_shader.use();
    lightingEnvironment.object_shader.setUniform("view",lightingEnvironment.view);
    lightingEnvironment.object_shader.setUniform("projection",lightingEnvironment.projection);
    lightingEnvironment.object_shader.setUniform("view_pos", camera.Position);
   lightingEnvironment.object_shader.setUniform("spot_light.position", camera.Position);
   lightingEnvironment.object_shader.setUniform("spot_light.direction", camera.Front);
   lightingEnvironment.object_shader.setUniform("spot_light.is_on", (bool)flash_light);
}
OpenGLShape::OpenGLShape(unsigned int vert_cnt, unsigned int ind_cnt, const float vertices[], const unsigned int indices[], const std::vector<unsigned int>& attributeSizes, unsigned int texture, bool hasTexture){
    this->ind_cnt = ind_cnt;
    this->vert_cnt = vert_cnt;
    this->texture = texture;
    this->hasTexture = hasTexture;
    create_a_shape(this->VAO, this->VBO, this->EBO, this->vert_cnt, this->ind_cnt, &vertices[0], &indices[0], attributeSizes);
}
void OpenGLShape::draw(Shader shader){
    if(hasTexture)
        drawTexturedShape(VAO, EBO, shader, vert_cnt, texture);
    else
        drawShape(VAO, EBO, shader, vert_cnt);
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
    Alarm alarm(SHAPE_INITIAL_SPAWN_TIME);
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
    
    
    std::map<SHAPE_TYPE, OpenGLShape*> shapeTypes{{CUBE , &cubeShape}, {PYRAMID, &pyramidShape}, {OCTAHEDRON, &octahedronShape}};
    
    
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
    lightingEnvironment.object_shader.setUniform("SHAPE_SPAWN_TIME", (float) SHAPE_SPAWN_TIME);
    lightingEnvironment.object_shader.setUniform("MAXIMUM_BATCHES", (float) MAXIMUM_BATCHES);
    lightingEnvironment.object_shader.setUniform("SHAPE_MAX_FADE", (float) SHAPE_MAX_FADE);
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
        for(int i = 0; i < emotionArray.size(); i++){
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
                SHAPE_TYPE type;
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
                        ShapeProperties shape = ShapeProperties{model,glm::vec4(emotionRGB,1.0f),type,deltaTimer.getElapsedTime(),0.0f, scaledCoordinates};
                        cubeMatrices.push_front(shape);
                    if(!shapesFull) {
                        shapesFull = true;
                        alarm.setWaitTime(SHAPE_SPAWN_TIME);
                    }
                }

            }
        }
        // Delete cubes
        while (cubeMatrices.size() > MAXIMUM_BATCHES*SHAPES_IN_BATCH*6){
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

Alarm::Alarm(float waitTime) : waitTime(waitTime)
{
}

void Alarm::setWaitTime(float waitTime)
{
    this->waitTime = waitTime;
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

void drawCubeMatrices(LightingEnvironment lightingEnvironment, std::vector<ShapeProperties> cubeMatrices, std::map<SHAPE_TYPE, OpenGLShape*> shapes,  DeltaTimer deltaTimer){

    lightingEnvironment.object_shader.setUniform("elapsedTime", (float)deltaTimer.getElapsedTime());
    // Draw cubes
    glDepthMask(false);
    for( ShapeProperties shape : cubeMatrices){
        lightingEnvironment.object_shader.setUniform("creationDate", (float)shape.creationTimeStamp);
       // float distance = fade;
       // glm::vec3 normDir(1.0f,0.0f,0.0f);
       // model = glm::translate(model, ((10.0f*distance)*normDir)-10.0f*normDir);



        lightingEnvironment.object_shader.setUniform("model", shape.model);
        lightingEnvironment.object_shader.setUniform("aColor", shape.color);
        shapes[shape.type]->draw(lightingEnvironment.object_shader);
    }
    glDepthMask(true);
}
void endRenderLoop(GLFWwindow*& mWindow, DeltaTimer deltaTimer, Camera& camera){
    // Flip Buffers and Draw
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
    std::cout << "FPS: " << 1/deltaTimer.getDeltaTime() << std::endl;
    processInput(mWindow, camera, deltaTimer.getDeltaTime());
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
    Alarm alarm(SHAPE_INITIAL_SPAWN_TIME);
    bool shapesFull = false;
    
    unsigned int diffuse_sampler_2d;
    //create_texture(diffuse_sampler_2d, "Glitter/Textures/container2.png", "png");
    generate_texture(diffuse_sampler_2d, "Glitter/Textures/container2.png", "png");


    
    
    std::vector<unsigned int> objectSizes{3, 2};
    OpenGLShape cubeShape((unsigned int) 36, (unsigned int)36, shapes::textured_cube, shapes::cube_ind, objectSizes, diffuse_sampler_2d, true);
    OpenGLShape pyramidShape((unsigned int) 12, (unsigned int)12, shapes::textured_triangle_pyramid, shapes::triangle_pyramid_ind, objectSizes, diffuse_sampler_2d, true);
    OpenGLShape octahedronShape((unsigned int) 24, (unsigned int)24, shapes::textured_octahedron, shapes::octahedron_ind, objectSizes, diffuse_sampler_2d, true);
    
    std::map<SHAPE_TYPE, OpenGLShape*> shapeTypes{{CUBE , &cubeShape}, {PYRAMID, &pyramidShape}, {OCTAHEDRON, &octahedronShape}};
    
    
    
    
    
    
    LightingEnvironment lightingEnvironment = initMPV("Glitter/Shaders/texture-mvp-color.vs", "Glitter/Shaders/texture-mvp-color.fs");

    lightingEnvironment.object_shader.use();
    lightingEnvironment.object_shader.setUniform("SHAPE_SPAWN_TIME", (float) SHAPE_SPAWN_TIME);
    lightingEnvironment.object_shader.setUniform("MAXIMUM_BATCHES", (float) MAXIMUM_BATCHES);
    lightingEnvironment.object_shader.setUniform("SHAPE_MAX_FADE", (float) SHAPE_MAX_FADE);
    lightingEnvironment.object_shader.setUniform("ourTexture1", (unsigned) 0);
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
            SHAPE_TYPE type = emotionArrayToShapeType(array);
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
                        ShapeProperties shape = ShapeProperties{model,glm::vec4(emotionRGB,1.0f),type,deltaTimer.getElapsedTime(),0.0f, scaledCoordinates};
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
        while (cubeMatrices.size() > MAXIMUM_BATCHES*SHAPES_IN_BATCH*6){
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
    Alarm alarm(SHAPE_INITIAL_SPAWN_TIME);
    bool shapesFull = false;
    
    unsigned int diffuse_sampler_2d;
    //create_texture(diffuse_sampler_2d, "Glitter/Textures/container2.png", "png");
    generate_texture(diffuse_sampler_2d, "Glitter/Textures/container2.png", "png");


    
    
    std::vector<unsigned int> objectSizes{3, 2};
    OpenGLShape cubeShape((unsigned int) 36, (unsigned int)36, shapes::textured_cube, shapes::cube_ind, objectSizes, diffuse_sampler_2d, true);
    OpenGLShape pyramidShape((unsigned int) 12, (unsigned int)12, shapes::textured_triangle_pyramid, shapes::triangle_pyramid_ind, objectSizes, diffuse_sampler_2d, true);
    OpenGLShape octahedronShape((unsigned int) 24, (unsigned int)24, shapes::textured_octahedron, shapes::octahedron_ind, objectSizes, diffuse_sampler_2d, true);
    
    std::map<SHAPE_TYPE, OpenGLShape*> shapeTypes{{CUBE , &cubeShape}, {PYRAMID, &pyramidShape}, {OCTAHEDRON, &octahedronShape}};
    
    
    
    
    
    
    LightingEnvironment lightingEnvironment = initMPV("Glitter/Shaders/texture-mvp-color.vs", "Glitter/Shaders/texture-mvp-color.fs");

    lightingEnvironment.object_shader.use();
    lightingEnvironment.object_shader.setUniform("SHAPE_SPAWN_TIME", (float) SHAPE_SPAWN_TIME);
    lightingEnvironment.object_shader.setUniform("MAXIMUM_BATCHES", (float) MAXIMUM_BATCHES);
    lightingEnvironment.object_shader.setUniform("SHAPE_MAX_FADE", (float) SHAPE_MAX_FADE);
    lightingEnvironment.object_shader.setUniform("ourTexture1", (unsigned) 0);
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
                SHAPE_TYPE type;
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
                        ShapeProperties shape = ShapeProperties{model,glm::vec4(emotionRGB,1.0f),type,deltaTimer.getElapsedTime(),0.0f, scaledCoordinates};
                        cubeMatrices.push_front(shape);
                    if(!shapesFull) {
                        shapesFull = true;
                        alarm.setWaitTime(SHAPE_SPAWN_TIME);
                    }
                }

            }
        }
        // Delete cubes
        while (cubeMatrices.size() > MAXIMUM_BATCHES*SHAPES_IN_BATCH*6){
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