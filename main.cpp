#include "main.h"
#include "graphics.h"
#include "math.h"

#include <iostream>

using namespace std;

float base_aterrizaje[4];
vector<vector<vector<float>>> niveles;
bool aterrizo = false;
float fuel;
float cx, cy;
float vx, vy;

int main(int argc, char *argv[])
{
    float num_nivel = 0;
    //float cantidad_niveles = 3;
    load_levels();

    //Esto debería volar más adelante
    if(argc > 1){
        num_nivel = atof(argv[1]);
        if(num_nivel > 3){
            cout << "No lo hice!" << endl;
            return -1;
        }
    }

    //Inicializo modo gráfico
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, get_screen_width(), get_screen_height(), 0, 0);
    BITMAP *buffer = create_bitmap(get_screen_width(), get_screen_height());

    //Inicializo sonido
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, "A");
    int rocket_vol = 255;
    SAMPLE *rocket = load_sample("sounds\\rocket.wav");
    SAMPLE *explosion = load_sample("sounds\\explosion.wav");

    bool is_burning;

    while(!key[KEY_SPACE]){
        textout_centre_ex(screen, font, "Press (SPACE) for the next level.", 370, 250, 0xFBFF00, 0x000000);
        rest(20);
    }

    int voice = 0;
    set_level(num_nivel);
    while(!key[KEY_ESC]){
        is_burning = false;
        clear_to_color(buffer, 0x000000);

        pintar_nivel(num_nivel, buffer);
        mover_nave(cx, cy, vx, vy);

        if(fuel > 0){
            if(key[KEY_UP]){
                aceleracion(0, vx, vy); //angulo 0 es aceleracion ghacia arriba
                pintar_motor(0, cx, cy, buffer);
                is_burning = true;

                if(voice == 0 || voice_check(voice) == NULL){
                    voice = play_sample(rocket, rocket_vol, 128, 1000, false);
                }

            }
            if(key[KEY_RIGHT]){
                aceleracion(-90, vx, vy); //rota a la derecha
                pintar_motor(-90, cx, cy, buffer);
                is_burning = true;
                if(voice_check(voice) == 0 || voice_check(voice) == NULL)
                    voice = play_sample(rocket, rocket_vol, 128, 1000, true);
            }
            if(key[KEY_LEFT]){
                aceleracion(90, vx, vy); //rota a la derecha
                pintar_motor(90, cx, cy, buffer);
                is_burning = true;
                if(voice_check(voice) == 0 || voice_check(voice) == NULL)
                    voice = play_sample(rocket, rocket_vol, 128, 1000, true);
            }

            if(!key[KEY_UP] && !key[KEY_LEFT] && !key[KEY_RIGHT]){
                if(voice_check(voice) != NULL)
                    stop_sample(rocket);
            }
        }

        pintar_medidor_combustible(is_burning, fuel, num_nivel, buffer);

        pintar_nave(cx, cy, buffer);
        blit(buffer, screen, 0, 0, 0, 0, get_screen_width(), get_screen_height());

        //Verifico si aterrizó.
        if(aterrizar(cx, cy, vx, vy, buffer, num_nivel)){
            if(voice_check(voice) != NULL)
                stop_sample(rocket);
cout << num_nivel << "/" << niveles.size() << endl;
            if(num_nivel < niveles.size())
                num_nivel++;

            while(!key[KEY_SPACE]){
                textout_centre_ex(screen, font, "Press (SPACE) for the next level.", 370, 250, 0xFBFF00, 0x000000);
                rest(20);
            }
            set_level(num_nivel);
        }

        //Verifico si perdió o salió del juego
        if(is_game_over(cx, cy, buffer, num_nivel, explosion)){
            explotar(cx, cy, buffer, num_nivel, explosion);
            set_level(num_nivel);
        }
        rest(20);
    }

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

bool is_game_over(float cx, float cy, BITMAP *buffer, int num_nivel, SAMPLE *explosion){
    //El tamaño de la nave es de 40 de ancho por 20 de alto.

    //Verifico si salió de la pantalla.
    if(cx - 20 >= get_screen_width() || cx + 20 <= 0 || cy + 20 <= 0 || cy - 20 >= get_screen_height()){
        explotar(cx, cy, buffer, num_nivel, explosion);
        return true;
    }

    //Verifico si choco con un triangulo
    if(colision_nave(cx, cy, num_nivel)){
        explotar(cx, cy, buffer, num_nivel, explosion);
        return true;
    }

    //Verifico si choco con la base
    if(cy + 20 >= base_aterrizaje[1]){
        if(cx - 20 >= base_aterrizaje[0] && cx + 20 <= base_aterrizaje[2]){
            if(vy >1.5){
                explotar(cx, cy, buffer, num_nivel, explosion);
                return true;
            }
        }else{
            //Prueba la pata izquierda.
            if(cx - 20 >= base_aterrizaje[0] && cx - 20 < base_aterrizaje[2]){
                explotar(cx, cy, buffer, num_nivel, explosion);
                return true;
            }else if(cx + 20 < base_aterrizaje[2] && cx + 20 >= base_aterrizaje[0]){    //Pruebo la para derecha
                explotar(cx, cy, buffer, num_nivel, explosion);
                return true;
            }
        }
    }

    return false;
}

bool aterrizar(float cx, float cy, float vx, float vy, BITMAP *buffer, int num_nivel){
    if(cy + 20 >= base_aterrizaje[1]){
        if(cx - 20 >= base_aterrizaje[0] && cx + 20 <= base_aterrizaje[2]){
            if(vy <= 1.5){
                aterrizo = true;
                return true;
            }
        }
    }

    return false;
}

