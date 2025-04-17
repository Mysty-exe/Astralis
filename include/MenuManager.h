#pragma once
#include <Manager.h>
#include <Transition.h>
#include <Animation.h>
#include <Scrollbar.h>
#include <Form.h>
#include <fstream>
#include <json.hpp>

using json = nlohmann::json;
using namespace std;

enum MenuState
{
    MAIN,
    NEW,
    LOAD,
    NEWEDIT,
    LOADEDIT,
    CONTROLS
};

class MenuManager : public Manager
{
private:
    MenuState currentState;
    Animation titleTxtAnimation;
    Texture backgroundImg, newTxt, newHoverTxt, loadTxt, loadHoverTxt, controlsTxt, controlsHoverTxt, quitTxt, quitHoverTxt;
    Texture newSimulationTxt, loadSimulationTxt, noPastSimsTxt, newHeaderTxt, controlsHeaderTxt, templateHeaderTxt, editSimulationTxt, allControlsTxt, simNameTxt;
    Texture arrow, arrowHover, editIcon, editHoverIcon, binIcon, binHoverIcon;
    vector<Simulation> templates;
    int currentTemplate;

    Scrollbar newScrollbar, loadScrollbar;
    Form form;

public:
    MenuManager();
    MenuManager(SDL_Renderer *renderer, int width, int height);
    void loadSimulationData(vector<Simulation> *simulations);
    void saveSimulationData(vector<Simulation> *simulations);
    void loadImages();
    MenuState getCurrentState();
    void setWindowSize(int width, int height);
    int run(EventsManager eventManager, Transition *transition, vector<Simulation> *simulations, int &simIndex, double timeStep);
    int runMainMenu(EventsManager eventManager, double timeStep);
    int runNewSim(EventsManager eventManager, vector<Simulation> *simulations, double timeStep);
    int runLoadSim(EventsManager eventManager, vector<Simulation> *simulations, int &simIndex, double timeStep);
    int runEditSim(EventsManager eventManager, vector<Simulation> *simulations, int &simIndex, double timeStep);
    int runControls(EventsManager eventManager, double timeStep);
    void setEditInputs(vector<Simulation> *simulations, int &simIndex);
    bool handleEdits(vector<Simulation> *simulations, int simIndex);
    void handleSim(vector<Simulation> *simulations, int &simIndex);
};
