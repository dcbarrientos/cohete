#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <allegro.h>
#include <vector>

#define WIDTH 148       //Number of blocks
#define HEIGHT 100      //Number of blocks

#define GAMING_STATE 0
#define LANDED_STATE 1
#define OVER_STATE 2

#define X_MAX_SPEED 10
#define Y_MAX_SPEED 5
#define Y_SPEED 0.1
#define X_SPEED 0
#define FUEL_DECREASE 0.2

#define CX_INDEX 0
#define CY_INDEX 1
#define VX_INDEX 2
#define VY_INDEX 3
#define FUEL_INDEX 4

#define BLOCK_SIZE 5    //Number of pixels per block
#define DEBUG FALSE

void set_level(int num_level);
void load_levels();
void mover_nave(float &cx, float &cy, float &vx, float &vy);
bool is_game_over(float cx, float cy, BITMAP *buffer, int num_nivel, SAMPLE *explosion);
bool aterrizar(float cx, float cy, float vx, float vy, BITMAP *buffer, int num_nivel);
bool colision_triangulo(float x1, float y1, float x2, float y2, float p1x, float p1y, float p2x, float p2y);
bool colision_nave(float cx, float cy, float num_nivel);

float get_screen_width();
float get_screen_height();
std::vector<float> get_base_aterrizaje(float num_nivel);
std::vector<std::vector<float>> get_nivel(float num_nivel);
#endif // MAIN_H_INCLUDED
