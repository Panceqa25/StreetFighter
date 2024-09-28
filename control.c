#include "control.h"

control* controlCreate() {

    control* newControl = (control*) malloc(sizeof(control));

    if (!newControl)
        return NULL;

    newControl->right = 0;
    newControl->left = 0;
    newControl->up = 0;
    newControl->down = 0;

    return newControl;

}

void controlRight(control *c, unsigned char *andando) {

    c->right = c->right ^ 1;
    (*andando) = (*andando) ^ 1;

}

void controlLeft(control *c, unsigned char *andando) {

    c->left = c->left ^ 1;
    (*andando) = (*andando) ^ 1;
}

void controlUp(control *c) {

    c->up = c->up ^ 1;

}

void controlDown(control *c) {

    c->down = c->down ^ 1;

}

void controlDestroy(control *c) {

    free(c);

}

