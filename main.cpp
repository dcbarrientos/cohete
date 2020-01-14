#include "main.h"
#include "graphics.h"
#include "math.h"

#include <iostream>
//#include <allegro.h>
//#include <math.h>


using namespace std;

int main()
{
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, get_screen_width(), get_screen_height(), 0, 0);
    BITMAP *buffer = create_bitmap(get_screen_width(), get_screen_height());

    float fuel = FUEL_MAX;
    float cx, cy;
    float vx, vy;
    vx = vy = 0;

    cx = 100;
    cy = 100;

    float num_nivel = 2;
    bool is_burning;
    while(!key[KEY_ESC] && !is_game_over(cx, cy, buffer, num_nivel)){
        is_burning = false;
        clear_to_color(buffer, 0x000000);
        pintar_nivel(num_nivel, buffer);
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

        if(cy > get_screen_height())
            explotar(cx, cy, buffer, num_nivel);

        pintar_medidor_combustible(is_burning, fuel, buffer);

        if(DEBUG)
            cout << "x: " << cx << ", y: " << cy <<  " fuel: " << fuel << endl;

        pintar_nave(cx, cy, buffer);
        blit(buffer, screen, 0, 0, 0, 0, get_screen_width(), get_screen_height());

        rest(20);
    }
    cout << "Perdiste!!!" << endl;

    return 0;
}
END_OF_MAIN()

void mover_nave(float &cx, float &cy, float &vx, float &vy){
    if(vx <= X_MAX_SPEED)
        vx += X_SPEED;
    if(vy <= Y_MAX_SPEED)
        vy += Y_SPEED;

    cx += vx;
    cy += vy;
}



bool is_game_over(float cx, float cy, BITMAP *buffer, int num_nivel){
    //El tamaño de la nave es de 40 de ancho por 20 de alto.
    if(cx - 20 >= get_screen_width() || cx + 20 <= 0 || cy + 20 <= 0 || cy - 20 >= get_screen_height()){
        explotar(cx, cy, buffer, num_nivel);
        return true;
    }

    return false;
}

float get_screen_width(){
    return WIDTH * BLOCK_SIZE;
}

float get_screen_height() {
    return HEIGHT * BLOCK_SIZE;
}
