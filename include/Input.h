#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <Texture.h>
#include <string>
#include <Timer.h>
#include <EventsManager.h>
#include <Widget.h>
#include <Slider.h>

class Input : public Widget
{
private:
    Texture headerTxtImg, hintTxtImg, errorTxtImg;
    string headerTxt, hintTxt, errorTxt;

    Timer cursorTimer;

    int startWidth, maxChars;
    bool showCursor;

    Slider slider;

public:
    Input();
    Input(SDL_Renderer *renderer, string header, string text, string hint, string error, int startWidth, int maxChars);
    float getX();
    float getEndX();
    float getY();
    float getEndY();
    float getWidth();
    float getHeight();
    bool getSliderActivated();
    void setPos(float x, float y);
    SDL_Rect *getRect();
    string getHeader();
    void setText(string text);
    void clearError();
    void setErrorText(string error);
    Slider getSlider();
    void setSlider(Slider slider);
    int getSliderValue();
    void setSliderValue(float value);
    void process(EventsManager eventManager);
    void render(SDL_Renderer *renderer, float width, float height, float xscroll = 0, float yscroll = 0);
};
