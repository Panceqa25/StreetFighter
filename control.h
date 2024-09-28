#ifndef __CONTROLE__
#define __CONTROLE__

#include "includes.h"

typedef struct controle {

    unsigned char right;
    unsigned char left;
    unsigned char up;
    unsigned char down;

}control;

//Criacao de destruicao do controle
control* controlCreate();
void controlDestroy(control *c);

//Manipulacao do controle
void controlRight(control *c, unsigned char *andando);
void controlLeft(control *c, unsigned char *andando);
void controlUp(control *c);
void controlDown(control *c);

#endif
