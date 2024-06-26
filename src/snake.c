#include <SDL2/SDL.h>
#include <stdio.h>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 360

int main(int argc, char **arg)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *win = SDL_CreateWindow(
        "title",
        30, 30,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    SDL_Surface *screen = SDL_GetWindowSurface(win);
    SDL_Delay(2000);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
