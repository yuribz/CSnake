all:
	gcc -Isrc/include -Lsrc/lib -o snake src/snake.c src/queue.c -lmingw32 -lSDL2main -lSDL2