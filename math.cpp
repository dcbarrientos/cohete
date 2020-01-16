
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

void get_puntos_hipotenusa(std::vector<float> puntos, float &x1, float &y1, float &x2, float &y2){
/*    puntos[0] puntos[1]
    puntos[2] puntos[3]
    puntos[4] puntos[5]*/
    if(puntos[0] != puntos[2] && puntos[1] != puntos[3]){
        x1 = puntos[0];
        y1 = puntos[1];
        x2 = puntos[2];
        y2 = puntos[3];

    }else if(puntos[2] != puntos[4] && puntos[3] != puntos[5]){
        x1 = puntos[2];
        y1 = puntos[3];
        x2 = puntos[4];
        y2 = puntos[5];
    }else {
        x1 = puntos[0];
        y1 = puntos[1];
        x2 = puntos[4];
        y2 = puntos[5];
    }
    float t1, t2;
    if(x1 > x2){
        t1 = x1;
        t2 = y1;
        x1 = x2;
        y1 = y2;
        x2 = t1;
        y2 = t2;
    }
    /*
    if(puntos[0] == puntos[2]){
        x2 = puntos[4];
        y2 = puntos[5];
        if(puntos[1] == y2){
            x1 = puntos[2];
            y1 = puntos[3];
        }else{
            x1 = puntos[2];
            y1 = puntos[3];
        }
    }else if(puntos[2] == puntos[4]){
        x1 = puntos[0];
        y1 = puntos[1];

    }*/
}
