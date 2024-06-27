#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdbool.h>
#include "queue.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

const int Q_DATASIZE = 4 * sizeof(short);

SDL_Window *win;
SDL_Renderer *ren;

typedef struct SnakeHead {
    short x, y, w, h;
    short vx;
    short vy; 
} SnakeHead;

typedef Queue SnakeBody;

int setup() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    win = SDL_CreateWindow(
        "title",
        30, 30,
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
    
    SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
    drawRect(h->x, h->y, h->w, h->h);

    // Go through all the segments in the snake's body.

    // Holds the data for the snake's segment
    short* data = malloc(Q_DATASIZE);
    
    for (int i = 0; i < b->size; i++) {
        // Remove the first segment from the snake and store it.
        dequeue(b, data);

        drawRect(data[0], data[1], data[2], data[3]);

        // Enter the segment back from the end.
        enqueue(b, data[0], data[1], data[3], data[4]);
    }
    // Destroy the stored data
    free(data);
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

    body = createQueue();


    head.x = 50;
    head.y = 50;
    head.w = 50;
    head.h = 50;
    head.vx = 0;
    head.vy = 0;


    // main loop
    while (running) {
        SDL_Event e;

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
                            head.vx = -1;
                        case SDLK_RIGHT:
                            head.vx = 1;
                        case SDLK_UP:
                            head.vy = -1;
                        case SDLK_DOWN:
                            head.vy = 1;
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
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderClear(ren);

        head.x += head.vx;
        head.y += head.vy;

        
        renderSnake(body, &head);

        SDL_RenderPresent(ren);
    }

    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}


