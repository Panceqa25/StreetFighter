#include "player.h"
#include "control.h"
#include <allegro5/allegro_font.h>
#include <allegro5/bitmap.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/bitmap_io.h>
#include <allegro5/color.h>
#include <allegro5/inline/fmaths.inl>
#include <stdio.h>

#define FLOOR_HEIGHT 120
#define JUMP_HEIGHT 120


//Criacao do player

//Inicializa um player
player* playerCreate(unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, int id) {

    player *newPlayer = (player*) malloc(sizeof(player));
    if (!newPlayer)
        return NULL;
   
    //Informacoes do personagem
    newPlayer->id = id;
    newPlayer->hp = 100;
    newPlayer->stamina = 100;
    if ((id == 2) || (id == 3))
        newPlayer->spAtkCounter = 1;
    else
        newPlayer->spAtkCounter = 0;
    newPlayer->sprites = spriteCreate(id);  //Cria sua sprite baseada no seu id
    newPlayer->roundsWon = 0;
    newPlayer->altura = al_get_bitmap_height(newPlayer->sprites->walk)/2 + 20;  //As dimensoes do personagem sao baseadas na sprite
    newPlayer->largura = al_get_bitmap_width(newPlayer->sprites->walk)/2/newPlayer->sprites->walkCols - 40;

    //Posicao do personagem
    newPlayer->x = x;
    newPlayer->y = y - newPlayer->altura/2;
    newPlayer->face = face;
    newPlayer->curFrame = 0;
    newPlayer->curFrameAtk = 0;

    //Acoes do personagem
    newPlayer->spAtk = 0;
    newPlayer->pulando = 0;
    newPlayer->caindo = 0;
    newPlayer->agachado = 0;
    newPlayer->socando = 0;
    newPlayer->chutando = 0;
    newPlayer->andando = 0;
    newPlayer->attack = attackCreate(50,50);
    newPlayer->control = controlCreate();


    if ((x - newPlayer->largura/2 < 0) || (x + newPlayer->largura/2 > max_x) || (y - newPlayer->altura/2 < 0) || (y + newPlayer->altura/2 > max_y))
        return NULL;

    return newPlayer;

}

void playerDestroy(player *p) {

    spriteDestroy(p->sprites);
    controlDestroy(p->control);
    attackDestroy(p->attack);
    free(p);

}
//--------------------------------------------------------------------------------------------------------------------------------

//Movimento do player

void botMove(player *p1, player *p2) {

    if (p2->x - p2->largura/2 - 50 > p1->x + p1->largura/2) {
        if (p2->control->left == 0)
            controlLeft(p2->control, &p2->andando);
        if (p2->control->right == 1)
            controlRight(p2->control, &p2->andando);
    }
    else if (p2->x + p2->largura/2 + 50 < p1->x - p1->largura/2) {
        if (p2->control->right == 0)
            controlRight(p2->control, &p2->andando);
        if (p2->control->left == 1)
            controlLeft(p2->control, &p2->andando);
    }
    else {
        if (p2->control->right == 1)
            controlRight(p2->control, &p2->andando);
        if (p2->control->left == 1)
            controlLeft(p2->control, &p2->andando);
        if ((!p2->socando) && (p2->stamina >= 10)) {
            int atk = rand() % 5;
            if (atk == 0)
                playerAttack(p2, 0);
        }
    }


}
unsigned char collision(player *player1, player *player2) {

    if ((((player2->y - player2->altura/2 >= player1->y - player1->altura/2) && (player1->y + player1->altura/2 >= player2->y - player2->altura/2)) 
        || ((player1->y - player1->altura/2 >= player2->y - player2->altura/2) && (player2->y + player2->altura/2 >= player1->y - player1->altura/2))) 
        && (((player2->x - player2->largura/2 >= player1->x - player1->largura/2) && (player1->x + player1->largura/2 >= player2->x - player2->largura/2)) 
        || ((player1->x - player1->largura/2 >= player2->x - player2->largura/2) && (player2->x + player2->largura/2 >= player1->x - player1->largura/2)))) return 1;
    else 
        return 0;
    
}

