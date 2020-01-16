#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED

#include <vector>
#include <math.h>

void rotar(float &x, float &y, float cx, float cy, float da);
void aceleracion(float da, float &vx, float &vy);
void get_puntos_hipotenusa(std::vector<float> puntos, float &x1, float &y1, float &x2, float &y2);

#endif // MATH_H_INCLUDED
