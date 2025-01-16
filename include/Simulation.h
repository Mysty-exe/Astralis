#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <cmath>
#include <Events.h>
#include <CelestialObject.h>
#include <Timer.h>
#include <vector>
#include <Overlay.h>
#include <Sidebar.h>
#include <Frame.h>
#include <Transition.h>

using namespace std;

enum SimulationState
{
    MAIN,
    RETURN,
    EDITING,
    INFO
};

class Simulation
{
private:
    SDL_Renderer *renderer;
    float width, height, gameWidth, gameHeight;
    string dragState, errorColor;
    SimulationState simState;
    bool pause, editing;
    int zoom;
    float zoomFactor;

    bool pointer;
    CelestialObject sun, earth, mars, neptune, rogue, moon;
    vector<CelestialObject> objects, editingObjects;

    Image pointerCursor, handCursor, backgroundImg, pausedTxt, dateTxt, rateTxt, returnTxt, yesTxt, noTxt, yesHoverTxt, noHoverTxt;
    Sidebar sidebar;
    Overlay overlay;
    Transition transition;
    SDL_Rect editingRect;

    int focusedObject, currentTimeRate;

    Uint64 NOW, LAST;
    double timeStep;

    Vector panningOffset, zoomOffset;
    int selectedObject;

public:
    Timer dateTimer, stepTimer;
    Simulation();
    Simulation(SDL_Renderer *renderer, int width, int height);
    void resetSimulation();
    void drawCursor(Events events);
    void drawBackground();
    void eventsHandler(Events events);
    string returnScreen(Events events, string state);
    void editScreen(Events events);
    void infoScreen(Events events);
    void mainScreen(Events events);
    string runSimulation(Events events, string state);
};
