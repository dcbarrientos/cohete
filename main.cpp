#include "main.h"
#include "graphics.h"
#include "math.h"

#include <iostream>

using namespace std;

float base_aterrizaje[4] = {0, 0, 0, 0};
vector<vector<float>> nivel;

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

    load_level(num_nivel);

    while(!key[KEY_ESC] && !is_game_over(cx, cy, buffer, num_nivel) && !aterrizar(cx, cy, vx, vy, buffer, num_nivel)){
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

bool aterrizar(float cx, float cy, float vx, float vy, BITMAP *buffer, int num_nivel){
    // 450, 10 y 100 son coordenadas de la base de aterrizaje.
    if(cy + 20 >= 450 && cx - 20 >= 10 && cx + 20 <= 100){
        if(vy <= 1.5)
            return true;
        else
            explotar(cx, cy, buffer, num_nivel);
    }

    return false;
}

void load_level(int num_level){
    if(num_level == 1){
        base_aterrizaje[0] = 10;
        base_aterrizaje[1] = 450;
        base_aterrizaje[2] = 100;
        base_aterrizaje[3] = 500;

    }
    if(num_level == 2){
        base_aterrizaje[0] = 10;
        base_aterrizaje[1] = 450;
        base_aterrizaje[2] = 100;
        base_aterrizaje[3] = 500;

        nivel.resize(3);
        nivel[0].resize(6);
        nivel[0][0] = 400;
        nivel[0][1] = 500;
        nivel[0][2] = 300;
        nivel[0][3] = 500;
        nivel[0][4] = 300;
        nivel[0][5] = 200;
        nivel[1].resize(6);
        nivel[1][0] = 300;
        nivel[1][1] = 0;
        nivel[1][2] = 500;
        nivel[1][3] = 0;
        nivel[1][4] = 500;
        nivel[1][5] = 400;
        nivel[2].resize(6);
        nivel[2][0] = 620;
        nivel[2][1] = 500;
        nivel[2][2] = 700;
        nivel[2][3] = 500;
        nivel[2][4] = 620;
        nivel[2][5] = 230;
    }
}

float* get_base_aterrizaje(){
    return base_aterrizaje;
}

vector<vector<float>> get_nivel(){
    return nivel;
}

float get_screen_width(){
    return WIDTH * BLOCK_SIZE;
}

float get_screen_height() {
    return HEIGHT * BLOCK_SIZE;
}

