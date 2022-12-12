#include "main.h"
#include "configuracion.h"

typedef struct _tScored{

    char name[MAX];
    Tconf conf;
    int scored;

}Tscored;

Tscored getDefaultScored(char name[]);
Tscored getScored(char name[]);
void saveScored(Tscored *scored, int nscored);
