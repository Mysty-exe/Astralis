#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <StateManager.h>

using namespace std;

const string TITLE = "Astralis";
const int WIDTH = 2560 / 2;
const int HEIGHT = 1440 / 2;

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    SDL_Window *window = SDL_CreateWindow(TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_Surface *icon = IMG_Load("assets/icon.png");
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    int imgFlags = IMG_INIT_PNG;
    IMG_Init(imgFlags);
    TTF_Init();

    SDL_ShowCursor(SDL_DISABLE);

    StateManager stateManager = StateManager(window, renderer, TITLE, WIDTH, HEIGHT);
    stateManager.run();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}