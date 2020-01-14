#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#define DEBUG TRUE

#define WIDTH 148       //Number of blocks
#define HEIGHT 100      //Number of blocks

#define X_MAX_SPEED 10
#define Y_MAX_SPEED 5
#define Y_SPEED 0.1
#define X_SPEED 0
#define FUEL_MAX 100

#define BLOCK_SIZE 5    //Number of pixels per block

#define SHIP_COLOR 0xeaeae6
#define FIRE_COLOR 0Xff0000
#define NORMAL_FUEL_COLOR 0x999999
#define DANGER_FUEL_COLOR 0xff0000


void pintar_nave(float cx, float cy, BITMAP *buffer);
void pintar_motor(float da, float cx, float cy, BITMAP *buffer);
void pintar_medidor_combustible(bool is_burning, float &fuel, BITMAP *buffer);
void mover_nave(float &cx, float &cy, float &vx, float &vy);
void rotar(float &x, float &y, float cx, float cy, float da);
void aceleracion(float da, float &vx, float &vy);
void pintar_nivel(int num_nivel, BITMAP *buffer);
void explotar(float cx, float cy, BITMAP *buffer, int num_nivel);
#endif // MAIN_H_INCLUDED
