CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: game

game: game.cpp
	g++ -no-pie $(CLFAGS) game.cpp eddieV.cpp rodrigoG.cpp christianC.cpp ppm.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o ogirdor

clean:
	rm -f ogirdor
	rm -f *.o