//Move o player por um numero determinado de passos em uma certa trajetoria
void playerMove(player *p, char steps, unsigned char trajectory, unsigned short max_x) {

    if (!trajectory) {  //Esquerda
        p->curFrame++;
        if ((p->x - steps*TAM_STEP) - p->largura/2 >= 0)
            p->x = p->x - steps*TAM_STEP;
        if (p->agachado) {
            p->altura *= 2;
            p->y -= p->altura/4;
            p->agachado = 0;
        }
    }
    else if (trajectory == 1) {  //Direita
        p->curFrame++;
        if ((p->x + steps*TAM_STEP) + p->largura/2 <= max_x)
            p->x = p->x + steps*TAM_STEP;
        if (p->agachado) {
            p->altura *= 2;
            p->y -= p->altura/4;
            p->agachado = 0;
        }
    }
    else if ((trajectory == 2) && (!p->pulando)) {  //Cima
        p->curFrame++;
        if (!p->pulando)
            p->pulando = 1;
        if (p->agachado) {
            p->altura *= 2;
            p->y -= p->altura/4;
            p->agachado = 0;
        }
    }
    else if (trajectory == 3) {  //Baixo
        if (!p->agachado) {
            p->altura /= 2;
            p->y += p->altura/2;
            p->agachado = 1;
        }
    }
}

void updatePosition(player *player1, player *player2) {

    if (player1->control->left) {
        playerMove(player1, 1, 0, X_SCREEN);
        if (collision(player1,player2))
            playerMove(player1, -1, 0, X_SCREEN);

    }
    if (player1->control->right) {
        playerMove(player1, 1, 1, X_SCREEN);
        if (collision(player1,player2))
            playerMove(player1, -1, 1, X_SCREEN);

    }
    if (player1->control->up) {
        playerMove(player1, 1, 2, X_SCREEN);
        if (collision(player1,player2))
            playerMove(player1, -1, 2, X_SCREEN);

    }
    if (player1->control->down) {
        playerMove(player1, 1, 3, X_SCREEN);
        if (collision(player1,player2))
            playerMove(player1, -1, 3, X_SCREEN);

    }
    if (player2->control->left) {
        playerMove(player2, 1, 0, X_SCREEN);
        if (collision(player2,player1))
            playerMove(player2, -1, 0, X_SCREEN);

    }
    if (player2->control->right) {
        playerMove(player2, 1, 1, X_SCREEN);
        if (collision(player2,player1))
            playerMove(player2, -1, 1, X_SCREEN);

    }
    if (player2->control->up) {
        playerMove(player2, 1, 2, X_SCREEN);
        if (collision(player2,player1))
            playerMove(player2, -1, 2, X_SCREEN);

    }
    if (player2->control->down) {
        playerMove(player2, 1, 3, X_SCREEN);
        if (collision(player2,player1))
            playerMove(player2, -1, 3, X_SCREEN);

    }
    if ((player1->caindo) && (collision(player1,player2)))
        player1->x += 50 - 100*player1->face;
     if ((player2->caindo) && (collision(player2,player1)))
        player2->x += 50 - 100*player2->face;   
}

void checkJump(player *p) {

    if (p->y - p->altura/2 <= Y_SCREEN - 580) { //Quando chega na altura max do pulo, comeca a cair
        p->caindo = 1;
        p->pulando = 0;
    }
    else if (p->y + p->altura/2 >= Y_SCREEN - 120)
        p->caindo = 0;
    if ((p->caindo) && (p->y + p->altura/2 < Y_SCREEN - 120)) {
        p->curFrame++;
        p->y += JUMP_HEIGHT/12;
    }
    else if ((p->pulando) && (p->y - p->altura/2 > Y_SCREEN - 580)) {
        p->curFrame++;
        p->y -= JUMP_HEIGHT/12;
    }
}

