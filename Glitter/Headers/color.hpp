#ifndef LOTS_OF_COLOR_H
#define LOTS_OF_COLOR_H

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include <iostream>
#include <string>
namespace cdata{
    struct Color{
        const unsigned char red;
        const unsigned char blue;
        const unsigned char green;
        const unsigned char alpha;
    };
    const unsigned char FULL_COLOR = 0xFF;
    const unsigned char EMPTY_COLOR = 0x00;
    std::string col2str(Color col);
}

namespace cvec4{
    const glm::vec4 FULL_COLOR(1.0f);
}


#endif