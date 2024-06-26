all:
	g++ -Isrc/include -Lsrc/lib -o snake src/snake.c -lmingw32 -lSDL2main -lSDL2