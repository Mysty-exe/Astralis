#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <Events.h>
#include <Menu.h>
#include <SimulationHandler.h>
#include <Vector.h>

using namespace std;

class StateManager
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    Mix_Music *bgMusic;
    string title, state;
    int width, height;

    Events events;
    bool click, holdClick;

    Menu menu;
    SimulationHandler simulationHandler;
    Timer stepTimer;
    double timeStep;

public:
    StateManager(SDL_Window *window, SDL_Renderer *renderer, string title, int width, int height);
    string getTitle();
    string getState();
    void setState(string state);
    int getWidth();
    int getHeight();
    void run();
};