void set_level(int num_level){
    //nivel.clear();
    cx = 680;
    cy = 50;
    vx = 0;
    vy = -2;
    fuel = FUEL_MAX;

    if(num_level == 0){
        base_aterrizaje[0] = 10;    //x1
        base_aterrizaje[1] = 450;   //y1
        base_aterrizaje[2] = 100;   //x2
        base_aterrizaje[3] = 500;   //y2

    }
    if(num_level == 1){
        base_aterrizaje[0] = 10;
        base_aterrizaje[1] = 450;
        base_aterrizaje[2] = 100;
        base_aterrizaje[3] = 500;
/*
        nivel.insert(nivel.end(), {110, 100, 300, 500, 110, 500, TRIANGULO_ABAJO});
        nivel.insert(nivel.end(), {500, 500, 600, 300, 600, 500, TRIANGULO_ABAJO});
        nivel.insert(nivel.end(), {600, 300, 800, 500, 600, 500, TRIANGULO_ABAJO});
        nivel.insert(nivel.end(), {200, 0, 400, 350, 400, 0, TRIANGULO_ARRIBA});*/
    }
    if(num_level == 2){
        base_aterrizaje[0] = 10;
        base_aterrizaje[1] = 450;
        base_aterrizaje[2] = 100;
        base_aterrizaje[3] = 500;
/*
        nivel.insert(nivel.end(), {400, 500, 300, 500, 300, 200, TRIANGULO_ABAJO});
        nivel.insert(nivel.end(), {300, 0, 500, 0, 500, 400, TRIANGULO_ARRIBA});
        nivel.insert(nivel.end(), {620, 500, 700, 500, 620, 230, TRIANGULO_ABAJO});
        nivel.insert(nivel.end(), {110, 100, 300, 500, 110, 500, TRIANGULO_ABAJO});*/
    }

}

void load_levels(){
    vector<vector<float>> nivel;
    niveles.insert(niveles.end(), nivel);

    nivel.insert(nivel.end(), {110, 100, 300, 500, 110, 500, TRIANGULO_ABAJO});
    nivel.insert(nivel.end(), {500, 500, 600, 300, 600, 500, TRIANGULO_ABAJO});
    nivel.insert(nivel.end(), {600, 300, 800, 500, 600, 500, TRIANGULO_ABAJO});
    nivel.insert(nivel.end(), {200, 0, 400, 350, 400, 0, TRIANGULO_ARRIBA});
    niveles.insert(niveles.end(), nivel);

    nivel.clear();
    nivel.insert(nivel.end(), {400, 500, 300, 500, 300, 200, TRIANGULO_ABAJO});
    nivel.insert(nivel.end(), {300, 0, 500, 0, 500, 400, TRIANGULO_ARRIBA});
    nivel.insert(nivel.end(), {620, 500, 700, 500, 620, 230, TRIANGULO_ABAJO});
    nivel.insert(nivel.end(), {110, 100, 300, 500, 110, 500, TRIANGULO_ABAJO});
    niveles.insert(niveles.end(), nivel);
}

float* get_base_aterrizaje(){
    return base_aterrizaje;
}

vector<vector<float>> get_nivel(float num_nivel){
    return niveles[num_nivel];
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
bool colision_triangulo(float x1, float y1, float x2, float y2, float p1x, float p1y, float p2x, float p2y, int tipo_triangulo){
    float m = (y2 - y1) / (x2 - x1);
    //m>0 triangulo con pendiente a la derecha, m < 0 triangulo izquierdo
    float b = y1 - m * x1;
    //Equacion de una recta es y = mx + b

    if(tipo_triangulo == TRIANGULO_ABAJO){
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
    }
    if(tipo_triangulo == TRIANGULO_ARRIBA){
        if(m > 0){
            if(x1 <= p2x && p2x <= x2){
                if(p1y <= m*p2x + b) return true;
            }else{
                if(p1x <= x2 && x2 <= p2x){
                    if(p1y <= y2) return true;
                }
            }
            return false;
        }else{
            if(x1 <= p1x && p1x <= x2){
                if(p1y <= m*p1x + b) return true;
            }else{
                if(p1x <= x1 && x1 <= p2x){
                    if(p1y <= y1) return true;
                }
            }
            return false;
        }
    }

    return false;
}

bool colision_nave(float cx, float cy, float num_nivel){
    //Hitbox de la pata izquierda
    float r1x = cx - 20, r1y = cy, r2x = cx - 10, r2y = cy + 20;
    //Hitbox de la pata derecha
    float p1x = cx + 10, p1y = cy, p2x = cx + 20, p2y = cy + 20;
    //Hitbox del cuerpo de la nave
    float q1x = cx - 10, q1y = cy - 15, q2x = cx + 10, q2y = cy;

    //Coordenadas de la hipotenusa de los triangulos
    float x1, y1, x2, y2;
    for(int unsigned i = 0; i < get_nivel(num_nivel).size(); i++){
        get_puntos_hipotenusa(get_nivel(num_nivel)[i], x1, y1, x2, y2);

        if(DEBUG)
            cout << "(" << x1 << ", " << y1 << ")(" << x2 << ", " << y2 << ")" << endl;

        if(colision_triangulo(x1, y1, x2, y2, r1x, r1y, r2x, r2y, get_nivel(num_nivel)[i][6]))
            return true;
        if(colision_triangulo(x1, y1, x2, y2, p1x, p1y, p2x, p2y, get_nivel(num_nivel)[i][6]))
            return true;
        if(colision_triangulo(x1, y1, x2, y2, q1x, q1y, q2x, q2y, get_nivel(num_nivel)[i][6]))
            return true;
    }
    return false;
}

