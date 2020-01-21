#include "graphics.h"
#include "math.h"
#include "main.h"
#include <iostream>

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

void pintar_medidor_combustible(float &fuel, float max_fuel, int num_level, BITMAP *buffer){
    std::string temp = "Level: " + std::to_string(num_level + 1);

    char txtLevel[temp.size()+1];
    strcpy(txtLevel, temp.c_str());

    textout_centre_ex(buffer, font, "Combustible " , 100, 30, 0x999999, 0x000000);
    textout_centre_ex(buffer, font, txtLevel , 600, 30, 0x999999, 0x000000);

    float medidor_size = 100;
    float danger_zone = 15;

    if(fuel > 0){
        int color;
        if(fuel > (max_fuel / danger_zone))
            color = NORMAL_FUEL_COLOR;
        else
            color = DANGER_FUEL_COLOR;

        float x = 50 + (fuel * medidor_size / max_fuel);
        float m = 3;
        rect(buffer, 50 - m, 50 - m, medidor_size + 50 + m, 55 + m, NORMAL_FUEL_COLOR);
        rectfill(buffer, 50, 50, x, 55, color);
    }
}

void pintar_nivel(int num_nivel, BITMAP *buffer){
    std::vector<float> base_aterrizaje = get_base_aterrizaje(num_nivel);
    std::vector<std::vector<float>> nivel = get_nivel(num_nivel);

    if(nivel.size() > 0){
        for(int unsigned i = 0; i < nivel.size(); i++){
            triangle(buffer, nivel[i][0], nivel[i][1], nivel[i][2], nivel[i][3], nivel[i][4], nivel[i][5], 0x999999);
        }
    }
    rectfill(buffer, base_aterrizaje[0], base_aterrizaje[1], base_aterrizaje[2], base_aterrizaje[3], 0x999999);
}

void explotar(float cx, float cy, float x[], float y[], BITMAP *buffer){
    //Desplazamiento de los elementos de la explosion
    float dx[6] = {7, 7, 0, -7, -7, 0};
    float dy[6] = {0, -7, -7, -7, 0, 7};
    for(int i = 0; i <= 10; i+=2){
        line(buffer, x[i], y[i], x[i+1], y[i + 1], 0xffffff);
        rotar(x[i + 1], y[i + 1], x[i], y[i], 5);

        x[i] += dx[i / 2];
        y[i] += dy[i / 2];
        x[i + 1] += dx[i / 2];
        y[i + 1] += dy[i / 2];
    }

    textout_centre_ex(buffer, font, "Press (Space) to try again.", 370, 240, 0xfbff00, 0x000000);
    textout_centre_ex(buffer, font, "Press (ESC) to exit.", 370, 250, 0xfbff00, 0x000000);

}

void linea(float x1, float y1, float x2, float y2, BITMAP *buffer){
    line(buffer, x1, y1, x2, y2, 0xff0000);
}

