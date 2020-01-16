#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <allegro.h>
#include <vector>

#define WIDTH 148       //Number of blocks
#define HEIGHT 100      //Number of blocks

#define X_MAX_SPEED 10
#define Y_MAX_SPEED 5
#define Y_SPEED 0.1
#define X_SPEED 0
#define FUEL_MAX 100

#define BLOCK_SIZE 5    //Number of pixels per block
#define DEBUG FALSE

void load_level(int num_level);
void mover_nave(float &cx, float &cy, float &vx, float &vy);
bool is_game_over(float cx, float cy, BITMAP *buffer, int num_nivel);
bool aterrizar(float cx, float cy, float vx, float vy, BITMAP *buffer, int num_nivel);
bool colision_triangulo(float x1, float y1, float x2, float y2, float p1x, float p1y, float p2x, float p2y);
bool colision_nave(float cx, float cy);

float get_screen_width();
float get_screen_height();
float* get_base_aterrizaje();
std::vector<std::vector<float>> get_nivel();
#endif // MAIN_H_INCLUDED
