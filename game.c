#include "game.h"
#include "menu.h"

int start_game(){

    /*
        Todas las variables que se ocuparan para el flujo del programa
    */
    ALLEGRO_EVENT_QUEUE *event_queue;//Puntero a una cola de eventos;
    ALLEGRO_DISPLAY *display;//Puntero hacia una pantalla;
    ALLEGRO_TIMER *timer;//Puntero a un temporizador
    ALLEGRO_BITMAP *sprites, *logo;//Puntero a mapa de bits
    ALLEGRO_EVENT event;//el que controlara todo evento

    ObjetoMenu miMenu;

    int errores = 0;
    int ventana = 0;
    bool repetir = true;

    display = al_create_display(WIDTH, HEIGHT);
    if(!display)
        return -1;

    event_queue = al_create_event_queue();
    if(!event_queue)
        return -2;

    timer = al_create_timer(1.0/FPS);
    if(!timer)
        return -3;

    sprites = al_load_bitmap("sprites.png");
    if(!sprites)
        return -4;

    logo = al_create_sub_bitmap(sprites, 1484, 810, 134, 134);
    if(!sprites)
        return -5;

    al_set_app_name("SIMON DICE ...SUS");
    al_set_window_title(display, "SIMON DICE");
    al_set_display_icon(display, logo);

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_start_timer(timer);

    miMenu = obtenerObjetoMenu();

    while(repetir){
        al_wait_for_event(event_queue, &event);

        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            repetir = false;

        if(event.type == ALLEGRO_EVENT_KEY_DOWN){

            if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                repetir = false;
            }else{
                if(event.keyboard.keycode == ALLEGRO_KEY_ENTER)
                    if(ventana == 0)
                        ventana = 1;

                if(event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                    if(ventana == 0){
                        cambioDeBotones(ABAJO, &miMenu);
                    }

                if(event.keyboard.keycode == ALLEGRO_KEY_UP)
                    if(ventana == 0){
                        cambioDeBotones(ARRIBA, &miMenu);
                    }

            }

        }
        /*
        if(event.type == ALLEGRO_EVENT_MOUSE_AXES)
            printf("x:%d y:%d\n",event.mouse.x, event.mouse.y);*/

        if(al_is_event_queue_empty(event_queue)){
            if(repetir){
                al_clear_to_color(al_map_rgb(0, 0, 0));
                if(errores == 0){
                    al_draw_scaled_bitmap(al_load_bitmap("fondo.png"), 0, 0, 1600, 900, 0, 0, WIDTH, HEIGHT, 0);
                    if(ventana == 0){

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
                        if(ventana == 1){
                            //errores =
                        }else{
                            if(ventana == 2){
                                //errores = records();
                            }else{
                                if(ventana == 3){

                                }
                            }
                        }
                    }

                    al_flip_display();
                }
            }
        }
    }


    al_destroy_bitmap(logo);
    al_destroy_bitmap(sprites);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);

    return 0;
}
