CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm
all: game

game: game.cpp
	g++ $(CLFAGS) game.cpp eddieV.cpp cesarL.cpp simranS.cpp rodrigoG.cpp christianC.cpp ppm.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o ogirdor \
	-D USE_OPENAL_SOUND \
	/usr/lib/x86_64-linux-gnu/libopenal.so \
	/usr/lib/x86_64-linux-gnu/libalut.so

clean:
	rm -f ogirdor
