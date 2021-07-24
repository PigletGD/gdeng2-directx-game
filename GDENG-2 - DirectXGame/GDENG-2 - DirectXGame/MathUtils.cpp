#include "MathUtils.h"
#include <stdlib.h>

const float PI = 3.1416f;

float MathUtils::randomFloat(float a, float b)
{
    float random = ((float)rand()) / ((float)RAND_MAX);
    float diff = b - a;
    float r = random * diff;

    return a + r;
}

float MathUtils::randomInt(int a, int b)
{
    int range = a - b + 1;
    int num = rand() % range + a;

    return num;
}

float MathUtils::RadToDeg(float angle)
{
    return (angle * 180.0f) / PI;
}

float MathUtils::DegToRad(float angle)
{
    return (angle * PI) / 180.0f;
}
