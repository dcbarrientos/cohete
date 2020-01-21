#include "main.h"
#include "graphics.h"
#include "math.h"

#include <iostream>

using namespace std;

vector<vector<float>> bases;
vector<vector<vector<float>>> niveles;
vector<vector<float>> configuracion_nave;

int state;
float fuel;
float cx, cy;
float vx, vy;
bool exploto = false;
float xe[12];
float ye[12];

int main(int argc, char *argv[])
{

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

    load_levels();


    while(!key[KEY_SPACE]){
        textout_centre_ex(screen, font, "Press (SPACE) for the next level.", 370, 250, 0xFBFF00, 0x000000);
        rest(20);
    }

    float num_nivel = 0;
    int voice = 0;
    set_level(num_nivel);
    state = GAMING_STATE;

    while(!key[KEY_ESC]){
        clear_to_color(buffer, 0x000000);
        pintar_nivel(num_nivel, buffer);

        state = get_state(cx,cy, vx, vy, num_nivel);

        if(state == GAMING_STATE){
            mover_nave(cx, cy, vx, vy);

            if(fuel > 0){
                if(key[KEY_UP]){
                    aceleracion(0, vx, vy); //angulo 0 es aceleracion ghacia arriba
                    pintar_motor(0, cx, cy, buffer);
                    fuel -= FUEL_DECREASE;
                    if(voice == 0 || voice_check(voice) == NULL){
                        voice = play_sample(rocket, rocket_vol, 128, 1000, false);
                    }

                }
                if(key[KEY_RIGHT]){
                    aceleracion(-90, vx, vy); //rota a la derecha
                    pintar_motor(-90, cx, cy, buffer);
                    fuel -= FUEL_DECREASE;
                    if(voice_check(voice) == 0 || voice_check(voice) == NULL)
                        voice = play_sample(rocket, rocket_vol, 128, 1000, true);
                }
                if(key[KEY_LEFT]){
                    aceleracion(90, vx, vy); //rota a la derecha
                    pintar_motor(90, cx, cy, buffer);
                    fuel -= FUEL_DECREASE;
                    if(voice_check(voice) == 0 || voice_check(voice) == NULL)
                        voice = play_sample(rocket, rocket_vol, 128, 1000, true);
                }

                if(!key[KEY_UP] && !key[KEY_LEFT] && !key[KEY_RIGHT]){
                    if(voice_check(voice) != NULL)
                        stop_sample(rocket);
                }
            }else if(fuel <= 0){
                stop_sample(rocket);
            }
        }

        if(state == LANDED_STATE){
            if(voice_check(voice) != NULL)
                stop_sample(rocket);

            textout_centre_ex(buffer, font, "Press (SPACE) for the next level.", 370, 250, 0xFBFF00, 0x000000);

            if(key[KEY_SPACE]){
                if(num_nivel < niveles.size() - 1)
                    num_nivel++;
                set_level(num_nivel);
            }
        }

        if(state == OVER_STATE){
            stop_sample(rocket);
            if((voice == 0 || voice_check(voice) == NULL) && !exploto){
                float x[12] = {cx - 10, cx + 10, cx, cx, cx + 15, cx - 15, cx + 5, cx - 10, cx + 10, cx - 5, cx - 10, cx + 10};
                float y[12] = {cy, cy, cy - 15, cy + 15, cy - 15, cy + 15, cy + 5, cy - 10, cy - 10, cy + 10, cy, cy};
                copy(begin(x), end(x), begin(xe));
                copy(begin(y), end(y), begin(ye));

                voice = play_sample(explosion, 255, 128, 1000, false);
                exploto = true;
            }

            explotar(cx, cy, xe, ye, buffer);

            if(key[KEY_SPACE]){
                set_level(num_nivel);
                stop_sample(explosion);
            }
        }

        pintar_medidor_combustible(fuel, configuracion_nave[num_nivel][FUEL_INDEX], num_nivel, buffer);

        if(state != OVER_STATE)
            pintar_nave(cx, cy, buffer);

        blit(buffer, screen, 0, 0, 0, 0, get_screen_width(), get_screen_height());

        rest(20);
    }

    destroy_sample(rocket);
    destroy_sample(explosion);

    return 0;
}
END_OF_MAIN()

int get_state(float cx, float cy, float vx, float vy, float num_nivel){
    //Verifico si aterrizó.
    if(state == GAMING_STATE && is_landed(cx, cy, vx, vy, num_nivel))
        return LANDED_STATE;

    //Verifico si perdio
    if(is_game_over(cx, cy,  num_nivel))
        return OVER_STATE;

    return state;
}

void mover_nave(float &cx, float &cy, float &vx, float &vy){
    if(vx <= X_MAX_SPEED)
        vx += X_SPEED;
    if(vy <= Y_MAX_SPEED)
        vy += Y_SPEED;

    cx += vx;
    cy += vy;
}

