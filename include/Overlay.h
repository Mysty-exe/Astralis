#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <cmath>
#include <Image.h>
#include <Utilities.h>

class Overlay
{
private:
    SDL_Renderer *renderer;
    TTF_Font *smallFont;
    SDL_Color textColor;
    float width, height;
    Image focusedTxt, modeTxt, dateTxt, rateTxt;
    double simSecs, irlSecs;

public:
    Overlay();
    Overlay(SDL_Renderer *renderer, int width, int height);
    void displayTimeRate(int rate, int zoom);
    void displaySimulationDate(int rate, int secs, int zoom);
    void displaySimulationStatus(bool paused, bool editing, int zoom);
    void displayFocusedObject(string name, string objType, int zoom);
};