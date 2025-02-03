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
    TTF_Font *bigFont, *smallFont;
    float width, height, gameWidth, gameHeight;
    string dragState, errorColor;
    SimulationState simState;
    bool pause, editing, view, distance, error;
    int zoom;
    float zoomFactor, distanceScroll, distanceMaxScroll;

    TTF_Font *font;

    bool pointer;
    vector<CelestialObject> objects, editingObjects;

    Animation background;
    Image pointerCursor, handCursor, nameTxt, pausedTxt, dateTxt, rateTxt, returnTxt, yesTxt, noTxt, yesHoverTxt, noHoverTxt, distancesTxt, distanceNameTxt, distanceTxt;
    Sidebar sidebar;
    Overlay overlay;
    Transition transition;
    SDL_Rect editingRect;

    int focusedObject, currentTimeRate;

    Uint64 NOW, LAST;
    double timeStep;

    float viewOffset, viewStart;
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
    void info(Events events);
    void distances(Events events);
    void viewState(Events events);
    string returnScreen(Events events, string state);
    void editScreen(Events events);
    void mainScreen(Events events);
    string runSimulation(Events events, string state);
};
