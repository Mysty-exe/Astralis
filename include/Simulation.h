#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <algorithm>
#include <string>
#include <sstream>
#include <cmath>
#include <thread>
#include <CelestialObject.h>
#include <vector>
#include <Timer.h>
#include <Texture.h>
#include <Overlay.h>

using namespace std;

class Simulation
{
private:
    SDL_Renderer *renderer;

    string name;
    int objectsNum;
    vector<CelestialObject> objects;
    double distRatio, simRadius;
    Timer dateTimer;
    int timeRate;
    int focusedObject;

    TTF_Font *font, *smallFont, *tinyFont;
    double irlSecs, simSecs;
    SDL_Color textColor;
    Texture focusedTxtImg, modeTxtImg, dateTxtImg, rateTxtImg, radiusTxtImg, massTxtImg, velocityTxtImg, kineticTxtImg, potentialTxtImg, trajectoryImg;
    Texture radiusUnitTxt, massUnitTxt, velocityUnitTxt, kineticUnitTxt, potentialUnitTxt;
    Texture distanceTxt, distanceNameTxt;

    bool paused, editing, overlapping, outOfBounds, calculatedTrajectory;

public:
    Simulation();
    Simulation(SDL_Renderer *renderer, string name, double distRatio, double simRadius);
    void reset();
    void loadImages(Vector windowRatio);
    void resizeViewObjects(float width, float height);
    string getName();
    double getDistRatio();
    double getSimRadius();
    int getObjectsNum();
    void addObjectsNum();
    Vector getRealPosition(Vector position);
    double getSimSecs();
    int getTimeRate();
    void setName(string name);
    void setDistRatio(double ratio);
    void setSimRadius(double radius);
    void setObjectsNum(int objectsNum);
    void setSimSecs(double simSecs);
    void resetErrors();
    void pause();
    void unPause();
    bool isPaused();
    bool isError();
    bool isEditing();
    void edit(bool e);
    bool isOverlapping();
    void toggleOverlapping(bool overlapping);
    bool isOutOfBounds();
    void toggleOutOfBounds(bool outOfBounds);
    void setTrajectoryCalculated(bool calculatedTrajectory);
    bool trajectoryCalculated();
    int getObjectsSize();
    long double scaleDistance(long double dist);
    long double scaleMass(long double mass);
    long double getRealDistance(long double dist);
    long double getRealMass(long double mass);
    bool isFocusing();
    CelestialObject getObject(int index);
    CelestialObject getFocusedObject();
    vector<CelestialObject> getObjects();
    void addObject(CelestialObject obj);
    void deleteObject(int index);
    void scaleObjects(string n);
    void updateObject(int index, string name, long double radius, long double mass, Vector velocity);
    void updateObjectPosition(int index, Vector position);
    void run(double timeStep);
    void applyForces(double timeStep);
    void applyVelocities(double timeStep);
    void dragObject(int obj, Vector dragOffset);
    void calculateEnergy();
    void focusObject();
    void focusObject(int index);
    void defocusObjects();
    void speedUp();
    void slowDown();
    void runTrajectoryThread(Vector panningOffset);
    void calculateTrajectory(Vector panningOffset);
    void drawTrajectory(CelestialObject obj, Vector panningOffset);
    void display(SDL_Rect *renderQuad, Vector panningOffset, double timeStep, int displayLast = -1);
    void updateAllSizes();
    void displayTimeRate(Vector windowRatio);
    void displaySimulationDate(int height, Vector windowRatio);
    void displayPauseStatus(Vector windowRatio);
    void displayFocusedObject(Vector windowRatio);
    int displayObjectInfo(CelestialObject obj, Overlay overlay, Vector windowRatio);
    int displayDistances(CelestialObject obj, Overlay overlay, Vector windowRatio);
};
