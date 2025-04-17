#pragma once
#include "SDL.h"
#include "Vector.h"

class Scrollbar
{
private:
    float width, height;
    float currentScroll, maximumScroll, scrollableContent, scrollbarLength;

public:
    Scrollbar();
    Scrollbar(float width, float height);
    float getScroll();
    void setDimensions(float width, float height, float scrollableContent);
    void scroll(float scrollSpeed);
    void display(SDL_Renderer *renderer, Vector pos);
};