void checkAttack(player *p) {

    if (p->socando) {
      if (p->curFrameAtk <= p->sprites->attackCols)
            p->curFrameAtk++;
        else if (p->curFrameAtk > p->sprites->attackCols) {
            p->socando = 0;
            p->curFrameAtk = 0;
        }
    }
    else if (p->chutando) {
        if (p->curFrameAtk <= p->sprites->kickCols)
            p->curFrameAtk++;
        else if (p->curFrameAtk > p->sprites->kickCols) {
            p->chutando = 0;
            p->curFrameAtk = 0;
        }
    }
    else if (p->spAtk) {
        if (p->curFrameAtk <= p->sprites->spAtkCols)
            p->curFrameAtk++;
        else if (p->curFrameAtk > p->sprites->spAtkCols) {
            p->spAtk = 0;
            p->curFrameAtk = 0;
        }
    }

}

//Reseta a posicao e as acoes dos players entre cada round
void resetPlayer(player *p, int x, unsigned char face) {

    p->hp = 100;

    p->x = x;
    p->y = Y_SCREEN - 120 - p->altura/2;
    p->face = face;
    p->curFrame = 0;
    p->curFrameAtk = 0;
    p->stamina = 100;
    if ((p->id == 2) || (p->id == 3))
        p->spAtkCounter = 1;

    p->pulando = 0;
    p->caindo = 0;
    p->agachado = 0;
    p->socando = 0;
    p->chutando = 0;
    p->spAtk = 0;
    p->andando = 0;

}

//Recuperacao passiva de stamina
void recoverStamina(player *p) {

    if ((p->chutando) || (p->socando))
        return;

    p->stamina += 1;

    if (p->stamina > 100)
        p->stamina = 100;

}

//Muda a direcao que o player esta olhando baseado na sua posicao
void checkFace(player *p1, player *p2) {

    if (p1->x - p1->largura/2 > p2->x + p2->largura/2) {
        p1->face = 1;
        p2->face = 0;
    }
    else {
        p1->face = 0;
        p2->face = 1;
    }


}

//--------------------------------------------------------------------------------------------------------------------------------

//Ataque do player

attack* attackCreate(int punchReach, int kickReach) {

    attack *newAttack = (attack*) malloc(sizeof(attack));
    if (!newAttack)
        return NULL;

    newAttack->x = -1;
    newAttack->y = -1;
    newAttack->punchReach = punchReach;
    newAttack->kickReach = kickReach;
    
    return newAttack;

}

void attackDestroy(attack *attack) {

    free(attack);

}

void playerAttack(player *p, int attackType) {

    if ((p->caindo) || (p->andando) || (p->pulando) || (p->agachado) || (p->socando) || (p->chutando) || (p->spAtk))
        return;

    printf("ANDANDO %c: %d\n", p->id, p->andando);

    if (attackType == 0) {
        if (p->face == 0)
            p->attack->x = p->x + p->largura/2;
        else
            p->attack->x = p->x - p->largura/2;
        p->attack->y = p->y - p->altura/5;
        p->socando = 1;
    }
    else if (attackType == 1) {
        if (p->face == 0)
            p->attack->x = p->x + p->largura/2;
        else
            p->attack->x = p->x - p->largura/2;
        p->attack->y = p->y + p->altura/5;
        p->chutando = 1;
    }
    else if ((attackType == 2) && ((p->id == 3) || (p->id == 2))) {
        if (p->face == 0)
            p->attack->x = p->x + p->largura/2;
        else
            p->attack->x = p->x - p->largura/2;
        if (p->id == 3)
            p->attack->y = p->y - (p->altura/5);
        p->spAtk = 1;
        p->spAtkCounter--;
    }
}

