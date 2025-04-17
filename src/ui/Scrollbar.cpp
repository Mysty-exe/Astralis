#include "Scrollbar.h"

Scrollbar::Scrollbar()
{
}

Scrollbar::Scrollbar(float width, float height)
{
    this->width = width;
    this->height = height;
    currentScroll = 0;
    maximumScroll = 0;
}

float Scrollbar::getScroll()
{
    return currentScroll;
}

void Scrollbar::setDimensions(float width, float height, float scrollableContent)
{
    this->width = width;
    this->height = height;
    this->scrollableContent = scrollableContent;

    maximumScroll = scrollableContent - height;
    maximumScroll = (maximumScroll < 0) ? 0 : maximumScroll;
}

void Scrollbar::scroll(float scrollSpeed)
{
    currentScroll -= scrollSpeed;
    currentScroll = (currentScroll < 0) ? 0 : currentScroll;
    currentScroll = (currentScroll > maximumScroll) ? maximumScroll : currentScroll;
}

void Scrollbar::display(SDL_Renderer *renderer, Vector pos)
{
    float scrollableLength = height - ((float)height / scrollableContent * height);
    float move = currentScroll / (float)maximumScroll * scrollableLength;
    SDL_RenderDrawLine(renderer, pos.x, pos.y + move, pos.x, pos.y + (height / scrollableContent * height) + move);
}
