#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <EventsManager.h>
#include <Texture.h>

class Widget
{
protected:
    SDL_Renderer *renderer;

    TTF_Font *headerFont, *regFont, *hintFont;
    SDL_Rect rect;

    Texture rectTxt;
    string text;

    bool error, isActivated;

public:
    Widget();
    Widget(SDL_Renderer *renderer, string text);
    SDL_Rect *getRect();
    bool getError();
    string getText();
    void deactivate();
    void activate();
    bool getActivated();
    virtual void clearError();
    virtual void setErrorText(string error);
    virtual float getX() = 0;
    virtual float getY() = 0;
    virtual float getEndY() = 0;
    virtual void setPos(float x, float y) = 0;
    virtual void setText(string text) = 0;
    virtual void process(EventsManager eventManager) = 0;
    virtual void render(SDL_Renderer *renderer, float width, float height, float xscroll = 0, float yscroll = 0) = 0;
};
