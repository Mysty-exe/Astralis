#include <Frame.h>

Frame::Frame(SDL_Renderer *renderer, string file, float size)
{
    this->renderer = renderer;
    this->file = file;
    this->size = size;
    object.loadAnimation(renderer, file, Vector(1, 1), 0.1);
    object.loadFrames(renderer, Vector((1 / object.getWidth()) * (size - 20), (1 / object.getHeight()) * (size - 20)));
}

void Frame::display(float x, float y, float xscroll, float yscroll)
{
    rect = {(int)(x - xscroll), (int)(y - yscroll), (int)size, (int)size};
    SDL_RenderDrawLine(renderer, rect.x + 5, rect.y + rect.h, rect.x + rect.w - 5, rect.y + rect.h);
    SDL_RenderDrawLine(renderer, rect.x + rect.w - 1, rect.y + 5, rect.x + rect.w - 1, rect.y + rect.h - 5);
    object.render(renderer, rect.x + rect.w / 2 - object.getWidth() / 2, rect.y + rect.h / 2 - object.getHeight() / 2);
}

string Frame::getFile()
{
    return file;
}

SDL_Rect *Frame::getRect()
{
    return &rect;
}

float Frame::getSize()
{
    return size;
}