bool is_game_over(float cx, float cy,  int num_nivel){//, SAMPLE *explosion){
    //El tamaño de la nave es de 40 de ancho por 20 de alto.

    //Verifico si salió de la pantalla.
    if(cx - 20 >= get_screen_width() || cx + 20 <= 0 || cy + 20 <= 0 || cy - 20 >= get_screen_height()){
        return true;
    }

    //Verifico si choco con un triangulo
    if(colision_nave(cx, cy, num_nivel)){
        return true;
    }

    //Verifico si choco con la base
    if(cy + 20 >= bases[num_nivel][1]){
        if(cx - 20 >= bases[num_nivel][0] && cx + 20 <= bases[num_nivel][2]){
            if(vy >1.5){
                return true;
            }
        }else{
            //Prueba la pata izquierda.
            if(cx - 20 >= bases[num_nivel][0] && cx - 20 < bases[num_nivel][2]){
                return true;
            }else if(cx + 20 < bases[num_nivel][2] && cx + 20 >= bases[num_nivel][0]){    //Pruebo la para derecha
                return true;
            }
        }
    }

    return false;
}

bool is_landed(float cx, float cy, float vx, float vy, int num_nivel){
    if(cy + 20 >= bases[num_nivel][1]){
        if(cx - 20 >= bases[num_nivel][0] && cx + 20 <= bases[num_nivel][2]){
            if(vy <= 1.5){
                //aterrizo = true;
                return true;
            }
        }
    }

    return false;
}

void set_level(int num_level){
    cx = configuracion_nave[num_level][CX_INDEX];
    cy = configuracion_nave[num_level][CY_INDEX];
    vx = configuracion_nave[num_level][VX_INDEX];
    vy = configuracion_nave[num_level][VY_INDEX];
    fuel = configuracion_nave[num_level][FUEL_INDEX];
    state = GAMING_STATE;
    exploto = false;
}

void load_levels(){
    vector<vector<float>> nivel;

    // Nivel 1
    ///////////////////////////////////////////////////////////////////////////////////
    //Configuración inicial nave
    configuracion_nave.insert(configuracion_nave.end(), {680, 50, 0, -2, 100});
    //Base de aterrizaje para el nivel 1
    bases.insert(bases.end(), {10, 450, 100, 500});
    //Obstaculos para el nivel 1. En este nivel está en cero porque no tiene obstaculos
    niveles.insert(niveles.end(), nivel);

    // Nivel 2
    ///////////////////////////////////////////////////////////////////////////////////
    //Configuración inicial nave
    configuracion_nave.insert(configuracion_nave.end(), {680, 50, 0, -2, 75});
    //Base de aterrizaje para el nivel 1
    bases.insert(bases.end(), {10, 450, 100, 500});
    //Obstaculos para el nivel 1
    nivel.insert(nivel.end(), {110, 100, 300, 500, 110, 500, TRIANGULO_ABAJO});
    nivel.insert(nivel.end(), {500, 500, 600, 300, 600, 500, TRIANGULO_ABAJO});
    nivel.insert(nivel.end(), {600, 300, 800, 500, 600, 500, TRIANGULO_ABAJO});
    nivel.insert(nivel.end(), {200, 0, 400, 350, 400, 0, TRIANGULO_ARRIBA});
    niveles.insert(niveles.end(), nivel);

    // Nivel 3
    ///////////////////////////////////////////////////////////////////////////////////
    //Configuración inicial nave
    configuracion_nave.insert(configuracion_nave.end(), {680, 50, 0, -2, 100});
    //Base de aterrizaje para el nivel 1
    bases.insert(bases.end(), {10, 450, 100, 500});
    //Obstaculos para el nivel 1
    nivel.clear();
    nivel.insert(nivel.end(), {400, 500, 300, 500, 300, 200, TRIANGULO_ABAJO});
    nivel.insert(nivel.end(), {300, 0, 500, 0, 500, 400, TRIANGULO_ARRIBA});
    nivel.insert(nivel.end(), {620, 500, 700, 500, 620, 230, TRIANGULO_ABAJO});
    nivel.insert(nivel.end(), {110, 100, 300, 500, 110, 500, TRIANGULO_ABAJO});
    niveles.insert(niveles.end(), nivel);
}

vector<float> get_base_aterrizaje(float num_nivel){
    return bases[num_nivel];
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

        if(colision_triangulo(x1, y1, x2, y2, r1x, r1y, r2x, r2y, get_nivel(num_nivel)[i][6]))
            return true;
        if(colision_triangulo(x1, y1, x2, y2, p1x, p1y, p2x, p2y, get_nivel(num_nivel)[i][6]))
            return true;
        if(colision_triangulo(x1, y1, x2, y2, q1x, q1y, q2x, q2y, get_nivel(num_nivel)[i][6]))
            return true;
    }
    return false;
}

