#include "math.h"

/*
    Si queremos rotar el vector un angulo S, las nuevas coordenadas seran
    x = x1 + r*cos(Q+S)
    y = y1 + r*sen(Q+S)

*/
void rotar(float &x, float &y, float cx, float cy, float da){
    //da es el angulo de rotacion en grados
    float dx = x - cx;
    float dy = y - cy;
    float r = sqrt(dx * dx + dy * dy);  //longitud del vector
    float a = atan2(dy, dx);

    //convertimos da a radianes
    float da_rad = da / 180 * M_PI;
    //En allegro esta invertido por eso no se suma, se resta
    x = cx + r * cos(a - da_rad);
    y = cy + r * sin(a - da_rad);
}

/*
 * da: angulo en grados que rota la nave.
 */
void aceleracion(float da, float &vx, float &vy){
    float ax = 0;
    float ay = -0.15;
    rotar(ax, ay, 0, 0, da);
    vx += ax;
    vy += ay;
}

