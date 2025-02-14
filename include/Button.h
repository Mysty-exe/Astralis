#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <Image.h>
#include <string>

using namespace std;

class Button
{
private:
    SDL_Renderer *renderer;

    TTF_Font *font;
    SDL_Rect rect;
    SDL_Color color;

    Image buttonTxt;
    string text;

public:
    Button();
    Button(SDL_Renderer *renderer, string text, SDL_Color color, int width);
    void setCoords(float x, float y);
    SDL_Rect *getRect();
    void render();
};
