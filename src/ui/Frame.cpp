#include <Frame.h>

Frame::Frame(string name, int x, int y, int w, int h)
{
    this->name = name;
    rect = {x, y, w, h};
}

void Frame::display(SDL_Renderer *renderer, float xscroll, float yscroll)
{
    SDL_Rect tempRect = {rect.x - xscroll, rect.y - yscroll, rect.w, rect.h};
    SDL_RenderDrawLine(renderer, tempRect.x + 5, tempRect.y + tempRect.h, tempRect.x + tempRect.w - 5, tempRect.y + tempRect.h);
    SDL_RenderDrawLine(renderer, tempRect.x + tempRect.w - 1, tempRect.y + 5, tempRect.x + tempRect.w - 1, tempRect.y + tempRect.h - 5);
}

string Frame::getName()
{
    return name;
}

SDL_Rect *Frame::getRect(float xscroll, float yscroll)
{
    rect.x -= xscroll;
    rect.y -= yscroll;
    return &rect;
}