int checkHit(player *p1, player *p2) {

    int reach;

    if ((p1->socando) && (p1->curFrameAtk != p1->sprites->attackCols))
        return 0;
    if ((p1->chutando) && (p1->curFrameAtk != p1->sprites->kickCols))
        return 0;
    if ((p1->spAtk) && (p1->curFrameAtk != p1->sprites->spAtkCols))
        return 0;

    if (p1->socando) {
        reach = p1->attack->punchReach;
        p1->stamina -= 20;
    }
    else if (p1->chutando) {
        reach = p1->attack->kickReach;
        p1->stamina -= 30;
    }
    else if (p1->spAtk) {
        reach = p1->attack->punchReach;
        p1->stamina = 100;
    }

    if (p1->face == 0) {
        if (((p1->attack->x + reach >= p2->x - p2->largura/2) && (p1->attack->x + reach <= p2->x + p2->largura/2)) &&
          ((p1->attack->y >= p2->y - p2->altura/2) && (p1->attack->y <= p2->y + p2->altura/2))) {
            if (p1->socando)
                p2->hp -= 5;
            else if (p1->chutando)
                p2->hp -= 10;
            else if (p1->spAtk)
                p2->hp -= 30;
        }
    }
    else if (p1->face == 1) {
        if (((p1->attack->x - reach <= p2->x + p2->largura/2) && (p1->attack->x - reach >= p2->x - p2->largura/2)) &&
          ((p1->attack->y >= p2->y - p2->altura/2) && (p1->attack->y <= p2->y + p2->altura/2))) {
            if (p1->socando)
                p2->hp -= 5;
            else if (p1->chutando)
                p2->hp -= 10;
            else if (p1->spAtk)
                p2->hp -= 30;
        }
    }

    if (p2->hp <= 0)
        return 1;
    return 0;

}

//--------------------------------------------------------------------------------------------------------------------------------

//Sprites

playerSprites* spriteCreate(int id) {

    playerSprites *newSprite = (playerSprites*) malloc(sizeof(playerSprites));

    if (id == 0) {
        
        newSprite->walk = al_load_bitmap("./characters/Shinobi/Walk.png");
        newSprite->walkCols = 8;
        newSprite->attack = al_load_bitmap("./characters/Shinobi/Attack_1.png");
        newSprite->attackCols = 5;
        newSprite->jump = al_load_bitmap("./characters/Shinobi/Jump.png");
        newSprite->jumpCols = 12;
        newSprite->idle = al_load_bitmap("./characters/Shinobi/Idle.png");
        newSprite->idleCols = 6;
        newSprite->dead = al_load_bitmap("./characters/Shinobi/Dead.png");
        newSprite->deadCols = 4;
        newSprite->kick = al_load_bitmap("./characters/Shinobi/Kick.png");
        newSprite->kickCols = 3;
        newSprite->spAtk = NULL;

    }
    else if (id == 1) {

        newSprite->walk = al_load_bitmap("./characters/Samurai/Walk.png");
        newSprite->walkCols = 8;
        newSprite->attack = al_load_bitmap("./characters/Samurai/Attack_1.png");
        newSprite->attackCols = 6;
        newSprite->jump = al_load_bitmap("./characters/Samurai/Jump.png");
        newSprite->jumpCols = 12;
        newSprite->idle = al_load_bitmap("./characters/Samurai/Idle.png");
        newSprite->idleCols = 6;
        newSprite->dead = al_load_bitmap("./characters/Samurai/Dead.png");
        newSprite->deadCols = 3;
        newSprite->kick = al_load_bitmap("./characters/Samurai/Kick.png");
        newSprite->kickCols = 3;
        newSprite->spAtk = NULL;
    }
    else if (id == 2) {
        
        newSprite->walk = al_load_bitmap("./characters/Fighter/Walk.png");
        newSprite->walkCols = 8;
        newSprite->attack = al_load_bitmap("./characters/Fighter/Attack_1.png");
        newSprite->attackCols = 4;
        newSprite->jump = al_load_bitmap("./characters/Fighter/Jump.png");
        newSprite->jumpCols = 10;
        newSprite->idle = al_load_bitmap("./characters/Fighter/Idle.png");
        newSprite->idleCols = 6;
        newSprite->dead = al_load_bitmap("./characters/Fighter/Dead.png");
        newSprite->deadCols = 3;
        newSprite->kick = al_load_bitmap("./characters/Fighter/Attack_3.png");
        newSprite->kickCols = 4;
        newSprite->spAtk = al_load_bitmap("./characters/Fighter/spAtk.png");
        newSprite->spAtkCols = 3;

    }
    else if (id == 3) {

        newSprite->walk = al_load_bitmap("./characters/Vampire/Walk.png");
        newSprite->walkCols = 6;
        newSprite->attack = al_load_bitmap("./characters/Vampire/Attack_1.png");
        newSprite->attackCols = 5;
        newSprite->jump = al_load_bitmap("./characters/Vampire/Jump.png");
        newSprite->jumpCols = 6;
        newSprite->idle = al_load_bitmap("./characters/Vampire/Idle.png");
        newSprite->idleCols = 5;
        newSprite->dead = al_load_bitmap("./characters/Vampire/Dead.png");
        newSprite->deadCols = 10;
        newSprite->kick = al_load_bitmap("./characters/Vampire/Kick.png");
        newSprite->kickCols = 3;
        newSprite->spAtk = al_load_bitmap("./characters/Vampire/Attack_4.png");
        newSprite->spAtkCols = 5;

    }

    return newSprite;

}

