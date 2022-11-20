#include "menu.h"

ObjetoMenu obtenerObjetoMenu(){

    ObjetoMenu objeto;
    static ALLEGRO_BITMAP *logo;
    static ALLEGRO_BITMAP *botones;
    static ALLEGRO_BITMAP *play;
    static ALLEGRO_BITMAP *config;
    static ALLEGRO_BITMAP *scored;

    int tamx_button = 503;
    int tamy_button = 89;


    botones = al_load_bitmap("botones_menu.png");
    //cargo de datos del logo
    logo = al_load_bitmap("simon_dice.png");

    objeto.logo.image = logo;
    objeto.logo.height = al_get_bitmap_height(logo);
    objeto.logo.width = al_get_bitmap_width(logo);
    objeto.logo.scale = 0;
    objeto.logo._angle = 0;
    objeto.logo.x = 0;
    objeto.logo.y = 0;
    objeto.logo.tamx = 865;
    objeto.logo.tamy = 108;

    //cargo de datos del play
    play = al_create_sub_bitmap(botones, 0, 0, tamx_button, (tamy_button*4)+(12*3));

    objeto.play.img.image = play;
    objeto.play.img.height = al_get_bitmap_height(play);
    objeto.play.img.width = al_get_bitmap_width(play);
    objeto.play.img.tamx = tamx_button;
    objeto.play.img.tamy = tamy_button;
    objeto.play.img.scale = 0;
    objeto.play.img.x = 0;
    objeto.play.img.y = 0;
    objeto.play.img.posx = (1280/2) - (objeto.play.img.width/2);
    objeto.play.img.posy = 300;
    objeto.play.img._angle = 0;
    objeto.play.press = false;

    return objeto;
}

int menu(ObjetoMenu *objeto){

    ALLEGRO_BITMAP *logo = objeto->logo.image;
    ALLEGRO_BITMAP *play = objeto->play.img.image;
    //ALLEGRO_BITMAP *play = objeto->play.img.image;
    //ALLEGRO_BITMAP *records = objeto->records.img.image;
    //ALLEGRO_BITMAP *conf = objeto->configs.img.image;

    al_convert_mask_to_alpha(logo, al_map_rgb(0, 0, 0));
    //al_draw_bitmap(logo, objeto->logo.x, objeto->logo.y, 0);
    al_draw_scaled_bitmap(logo, objeto->logo.x, objeto->logo.y, objeto->logo.tamx, objeto->logo.tamy, objeto->logo.width/4, 0, objeto->logo.width/2, objeto->logo.height/4, 0);
    al_draw_scaled_bitmap(play, objeto->play.img.x, objeto->play.img.y, objeto->play.img.tamx, objeto->play.img.tamy, objeto->play.img.posx, objeto->play.img.posy, objeto->play.img.tamx, objeto->play.img.tamy, 0);

    return 0;
}
