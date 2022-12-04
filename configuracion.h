#ifndef CONFIGURACION_SIMON_DICE
#define CONFIGURACION_SIMON_DICE

#include "configuracion.h"

#define MAX_USER 5

typedef struct _tconf{
    unsigned char keys[MAX_BLOQUE];
    unsigned char ascii[MAX_BLOQUE];
    char usuario[MAX_USER];
}Tconf;


#endif // CONFIGURACION_SIMON_DICE
