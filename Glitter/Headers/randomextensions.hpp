#ifndef RANDOMEXTENSIONS_H
#define RANDOMEXTENSIONS_H
#include <random>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <geometry.hpp>
// #Random -------------------------------------------------------------

glm::vec3 randScale(float min=0.0f, float max=1.0f);
glm::vec3 randCoords();
glm::vec2 randCoords2D();
float rand_range_uniform(float min, float max);
unsigned int rand_range_uniform(unsigned int min, unsigned int max);
float rand_range_normal(float mean, float stddev);
glm::vec3 randRGB();
glm::vec4 randRGBA();

#endif