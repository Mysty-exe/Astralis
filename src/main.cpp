#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <EventsManager.h>
#include <StateManager.h>
#include <Transition.h>
#include <filesystem>

using namespace std;

const string TITLE = "Astralis";
int WIDTH = 1280;
int HEIGHT = 720;

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_SetWindowMinimumSize(window, 900, 600);

    std::filesystem::current_path(filesystem::path(argv[0]).parent_path());

    SDL_Surface *icon = IMG_Load("Assets/Icons/gameIcon.png");
    SDL_SetWindowIcon(window, icon);
    SDL_ShowCursor(SDL_DISABLE);

    int imgFlags = IMG_INIT_PNG;
    IMG_Init(imgFlags);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    EventsManager eventManager;
    Transition *transition = new Transition(renderer, WIDTH, HEIGHT, 0.5);
    MessageManager messageManager = MessageManager(WIDTH, HEIGHT);
    StateManager stateManager = StateManager(renderer, WIDTH, HEIGHT);
    stateManager.run(eventManager, messageManager, transition);

    SDL_FreeSurface(icon);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}