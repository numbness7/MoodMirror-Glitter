#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/ext.hpp>
#include <variablemanipulation.hpp>
#include <stdexcept>
#include <mathoperations.hpp>
// #CONSTANTS -------------------------------------------------------------

namespace dir3D{
    const glm::vec3 UP(0.0f,1.0f,0.0f);
    const glm::vec3 DOWN(0.0f,-1.0f,0.0f);
    const glm::vec3 RIGHT(1.0f,0.0f,0.0f);
    const glm::vec3 LEFT(-1.0f,0.0f,0.0f);
    const glm::vec3 UP_RIGHT = glm::normalize(UP + RIGHT);
    const glm::vec3 DOWN_LEFT = glm::normalize(DOWN + LEFT);
    const glm::vec3 FORWARD(0.0f, 0.0f, -1.0f);
    const glm::vec3 BACKWARD(0.0f, 0.0f, 1.0f);
}

namespace dir2D{
    const glm::vec2 UP(0.0f,1.0f);
    const glm::vec2 DOWN(0.0f,-1.0f);
    const glm::vec2 RIGHT(1.0f,0.0f);
    const glm::vec2 LEFT(-1.0f,0.0f);
    const glm::vec2 UP_RIGHT = glm::normalize(UP + RIGHT);
    const glm::vec2 DOWN_LEFT = glm::normalize(DOWN + LEFT);
}
// #Linear Transformations -------------------------------------------------------------

float boundTransformation(float x, std::tuple<float,float> line);
std::tuple<float,float> linearTransformation(float min, float max, float minT, float maxT);
float boundTransformation(float x, float slope, float intercept);

/// @brief Convert the range from [min, max] to [minT, maxT].
/// 
/// Useful for normalizing ranges and transforming normalized ranges to other ranges.
/// @param x Number in range [min, max].
/// @param min 
/// @param max 
/// @param minT 
/// @param maxT 
/// @return x that was transformed to be within range [minT, maxT].
float boundTransformation(float x, float min, float max, float minT, float maxT);
/// @brief Obtain a texture that encodes the distance from the border at each pixel.
/// @param pngData stores texture data.
/// @param borderVertices vertices of shape border in clockwise order.
/// @return reference to OpenGL texture.
glm::vec2 boundTransformation(glm::vec2 pos, glm::vec2 min, glm::vec2 max, glm::vec2 minT, glm::vec2 maxT);
bool tryGetIntersection(glm::vec2& intersection, const std::tuple<glm::vec2,glm::vec2> lineSegment1, const std::tuple<glm::vec2, glm::vec2> lineSegment2, bool lineSegmentsDefineLines=false);
bool tryGetIntersection(glm::vec2& intersection, const std::tuple<float, float, float> line1, const std::tuple<float, float, float> line2);
bool hasIntersection(const std::tuple<glm::vec2,glm::vec2> lineSegment1, const std::tuple<glm::vec2, glm::vec2> lineSegment2, const glm::vec2 lineIntersection);
bool hasIntersection(const std::tuple<float, float, float> line1, const std::tuple<float, float, float> line2);
/// @brief Returns distance from point to line
/// @param line a line defined by Ax + By + C = 0. A B and C are passed as a tuple.
/// @param point 
/// @return 
float distFromLine(std::tuple<float,float,float> line, glm::vec2 point);
bool hasIntersection(const std::tuple<glm::vec2,glm::vec2> lineSegment1, const std::tuple<glm::vec2, glm::vec2> lineSegment2, bool lineSegmentsDefineLines);
void generateDistancesFromBorder(std::vector<float>& distances, int width, int height, const std::vector<glm::vec2>& borderVertices);
std::tuple<float,float,float> getLineFromVertices(const float xi, const float xf, const float yi, const float yf);
std::tuple<float,float,float> getLineFromVertices(const glm::vec2 vi, const glm::vec2 vf);
bool isInSquare(glm::vec2 point, glm::vec2 center,  float length);
bool isInSquare(float x, float y, float centerX, float centerY, float length);
bool isOnLine(std::tuple<float,float,float> line, glm::vec2 point);
glm::vec2 rotate(const glm::vec2& vec2, const float& radians);
namespace glm{
    inline float angle(const glm::vec2& vec2) { return glm::atan(vec2.y, vec2.x); }
    glm::mat3 rotate(glm::mat3 mat3, float angle);
    glm::mat3 scale(glm::mat3 mat3, glm::vec2 vec2);
    glm::mat3 translate(glm::mat3 mat3, glm::vec2 vec2);
    glm::vec2 operator*(glm::mat3 mat3, glm::vec2 vec2);
    glm::vec2 vec3ToVec2(glm::vec3 vec3);
};
#endif