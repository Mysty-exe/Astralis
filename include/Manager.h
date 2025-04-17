#pragma once
#include "SDL.h"
#include "SDL_mixer.h"
#include "EventsManager.h"
#include "MessageManager.h"
#include "Simulation.h"
#include "Texture.h"
#include "Timer.h"

enum Color
{
    WHITE,
    BLACK,
    PURPLE
};

class Manager
{
protected:
    SDL_Renderer *renderer;
    EventsManager eventManager;
    int width, height;
    bool hovering;

    Mix_Music *bgMusic;
    Mix_Chunk *clickSound;
    Texture pointerCursor, handCursor;
    TTF_Font *largeFont, *bigFont, *smallFont;

public:
    Manager();
    Manager(SDL_Renderer *renderer, int width, int height);
    void setWindowSize(int width, int height);
    Vector getWindowRatio();
    void drawCursor(Vector pos, bool click);
    void playBgMusic();
    void playClickSound();
    void setColor(Color color);
};
