#ifndef SHAPES
#define SHAPES
#include <glm/vec3.hpp>

namespace shapes {
    extern const float e_tri[];
    extern const unsigned int e_tri_ind[];
    extern const float rect[];
    extern const float rect_c_t[];
    extern const float rect_t[];
    extern const float e_tri_t[];
    extern const unsigned int rect_ind[];
    extern const float textured_cube[];
    extern const float normal_cube[];
    extern const float cube[];
    extern const float normal_textured_cube[];
    extern const unsigned int cube_ind[];
    extern const std::vector<glm::vec3> cubePositions;
    extern const unsigned int normal_textured_triangle_pyramid_ind[];
    extern const float normal_textured_triangle_pyramid[];
    extern const unsigned int normal_triangle_pyramid_ind[];
    extern const float normal_triangle_pyramid[];
    extern const unsigned int triangle_pyramid_ind[];
    extern const float textured_triangle_pyramid[];
    extern const float triangle_pyramid[];
    extern const unsigned int octahedron_ind[];
    extern const float octahedron[];
    extern const float textured_octahedron[];
};
#endif