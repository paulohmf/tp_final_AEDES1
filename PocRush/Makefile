ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=\include

all: PocRush.exe


PocRush.exe: PocRush.o
	gcc -o PocRush.exe PocRush.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

PocRush.o: PocRush.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c PocRush.c	


	

clean:

	del PocRush.o
	del PocRush.exe

