#ifndef MATHOPERATIONS_H
#define MATHOPERATIONS_H
#include <math.h>
#include <cmath>
#define SUPER_SMALL_FLOAT 0.00000000000001f
#define SMALL_FLOAT 0.002f
#define NOT_THAT_SMALL_FLOAT 0.01f
float clamp(const float x, const float min, const float max);
float floorWrap(float x, float y);
float floorWrap(float quotient);
float ceilWrap(float x, float y);
float ceilWrap(float dividend);
int floorWrap(int x, int y);
int ceilWrap(int x, int y);
bool isInRange(float x, float min, float max);
namespace mm_math{
    const float PI = ((float)M_PI);
}
#endif