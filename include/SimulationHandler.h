#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <cmath>
#include <Events.h>
#include <CelestialObject.h>
#include <Timer.h>
#include <vector>
#include <Sidebar.h>
#include <Frame.h>
#include <Transition.h>
#include <Simulation.h>
#include <TextInput.h>
#include <Button.h>
#include <json.hpp>

using json = nlohmann::json;
using namespace std;

class SimulationHandler
{
private:
    SDL_Renderer *renderer;
    TTF_Font *bigFont, *smallFont;
    Mix_Chunk *errorSound, *clickSound, *click2Sound;
    float width, height;
    bool pause, editing, view, distance, error, pointer;
    float distanceScroll, distanceMaxScroll;
    float newSimScroll, loadSimScroll;

    vector<CelestialObject> objects, editingObjects;

    Animation background;
    Image newSimulationTxt, loadSimulationTxt, noPastSimsTxt, newTxt, controlsHeaderTxt, controlsTxt, templateTxt, editIcon, editHoverIcon;
    Image editSimTxt, nameTxt, distRatioTxt, massRatioTxt, simRadiusTxt;
    TextInput nameInput, distRatioInput, massRatioInput, simRadiusInput;
    Image nameHelperTxt, distRatioHelperTxt, simRadiusHelperTxt;
    Button confirmBtn;
    Image arrow, arrowHover, pointerCursor, handCursor, pausedTxt, dateTxt, rateTxt, returnTxt, yesTxt, noTxt, yesHoverTxt, noHoverTxt, simNameTxt, distancesTxt, distanceNameTxt, distanceTxt;
    Sidebar sidebar;
    SDL_Rect editingRect;

    int focusedObject, currentTimeRate;

    float viewOffset, viewStart;
    Vector panningOffset;

    vector<Simulation> simulations;
    vector<Simulation> templates;
    int currentSimulation, currentTemplate, editSimulationIndex;

public:
    Transition transition;
    SimulationHandler();
    SimulationHandler(SDL_Renderer *renderer, int width, int height);
    void loadSimulationData();
    void saveSimulationData();
    string controls(Events events, string state, double timeStep);
    string newSimulation(Events events, string state, double timeStep);
    string loadSimulation(Events events, string state, double timeStep);
    string editSimulation(Events events, string state, double timeStep);
    string handleEdits(Events events, pair<string, string> distRatioPair, pair<string, string> simRadiusPair, string state);
    string runSimulation(Events events, string state, double timeStep);
    void setInputs(string state);
    void clearInputs();
    void resetSimulation();
    void drawCursor(Events events);
    void drawBackground();
    void eventsHandler(Events events);
    void info(Events events);
    void distances(Events events, double timeStep);
    void viewState(Events events, double timeStep);
    string returnScreen(Events events, string state, double timeStep);
    void editScreen(Events events, double timeStep);
    void mainScreen(Events events, double timeStep);
};
