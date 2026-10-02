
#include <helpers.hpp>


// Macros
#define SMALL_FLOAT 0.00000001f

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

struct CubePair {
    glm::mat4 model;
    glm::vec4 color;
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

        glfwSetInputMode(mWindow, GLFW_CURSOR, mouseCaptured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
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
void create_a_shape(unsigned int &VAO, unsigned int &VBO, unsigned int& EBO, const unsigned int ind_cnt, const float vertices[], const unsigned int indices[], const std::vector<unsigned int>& attributeSizes){
    unsigned int totalSize = 0;
    for(unsigned int count : attributeSizes){
        totalSize += count;
    }

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, totalSize*ind_cnt*sizeof(float), vertices, GL_STATIC_DRAW);
    
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
                        data[i*width*3+j+color] = 0x01;
                    if(color == 1)
                        data[i*width*3+j+color] = 0x01;
                    if(color == 2)
                        data[i*width*3+j+color] = 0x01;
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

GLFWwindow* initOpenGL(int& return_status, int width, int height){
    // Load GLFW and Create a Window
    auto mWindow = loadGLFWCreateWindow(width, height);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return_status = EXIT_FAILURE;
        return mWindow;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
    // Source - https://stackoverflow.com/a/1617379
    // Posted by Goz
    // Retrieved 2026-09-26, License - CC BY-SA 2.5
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );


    
    unsigned int VBO_O, VAO_O, EBO_O;
    

    glEnable(GL_DEPTH_TEST);
    return_status = EXIT_SUCCESS;
    return mWindow;
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
    auto mWindow = initOpenGL(return_status, mWidth, mHeight);
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
    auto mWindow = initOpenGL(return_status, mWidth, mHeight);
    if(return_status != EXIT_SUCCESS) return return_status;
    
    DeltaTimer deltaTimer{};
    

    
    unsigned int texture1;
    create_texture(texture1, "Glitter/Textures/container.jpg", "jpg");
    unsigned int texture2;
    create_texture(texture2, "Glitter/Textures/awesomeface.png", "png");


    Shader shaderDoubleTextureMVP("Glitter/Shaders/double-texture-mvp.vs", "Glitter/Shaders/double-texture.fs");
    
    unsigned int VBO_T, VAO_T, EBO_T;
    
    
    
    
    create_textured_shape(VAO_T, VBO_T, EBO_T, shapes::textured_cube, 5*36, shapes::cube_ind, 36);
    
    

    shaderDoubleTextureMVP.use();
    shaderDoubleTextureMVP.setUniform("ourTexture1",(unsigned int)0);
    shaderDoubleTextureMVP.setUniform("ourTexture2",(unsigned int)1);
    
    glm::mat4 proj;
    proj = glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.1f, 100.0f);

    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));

    glm::mat4 model(1.0f);

    shaderDoubleTextureMVP.setUniform("projection", proj);
    shaderDoubleTextureMVP.setUniform("view", view);
    shaderDoubleTextureMVP.setUniform("model",model);
    
    
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
        shaderDoubleTextureMVP.setUniform("projection",proj);
        

        view = camera.GetViewMatrix();
        shaderDoubleTextureMVP.setUniform("view",view);


        for(unsigned int i = 0; i < std::size(shapes::cubePositions); i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, shapes::cubePositions[i]);
            model = glm::rotate(model, glm::radians(15.0f)*(float)i, glm::vec3(0.4f,0.95f,0.2f));
            shaderDoubleTextureMVP.setUniform("model",model);
            drawDoubleTexturedShape(VAO_T, EBO_T, shaderDoubleTextureMVP, 36, texture1, texture2);
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
    auto mWindow = initOpenGL(return_status, mWidth, mHeight);
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
    auto mWindow = initOpenGL(return_status, mWidth, mHeight);
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
    auto mWindow = initOpenGL(return_status, mWidth, mHeight);
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
    std::cout << "point_on_path: " << point_on_path.x << " " << point_on_path.y << " " << point_on_path.z << std::endl;
    return start_point + change_vector;
}