void spriteDestroy(playerSprites *sprites) {

    al_destroy_bitmap(sprites->walk);
    al_destroy_bitmap(sprites->attack);
    al_destroy_bitmap(sprites->jump);
    al_destroy_bitmap(sprites->idle);
    al_destroy_bitmap(sprites->dead);
    al_destroy_bitmap(sprites->kick);
    if (sprites->spAtk != NULL)
        al_destroy_bitmap(sprites->spAtk);

    free(sprites);

}

void drawHpBars(player *p1, player *p2, ALLEGRO_FONT *font) {

    unsigned char r = 0, g = 255, b = 0;
    if (p1->hp <= 50)
        r = 255;
    if (p1->hp <= 30)
        g = 0;
        
    
    //Player1's HP Bar
    al_draw_textf(font,al_map_rgb(255,255,255),10,5,0,"%d",p1->roundsWon); //p1's points
    al_draw_textf(font,al_map_rgb(255,255,255),10,50,0,"SpAtk: %d",p1->spAtkCounter); //p1's spAtk counter
    al_draw_rectangle(50, 10, X_SCREEN/2 - 10, 50, al_map_rgb(255, 255, 255), 4);
    al_draw_filled_rectangle(50 + ( 516 - (516*p1->hp)/100), 10, X_SCREEN/2 - 10, 50, al_map_rgb(r, g, b));

    //Player1's Stamina Bar
    al_draw_rectangle(X_SCREEN/2 - X_SCREEN/4, 55, X_SCREEN/2 - 10, 80, al_map_rgb(255, 255, 255), 2);
    al_draw_filled_rectangle(X_SCREEN/2 - X_SCREEN/4 + ( 288 - (288*p1->stamina)/100), 55, X_SCREEN/2 - 10, 80, al_map_rgb(0, 0, 255));

    g = 255, r = 0;
    if (p2->hp <= 50)
        r = 255;
    if (p2->hp <= 30)
        g = 0;

    //Player2's HP Bar
    al_draw_textf(font,al_map_rgb(255,255,255),X_SCREEN-40,5,0,"%d",p2->roundsWon); // p2's points
    al_draw_textf(font,al_map_rgb(255,255,255),X_SCREEN-213,50,0,"SpAtk: %d",p2->spAtkCounter);  //p2's spAtk counter
    al_draw_rectangle(X_SCREEN - 50, 10, X_SCREEN/2 + 10, 50, al_map_rgb(255, 255, 255), 4);
    al_draw_filled_rectangle(X_SCREEN - 50 - (516 - (516*p2->hp)/100), 10, X_SCREEN/2 + 10, 50, al_map_rgb(r, g, b));

    //Player2's Stamina Bar
    al_draw_rectangle(X_SCREEN - X_SCREEN/2 + X_SCREEN/4, 55, X_SCREEN/2 + 10, 80, al_map_rgb(255, 255, 255), 2);
    al_draw_filled_rectangle(X_SCREEN - X_SCREEN/2 + X_SCREEN/4 - ( 288 - (288*p2->stamina)/100), 55, X_SCREEN/2 + 10, 80, al_map_rgb(0, 0, 255));
}

