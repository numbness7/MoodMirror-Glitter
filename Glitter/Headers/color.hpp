#ifndef COLOR_H
#define COLOR_H

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
namespace cdata{
    struct color{
        unsigned char red;
        unsigned char blue;
        unsigned char green;
        unsigned char alpha;
    };
    extern const unsigned char FULL_COLOR;
    extern const unsigned char EMPTY_COLOR;
}

namespace cvec4{
    extern const glm::vec4 FULL_COLOR;
}


#endif