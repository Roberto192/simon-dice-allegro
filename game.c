#include "game.h"
#include "menu.h"
#include "bloque.h"
int start_game(){

    /*
        Todas las variables que se ocuparan para el flujo del programa
    */

    srand(time(NULL));
    ALLEGRO_EVENT_QUEUE *event_queue;//Puntero a una cola de eventos;
    ALLEGRO_DISPLAY *display;//Puntero hacia una pantalla;
    ALLEGRO_TIMER *timer, *timerSecond, *timerTicks;//Puntero a un temporizador
    ALLEGRO_BITMAP *sprites, *logo, *bloquesSprites;//Puntero a mapa de bits
    ALLEGRO_EVENT event;//el que controlara todo evento
    ALLEGRO_FONT *font = al_load_ttf_font("VT323.ttf", 64, ALLEGRO_TTF_MONOCHROME);

    Tconf conf;
    ObjetoMenu miMenu;
    Tbloque bloques[MAX_BLOQUE];
    bool handleTimer = false;
    float *formulax, *formulay;
    int n = (int) sqrt(MAX_BLOQUE);
    int i = 0, j = 0, k = 0, ant;//i es el index para textUser y j es el index para textGame
    bool timer_event = false;
    int estado = 2;
    int c;

    char textGame[MAX_ELEMETS];
    char textUser[MAX_ELEMETS];

    int errores = 0;
    int escenario = ESCENARIO_MENU;
    bool repetir = true;

    display = al_create_display(WIDTH, HEIGHT);
    if(!display)
        return -1;

    event_queue = al_create_event_queue();
    if(!event_queue)
        return -2;

    timer = al_create_timer(1.0/FPS);
    timerSecond = al_create_timer(1.0/FPS);
    timerTicks = al_create_timer(1.0/FPS);
    if(!timer || !timerSecond || !timerTicks)
        return -3;

    sprites = al_load_bitmap("sprites.png");
    if(!sprites)
        return -4;

    bloquesSprites = al_create_sub_bitmap(sprites, 1484, 141, 756, 533);
    logo = al_create_sub_bitmap(sprites, 1484, 810, 134, 134);
    if(!sprites || !bloquesSprites)
        return -5;

    al_set_app_name("SIMON DICE ...SUS");
    al_set_window_title(display, "SIMON DICE");
    al_set_display_icon(display, logo);

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_timer_event_source(timerSecond));
    al_register_event_source(event_queue, al_get_timer_event_source(timerTicks));
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_start_timer(timer);

    formulax = malloc(sizeof(float) * (n + 1));
    formulay = malloc(sizeof(float) * (n + 1));



    conf.keys[0] = ALLEGRO_KEY_1;
    conf.keys[1] = ALLEGRO_KEY_2;
    conf.keys[2] = ALLEGRO_KEY_3;
    conf.keys[3] = ALLEGRO_KEY_4;
    conf.keys[4] = ALLEGRO_KEY_5;
    conf.keys[5] = ALLEGRO_KEY_6;
    conf.keys[6] = ALLEGRO_KEY_7;
    conf.keys[7] = ALLEGRO_KEY_8;
    conf.keys[8] = ALLEGRO_KEY_9;

    conf.ascii[0] = '1';
    conf.ascii[1] = '2';
    conf.ascii[2] = '3';
    conf.ascii[3] = '4';
    conf.ascii[4] = '5';
    conf.ascii[5] = '6';
    conf.ascii[6] = '7';
    conf.ascii[7] = '8';
    conf.ascii[8] = '9';

    miMenu = obtenerObjetoMenu();
    obtenerBloques(bloquesSprites, bloques, MAX_BLOQUE, &conf);
    obtenerFomula(formulax, formulay, n, 140, WIDTH, HEIGHT);
    ///bloque = al_create_sub_bitmap(sprites, 2156, 365, 84, 84);
    /// separacion de 140 pixeles

    textGame[0] = conf.ascii[rand()%MAX_BLOQUE];
    textGame[1] = '\0';

    while(repetir){
        al_wait_for_event(event_queue, &event);

        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            repetir = false;
            al_stop_timer(timer);
        }
        if(event.type == ALLEGRO_EVENT_KEY_DOWN){

            if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                repetir = false;
                al_stop_timer(timer);
            }else{
                if(escenario == ESCENARIO_MENU){
                    handleMenuKeyboardDown(&miMenu, &escenario, event.keyboard.keycode, timerSecond);
                    timer_event = al_get_timer_started(timerSecond);
                }else{
                    if(escenario == ESCENARIO_PLAY){
                        if(!timer_event)
                            if(handlePlayKeyboard(event.keyboard.keycode, &conf, textUser, &i)){
                                //printf("%s[%d] -> %c == %c\n", textUser, j,textUser[j], textGame[j]);
                                if(textGame[j] != textUser[j]){
                                    //printf("Haz perdido UnU\n");
                                    memset(textGame, '\0', MAX_ELEMETS);
                                    memset(textUser, '\0', MAX_ELEMETS);
                                    textGame[0] = conf.ascii[rand()%MAX_BLOQUE];
                                    textGame[1] = '\0';
                                    j = i = k = 0;
                                    escenario = ESCENARIO_MENU;
                                }else{
                                    j++;
                                }
                            }
                    }
                }
            }

        }

        if(event.type == ALLEGRO_EVENT_TIMER){
            if(al_get_timer_count(timerSecond) >= FPS*1.5){
                al_set_timer_count(timerSecond, 0);
                if(textGame[k] != '\0'){
                    //printf("%s %c\n",textGame, textGame[k]);
                    for(int a = 0; a < MAX_BLOQUE; a++){
                        if(textGame[k] == bloques[a].letra){
                            bloques[a].imagenPrincipal.y = COORD_Y(2);
                            bloques[a].status = 1;
                            al_start_timer(timerTicks);
                        }
                    }
                    k++;
                }
                else
                    if(timer_event){
                        //printf("Esperando entreda...\n");
                        al_stop_timer(timerSecond);
                        timer_event = false;
                    }
            }

            if(al_get_timer_count(timerTicks) >= 20){
                al_set_timer_count(timerTicks, 0);
                al_stop_timer(timerTicks);
                for(int a = 0; textGame[a] != '\0'; a++)
                  for(int bIndex = 0; bIndex < MAX_BLOQUE; bIndex++)
                        if(bloques[bIndex].letra == textGame[a])
                            if(bloques[bIndex].status){
                                bloques[bIndex].status = 0;
                                bloques[bIndex].imagenPrincipal.y = COORD_Y(1);
                            }
            }
        }

        //if(event.type == ALLEGRO_EVENT_MOUSE_AXES)
        //    printf("x:%d y:%d\n",event.mouse.x, event.mouse.y);
        if(al_is_event_queue_empty(event_queue)){
            if(repetir){
                al_clear_to_color(al_map_rgb(0, 0, 0));
                if(errores == 0){
                    al_draw_scaled_bitmap(al_load_bitmap("fondo.png"), 0, 0, 1600, 900, 0, 0, WIDTH, HEIGHT, 0);
                    if(escenario == ESCENARIO_MENU){

                        //Logo brille
                        miMenu.logo.y += miMenu.logo.tamy;

                        if(miMenu.logo.y > miMenu.logo.tamy * 5)
                            miMenu.logo.y = 0;
                        //Fin de logo brille

                        botonAnimacion(&miMenu);

                        errores = menu(&miMenu);
                        //button = create_button(1,1,1,1,1);
                        //mostrar_butto();
                    }else{
                        if(escenario == ESCENARIO_PLAY){
                            al_draw_textf(font, al_map_rgb(0xff, 0xff, 0xff), 0, 0, 0, "Record: %d",strlen(textGame));
                            play(bloques, MAX_BLOQUE, formulax, formulay, n);
                            if(textGame[j] == '\0'){
                                al_start_timer(timerSecond);
                                textGame[k] = conf.ascii[rand()%MAX_BLOQUE];
                                textGame[k+1] = '\0';
                                k = j = 0;
                                timer_event = true;
                                memset(textUser, '\0', MAX_ELEMETS);
                                i = 0;
                            }
                        }else{
                            if(escenario == 2){
                                //errores = records();

                            }else{
                                if(escenario == 3){

                                }
                            }
                        }
                    }

                    al_flip_display();
                }
            }
        }
    }

    al_destroy_timer(timerSecond);
    al_destroy_timer(timerTicks);
    al_destroy_bitmap(bloquesSprites);
    al_destroy_bitmap(logo);
    al_destroy_bitmap(sprites);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);

    return 0;
}
