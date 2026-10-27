// Local Headers
#include "glitter.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include <iterator>
#include <shader.hpp>
#include <helpers.hpp>

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <stb_image.h>
#include <stdexcept>




int main(int argc, char * argv[]) {
    // Tell GLFW to use OpenGL ES instead of regular Desktop OpenGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    mainCoordsBasic2D(argc, argv);
}
