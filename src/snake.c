#include <SDL2/SDL.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "queue.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define Q_DATASIZE (4 * sizeof(short))
#define FPS 5
#define FRAME_DELAY (1000 / FPS)
#define GRID_SIZE 25

SDL_Window *win;
SDL_Renderer *ren;

typedef struct SnakeHead {
    short x, y, w, h;
    short vx;
    short vy; 
} SnakeHead;

typedef Queue SnakeBody;

typedef struct Apple {
    short x, y, w, h;
} Apple;

int setup() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    win = SDL_CreateWindow(
        "CSnake",
        50, 50,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (win == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (ren == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    return 0;
}

void addSegment(SnakeBody *body, short val1, short val2, short val3, short val4) {
    enqueue(body, val1, val2, val3, val4);
}

short* removeSegment(SnakeBody *body) {
    short* data = malloc(sizeof(Q_DATASIZE));
    dequeue(body, data);
    return data;
}

void drawRect(int x, int y, int w, int h) {
    SDL_Rect rect;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_RenderFillRect(ren, &rect);
}

void renderSnake(SnakeBody *b, SnakeHead *h){
    
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    drawRect(h->x, h->y, h->w, h->h);

    // Go through all the segments in the snake's body.

    // Holds the data for the snake's segment
    short* data = malloc(Q_DATASIZE);
    
    for (int i = 0; i < b->size; i++) {
        // Remove the first segment from the snake and store it.
        dequeue(b, data);

        printf("%.3d %.3d %.3d %.3d\n", data[0], data[1], data[2], data[3]);

        drawRect(data[0], data[1], data[2], data[3]);

        // Enter the segment back from the end.
        enqueue(b, data[0], data[1], data[3], data[4]);
    }
    // Destroy the stored data
    free(data);
}

void renderApple(Apple* apple) {
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);

    SDL_Rect rect;

    rect.x = apple->x;
    rect.y = apple->y;
    rect.w = apple->w;
    rect.h = apple->h;

    SDL_RenderFillRect(ren, &rect);
}

int main(int argc, char **arg)
{
    bool running;

    if (setup() != 0) {
        SDL_Quit();
        running = false;
    }
    else {
        running = true;
    }

    // setup the game variables

    SnakeHead head;
    SnakeBody *body;
    Apple apple;

    body = createQueue();


    head.x = 25;
    head.y = 25;
    head.w = GRID_SIZE;
    head.h = GRID_SIZE;
    head.vx = 0;
    head.vy = 0;

    apple.x = 250;
    apple.y = 250;
    apple.w = GRID_SIZE;
    apple.h = GRID_SIZE;

    srand(time(NULL));


    // main loop
    while (running) {
        SDL_Event e;

        int delta = SDL_GetTicks();

        // event handler
        while(SDL_PollEvent(&e)) {
            switch (e.type) {
                // quitting the game
                case SDL_QUIT :
                    running = false;
                    break;
                
                // key pressed
                case SDL_KEYDOWN :
                    switch(e.key.keysym.sym) {
                        case SDLK_ESCAPE :
                            SDL_Event quit_event;
                            quit_event.type = SDL_QUIT;
                            SDL_PushEvent(&quit_event);
                            break;
                        case SDLK_LEFT:
                            head.vx = -GRID_SIZE;
                            head.vy = 0;
                            break;
                        case SDLK_RIGHT:
                            head.vx = GRID_SIZE;
                            head.vy = 0;
                            break;
                        case SDLK_UP:
                            head.vy = -GRID_SIZE;
                            head.vx = 0;
                            break;
                        case SDLK_DOWN:
                            head.vy = GRID_SIZE;
                            head.vx = 0;
                            break;
                        default:
                            break;
                    }
                
                // key released
                case SDL_KEYUP :
                    switch(e.key.keysym.sym) {
                        default:
                            break;
                    }
            }
        }

        // The fun part
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        head.x += head.vx;
        head.y += head.vy;

        addSegment(body, head.x, head.y, head.w, head.h);
        removeSegment(body);
        
        
        renderApple(&apple);
        renderSnake(body, &head);

        if (apple.x == head.x && apple.y == head.y) {
            apple.x = (short) rand() % (SCREEN_WIDTH + 1);
            apple.y = (short) rand() % (SCREEN_HEIGHT + 1);

            apple.x = (apple.x / GRID_SIZE) * GRID_SIZE;
            apple.y = (apple.y / GRID_SIZE) * GRID_SIZE;

            addSegment(body, head.x, head.y, head.w, head.h);
        }

        printf("Snake size: %d\n", body->size);

        
        SDL_RenderPresent(ren);

        delta = SDL_GetTicks() - delta;
        if (FRAME_DELAY > delta) {
            SDL_Delay(FRAME_DELAY - delta);
        }
    }

    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}


