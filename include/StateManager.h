#pragma once
#include <SDL.h>
#include <iostream>
#include <Manager.h>
#include <MenuManager.h>
#include <SimulationManager.h>
#include <Transition.h>
#include <Camera.h>
#include <Timer.h>

enum ManagerState
{
    MENU,
    SIMULATION,
    SETTINGS,
    QUIT
};

class StateManager : public Manager
{
private:
    MenuManager menuManager;
    SimulationManager simulationManager;
    ManagerState currentState;
    Timer stepTimer, saveTimer;
    double timeStep;
    bool mute;

public:
    StateManager(SDL_Renderer *renderer, int width, int height);
    ManagerState getState();
    void saveData();
    void handleMusic(EventsManager EventsManager, MessageManager &messageManager);
    void run(EventsManager eventManager, MessageManager messageManager, Transition *transition);
};