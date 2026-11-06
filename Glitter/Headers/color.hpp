#ifndef LOTS_OF_COLOR_H
#define LOTS_OF_COLOR_H

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include <iostream>
#include <string>
namespace cdata{
    unsigned char vec3PartToColorPart(const glm::vec3 vec3, const unsigned char part);
    unsigned char vec4PartToColorPart(const glm::vec4 vec4, const unsigned char part);
    struct ColorRGBA{
        ColorRGBA(glm::vec4 vec4) : red{vec4PartToColorPart(vec4,0)}, green{vec4PartToColorPart(vec4, 1)}, blue{vec4PartToColorPart(vec4, 2)}, alpha{vec4PartToColorPart(vec4, 3)} {};
        ColorRGBA(const unsigned char red, const unsigned char green, const unsigned char blue, unsigned char alpha) : red{red}, green{green}, blue{blue}, alpha{alpha} {};
        const unsigned char red;
        const unsigned char green;
        const unsigned char blue;
        const unsigned char alpha;
        bool operator==(const ColorRGBA& color) const;
    };
    struct ColorRGB{
        ColorRGB(glm::vec3 vec3) : red{vec3PartToColorPart(vec3,0)}, green{vec3PartToColorPart(vec3, 1)}, blue{vec3PartToColorPart(vec3, 2)} {};
        ColorRGB(const unsigned char red, const unsigned char green, const unsigned char blue) : red{red}, green{green}, blue{blue} {};
        const unsigned char red;
        const unsigned char green;
        const unsigned char blue;
    };
    const unsigned char FULL_COLOR_D = 0xFF;
    const unsigned char EMPTY_COLOR_D = 0x00;
    const ColorRGBA WHITE_D_RGBA(FULL_COLOR_D,FULL_COLOR_D,FULL_COLOR_D,FULL_COLOR_D);
    const ColorRGBA RED_D_RGBA(FULL_COLOR_D,EMPTY_COLOR_D,EMPTY_COLOR_D,FULL_COLOR_D);
    const ColorRGBA GREEN_D_RGBA(EMPTY_COLOR_D,FULL_COLOR_D,EMPTY_COLOR_D,FULL_COLOR_D);
    const ColorRGBA BLUE_D_RGBA(EMPTY_COLOR_D,EMPTY_COLOR_D,FULL_COLOR_D,FULL_COLOR_D);
    const ColorRGBA BLACK_D_RGBA(EMPTY_COLOR_D,EMPTY_COLOR_D,EMPTY_COLOR_D,FULL_COLOR_D);
    const ColorRGB WHITE_D_RGB(FULL_COLOR_D,FULL_COLOR_D,FULL_COLOR_D);
    const glm::vec3 WHITE_V3(1.0f);
    const glm::vec3 RED_V3(1.0f,0.0f,0.0f);
    const glm::vec3 GREEN_V3(0.0f,1.0f,0.0f);
    const glm::vec3 BLUE_V3(0.0f,0.0f,1.0f);
    const glm::vec3 YELLOW_V3 = RED_V3 + GREEN_V3;
    const glm::vec3 DARK_YELLOW_V3 = YELLOW_V3/2.0f;
    const glm::vec3 PURPLE_V3 = RED_V3 + BLUE_V3;
    const glm::vec3 DARK_PURPLE_V3 = PURPLE_V3/2.0f;
    const glm::vec3 TURQUOISE_V3 = GREEN_V3 + BLUE_V3;
    const glm::vec3 DARK_TURQUOISE_V3 = TURQUOISE_V3/2.0f;
    const glm::vec3 BLACK_V3(0.0f);
    const glm::vec4 BLACK_V4(BLACK_V3, 1.0f);
    const glm::vec4 WHITE_V4(1.0f);
    const glm::vec4 RED_V4(RED_V3,1.0f);
    const glm::vec4 GREEN_V4(GREEN_V3,1.0f);
    const glm::vec4 BLUE_V4(BLUE_V3,1.0f);
    const glm::vec4 DARK_BLUE_V4 = glm::vec4(0.0f,0.0f,0.5f, 1.0f);
    const glm::vec4 PURPLE_V4 = glm::vec4(PURPLE_V3,1.0f);
    const glm::vec4 DARK_PURPLE_V4 = glm::vec4(PURPLE_V3/2.0f,1.0f);
    const glm::vec4 YELLOW_V4 = glm::vec4(YELLOW_V3,1.0f);
    const glm::vec4 TURQUOISE_V4 = glm::vec4(TURQUOISE_V3,1.0f);
    const glm::vec4 DARK_TURQUOISE_V4 = glm::vec4(DARK_TURQUOISE_V3,1.0f);
    const glm::vec4 DARK_YELLOW_V4 = YELLOW_V4/2.0f + glm::vec4(glm::vec3(0.0f),0.5f);
    std::string col2str(ColorRGBA col);
    const ColorRGBA vec4ToCol(const glm::vec4 vec4);
}
namespace cvec4 = cdata;

#endif