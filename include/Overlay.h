#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <cmath>
#include <Image.h>
#include <Utilities.h>
#include <CelestialObject.h>
#include <sstream>

class Overlay
{
private:
    SDL_Renderer *renderer;
    TTF_Font *font, *smallFont;
    SDL_Color textColor;
    float width, height;
    Image radiusTxt, massTxt, velocityTxt, kineticTxt, potentialTxt, rTxt, mTxt, vTxt, kTxt, pTxt, focusedTxt, modeTxt, dateTxt, rateTxt;
    double simSecs, irlSecs;

public:
    Overlay();
    Overlay(SDL_Renderer *renderer, int width, int height);
    void displayTimeRate(int rate);
    void displaySimulationDate(int rate, int secs);
    void displaySimulationStatus(bool paused, bool editing);
    void displayFocusedObject(string name, string objType);
    void displayObjectInfo(CelestialObject obj, int x, int y, int timeRate);
};