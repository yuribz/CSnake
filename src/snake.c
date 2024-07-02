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
    // printf("Segment added! Size: %d\n", body->size);
}

void removeSegment(SnakeBody *body) {
    short* data = malloc(sizeof(Q_DATASIZE));
    dequeue(body, data);
    free(data);
}

void drawRect(int x, int y, int w, int h) {
    SDL_Rect rect = {
        x = x,
        y = y,
        w = w,
        h = h
    };

    SDL_RenderFillRect(ren, &rect);
    SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
    SDL_RenderDrawRect(ren, &rect);
}

void renderSnake(SnakeBody *b, SnakeHead *h){
    
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    drawRect(h->x, h->y, h->w, h->h);

    // Go through all the segments in the snake's body.

    // Holds the data for the snake's segment
    short* data = malloc(Q_DATASIZE);

    int size = queueSize(b);

    // printf("The head segment is at %.3d %.3d\n", h->x, h->y);

    // printf("The size of the snake is currently at %d\n", size);
    
    for (int i = 0; i < size; i++) {
        // Remove the first segment from the snake and store it.
        
        dequeue(b, data);

        // if (i == 0) printf("The first segment is at %.3d %.3d\n", data[0], data[1]);
        // else if (i == size - 1) printf("The last segment is at %.3d %.3d\n", data[0], data[1]);

        if (i > 3 && i != size - 1 && data[0] == h->x && data[1] == h->y) {
            SDL_Event quit_event;
            quit_event.type = SDL_QUIT;
            SDL_PushEvent(&quit_event);
        }


        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        drawRect(data[0], data[1], data[2], data[3]);

        // Enter the segment back from the end.
        enqueue(b, data[0], data[1], data[2], data[3]);
    }
    // Destroy the stored data
    free(data);
}

void renderApple(Apple* apple) {
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);

    drawRect(apple->x, apple->y, apple->w, apple->h);
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

        // Render the background and the snake
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        renderApple(&apple);
        renderSnake(body, &head);

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
                
            }
        }

        // The fun part
        addSegment(body, head.x, head.y, head.w, head.h);

        head.x += head.vx;
        head.y += head.vy;

        removeSegment(body);

        if (
            head.x < 0 || head.x > SCREEN_WIDTH - GRID_SIZE ||
            head.y < 0 || head.y > SCREEN_HEIGHT - GRID_SIZE
            ) {
                SDL_Event quit_event;
                quit_event.type = SDL_QUIT;
                SDL_PushEvent(&quit_event);
            }
        
       

        // Check for apple/snake collision.
        if (apple.x == head.x && apple.y == head.y) {
            apple.x = (short) rand() % (SCREEN_WIDTH + 1);
            apple.y = (short) rand() % (SCREEN_HEIGHT + 1);

            apple.x = (apple.x / GRID_SIZE) * GRID_SIZE;
            apple.y = (apple.y / GRID_SIZE) * GRID_SIZE;

            addSegment(body, head.x, head.y, head.w, head.h);
            
        }

        // printf("Snake size: %d\n", body->size);

        
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


