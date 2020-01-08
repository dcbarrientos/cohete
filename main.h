#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#define WIDTH 148       //Number of blocks
#define HEIGHT 100      //Number of blocks

#define X_MAX_SPEED 10
#define Y_MAX_SPEED 5
#define Y_SPEED 0.1
#define X_SPEED 0

#define BLOCK_SIZE 5    //Number of pixels per block

#define SHIP_COLOR 0xeaeae6


void pintar_nave(float cx, float cy, BITMAP *buffer);
void pintar_motor(float da, float cx, float cy, BITMAP *buffer);
void mover_nave(float &cx, float &cy, float &vx, float &vy);
void rotar(float &x, float &y, float cx, float cy, float da);
void aceleracion(float da, float &vx, float &vy);
#endif // MAIN_H_INCLUDED
