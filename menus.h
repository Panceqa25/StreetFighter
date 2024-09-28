#include "includes.h"

//tamanho do display
#define X_SCREEN 1280
#define Y_SCREEN 960

//Altura do chao
#define FLOOR_HEIGHT 120

int menu(ALLEGRO_EVENT_QUEUE *queue);

void charSelect(int *idP1, int *idP2, int mode);

int mapSelectMenu(ALLEGRO_EVENT_QUEUE *queue);

int pauseMenu(ALLEGRO_EVENT_QUEUE *queue);
