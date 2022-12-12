#include "game.h"
#include "menu.h"
#include "configuracion.h"
#include "bloque.h"
#include "scored.h"

int start_game(){

    /*
        Todas las variables que se ocuparan para el flujo del programa
    */

    srand(time(NULL));
    ALLEGRO_EVENT_QUEUE *event_queue;//Puntero a una cola de eventos;
    ALLEGRO_DISPLAY *display;//Puntero hacia una pantalla;
    ALLEGRO_TIMER *timer, *timerSecond, *timerTicks;//Puntero a un temporizador
    ALLEGRO_BITMAP *sprites, *logo, *bloquesSprites, *background;//Puntero a mapa de bits
    ALLEGRO_EVENT event;//el que controlara todo evento
    ALLEGRO_FONT *font = al_load_ttf_font("VT323.ttf", 84, ALLEGRO_TTF_MONOCHROME);
    ALLEGRO_MONITOR_INFO monitor_info;

    Tscored scored, scoreds[100];
    ObjetoMenu miMenu;
    Tbloque bloques[MAX_BLOQUE];
    bool handleTimer = false;
    float *formulax, *formulay;
    int n = (int) sqrt(MAX_BLOQUE);
    int i = 0, j = 0, k = 0, ant;//i es el index para textUser y j es el index para textGame
    bool timer_event = false, redraw = false, press = false, load_scoreds = false;
    int estado = 2;
    int c = '\0';

    char textGame[MAX_ELEMETS];
    char textUser[MAX_ELEMETS];

    char name[MAX];
    memset(name, '\0', MAX);

    int errores = 0;
    int escenario = ESCENARIO_MENU;
    bool repetir = true;

    al_get_monitor_info(0, &monitor_info);

    display = al_create_display(440,100);
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
    background = al_load_bitmap("fondo.png");
    if(!sprites || !bloquesSprites || !background)
        return -5;

    al_set_app_name("SIMON DICE ...SUS");
    al_set_window_title(display, "SIMON DICE");
    al_set_display_icon(display, logo);

    getText(name, display);
    printf("%s",name);
    al_resize_display(display, WIDTH, HEIGHT);
    al_set_window_position(display,monitor_info.x2/2 - WIDTH/2, monitor_info.y2/2 - HEIGHT/2);
    scored = getScored(name);


    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_timer_event_source(timerSecond));
    al_register_event_source(event_queue, al_get_timer_event_source(timerTicks));
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_start_timer(timer);

    formulax = malloc(sizeof(float) * (n + 1));
    formulay = malloc(sizeof(float) * (n + 1));

    miMenu = obtenerObjetoMenu();
    obtenerBloques(bloquesSprites, bloques, MAX_BLOQUE, &scored.conf);
    obtenerFomula(formulax, formulay, n, 140, WIDTH, HEIGHT);
    ///bloque = al_create_sub_bitmap(sprites, 2156, 365, 84, 84);
    /// separacion de 140 pixeles

    while(repetir){
        al_wait_for_event(event_queue, &event);

        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            repetir = false;
            al_stop_timer(timer);
        }

        if(escenario == ESCENARIO_CONFIG){
            if(!press){
                if(event.type == ALLEGRO_EVENT_KEY_CHAR){
                    c = event.keyboard.unichar;
                    if(k >= 1){
                        scored.conf.ascii[k-1] = c;
                        scored.conf.keys[k-1] = event.keyboard.keycode;
                        bloques[k-1].letra = c;
                        //printf("-> %d %d %c <-", event.keyboard.keycode, c, c);
                        printf("\nkey: %s\n",scored.conf.keys);
                        k = 0;
                    }
                }
            }
        }

        if(event.type == ALLEGRO_EVENT_KEY_DOWN){

            if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                repetir = false;
                al_stop_timer(timer);
            }else{
                if(escenario == ESCENARIO_MENU){
                    handleMenuKeyboardDown(&miMenu, &escenario, event.keyboard.keycode, timerSecond);
                    timer_event = al_get_timer_started(timerSecond);
                    if(timer_event){
                        textGame[0] = scored.conf.ascii[rand()%MAX_BLOQUE];
                        textGame[1] = '\0';
                    }
                }else{
                    if(escenario == ESCENARIO_PLAY){
                        if(!timer_event)
                            if(handlePlayKeyboard(event.keyboard.keycode, &scored.conf, textUser, &i)){
                                //printf("%s [%d] -> %c == %c\n", conf.ascii, j,textUser[j], textGame[j]);
                                if(textGame[j] != textUser[j]){
                                    //printf("Haz perdido UnU\n");
                                    saveScored(&scored, strlen(textGame) - 1);
                                    memset(textGame, '\0', MAX_ELEMETS);
                                    memset(textUser, '\0', MAX_ELEMETS);
                                    textGame[0] = scored.conf.ascii[rand()%MAX_BLOQUE];
                                    textGame[1] = '\0';
                                    j = i = k = 0;
                                    escenario = ESCENARIO_MENU;
                                }else{
                                    j++;
                                }
                            }
                    }else{
                        if(escenario == ESCENARIO_CONFIG){
                            if(!press){
                                if(event.keyboard.keycode == ALLEGRO_KEY_S){
                                    saveScored(&scored, scored.scored);
                                    escenario = ESCENARIO_MENU;
                                }

                                if(event.keyboard.keycode == ALLEGRO_KEY_1){
                                    press = true;
                                    k = 1;
                                }

                                if(event.keyboard.keycode == ALLEGRO_KEY_2){
                                    press = true;
                                    k = 2;
                                }

                                if(event.keyboard.keycode == ALLEGRO_KEY_3){
                                    press = true;
                                    k = 3;
                                }

                                if(event.keyboard.keycode == ALLEGRO_KEY_4){
                                    press = true;
                                    k = 4;
                                }

                                if(event.keyboard.keycode == ALLEGRO_KEY_5){
                                    press = true;
                                    k = 5;
                                }

                                if(event.keyboard.keycode == ALLEGRO_KEY_6){
                                    press = true;
                                    k = 6;
                                }

                                if(event.keyboard.keycode == ALLEGRO_KEY_7){
                                    press = true;
                                    k = 7;
                                }

                                if(event.keyboard.keycode == ALLEGRO_KEY_8){
                                    press = true;
                                    k = 8;
                                }

                                if(event.keyboard.keycode == ALLEGRO_KEY_9){
                                    press = true;
                                    k = 9;
                                }
                            }else{
                                press = false;
                            }
                        }
                    }
                }
            }
        }

        if(event.type == ALLEGRO_EVENT_TIMER){
            if(al_get_timer_count(timerSecond) >= FPS){
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

            if(al_get_timer_count(timerTicks) >= FPS/2){
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

            if(al_get_timer_count(timer) > FPS/8){
                al_set_timer_count(timer, 0);
                botonAnimacion(&miMenu);

                //Logo brille
                miMenu.logo.y += miMenu.logo.tamy;

                if(miMenu.logo.y > miMenu.logo.tamy * 5)
                    miMenu.logo.y = 0;
                //Fin de logo brille
                redraw = true;
            }
        }

        //if(event.type == ALLEGRO_EVENT_MOUSE_AXES)
        //    printf("x:%d y:%d\n",event.mouse.x, event.mouse.y);
        if(al_is_event_queue_empty(event_queue) || redraw){
            if(repetir){
                al_clear_to_color(al_map_rgb(0, 0, 0));
                if(errores == 0){
                    al_draw_scaled_bitmap(background, 0, 0, 1600, 900, 0, 0, WIDTH, HEIGHT, 0);
                    if(escenario == ESCENARIO_MENU){
                        errores = menu(&miMenu);
                        //button = create_button(1,1,1,1,1);
                        //mostrar_butto();
                    }else{
                        if(escenario == ESCENARIO_PLAY){
                            play(bloques, MAX_BLOQUE, formulax, formulay, n, font, strlen(textGame));
                            if(textGame[j] == '\0'){
                                al_start_timer(timerSecond);
                                textGame[k] = scored.conf.ascii[rand()%MAX_BLOQUE];
                                textGame[k+1] = '\0';
                                k = j = 0;
                                timer_event = true;
                                memset(textUser, '\0', MAX_ELEMETS);
                                i = 0;
                            }
                        }else{
                            if(escenario == ESCENARIO_RECORDS){

                                if(!load_scoreds){
                                    load_scoreds = true;
                                    ALLEGRO_FILE *arch;
                                    int o = 0;
                                    arch = al_fopen("scoreds.smndc", "rb");
                                    while(al_fread(arch, &scoreds[o], sizeof(struct _tScored))){
                                        o++;

                                        if(o > 1){
                                            for(int p = 0; p < o; p++){
                                                for(int y = 0; y < o - 1; y++){
                                                    if(scoreds[y].scored < scoreds[y+1].scored){
                                                        Tscored aux = scoreds[y];
                                                        scoreds[y] = scoreds[y+1];
                                                        scoreds[y+1] = aux;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    for(int y = o; y < MAX; y++){
                                        scoreds[y].name[0] = '\0';
                                    }
                                }

                                for(int u = 0; u < MAX; u++){
                                    if(scoreds[u].name[0] != '\0')
                                        al_draw_textf(font, al_map_rgb(0xff, 0xff, 0xff), 0, 84*u, 0, "%d Nombre: %s Scored: %d", u + 1, scoreds[u].name, scoreds[u].scored);
                                }

                            }else{
                                if(escenario == ESCENARIO_CONFIG){
                                    configuracion(&scored.conf);
                                    if(press){
                                        //al_draw_filled_rectangle(WIDTH/8, 350, WIDTH/2+WIDTH/3+50, 500, al_map_rgb(0,0,0));
                                        al_draw_text(font, al_map_rgb(0xff, 0xff, 0xff), WIDTH/2, HEIGHT/2, ALLEGRO_ALIGN_CENTRE, "Presiona cualquier tecla...");
                                    }
                                }
                            }
                        }
                    }

                    al_flip_display();
                }
            }

            redraw = false;
        }
    }

    al_destroy_bitmap(background);
    al_destroy_timer(timerSecond);
    al_destroy_timer(timerTicks);
    al_destroy_font(font);
    al_destroy_bitmap(bloquesSprites);
    al_destroy_bitmap(logo);
    al_destroy_bitmap(sprites);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);

    return 0;
}

char *getText(char text[], ALLEGRO_DISPLAY *display){

    ALLEGRO_EVENT event;
    ALLEGRO_EVENT_QUEUE *event_queue;
    ALLEGRO_TIMER *timer;
    ALLEGRO_FONT *font, *font2;

    int j = 0;
    event_queue = al_create_event_queue();
    if(!event_queue)
        return NULL;

    timer = al_create_timer(1.0/60);
    if(!timer)
        return NULL;

    font = al_load_font("VT323.ttf", 26, ALLEGRO_TTF_MONOCHROME);
    font2 = al_load_font("VT323.ttf", 40, ALLEGRO_TTF_MONOCHROME);
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_start_timer(timer);
    while(true){
        al_wait_for_event(event_queue, &event);
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            break;
        }

        if(event.type == ALLEGRO_EVENT_KEY_DOWN){
            if(event.keyboard.keycode == ALLEGRO_KEY_ENTER)
                break;
        }

        if(event.type == ALLEGRO_EVENT_KEY_CHAR){
            if(event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE){
                if(j > 0){
                    text[j-1] = '\0';
                    j--;
                }
            }else{
                if(j < MAX-1){
                    if(isalpha(event.keyboard.unichar) || isalnum(event.keyboard.unichar)){
                        text[j] = event.keyboard.unichar;
                        j++;
                        text[j] = '\0';
                    }
                }else{
                    text[j] = '\0';
                }
            }
        }

        if(al_event_queue_is_empty(event_queue)){
            al_clear_to_color(al_map_rgb(0x55,0x55,0x55));
            al_draw_text(font, al_map_rgb(0xff, 0xff, 0xff), 20, 0, 0, "Ingresa tu nombre: ");
            //al_draw_circle(4,100,10, al_map_rgb(0xff, 0xff, 0xff), 0);
            for(int i = 0; i < MAX-1; i++){
                al_draw_filled_rectangle(20+((20+50)*i), 89, 20+50+((20+50)*i), 90, al_map_rgb(0xff, 0xff, 0xff));
                al_draw_textf(font2, al_map_rgb(0xff, 0xff, 0xff), 20+((20+50)*i)+25, 89-40, ALLEGRO_ALIGN_CENTRE, "%c",text[i]);
            }
            al_flip_display();
        }
    }

    printf("-> %c\n",text[j]);
    //ALLEGRO_DISPLAY *display;
    //ALLEGRO_EVENT event;
    //ALLEGRO_EVENT_QUEUE *event_queue;
    //ALLEGRO_TIMER *timer;
    //ALLEGRO_FONT *font, *font2;

    return text;
}
