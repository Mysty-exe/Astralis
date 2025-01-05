#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <cmath>
#include <CelestialObject.h>
#include <Timer.h>
#include <vector>
#include <Overlay.h>
#include <Sidebar.h>
#include <Frame.h>
#include <Transition.h>

using namespace std;

class Simulation
{
private:
    SDL_Renderer *renderer;
    float width, height, gameWidth, gameHeight;
    string dragState, errorColor;
    int zoom;
    float zoomFactor;

    bool pause, escape, addObjectsTab;
    CelestialObject sun, earth, mars, neptune, rogue, moon;
    vector<CelestialObject> objects;

    Image pointerCursor, handCursor, backgroundImg, pausedTxt, dateTxt, rateTxt, returnTxt, yesTxt, noTxt, yesHoverTxt, noHoverTxt;
    Sidebar addSidebar;
    Overlay overlay;
    Transition transition;

    int currentTimeRate;

    Uint64 NOW, LAST;
    double deltaTime;

    Vector mouse;
    Vector panningOffset, zoomOffset;

public:
    Timer dateTimer, stepTimer;
    Simulation();
    Simulation(SDL_Renderer *renderer, int width, int height);
    void resetSimulation();
    void drawCursor(bool holdClick);
    void drawBackground();
    string escapeTab(string state, bool click);
    void dragHandler(Vector mouseWheel, bool holdClick);
    void eventsHandler(vector<string> events);
    void zoomHandler(Vector mouseWheel);
    string runSimulation(string state, vector<string> events, bool click, bool holdClick, Vector mouseWheel);
};
