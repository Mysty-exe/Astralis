#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <Image.h>
#include <string>
#include <Timer.h>
#include <Events.h>

using namespace std;

class TextInput
{
private:
    SDL_Renderer *renderer;

    TTF_Font *font, *smallFont;
    SDL_Rect rect;
    SDL_Color color;
    bool showCursor;

    Image buttonTxt, errorTxt;
    string text, errorText;

    Timer cursorTimer;

public:
    bool activated, error;
    TextInput();
    TextInput(SDL_Renderer *renderer, string text, SDL_Color color);
    void setCoords(float x, float y);
    string getText();
    void setText(string text, int maxWidth);
    void setErrorText(string text);
    SDL_Rect *getRect();
    void render(Events events, int maxWidth, float xscroll = 0, float yscroll = 0);
};
