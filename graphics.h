#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include "main.h"

#define SHIP_COLOR 0xeaeae6
#define FIRE_COLOR 0Xff0000
#define NORMAL_FUEL_COLOR 0x999999
#define DANGER_FUEL_COLOR 0xff0000

#define TRIANGULO_ARRIBA 0
#define TRIANGULO_ABAJO 1

void pintar_nave(float cx, float cy, BITMAP *buffer);
void pintar_motor(float da, float cx, float cy, BITMAP *buffer);
void pintar_medidor_combustible(bool is_burning, float &fuel, BITMAP *buffer);
void pintar_nivel(int num_nivel, BITMAP *buffer);
void explotar(float cx, float cy, BITMAP *buffer, int num_nivel);
void linea(float x1, float y1, float x2, float y2, BITMAP *buffer);

#endif // GRAPHICS_H_INCLUDED
