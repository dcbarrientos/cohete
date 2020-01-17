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

void pintar_nivel(int num_nivel, BITMAP *buffer){
    float *base_aterrizaje = get_base_aterrizaje();
    std::vector<std::vector<float>> nivel = get_nivel();

    if(nivel.size() > 0){
        for(int unsigned i = 0; i < nivel.size(); i++){
            triangle(buffer, nivel[i][0], nivel[i][1], nivel[i][2], nivel[i][3], nivel[i][4], nivel[i][5], 0x999999);
        }
    }
    rectfill(buffer, base_aterrizaje[0], base_aterrizaje[1], base_aterrizaje[2], base_aterrizaje[3], 0x999999);
}

void explotar(float cx, float cy, BITMAP *buffer, int num_nivel, SAMPLE *explosion){
    float x[12] = {cx - 10, cx + 10, cx, cx, cx + 15, cx - 15, cx + 5, cx - 10, cx + 10, cx - 5, cx - 10, cx + 10};
    float y[12] = {cy, cy, cy - 15, cy + 15, cy - 15, cy + 15, cy + 5, cy - 10, cy - 10, cy + 10, cy, cy};

    //Desplazamiento de los elementos de la explosion
    float dx[6] = {7, 7, 0, -7, -7, 0};
    float dy[6] = {0, -7, -7, -7, 0, 7};
    int v = play_sample(explosion, 255, 128, 1000, false);

    clear(screen);
    do{
        clear(buffer);
        pintar_nivel(num_nivel, buffer);
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

        blit(buffer, screen, 0, 0, 0, 0, get_screen_width(), get_screen_height());
        rest(20);
        std::cout << "V " << voice_check(v) << std::endl;
    }while(!key[KEY_ESC] && !key[KEY_SPACE]);
        std::cout << "V " << voice_check(v) << std::endl;
    stop_sample(explosion);
        std::cout << "V " << voice_check(v) << std::endl;
    //destroy_sample(explosion);
}

void linea(float x1, float y1, float x2, float y2, BITMAP *buffer){
    line(buffer, x1, y1, x2, y2, 0xff0000);
}

