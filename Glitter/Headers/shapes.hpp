#ifndef SHAPES
#define SHAPES
#include <glm/vec3.hpp>
#include <vector>

namespace shapes {
    class Shape {
        public:
            Shape(const float* vertices, const unsigned int vertCnt, const std::vector<unsigned int>& attributeSizes, 
                const unsigned int* indices, const unsigned int indCnt) 
                : vertices{vertices}, vertCnt{vertCnt}, attributeSizes{attributeSizes}, indices{indices}, indCnt{indCnt}{};

            unsigned int getStride() const{
                unsigned int sum = 0;
                for(const auto size : attributeSizes){
                    sum += size;
                }
                return sum;
            }
                const std::vector<unsigned int> attributeSizes;
                const unsigned int vertCnt;
                const unsigned int indCnt;
                const float* vertices;
                const unsigned int * indices;
    };
    const float e_tri[] = {
        0.0f,  0.5f, 0.0f,  // top
        0.5f, -0.5f,  0.0f,  // bottom-right
       -0.5f, -0.5f,  0.0f,  // bottom-left
    };
    const float e_tri_t[] = {
        // Position         Texture Coords
        0.0f,  0.5f,  0.0f,  0.5f, 1.0f, // top-middle
        0.5f, -0.5f,  0.0f,  1.0f, 0.0f, // bottom-right
       -0.5f, -0.5f,  0.0f,  0.0f, 0.0f, // bottom-left
    };
    
    const unsigned int e_tri_ind[] = {
        0, 1, 2
    };
    
    
    const float rectVerts[] = {
        // position       
        -0.5,  0.5f, 0.0f,//top-left
         0.5,  0.5f, 0.0f, //top-right
         0.5, -0.5f, 0.0f, //bottom-right
        -0.5, -0.5f, 0.0f, //bottom-left
    };
    const std::vector<unsigned int> rect_t_attribute_sizes{3, 2};

    const float rect_c_t[] = {
        // position         color           texture coor
        -0.5,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,//top-left
         0.5,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, //top-right
         0.5, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, //bottom-right
        -0.5, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, //bottom-left
    };
    const float rect_t[] = {
        // position           texture coor
        -0.5,  0.5f, 0.0f,  0.0f, 1.0f,//top-left
         0.5,  0.5f, 0.0f,  1.0f, 1.0f, //top-right
         0.5, -0.5f, 0.0f,  1.0f, 0.0f, //bottom-right
        -0.5, -0.5f, 0.0f,  0.0f, 0.0f, //bottom-left
    };
    
    const unsigned int rect_ind[] = {
        0, 1, 2,
        0, 2, 3,

    };
    
