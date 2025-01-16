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
    Image inArrow, outArrow, sidebarTxt, distancesTxt, distanceNameTxt, distanceTxt, headerTxt, nameTxt, radiusTxt, radiusHelperTxt, massTxt, massHelperTxt, velocityTxt, velocityHelperTxt, kmTxt, kgTxt, msTxt;
    TextInput nameInput, radiusInput, massInput, velocityInput;
    string header;

    vector<Image> headers;
    vector<Frame> frames;

public:
    int editingObject, selectedObject;
    bool addState, editState, distanceState, addActivated, editActivated;
    float scroll, distanceScroll, maxScroll, distanceMaxScroll;
    Button deleteBtn, duplicateBtn;
    Sidebar();
    Sidebar(SDL_Renderer *renderer, int width, int height);
    SDL_Rect *getRect();
    SDL_Rect *getHeaderRect();
    vector<Frame> getFrames();
    void setHeader(string header);
    void setFrames();
    void addObject(Events events, Vector spawnLoc, vector<CelestialObject> &objects, double deltaTime);
    void editObject(Events events, Vector spawnLoc, vector<CelestialObject> &objects);
    void distancesObject(Events events, vector<CelestialObject> &objects, double deltaTime);
    void runSidebar(Events events, Vector spawnLoc, vector<CelestialObject> &objects, double timeStep);
    bool checkHover(SDL_Point point, vector<CelestialObject> objects);
    void updateObject(Events events, vector<CelestialObject> &objects);
    void setInputs(CelestialObject &object);
    pair<string, string> parseInput(string input);
    void clearInputs();
    void reset();
};