int mainPathCube(int argc, char * argv[]){
    
    // Initialize OpenGL
    int return_status = EXIT_SUCCESS;
    auto mWindow = initOpenGL(return_status, mWidth, mHeight);
    if(return_status != EXIT_SUCCESS) return return_status;
    
    DeltaTimer deltaTimer{};
    

    
    unsigned int texture1;
    generate_texture(texture1, "Glitter/Textures/awesomeface.png", "png");



    Shader shaderTextureMVP("Glitter/Shaders/texture-mvp.vs", "Glitter/Shaders/texture-mvp.fs");
    
    unsigned int VBO_T, VAO_T, EBO_T;
    
    
    
    
    create_textured_shape(VAO_T, VBO_T, EBO_T, shapes::textured_triangle_pyramid, 5*36, shapes::textured_triangle_pyramid_ind, 36);
    
    

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
    auto mWindow = initOpenGL(return_status, mWidth, mHeight);
    if(return_status != EXIT_SUCCESS) return return_status;
    
    DeltaTimer deltaTimer{};
    
    unsigned int diffuse_sampler_2d;
    unsigned int specular_sampler_2d;
    generateSampler2Ds(diffuse_sampler_2d, specular_sampler_2d);


    
    
    std::vector<unsigned int> lightSizes{};
    lightSizes.push_back(3);
    OpenGLShape lightShape((unsigned int)36, shapes::cube, shapes::cube_ind, lightSizes, (unsigned int)0, false);
    std::vector<unsigned int> objectSizes{3,3,2};
    OpenGLShape objectShape((unsigned int)36, shapes::normal_textured_cube, shapes::cube_ind, objectSizes, diffuse_sampler_2d, true);
    
    
    
    
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


    
    std::deque<CubePair> cubeMatrices{};
    

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
        glm::vec3 emotionRGB = emotionArrayToColor(emotionArray);
        glm::vec3 rotationAxis = glm::vec3(sin(deltaTimer.getElapsedTime()*1.0f + M_1_PI),-sin(deltaTimer.getElapsedTime()*1.0f/2.7865432f + M_1_PI/2),sin(deltaTimer.getElapsedTime()*1.0f/3.125105f + M_1_PI/3));
        rotatedModel = glm::rotate(glm::mat4(1.0f), (float)(1.0f*((float)(M_PI))), rotationAxis);
        std::cout << "rotation axis: " << "(" << rotationAxis.x << "," << rotationAxis.y << "," << rotationAxis.z << ")" << std::endl;

        
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

        cubeMatrices.push_front(CubePair{lightingEnvironment.model,glm::vec4(emotionRGB,0.5f)});
        
        // Delete a cube
        if (cubeMatrices.size() > 10000){
            cubeMatrices.pop_back();
        }

        // Draw cubes
        for( CubePair cubePair : cubeMatrices){
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
    auto mWindow = initOpenGL(return_status, mWidth, mHeight);
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

glm::vec3 emotionArrayToColor(std::vector<std::vector<float>> emotionArray){
    std::vector<float> array0 = emotionArray[0];
    float red   =   0.5f + (array0[3] - array0[0])/2;
    float green =   0.5f + (array0[4] - array0[1])/2;
    float blue  =   0.5f + (array0[5] - array0[2])/2;
    return glm::vec3(red,green,blue);
    
}
glm::vec3 emotionArrayToVec3(std::vector<std::vector<float>> emotionArray){
    std::vector<float> array0 = emotionArray[0];
    float x = array0[3] - array0[0];
    float y = array0[4] - array0[1];
    float z = array0[5] - array0[2];
    return glm::vec3(x,y,z);
}
void initMouse(GLFWwindow* mWindow){
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);
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
   object_shader.setUniform("spot_light.cut_off", glm::cos(glm::radians(14.5f)));
   object_shader.setUniform("spot_light.outer_cut_off", glm::cos(glm::radians(20.0f)));


    object_shader.setUniform("dir_light.ambient",  glm::vec3(0.1f));
    object_shader.setUniform("dir_light.diffuse",  glm::vec3(0.2f)); // darken diffuse light a bit
    object_shader.setUniform("dir_light.specular", glm::vec3(0.2f)); 
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
void setupLightingEnvironmentToDraw(LightingEnvironment& lightingEnvironment, glm::vec3 light_positions[POINT_LIGHT_COUNT], glm::vec3 diffuses[POINT_LIGHT_COUNT], OpenGLShape& lightShape){

    lightingEnvironment.projection = glm::perspective(glm::radians(camera.Zoom), (float)mWidth / (float)mHeight, 0.1f, 100.0f);
    lightingEnvironment.view = camera.GetViewMatrix();
    
    
    

    lightingEnvironment.light_shader.use();
    lightingEnvironment.light_shader.setUniform("projection",lightingEnvironment.projection);
    lightingEnvironment.light_shader.setUniform("view",lightingEnvironment.view);

    for(int i = 0; i < POINT_LIGHT_COUNT; i++){
        lightingEnvironment.model = glm::mat4(1.0f);
        lightingEnvironment.model = glm::translate(lightingEnvironment.model, light_positions[i]);
        lightingEnvironment.model = glm::scale(lightingEnvironment.model, glm::vec3(0.2f));
        lightingEnvironment.light_shader.setUniform("model", lightingEnvironment.model);
        lightingEnvironment.light_shader.setUniform("aColor", diffuses[i]);
        lightShape.draw(lightingEnvironment.light_shader);
    }

    lightingEnvironment.object_shader.use();
    lightingEnvironment.object_shader.setUniform("view",lightingEnvironment.view);
    lightingEnvironment.object_shader.setUniform("projection",lightingEnvironment.projection);
    lightingEnvironment.object_shader.setUniform("view_pos", camera.Position);
   lightingEnvironment.object_shader.setUniform("spot_light.position", camera.Position);
   lightingEnvironment.object_shader.setUniform("spot_light.direction", camera.Front);
   lightingEnvironment.object_shader.setUniform("spot_light.is_on", (bool)flash_light);
}
OpenGLShape::OpenGLShape(unsigned int ind_cnt, const float vertices[], const unsigned int indices[], const std::vector<unsigned int>& attributeSizes, unsigned int texture, bool hasTexture){
    this->ind_cnt = ind_cnt;
    this->texture = texture;
    this->hasTexture = hasTexture;
    create_a_shape(this->VAO, this->VBO, this->EBO, this->ind_cnt, &vertices[0], &indices[0], attributeSizes);
}
void OpenGLShape::draw(Shader shader){
    if(hasTexture)
        drawTexturedShape(VAO, EBO, shader, ind_cnt, texture);
    else
        drawShape(VAO, EBO, shader, ind_cnt);
}
int mainCoords(int argc, char * argv[]){
    // Initialize OpenGL
    int return_status = EXIT_SUCCESS;
    auto mWindow = initOpenGL(return_status, mWidth, mHeight);
    if(return_status != EXIT_SUCCESS) return return_status;
    
    DeltaTimer deltaTimer{};
    
    unsigned int diffuse_sampler_2d;
    unsigned int specular_sampler_2d;
    generateSampler2Ds(diffuse_sampler_2d, specular_sampler_2d);


    
    
    std::vector<unsigned int> lightSizes{};
    lightSizes.push_back(3);
    OpenGLShape lightShape((unsigned int)36, shapes::cube, shapes::cube_ind, lightSizes, (unsigned int)0, false);
    std::vector<unsigned int> objectSizes{3,3,2};
    OpenGLShape objectShape((unsigned int)36, shapes::normal_textured_cube, shapes::cube_ind, objectSizes, diffuse_sampler_2d, true);
    
    
    
    
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


    
    std::deque<CubePair> cubeMatrices{};
    


    
    
    // Rendering Loop
    while (!glfwWindowShouldClose(mWindow)) {

        // Background Fill Color
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        setupLightingEnvironmentToDraw(lightingEnvironment, light_positions, diffuses, lightShape);



        std::vector<std::vector<float>> emotionArray = readEmotionArray("../data.json");
        
        // Use emotion array to set properties of shapes
        glm::vec3 emotionRGB = emotionArrayToColor(emotionArray);
        glm::vec3 coords = emotionArrayToVec3(emotionArray);
        
        lightingEnvironment.model = glm::mat4(1.0f);
        float scaleCoords = 2.0f;
        lightingEnvironment.model = glm::translate(lightingEnvironment.model, scaleCoords*coords);

        

        cubeMatrices.push_front(CubePair{lightingEnvironment.model,glm::vec4(emotionRGB,0.5f)});
        
        // Delete a cube
        if (cubeMatrices.size() > 10000){
            cubeMatrices.pop_back();
        }

        // Draw cubes
        for( CubePair cubePair : cubeMatrices){
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