void drawSprite(player *p) {

    int frameWidth, frameHeight, fx;

    if ((p->control->left) && (!p->pulando) && (!p->caindo)) {  //Esquerda
        frameWidth = al_get_bitmap_width(p->sprites->walk);
        frameHeight = al_get_bitmap_height(p->sprites->walk);       
        fx = (p->curFrame % p->sprites->walkCols) * frameWidth/p->sprites->walkCols;
        int a = frameWidth - frameWidth/p->sprites->walkCols;
        if (p->face == 0)
            al_draw_bitmap_region(p->sprites->walk, a - fx, 0, frameWidth/p->sprites->walkCols, frameHeight, p->x - frameWidth/2/p->sprites->walkCols, p->y - frameHeight/2/p->sprites->walkCols - 200, 0);
        else if (p->face == 1)
            al_draw_bitmap_region(p->sprites->walk, a - fx, 0, frameWidth/p->sprites->walkCols, frameHeight, p->x - frameWidth/2/p->sprites->walkCols, p->y - frameHeight/2/p->sprites->walkCols - 200, ALLEGRO_FLIP_HORIZONTAL);
    }
    else if ((p->control->right) && (!p->pulando) && (!p->caindo)) {  //Direita
        frameWidth = al_get_bitmap_width(p->sprites->walk);
        frameHeight = al_get_bitmap_height(p->sprites->walk);
        fx = (p->curFrame % p->sprites->walkCols) * frameWidth/p->sprites->walkCols;
        if (p->face == 0)
            al_draw_bitmap_region(p->sprites->walk, fx, 0, frameWidth/p->sprites->walkCols, frameHeight, p->x - frameWidth/2/p->sprites->walkCols, p->y - frameHeight/2/p->sprites->walkCols - 200, 0);
        else if (p->face == 1)
            al_draw_bitmap_region(p->sprites->walk, fx, 0, frameWidth/p->sprites->walkCols, frameHeight, p->x - frameWidth/2/p->sprites->walkCols, p->y - frameHeight/2/p->sprites->walkCols - 200, ALLEGRO_FLIP_HORIZONTAL);
    }
    else if (p->control->down) {  //Agachar
        frameWidth = al_get_bitmap_width(p->sprites->dead);
        frameHeight = al_get_bitmap_height(p->sprites->dead);
        if (p->face == 0)
            al_draw_bitmap_region(p->sprites->dead, frameWidth/p->sprites->deadCols, 0, frameWidth/p->sprites->deadCols, frameHeight, p->x - frameWidth/2/p->sprites->deadCols, p->y - frameHeight/2/p->sprites->deadCols - 200 - p->altura/2 + 5, 0);
        else if (p->face == 1)
            al_draw_bitmap_region(p->sprites->dead, frameWidth/p->sprites->deadCols, 0, frameWidth/p->sprites->deadCols, frameHeight, p->x - frameWidth/2/p->sprites->deadCols, p->y - frameHeight/2/p->sprites->deadCols - 200 - p->altura/2 + 5, ALLEGRO_FLIP_HORIZONTAL);
    }
    else if ((p->control->up) || (p->pulando) || (p->caindo)) {  //Pular
        frameWidth = al_get_bitmap_width(p->sprites->jump);
        frameHeight = al_get_bitmap_height(p->sprites->jump);
        fx = (p->curFrame % p->sprites->jumpCols) * frameWidth/p->sprites->jumpCols;
        if (p->face == 0)
            al_draw_bitmap_region(p->sprites->jump, fx, 0, frameWidth/p->sprites->jumpCols, frameHeight, p->x - frameWidth/2/p->sprites->jumpCols, p->y - frameHeight/2/p->sprites->jumpCols- 200,0);
        else if (p->face == 1)
            al_draw_bitmap_region(p->sprites->jump, fx, 0, frameWidth/p->sprites->jumpCols, frameHeight, p->x - frameWidth/2/p->sprites->jumpCols, p->y - frameHeight/2/p->sprites->jumpCols- 200,ALLEGRO_FLIP_HORIZONTAL);
    }
    else if (p->socando) {
        frameWidth = al_get_bitmap_width(p->sprites->attack);
        frameHeight = al_get_bitmap_height(p->sprites->attack);
        fx = (p->curFrameAtk % p->sprites->attackCols) * frameWidth/p->sprites->attackCols;
        if (p->face == 0)
            al_draw_bitmap_region(p->sprites->attack, fx, 0, frameWidth/p->sprites->attackCols, frameHeight, p->x - frameWidth/2/p->sprites->attackCols, p->y - frameHeight/2/p->sprites->attackCols - 200, 0);
        else if (p->face == 1)
            al_draw_bitmap_region(p->sprites->attack, fx, 0, frameWidth/p->sprites->attackCols, frameHeight, p->x - frameWidth/2/p->sprites->attackCols, p->y - frameHeight/2/p->sprites->attackCols - 200, ALLEGRO_FLIP_HORIZONTAL);
    }
    else if (p->chutando) {
        frameWidth = al_get_bitmap_width(p->sprites->kick);
        frameHeight = al_get_bitmap_height(p->sprites->kick);
        fx = (p->curFrameAtk % p->sprites->kickCols) * frameWidth/p->sprites->kickCols;
        if (p->face == 0)
            al_draw_bitmap_region(p->sprites->kick, fx, 0, frameWidth/p->sprites->kickCols, frameHeight, p->x - frameWidth/2/p->sprites->kickCols, p->y - frameHeight/2/p->sprites->kickCols - 200, 0);
        else if (p->face == 1)
            al_draw_bitmap_region(p->sprites->kick, fx, 0, frameWidth/p->sprites->kickCols, frameHeight, p->x - frameWidth/2/p->sprites->kickCols, p->y - frameHeight/2/p->sprites->kickCols - 200, ALLEGRO_FLIP_HORIZONTAL);
    }
    else if (p->spAtk) {
        frameWidth = al_get_bitmap_width(p->sprites->spAtk);
        frameHeight = al_get_bitmap_height(p->sprites->spAtk);
        fx = (p->curFrameAtk % p->sprites->spAtkCols) * frameWidth/p->sprites->spAtkCols;
        if (p->face == 0)
            al_draw_bitmap_region(p->sprites->spAtk, fx, 0, frameWidth/p->sprites->spAtkCols, frameHeight, p->x - frameWidth/2/p->sprites->spAtkCols, p->y - frameHeight/2/p->sprites->spAtkCols - 200, 0);
        else if (p->face == 1)
            al_draw_bitmap_region(p->sprites->spAtk, fx, 0, frameWidth/p->sprites->spAtkCols, frameHeight, p->x - frameWidth/2/p->sprites->spAtkCols, p->y - frameHeight/2/p->sprites->spAtkCols - 200, ALLEGRO_FLIP_HORIZONTAL);
    }
    else if (!p->socando) {  //Idle
        if (p->agachado) {
            p->altura *= 2;
            p->y -= p->altura/4;
            p->agachado = 0;
        }
        frameWidth = al_get_bitmap_width(p->sprites->idle);
        frameHeight = al_get_bitmap_height(p->sprites->idle);
        fx = (p->curFrame % p->sprites->idleCols) * frameWidth/p->sprites->idleCols;
        if (p->face == 0)
            al_draw_bitmap_region(p->sprites->idle, fx, 0, frameWidth/p->sprites->idleCols, frameHeight, p->x - frameWidth/2/p->sprites->idleCols, p->y - frameHeight/2/p->sprites->idleCols- 200, 0);
        else if (p->face == 1)
            al_draw_bitmap_region(p->sprites->idle, fx, 0, frameWidth/p->sprites->idleCols, frameHeight, p->x - frameWidth/2/p->sprites->idleCols, p->y - frameHeight/2/p->sprites->idleCols- 200, ALLEGRO_FLIP_HORIZONTAL);
    }
    

}
