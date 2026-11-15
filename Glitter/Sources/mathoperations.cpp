#include <mathoperations.hpp>
float clamp(const float x, const float min, const float max){
    if(x < min) return min;
    if(x > max) return max;
    return x;
}
float floorWrap(float x, float y)
{
    return floor(x/y);
}

float floorWrap(float quotient)
{
    return floor(quotient);
}

float ceilWrap(float x, float y)
{
    return ceil(x/y);
}

float ceilWrap(float dividend)
{
    return ceil(dividend);
}

int floorWrap(int x, int y) {
    int mod = x % y;
    if(mod == 0)
        return x/y;
    else
        return (x-mod)/y;
}

int ceilWrap(int x, int y)
{
    int mod = x % y;
    if(mod == 0)
        return x/y;
    else
        return (x-mod)/y + 1;
}
