#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <allegro.h>

#define DEBUG TRUE

#define WIDTH 148       //Number of blocks
#define HEIGHT 100      //Number of blocks

#define X_MAX_SPEED 10
#define Y_MAX_SPEED 5
#define Y_SPEED 0.1
#define X_SPEED 0
#define FUEL_MAX 100

#define BLOCK_SIZE 5    //Number of pixels per block


void mover_nave(float &cx, float &cy, float &vx, float &vy);
bool is_game_over(float cx, float cy, BITMAP *buffer, int num_nivel);
float get_screen_width();
float get_screen_height();
#endif // MAIN_H_INCLUDED
