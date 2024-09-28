#ifndef __PLAYER__
#define __PLAYER__

#include "includes.h"
#include "control.h"
#include "menus.h"

#define TAM_STEP 20

typedef struct sprites {

    ALLEGRO_BITMAP *walk;
    int walkCols;

    ALLEGRO_BITMAP *attack;
    int attackCols;

    ALLEGRO_BITMAP *jump;
    int jumpCols;

    ALLEGRO_BITMAP *idle;
    int idleCols;

    ALLEGRO_BITMAP *dead;
    int deadCols;

    ALLEGRO_BITMAP *kick;
    int kickCols;

    ALLEGRO_BITMAP *spAtk;
    int spAtkCols;

}playerSprites;

typedef struct attack {

    int x;
    int y;
    int punchReach;
    int kickReach;

}attack;

typedef struct player {

    //Informacoes do personagem
    int stamina;
    int roundsWon;
    unsigned char id;
    char hp;
    unsigned char largura;
    unsigned char altura;
    playerSprites *sprites;
    int spAtkCounter;

    //Posicao do personagem
    int curFrame;
    int curFrameAtk;
    unsigned short x;
    unsigned short y;
    unsigned char face;

    //Acoes do personagem
    control *control;
    attack *attack;
    unsigned char spAtk;
    unsigned char caindo;
    unsigned char socando;
    unsigned char pulando;
    unsigned char andando;
    unsigned char agachado;
    unsigned char chutando;

}player;

//Criacao do player
player* playerCreate(unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, int id);
void playerDestroy(player *p);

//Movimento
void playerMove(player *p, char steps, unsigned char trajectory, unsigned short max_x);
unsigned char collision(player *player1, player *player2);
void resetPlayer(player *p, int x, unsigned char face);
void updatePosition(player *player1, player *player2);
void checkFace(player *p1, player *p2);
void botMove(player *p1, player *p2);
void recoverStamina(player *p);
void checkJump(player *p);

//Ataque
attack* attackCreate(int punchReach, int kickReach);
void playerAttack(player *p, int attackType);
int checkHit(player *p1, player *p2);
void attackDestroy(attack *attack);
void checkAttack(player *p);

//Sprites/Desenhos
void drawHpBars(player *p1, player *p2, ALLEGRO_FONT *font);
void spriteDestroy(playerSprites *sprites);
playerSprites* spriteCreate(int id);
void drawSprite(player *p);

#endif
