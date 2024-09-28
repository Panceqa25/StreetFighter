#include "menus.h"
#include <allegro5/bitmap.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/bitmap_io.h>
#include <allegro5/color.h>
#include <allegro5/drawing.h>
#include <allegro5/events.h>
#include <stdio.h>


int menu(ALLEGRO_EVENT_QUEUE *queue) {

    int op = 0, y_select = 629;

    ALLEGRO_EVENT event;
    ALLEGRO_BITMAP *mainMenu = NULL;
    mainMenu = al_load_bitmap("./menus/menuImage.png");

    while (event.keyboard.keycode != 67) {
        
        al_draw_bitmap(mainMenu, 0, 0, 0);
        al_draw_rectangle((X_SCREEN/2) - 280, y_select, (X_SCREEN/2) + 280, y_select+65, al_map_rgb(160,32,240),3); //Desenha o indicador
        al_flip_display();

        al_wait_for_event(queue, &event);
        if (((event.keyboard.keycode == 85) || (event.keyboard.keycode == 84)) && (event.type != 12)) {  //Arrow Down/Up
            if (op == 0) {
                if (event.keyboard.keycode == 84) {  // 0 -> 2
                    y_select = 784;
                    op = 2;
                }
                else if (event.keyboard.keycode == 85) {  // 0 -> 1
                    y_select = 707;
                    op = 1;
                }
            }
            else if (op == 1) {
                if (event.keyboard.keycode == 84) { // 1 -> 0 
                    y_select = 629;
                    op = 0;
                }
                else if (event.keyboard.keycode == 85) {  // 1 -> 2
                    y_select = 784;
                    op = 2;
                }
            }
            else if (op == 2) {
                if (event.keyboard.keycode == 84) {  // 2 -> 1
                    y_select = 707;
                    op = 1;
                }
                else if (event.keyboard.keycode == 85) { // 2 -> 0
                    y_select = 629;
                    op = 0;
                }
            }
        }
        al_flush_event_queue(queue);
    }

    //Destroi o bitmap mainMenu
    al_destroy_bitmap(mainMenu);

    return op;
}

void charSelect(int *idP1, int *idP2, int gameMode) {

    int op = 0, y_select = (Y_SCREEN/2) - 212, x_select = (X_SCREEN/2) - 215, cont = 1;

    ALLEGRO_EVENT event;

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());

    ALLEGRO_BITMAP *selectScreen = NULL;
    selectScreen = al_load_bitmap("./menus/charSelect.png");


    while (cont < 2 + gameMode) {  //Executa o loop uma ou duas vezes, depende do gameMode

        //Remove o buffer da fila
        al_flush_event_queue(queue);
        al_wait_for_event(queue, &event);
        event.keyboard.keycode = 1;

        while (event.keyboard.keycode != 67) { //Executa ate pressionar ENTER

            al_draw_bitmap(selectScreen, 0, 0, 0);  //Desenha o fundo da selecao de personagem
            al_draw_rectangle(x_select-35, y_select-35, x_select+178, y_select+267, al_map_rgb(160,32,240),5); //Desenha o indicador
            al_flip_display();
        
            al_wait_for_event(queue, &event);
            if ((event.keyboard.keycode == 85) || (event.keyboard.keycode == 84)) {  //Arrow Down/Up
                if ((op == 0) || (op == 1)) {
                    y_select = (Y_SCREEN/2) + 149;
                    op += 2;
                }
                else if ((op == 2) || (op == 3)) {
                    y_select = (Y_SCREEN/2) - 212;
                    op -= 2;
                }
            }
            else if ((event.keyboard.keycode == 82) || (event.keyboard.keycode == 83)) {  //Arrow Left/Right
                if ((op == 0) || (op == 2)) {
                    x_select = (X_SCREEN/2) + 72;
                    op += 1;
                }
                else if ((op == 1) || (op == 3)) {
                    x_select = (X_SCREEN/2) - 215;
                    op -= 1;
                }
            }
        }
        if (cont == 1)
            *idP1 = op;  //Indica o indice do personagem do Player 1
        else
            *idP2 = op;  //Indica o indice do personagem do Player 2
        cont++;
    }
    
    al_destroy_bitmap(selectScreen);
    al_destroy_event_queue(queue);

}

int mapSelectMenu(ALLEGRO_EVENT_QUEUE *queue) {

    int map = 0, x_select = (X_SCREEN/2) - 419;

    ALLEGRO_EVENT event;
    ALLEGRO_BITMAP *mapSelect = NULL;
    mapSelect = al_load_bitmap("./menus/mapSelect.png");

    al_flush_event_queue(queue);
    event.keyboard.keycode = 1;

    while (event.keyboard.keycode != 67) {
        
        al_draw_bitmap(mapSelect, 0, 0, 0);
        al_draw_rectangle(x_select, (Y_SCREEN/2) - 33, x_select + 343, (Y_SCREEN/2) + 303, al_map_rgb(160,32,240),7); //Desenha o indicador
        al_flip_display();

        al_wait_for_event(queue, &event);
        if (((event.keyboard.keycode == 82) || (event.keyboard.keycode == 83)) && (event.type != 12)) {  //Arrow Left/Right
            if (map == 0) {
                x_select = (X_SCREEN/2) + 81;
                map = 1;
            }
            else if (map == 1) {
                x_select = (X_SCREEN/2) - 419;
                map = 0;
            }
        }
        al_flush_event_queue(queue);
    }

    //Destroi o bitmap mainMenu
    al_destroy_bitmap(mapSelect);

    return map;
}

int pauseMenu(ALLEGRO_EVENT_QUEUE *queue) {

    int op = 0, y_select = 709;

    ALLEGRO_EVENT event;
    ALLEGRO_BITMAP *pause = NULL;
    pause = al_load_bitmap("./menus/pauseMenu.png");

    al_flush_event_queue(queue);
    al_wait_for_event(queue, &event);
    event.keyboard.keycode = 1;

    while (event.keyboard.keycode != 67) {
       
        al_draw_bitmap(pause,0,0,0);
        al_draw_rectangle((X_SCREEN/2) - 200, y_select, (X_SCREEN/2) + 203, y_select+65, al_map_rgb(160,32,240),3); //Desenha o indicador
        al_flip_display();
        
        al_wait_for_event(queue, &event);
        if (((event.keyboard.keycode == 85) || (event.keyboard.keycode == 84)) && (event.type != 12)) {  //Arrow Down/Up
            if (op == 0) {
                y_select = 792;
                op = 1;
            }
            else if (op == 1) {
                y_select = 709;
                op = 0;
            }
        }
        al_flush_event_queue(queue);
    }
    
    al_destroy_bitmap(pause);

    return op;

}
