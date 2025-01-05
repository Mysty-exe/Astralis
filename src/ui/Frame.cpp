#include <Frame.h>

Frame::Frame(string name, int x, int y, int w, int h)
{
    this->name = name;
    rect = {x, y, w, h};
}

SDL_Rect *Frame::getRect(int scroll)
{
    SDL_Rect tempRect = {rect.x, rect.y - scroll, rect.w, rect.h};
    return &rect;
}

void Frame::display(SDL_Renderer *renderer, int scroll)
{
    SDL_Rect tempRect = {rect.x, rect.y - scroll, rect.w, rect.h};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &tempRect);
}
