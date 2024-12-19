#include <iostream>
#include <SDL2/SDL.h>
#include <string>

using namespace std;

const string TITLE = "Orbital Simulator";
const int WIDTH = 1200;
const int HEIGHT = 800;
bool quit = false;

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    gWindow = SDL_CreateWindow(TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Event windowEvent;
    while (quit == false)
    {
        while (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
            {
                quit = true;
            }
        }
    }

    SDL_DestroyWindow(gWindow);
    SDL_Quit();

    return 0;
}
