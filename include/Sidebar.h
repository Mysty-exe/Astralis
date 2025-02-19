#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <Image.h>
#include <vector>
#include <Events.h>
#include <Frame.h>
#include <CelestialObject.h>
#include <Utilities.h>
#include <Button.h>
#include <TextInput.h>
#include <Simulation.h>
#include <CheckBox.h>
#define PI 3.14159265358979323846

class Sidebar
{
private:
    SDL_Renderer *renderer;
    int width, height;
    int sidebarWidth, sidebarHeight;
    float addTransitionOffset, editTransitionOffset;
    float scrollableHeight;

    SDL_Rect headerRect, addRect, editRect;

    TTF_Font *bigFont, *font, *smallFont;
    SDL_Color color;
    Image check;
    Image circle, inArrow, outArrow, sidebarTxt, objectTxt, objectTypeTxt, distancesTxt, distanceNameTxt, distanceTxt, headerTxt, nameTxt, radiusTxt, radiusHelperTxt, massTxt, massHelperTxt, velocityTxt, velocityHelperTxt, directionTxt, degreeHelperTxt, particlesTxt, particlesHelperTxt, kmTxt, kgTxt, msTxt, degreeTxt;
    TextInput nameInput, radiusInput, massInput, velocityInput, degreeInput;
    string header;
    CheckBox checkBox;

    vector<Image> headers;
    vector<Frame> frames;

public:
    int editingObject, selectedObject;
    bool viewState, addState, editState, distanceState, addActivated, editActivated, sliderActivated;
    float scroll, viewScroll, editScroll, editMaxScroll, distanceScroll, maxScroll, viewMaxScroll, distanceMaxScroll;
    Button deleteBtn, duplicateBtn;
    Sidebar();
    Sidebar(SDL_Renderer *renderer, int width, int height);
    SDL_Rect *getRect();
    SDL_Rect *getHeaderRect();
    vector<Frame> getFrames();
    void setHeader(string header);
    void setFrames();
    void viewObjects(Events events, Simulation &sim, Vector &panningOffset, double deltaTime);
    void addObject(Events events, Vector spawnLoc, Simulation &sim, double deltaTime);
    void editObject(Events events, Vector spawnLoc, Simulation &sim, double deltaTime);
    void distancesObject(Events events, Simulation &sim, double deltaTime);
    void runSidebar(Events events, Vector spawnLoc, Simulation &sim, Vector &panningOffset, double timeStep);
    bool checkHover(SDL_Point point, Simulation sim);
    void updateObject(Events events, Simulation &sim, double deltaTime);
    void setInputs(Simulation sim, CelestialObject &object);
    pair<string, string> parseInput(string input);
    void clearInputs();
    void reset();
};
