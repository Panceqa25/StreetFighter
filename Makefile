nomePrograma=streetFighter

CFLAGS = -Wall -Wextra -std=c99 $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 --cflags)
LIBS = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 --libs)

all: $(nomePrograma)

$(nomePrograma): streetFighter.o control.o menus.o player.o
	gcc -o $(nomePrograma) streetFighter.o control.o menus.o player.o $(CFLAGS) $(LIBS)

streetFighter.o: streetFighter.c
	gcc -c streetFighter.c $(CFLAGS)

control.o:  control.c control.h
	gcc -c control.c $(CFLAGS)

menus.o: menus.c menus.h
	gcc -c menus.c $(CFLAGS)

player.o: player.c player.h
	gcc -c player.c $(CFLAGS)


clean:
	rm -f *.o *.gch

purge:
	rm -f *.o *.gch $(nomePrograma)
