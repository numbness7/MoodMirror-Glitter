#ifndef MATHOPERATIONS_H
#define MATHOPERATIONS_H
#include <math.h>
#include <cmath>
float clamp(const float x, const float min, const float max);
float floorWrap(float x, float y);
float floorWrap(float quotient);
float ceilWrap(float x, float y);
float ceilWrap(float dividend);
int floorWrap(int x, int y);
int ceilWrap(int x, int y);
#endif