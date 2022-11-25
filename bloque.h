#ifndef BLOQUE_SIMON_DICE
#define BLOQUE_SIMON_DICE

#include "main.h"

typedef struct _tbloque{

    Timagen imagenPrincipal;
    Timagen imagenLetra;
    char letra;
    int status;

}Tbloque;

void dibujarBloque(Tbloque *bloque);

#endif // BLOQUE_SIMON_DICE
