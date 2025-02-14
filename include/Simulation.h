#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <cmath>
#include <CelestialObject.h>
#include <vector>
#include <Timer.h>
#include <Image.h>

using namespace std;

enum SimulationState
{
    SIMULATION,
    RETURN,
    EDITING,
    INFOc
};

class Simulation
{
public:
    SDL_Renderer *renderer;

    string name;
    SimulationState state;
    int objectsNum;
    vector<CelestialObject> objects;
    double distRatio;
    double simRadius;
    Timer dateTimer;

    TTF_Font *font, *smallFont;
    double irlSecs, simSecs;
    SDL_Color textColor;
    Image focusedTxt, modeTxt, dateTxt, rateTxt, radiusTxt, massTxt, velocityTxt, kineticTxt, potentialTxt;
    Image rTxt, mTxt, vTxt, kTxt, pTxt;

    Simulation();
    Simulation(SDL_Renderer *renderer, string name, double distRatio, double simRadius);
    long double scaleDistance(long double dist);
    long double scaleMass(long double mass);
    long double getRealDistance(long double dist);
    long double getRealMass(long double mass);
    void scaleObjects(string n);
    void applyForces(double timeStep, int timeRate);
    void applyVelocities(double timeStep);
    void calculateEnergy();
    void display(Vector panningOffset, int timeRate, double timeStep);
    void updateAllSizes();
    void displayTimeRate(int rate);
    void displaySimulationDate(int rate, int height);
    void displaySimulationStatus(bool paused, bool editing);
    void displayFocusedObject(string name, string objType);
    void displayObjectInfo(CelestialObject obj, int x, int y, int timeRate);
};
