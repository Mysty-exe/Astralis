#include <CheckBox.h>

CheckBox::CheckBox()
{
}

CheckBox::CheckBox(SDL_Renderer *renderer, bool toggle, int s)
{
    this->renderer = renderer;
    this->toggle = toggle;
    size = s;
}

void CheckBox::setCoords(float x, float y)
{
    rect = {x, y, 30, 30};
}

void CheckBox::setToggle(bool toggle)
{
    this->toggle = toggle;
}

SDL_Rect *CheckBox::getRect()
{
    return &rect;
}