    const Shape rectT(rect_t,  sizeof(rect_t)/sizeof(float), rect_t_attribute_sizes,      rect_ind, sizeof(rect_ind)/sizeof(unsigned int));
    const Shape eTriT(e_tri_t, sizeof(e_tri_t)/sizeof(float),std::vector<unsigned>{3,2}, e_tri_ind, 3);
    const float cube[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    };
const float normal_cube[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

const float normal_textured_cube[] = {
            // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
    const float textured_cube[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    const unsigned int cube_ind[] = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        15, 16, 17,
        18, 19, 20,
        21, 22, 23,
        24, 25, 26,
        27, 28, 29,
        30, 31, 32,
        33, 34, 35
    };
    
    const std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

const float normal_triangle_pyramid[] = {
    
    -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f, // Bottom left
     0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f, // Bottom right
     0.0f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.5f, 1.0f, // Bottom back-ward
     
    -0.5f, -0.5f, -0.5f,   0.0f,  1.0f, -2.0f,   0.0f, 0.0f, // Bottom left
     0.5f, -0.5f, -0.5f,   0.0f,  1.0f, -2.0f,   1.0f, 0.0f, // Bottom right
     0.0f,  0.5f,  0.0f,   0.0f,  1.0f, -2.0f,   0.5f, 1.0f, // Bottom back-ward

    -0.5f, -0.5f, -0.5f,   1.0f, -0.25f, -0.5f,   0.0f, 0.0f, // Bottom left
     0.0f, -0.5f,  0.5f,   1.0f, -0.25f, -0.5f,   1.0f, 0.0f, // Bottom right
     0.0f,  0.5f,  0.0f,   1.0f, -0.25f, -0.5f,   0.5f, 1.0f, // Bottom back-ward
    
     0.5f, -0.5f, -0.5f,   1.0f,  0.25f, 0.5f,   0.0f, 0.0f, // Bottom left
     0.0f, -0.5f,  0.5f,   1.0f,  0.25f, 0.5f,   1.0f, 0.0f, // Bottom right
     0.0f,  0.5f,  0.0f,   1.0f,  0.25f, 0.5f,   0.5f, 1.0f, // Bottom back-ward

};
const float triangle_pyramid[] = {
    -0.5f, -0.5f, -0.5f, // Bottom left
     0.5f, -0.5f, -0.5f, // Bottom right
     0.0f, -0.5f,  0.5f, // Bottom back-ward
                                              
    -0.5f, -0.5f, -0.5f, // Bottom left
     0.5f, -0.5f, -0.5f, // Bottom right
     0.0f,  0.5f,  0.0f, // Bottom back-ward
                                              
    -0.5f, -0.5f, -0.5f,  // Bottom left
     0.0f, -0.5f,  0.5f,  // Bottom right
     0.0f,  0.5f,  0.0f,  // Bottom back-ward
                                              
     0.5f, -0.5f, -0.5f, // Bottom left
     0.0f, -0.5f,  0.5f, // Bottom right
     0.0f,  0.5f,  0.0f, // Bottom back-ward

};

const unsigned int normal_triangle_pyramid_ind[] = {
    0, 1, 2,
    3, 4, 5,
    6, 7, 8,
    9, 10, 11,
};
    
const float textured_triangle_pyramid[] = {
    
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom left
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // Bottom right
     0.0f, -0.5f,  0.5f,  0.5f, 1.0f, // Bottom back-ward
     
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom left
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // Bottom right
     0.0f,  0.5f,  0.0f,  0.5f, 1.0f, // Bottom back-ward

    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom left
     0.0f, -0.5f,  0.5f,  1.0f, 0.0f, // Bottom right
     0.0f,  0.5f,  0.0f,  0.5f, 1.0f, // Bottom back-ward
    
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom left
     0.0f, -0.5f,  0.5f,  1.0f, 0.0f, // Bottom right
     0.0f,  0.5f,  0.0f,  0.5f, 1.0f, // Bottom back-ward


};
const float textured_octahedron[] = {
    0.0f,  0.5f,  0.0f, 0.0f, 0.0f,/* Top */
   -0.5f,  0.0f,  0.5f, 1.0f, 0.0f,/* Middle Left Back */
    0.5f,  0.0f,  0.5f, 0.5f, 1.0f,/* Middle Right Back */

    0.0f,  0.5f,  0.0f, 0.0f, 0.0f,/* Top */
   -0.5f,  0.0f, -0.5f, 1.0f, 0.0f,/* Middle Left Forward */
   -0.5f,  0.0f,  0.5f, 0.5f, 1.0f,/* Middle Left Back */
    
    0.0f,  0.5f,  0.0f, 0.0f, 0.0f,/* Top */
   -0.5f,  0.0f, -0.5f, 1.0f, 0.0f,/* Middle Left Forward */
    0.5f,  0.0f, -0.5f, 0.5f, 1.0f,/* Middle Right Forward */

    0.0f,  0.5f,  0.0f, 0.0f, 0.0f,/* Top */
    0.5f,  0.0f, -0.5f, 1.0f, 0.0f,/* Middle Right Forward */
    0.5f,  0.0f,  0.5f, 0.5f, 1.0f,/* Middle Right Back */

    0.0f, -0.5f,  0.0f, 0.0f, 0.0f,/* Bottom */
   -0.5f,  0.0f,  0.5f, 1.0f, 0.0f,/* Middle Left Back */
    0.5f,  0.0f,  0.5f, 0.5f, 1.0f,/* Middle Right Back */

    0.0f, -0.5f,  0.0f, 0.0f, 0.0f,/* Bottom */
   -0.5f,  0.0f, -0.5f, 1.0f, 0.0f,/* Middle Left Forward */
   -0.5f,  0.0f,  0.5f, 0.5f, 1.0f,/* Middle Left Back */
    
    0.0f, -0.5f,  0.0f, 0.0f, 0.0f,/* Bottom */
   -0.5f,  0.0f, -0.5f, 1.0f, 0.0f,/* Middle Left Forward */
    0.5f,  0.0f, -0.5f, 0.5f, 1.0f,/* Middle Right Forward */

    0.0f, -0.5f,  0.0f, 0.0f, 0.0f,/* Bottom */
    0.5f,  0.0f, -0.5f, 1.0f, 0.0f,/* Middle Right Forward */
    0.5f,  0.0f,  0.5f, 0.5f, 1.0f,/* Middle Right Back */
};

const float octahedron[] = {
    0.0f,  0.5f,  0.0f, /* Top */
   -0.5f,  0.0f,  0.5f, /* Middle Left Back */
    0.5f,  0.0f,  0.5f, /* Middle Right Back */

    0.0f,  0.5f,  0.0f, /* Top */
   -0.5f,  0.0f, -0.5f, /* Middle Left Forward */
   -0.5f,  0.0f,  0.5f, /* Middle Left Back */
    
    0.0f,  0.5f,  0.0f, /* Top */
   -0.5f,  0.0f, -0.5f, /* Middle Left Forward */
    0.5f,  0.0f, -0.5f, /* Middle Right Forward */

    0.0f,  0.5f,  0.0f, /* Top */
    0.5f,  0.0f, -0.5f, /* Middle Right Forward */
    0.5f,  0.0f,  0.5f, /* Middle Right Back */

    0.0f, -0.5f,  0.0f, /* Bottom */
   -0.5f,  0.0f,  0.5f, /* Middle Left Back */
    0.5f,  0.0f,  0.5f, /* Middle Right Back */

    0.0f, -0.5f,  0.0f, /* Bottom */
   -0.5f,  0.0f, -0.5f, /* Middle Left Forward */
   -0.5f,  0.0f,  0.5f, /* Middle Left Back */
    
    0.0f, -0.5f,  0.0f, /* Bottom */
   -0.5f,  0.0f, -0.5f, /* Middle Left Forward */
    0.5f,  0.0f, -0.5f, /* Middle Right Forward */

    0.0f, -0.5f,  0.0f, /* Bottom */
    0.5f,  0.0f, -0.5f, /* Middle Right Forward */
    0.5f,  0.0f,  0.5f, /* Middle Right Back */
};

const unsigned int octahedron_ind[] = {
     0,  1,  2,
     3,  4,  5,
     6,  7,  8,
     9, 10, 11,
    12, 13, 14,
    15, 16, 17,
    18, 19, 20,
    21, 22, 23,
};

const unsigned int triangle_pyramid_ind[] = {
    0, 1, 2,
    3, 4, 5,
    6, 7, 8,
    9, 10, 11,
};
const float normal_textured_triangle_pyramid[] = {
    
    -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f, // Bottom left
     0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f, // Bottom right
     0.0f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.5f, 1.0f, // Bottom back-ward
     
    -0.5f, -0.5f, -0.5f,   0.0f,  1.0f, -2.0f,   0.0f, 0.0f, // Bottom left
     0.5f, -0.5f, -0.5f,   0.0f,  1.0f, -2.0f,   1.0f, 0.0f, // Bottom right
     0.0f,  0.5f,  0.0f,   0.0f,  1.0f, -2.0f,   0.5f, 1.0f, // Bottom back-ward

    -0.5f, -0.5f, -0.5f,   1.0f, -0.25f, -0.5f,   0.0f, 0.0f, // Bottom left
     0.0f, -0.5f,  0.5f,   1.0f, -0.25f, -0.5f,   1.0f, 0.0f, // Bottom right
     0.0f,  0.5f,  0.0f,   1.0f, -0.25f, -0.5f,   0.5f, 1.0f, // Bottom back-ward
    
     0.5f, -0.5f, -0.5f,   1.0f,  0.25f, 0.5f,   0.0f, 0.0f, // Bottom left
     0.0f, -0.5f,  0.5f,   1.0f,  0.25f, 0.5f,   1.0f, 0.0f, // Bottom right
     0.0f,  0.5f,  0.0f,   1.0f,  0.25f, 0.5f,   0.5f, 1.0f, // Bottom back-ward

};

const unsigned int normal_textured_triangle_pyramid_ind[] = {
    0, 1, 2,
    3, 4, 5,
    6, 7, 8,
    9, 10, 11,
};
};
#endif