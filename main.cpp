#include "main.h"
#include "graphics.h"
#include "math.h"

#include <iostream>

using namespace std;

float base_aterrizaje[4];
vector<vector<float>> nivel;
bool aterrizo = false;

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

    cx = 50;
    cy = 50;

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
    if(aterrizo)
        cout << "Aterrizaste!!!!" << endl;
    else
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
    if(colision_nave(cx, cy)){
        explotar(cx, cy, buffer, num_nivel);
        return true;
    }

    return false;
}

bool aterrizar(float cx, float cy, float vx, float vy, BITMAP *buffer, int num_nivel){
    // 450, 10 y 100 son coordenadas de la base de aterrizaje.
    if(cy + 20 >= base_aterrizaje[1]){
        if(cx - 20 >= base_aterrizaje[0] && cx + 20 <= base_aterrizaje[2]){
            if(vy <= 1.5){
                aterrizo = true;
                return true;
            }else
                explotar(cx, cy, buffer, num_nivel);
        }else{
            if(cx - 20 >= base_aterrizaje[0] && cx + 20 > base_aterrizaje[2]){
                explotar(cx, cy, buffer, num_nivel);
            }else if(cx - 20 < base_aterrizaje[0] && cx + 20 <= base_aterrizaje[2]){
                explotar(cx, cy, buffer, num_nivel);
            }
        }
    }

    return false;
}

void load_level(int num_level){
    if(num_level == 1){
        base_aterrizaje[0] = 10;    //x1
        base_aterrizaje[1] = 450;   //y1
        base_aterrizaje[2] = 100;   //x2
        base_aterrizaje[3] = 500;   //y2

    }
    if(num_level == 2){
        base_aterrizaje[0] = 10;
        base_aterrizaje[1] = 450;
        base_aterrizaje[2] = 100;
        base_aterrizaje[3] = 500;

        nivel.insert(nivel.end(), {400, 500, 300, 500, 300, 200});
        nivel.insert(nivel.end(), {300, 0, 500, 0, 500, 400});
        nivel.insert(nivel.end(), {620, 500, 700, 500, 620, 230});
        //nivel.insert(nivel.end(), {110, 100, 300, 500, 110, 500});
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

/*
* p1x y p1y son el punto superior izquierdo de la nave.
* p2x y p2y son el punto inferior derecho de la nave.
*/
bool colision_triangulo(float x1, float y1, float x2, float y2, float p1x, float p1y, float p2x, float p2y){
    float m = (y2 - y1) / (x2 - x1);
    //m>0 triangulo con pendiente a la derecha, m < 0 triangulo izquierdo
    float b = y1 - m * x1;
    //Equacion de una recta es y = mx + b
    if(m > 0){
        if(x1 <= p1x && p1x <= x2){
            if(p2y >= m*p1x + b) return true;
        }else{
            if(p1x <= x1 && x1 <= p2x){
                if(y1 <= p2y) return true;
            }
        }
    }else if(m < 0){
        if(x1 <= p2x && p2x <= x2){
            if(p2y >= m*p2x + b) return true;
        }else{
            if(p1x <= x2 && x2 <= p2x){
                if(y2 <= p2y) return true;
            }
        }
    }
    return false;
}

bool colision_nave(float cx, float cy){
    float r1x = cx - 20;
    float r1y = cy - 15;
    float r2x = cx + 20;
    float r2y = cy + 15;

    for(int unsigned i = 0; i < nivel.size(); i++){
        if(colision_triangulo(nivel[i][0], nivel[i][1], nivel[i][4], nivel[i][5], r1x, r1y, r2x, r2y))
            return true;
    }
    return false;
}

