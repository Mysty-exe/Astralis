#include <Frame.h>

Frame::Frame(SDL_Renderer *renderer, string name, string file, int x, int y, int w, int h)
{
    this->renderer = renderer;
    this->name = name;
    this->file = file;
    rect = {x, y, w, h};
    object = Animation(renderer, file, 1, 0.1);
    object.multiplier = (1 / object.getWidth()) * (w - 50);
    object.loadFrames();
}

void Frame::display(float xscroll, float yscroll)
{
    SDL_Rect tempRect = {(int)(rect.x - xscroll), (int)(rect.y - yscroll), (int)rect.w, (int)rect.h};
    SDL_RenderDrawLine(renderer, tempRect.x + 5, tempRect.y + tempRect.h, tempRect.x + tempRect.w - 5, tempRect.y + tempRect.h);
    SDL_RenderDrawLine(renderer, tempRect.x + tempRect.w - 1, tempRect.y + 5, tempRect.x + tempRect.w - 1, tempRect.y + tempRect.h - 5);
    object.render(tempRect.x + tempRect.w / 2 - object.getWidth() / 2, tempRect.y + tempRect.h / 2 - object.getHeight() / 2);
}

string Frame::getName()
{
    return name;
}

string Frame::getFile()
{
    return file;
}

SDL_Rect Frame::getRect()
{
    return rect;
}
