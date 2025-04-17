#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <EventsManager.h>
#include <Vector.h>
#include <vector>
#include <Texture.h>
#include <Scrollbar.h>

class Overlay
{
private:
    SDL_Renderer *renderer;
    TTF_Font *smallFont;
    float width, height, x, y;
    int currentTab;
    SDL_Rect overlay, buttonRect;
    vector<string> buttons;
    vector<int> endYs;
    vector<Scrollbar> scrollbars;
    Texture buttonTxt;

public:
    Overlay();
    Overlay(SDL_Renderer *renderer, float width, float height, float x, float y);
    void resizeDimensions(float width, float height, float x, float y);
    void setEndY(int y);
    Vector getPos();
    float getWidth();
    float getHeight();
    int getScroll();
    int getCurrentTab();
    SDL_Rect *getRect();
    SDL_Rect *getButtonRect();
    void addButton(string name);
    void render(EventsManager eventManager);
    bool renderWidgets(EventsManager eventManager);
    void reset();
};
