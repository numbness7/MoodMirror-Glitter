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
#include <json.hpp>
#include <third_party.hpp>
#include "helpers.hpp"






int main(int argc, char * argv[]) {
    return mainGrid(argc,argv);
}
