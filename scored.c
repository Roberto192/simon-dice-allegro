#include "scored.h"
#include "game.h"

Tscored getDefaultScored(char name[]){

    Tscored scored;

    strcpy_s(scored.name, MAX+1, name);
    scored.conf = getConfigDeafault();
    scored.scored = 0;
    return scored;

}

Tscored getScored(char name[]){

    Tscored scored;
    ALLEGRO_FILE *arch;

    arch = al_fopen("scoreds.smndc", "rb");
    if(!arch){
        arch = al_fopen("scoreds.smndc", "wb");
        al_fclose(arch);
        return getDefaultScored(name);
    }

    while(al_fread(arch, &scored, sizeof(struct _tScored))){
        if(strcmp(name, scored.name) == 0){
            al_fclose(arch);
            return scored;
        }
    }

    al_fclose(arch);
    return getDefaultScored(name);
}

void saveScored(Tscored *scored, int nscored){

    ALLEGRO_FILE *arch;
    Tscored buff;
    scored->scored = nscored;
    arch = al_fopen("scoreds.smndc", "r+b");
    int i = 0;
    while(al_fread(arch, &buff, sizeof(struct _tScored))){
        if(strcmp(buff.name, scored->name) == 0){
            break;
        }else{
            i++;
        }
    }
    printf("pos -> %d\n",i);
    al_fseek(arch, sizeof(struct _tScored) * i, SEEK_SET);
    al_fwrite(arch, scored, sizeof(struct _tScored));
    al_fclose(arch);

}
