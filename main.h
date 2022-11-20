#ifndef DEFINE_MAIN
#define DEFINE_MAIN

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

typedef struct _timagen{
    ALLEGRO_BITMAP *image;
    int width;
    int height;
    int tamx;
    int tamy;
    int posx;
    int posy;
    float x;
    float y;
    float scale;
    float _angle;
}Timagen;

int cargar_allegro();

#endif // DEFINE_MAIN
