#include <iostream>
#include <allegro.h>
#include <math.h>

#include "main.h"

using namespace std;

int main()
{
    int width = WIDTH * BLOCK_SIZE;
    int height = HEIGHT * BLOCK_SIZE;
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, width, height, 0, 0);
    BITMAP *buffer = create_bitmap(width, height);

    float fuel = FUEL_MAX;
    float cx, cy;
    float vx, vy;
    vx = vy = 0;

    cx = 100;
    cy = 100;

    bool is_burning;
    while(!key[KEY_ESC]){
        is_burning = false;
        clear_to_color(buffer, 0x000000);
        mover_nave(cx, cy, vx, vy);

        if(fuel > 0){
            if(key[KEY_UP]){
                aceleracion(0, vx, vy); //angulo 0 es aceleracion ghacia arriba
                pintar_motor(0, cx, cy, buffer);
                is_burning = true;
            }
            if(key[KEY_RIGHT]){
                aceleracion(-90, vx, vy); //rota a la derecha
                pintar_motor(-90, cx, cy, buffer);
                is_burning = true;
            }
            if(key[KEY_LEFT]){
                aceleracion(90, vx, vy); //rota a la derecha
                pintar_motor(90, cx, cy, buffer);
                is_burning = true;
            }
        }
        pintar_medidor_combustible(is_burning, fuel, buffer);

        if(DEBUG)
            cout << "x: " << cx << ", y: " << cy <<  " fuel: " << fuel << endl;

        pintar_nave(cx, cy, buffer);
        blit(buffer, screen, 0, 0, 0, 0, width, height);

        rest(20);
    }

    return 0;
}
END_OF_MAIN()

void pintar_nave(float cx, float cy, BITMAP *buffer){
    const float nave[22] = {-4, 4, -4, 2, -2, 0, -2, -2, 0, -3, 2, -2, 2, 0, 4, 2, 4, 4, -2, 0, 2, 0};

    for(int i = 0; i < 19; i+=2){
        line(buffer, cx + (BLOCK_SIZE * nave[i]), cy + (BLOCK_SIZE * nave[i + 1]), cx + (BLOCK_SIZE * nave[i + 2]), cy + (BLOCK_SIZE * nave[ i + 3]), SHIP_COLOR);
        if(i == 14) i += 2;
    }
}

void pintar_motor(float da, float cx, float cy, BITMAP *buffer){
    float fuego[14] = {-1, 1, -2, 4, -1, 4, 0, 7, 1, 4, 2, 4, 1, 1};

    if(da != 0) cy -= 5;
    if(da > 0) cx += 10;
    if(da < 0) cx -= 10;

    for(int i = 0; i < 13; i += 2){
        fuego[i] = cx + fuego[i] * BLOCK_SIZE;
        fuego[i + 1] = cy + fuego[i + 1] * BLOCK_SIZE;
        rotar(fuego[i], fuego[i + 1], cx, cy, da);
    }

    for(int i = 0; i < 12; i+=2){
        line(buffer, fuego[i], fuego[i+1], fuego[i+2], fuego[i+3], SHIP_COLOR);
    }
}

void pintar_medidor_combustible(bool is_burning, float &fuel, BITMAP *buffer){
    textout_centre_ex(buffer, font, "Combustible " , 100, 30, 0x999999, 0x000000);
    float medidor_size = 100;
    float danger_zone = 15;

    if(fuel > 0){
        if(fuel > (FUEL_MAX / danger_zone))
            rectfill(buffer, 50, 50, 50 + (fuel * medidor_size / FUEL_MAX), 55, NORMAL_FUEL_COLOR);
        else
            rectfill(buffer, 50, 50, 50 + (fuel * medidor_size / FUEL_MAX), 55, DANGER_FUEL_COLOR);
    }
    if(is_burning) fuel -= .2;
}

void mover_nave(float &cx, float &cy, float &vx, float &vy){
    if(vx <= X_MAX_SPEED)
        vx += X_SPEED;
    if(vy <= Y_MAX_SPEED)
        vy += Y_SPEED;

    cx += vx;
    cy += vy;
}

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


