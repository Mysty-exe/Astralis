#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <Image.h>
#include <vector>
#include <Frame.h>
#include <Utilities.h>

class Sidebar
{
private:
    SDL_Renderer *renderer;
    int width, height;
    int sidebarWidth, sidebarHeight;

    SDL_Rect rect;

    TTF_Font *font;
    SDL_Color color;

    vector<Image> headers;
    vector<Frame> frames;

public:
    string state;
    int scroll, maxScroll;
    Sidebar();
    Sidebar(SDL_Renderer *renderer, int width, int height);
    SDL_Rect *getRect();
    vector<Frame> getFrames();
    void setFrames();
    void drawSidebar();
};