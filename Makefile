all:
	gcc -c -o builds/linkedlist.o src/linkedlist.c
	gcc -c -o builds/queue.o src/queue.c
	gcc -Isrc/include -Lsrc/lib -o snake src/snake.c builds/queue.o builds/linkedlist.o -lmingw32 -lSDL2main -lSDL2