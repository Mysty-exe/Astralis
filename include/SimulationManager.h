#pragma once
#include <SDL.h>
#include <iostream>
#include <Manager.h>
#include <EventsManager.h>
#include <MessageManager.h>
#include <Transition.h>
#include <Vector.h>
#include <Camera.h>
#include <Vector.h>
#include <vector>
#include <Animation.h>
#include <Overlay.h>
#include <Frame.h>
#include <Form.h>

enum SimulationState
{
    SIM,
    RETURN,
    EDITING
};

class SimulationManager : public Manager
{
private:
    SimulationState currentState;
    vector<Simulation> simulations;
    SDL_Point point, startClickPos;
    int currentSimulation, viewScroll;

    Camera camera;
    Animation background, loadingTrajectory;
    bool view, hovering, editingError, isAddViewOverlay, isEditOverlay;

    Overlay infoOverlay, addViewOverlay, editOverlay;
    SDL_Rect renderQuad;

    float zoomFactor;

    Texture simulationTexture, returnTxt, yesTxt, noTxt, yesHoverTxt, noHoverTxt;
    Texture inArrowTab, outArrowTab, duplicateIcon, duplicateHoverIcon, binIcon, binHoverIcon;

    int dragObject, editingObject;
    vector<vector<Frame>> frames;
    vector<Input> distanceInputs;

    Form editForm;

public:
    SimulationManager();
    SimulationManager(SDL_Renderer *renderer, int width, int height);
    void resetSimulation();
    void loadImages();
    void loadFrames();
    void setWindowSize(int width, int height);
    SimulationState getCurrentState();
    vector<Simulation> *getSimulations();
    int getSimIndex();
    void setSimIndex(int currentSim);
    Simulation *getSim();
    void drawBackground();
    void eventHandler(EventsManager eventManager, MessageManager &messageManager);
    void showInfoDisplay(EventsManager eventManager);
    void viewObjects(EventsManager eventManager);
    void handleZoom(EventsManager eventManager);
    int runSimulation(EventsManager eventManager, double timeStep);
    int editAddObjects(EventsManager eventManager, double timeStep);
    int editViewObjects(EventsManager eventManager, double timeStep);
    void addViewObjects(EventsManager eventManager, double timeStep);
    int editAnObject(EventsManager eventManager, double timeStep);
    void setFormInputs();
    void checkFormChange();
    void handleEdits(EventsManager eventManager);
    void setDistanceInputs();
    void updateDistances();
    int editDistances(EventsManager eventManager, double timeStep);
    void editObjects(EventsManager eventManager, double timeStep);
    int runEditor(EventsManager eventManager, MessageManager &messageManager, double timeStep);
    void setEditingError(MessageManager &messageManager);
    void resetEditor();
    int runReturn(EventsManager eventManager);
    void displaySimulation(EventsManager eventManager, double timeStep);
    Vector getRenderOffset();
    int run(EventsManager eventManager, MessageManager &messageManager, Transition *transition, double timeStep);
};
