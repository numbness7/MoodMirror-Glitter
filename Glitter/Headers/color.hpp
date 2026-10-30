#ifndef LOTS_OF_COLOR_H
#define LOTS_OF_COLOR_H

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include <iostream>
#include <string>
namespace cdata{
    struct ColorRGBA{
        const unsigned char red;
        const unsigned char blue;
        const unsigned char green;
        const unsigned char alpha;
    };
    struct ColorRGB{
        const unsigned char red;
        const unsigned char blue;
        const unsigned char green;
    };
    const unsigned char FULL_COLOR_D = 0xFF;
    const unsigned char EMPTY_COLOR_D = 0x00;
    const ColorRGBA WHITE_D_RGBA(FULL_COLOR_D,FULL_COLOR_D,FULL_COLOR_D,FULL_COLOR_D);
    const ColorRGBA BLACK_D_RGBA(EMPTY_COLOR_D,EMPTY_COLOR_D,EMPTY_COLOR_D,FULL_COLOR_D);
    const ColorRGB WHITE_D_RGB(FULL_COLOR_D,FULL_COLOR_D,FULL_COLOR_D);
    std::string col2str(ColorRGBA col);
    const glm::vec3 WHITE_V3(1.0f);
    const glm::vec3 RED_V3(1.0f,0.0f,0.0f);
    const glm::vec3 GREEN_V3(0.0f,1.0f,0.0f);
    const glm::vec3 BLUE_V3(0.0f,0.0f,1.0f);
    const glm::vec3 BLACK_V3(0.0f);
    const glm::vec4 BLACK_V4(BLACK_V3, 1.0f);
    const glm::vec4 WHITE_V4(1.0f);
    const glm::vec4 RED_V4(RED_V3,1.0f);
    const glm::vec4 GREEN_V4(GREEN_V3,1.0f);
    const glm::vec4 BLUE_V4(BLUE_V3,1.0f);
}
namespace cvec4 = cdata;



#endif