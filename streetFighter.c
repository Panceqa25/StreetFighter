#include "menus.h"
#include "player.h"
#include "control.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/bitmap.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/bitmap_io.h>
#include <allegro5/color.h>
#include <allegro5/display.h>
#include <allegro5/events.h>
#include <stdio.h>

int main() {

    //Todas as inicializacoes necessarias
    al_init();
    al_init_ttf_addon();
    al_init_font_addon();
    al_init_image_addon();
    al_init_primitives_addon();

    al_install_keyboard();

    ALLEGRO_TIMER *timer = al_create_timer(1.0/30.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_FONT *font = al_load_font("./fonts/retro.ttf",40,0);
    ALLEGRO_DISPLAY *display = al_create_display(X_SCREEN, Y_SCREEN);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    int op = menu(queue);

    if (op == 2) {  //Quit Game
        al_destroy_font(font);
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_event_queue(queue);
        return 0;
    }
    
    int gameMode = op; //0 -> Singleplayer, 1 -> Multiplayer
    
    int idP1 = 0, idP2 = 0;
    charSelect(&idP1,&idP2,gameMode);

    ALLEGRO_EVENT event;

    player *player1 = playerCreate(0, X_SCREEN/2 - 200, Y_SCREEN - 250, X_SCREEN, Y_SCREEN, idP1);
    if (!player1)
        return 1;

    player *player2 = playerCreate(0, X_SCREEN/2 + 200, Y_SCREEN - 250, X_SCREEN, Y_SCREEN, idP2);
    if (!player2)
        return 2;

    ALLEGRO_BITMAP *background;
    int map = 0, quit = 0;

    map = mapSelectMenu(queue);

    if (map == 0)
        background = al_load_bitmap("./backgrounds/mapBackground2.jpg");
    else if (map == 1)
        background = al_load_bitmap("./backgrounds/mapBackground1.jpg");
    if (!background)
        return 1;

    al_start_timer(timer);

    int p1Dead = 0, p2Dead = 0;

    while ((player1->roundsWon < 2) && (player2->roundsWon < 2) && (!quit)) {

        resetPlayer(player1, X_SCREEN/2 - 200, 0);
        resetPlayer(player2, X_SCREEN/2 + 200, 0);
        p1Dead = 0;
        p2Dead = 0;

        while (!p1Dead && !p2Dead && !quit) {
       
            al_wait_for_event(queue, &event);
            if (event.type == 30) {
                if (gameMode == 0)  //Caso seja singleplayer faz o movimento do bot
                    botMove(player1,player2);
                updatePosition(player1,player2);
                if ((!player1->pulando) && (!player2->pulando) && (!player1->caindo) && (!player2->caindo))  //Somente muda a face qnd nao esta no ar
                    checkFace(player1,player2);
                p2Dead = checkHit(player1,player2);  //Confere acerto no player, e retorna 1 caso tenha morrido
                p1Dead = checkHit(player2,player1);
                recoverStamina(player1);  //Recupera stamina do player
                recoverStamina(player2);
                checkAttack(player1);  //Confere se esta atacando
                checkAttack(player2);
                checkJump(player1);  //Confere se esta pulando
                checkJump(player2);
                al_draw_bitmap(background, 0, 0, 0);  //Desenha o cenario
                drawHpBars(player1, player2, font);  //Desenha as barras de vida e stamina
                drawSprite(player1);  //Desenha as sprites dos players
                drawSprite(player2);
                al_flip_display();
            }
            else if ((event.type == 10) || (event.type == 12)) {
                if (event.keyboard.keycode == 1) //A
                    controlLeft(player1->control, &player1->andando);
                else if (event.keyboard.keycode == 4) //D
                    controlRight(player1->control, &player1->andando);
                else if (event.keyboard.keycode == 23)  //W
                    controlUp(player1->control);
                else if (event.keyboard.keycode == 19)   //S
                    controlDown(player1->control);
                else if ((event.keyboard.keycode == 10) && (gameMode == 1))  //J
                    controlLeft(player2->control, &player2->andando);
                else if ((event.keyboard.keycode == 12) && (gameMode == 1))  //L
                    controlRight(player2->control, &player2->andando);
                else if ((event.keyboard.keycode == 9) && (gameMode == 1))  //I
                    controlUp(player2->control);
                else if ((event.keyboard.keycode == 11) && (gameMode == 1))  //K
                    controlDown(player2->control);
                else if ((event.keyboard.keycode == 6) && (player1->stamina >= 10)) // F
                    playerAttack(player1,0);
                else if ((event.keyboard.keycode == 68) && (player2->stamina >= 10) && (gameMode == 1)) // RSHIFT
                    playerAttack(player2,0);
                else if ((event.keyboard.keycode == 5) && (player1->stamina >= 20)) // E
                    playerAttack(player1, 1);
                else if ((event.keyboard.keycode == 15) && (player2->stamina >= 20) && (gameMode == 1))  //RCTRL
                    playerAttack(player2, 1);
                else if ((event.keyboard.keycode == 18) && (event.type != 12) && (player1->spAtkCounter > 0)) // R
                    playerAttack(player1, 2);
                else if ((event.keyboard.keycode == 16) && (event.type != 12) && (player2->spAtkCounter > 0) && (gameMode == 1)) //DEL
                    playerAttack(player2, 2);
                else if (event.keyboard.keycode == 59) // ESC
                    quit = pauseMenu(queue);
                else {
                    player1->curFrame = 0;
                    player2->curFrame = 0;
                }
            }
            else if (event.type == 42) //Clicar no X da janela manda um sinal de quit
                quit = 1;

        }

        if (p1Dead && p2Dead) {
            player1->roundsWon++;
            player2->roundsWon++;
        }
        else if (p1Dead)
            player2->roundsWon++;
        else if (p2Dead)
            player1->roundsWon++;
    }

    if (p1Dead || p2Dead) {
        
        ALLEGRO_BITMAP *endScreen = NULL;
        if (p1Dead && p2Dead)
            endScreen = al_load_bitmap("./menus/tie.png");
        else if (p1Dead)
            endScreen = al_load_bitmap("./menus/p2Victory.png");
        else if (p2Dead)
            endScreen = al_load_bitmap("./menus/p1Victory.png");
        al_draw_bitmap(endScreen,0,0,0);
        al_flip_display();
        al_flush_event_queue(queue);
        while ((event.type != 10) || (event.keyboard.keycode != 67)) //Aguarda ENTER
            al_wait_for_event(queue, &event);
        al_destroy_bitmap(endScreen);
    }

    playerDestroy(player1);
    playerDestroy(player2);

    al_destroy_bitmap(background);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
