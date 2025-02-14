#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <Image.h>
#include <string>

using namespace std;

class CheckBox
{
private:
    SDL_Renderer *renderer;
    SDL_Rect rect;
    int size;

public:
    bool toggle;
    CheckBox();
    CheckBox(SDL_Renderer *renderer, bool toggle, int size);
    void setCoords(float x, float y);
    void setToggle(bool toggle);
    SDL_Rect *getRect();
};
