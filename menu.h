#ifndef MENU_SIMON_DICE

#include "main.h"

typedef struct _tbutton{
    Timagen img;
    bool press;
}Tbutton;

typedef struct _objetoMenu{

    Timagen logo;
    Tbutton play;
    Tbutton records;
    Tbutton configs;

}ObjetoMenu;

ObjetoMenu obtenerObjetoMenu();
int menu(ObjetoMenu *objeto);

#endif // MENU_SIMON_DICE
