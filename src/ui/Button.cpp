#include <Button.h>

Button::Button()
{
}

Button::Button(SDL_Renderer *renderer, string text) : Widget(renderer, text)
{
    this->renderer = renderer;
    this->text = text;
    rect = {0, 0, 100, 30};
    rectTxt.loadFromRenderedText(renderer, regFont, text, {255, 255, 255});
    if (rect.w < (rectTxt.getWidth() + 20))
    {
        rect.w = rectTxt.getWidth() + 20;
    }
}

bool Button::getPressed(EventsManager eventManager)
{
    SDL_Point point = eventManager.getPoint();
    SDL_Point startClickPos = eventManager.getStartClickPos();

    if (SDL_PointInRect(&point, &rect) && SDL_PointInRect(&startClickPos, &rect) && eventManager.isLeftClick())
    {
        return true;
    }
    return false;
}

void Button::clearError()
{
}

void Button::setErrorText()
{
}

float Button::getX()
{
    return rect.x;
}

float Button::getY()
{
    return rect.y;
}

float Button::getEndY()
{
    return rect.y + rect.h;
}

void Button::setPos(float x, float y)
{
    rect.x = x;
    rect.y = y;
    rectTxt.setCoords(rect.x + 10, rect.y + (rect.h / 2) - rectTxt.getHeight() / 2);
    if (rect.w < (rectTxt.getWidth() + 20))
    {
        rect.w = rectTxt.getWidth() + 20;
    }
}

void Button::setText(string text)
{
    this->text = text;
    if (!text.empty())
    {
        rectTxt.loadFromRenderedText(renderer, regFont, text, {255, 255, 255});
    }
}

void Button::process(EventsManager eventManager)
{
    SDL_Point point = eventManager.getPoint();

    hovering = false;
    if (SDL_PointInRect(&point, &rect))
    {
        hovering = true;
    }
}

void Button::render(SDL_Renderer *renderer, float width, float height, float xscroll, float yscroll)
{
    SDL_SetRenderDrawColor(renderer, 203, 195, 227, 255);
    if (hovering)
    {
        SDL_RenderFillRect(renderer, &rect);
    }
    else
    {
        SDL_RenderDrawRect(renderer, &rect);
    }

    rectTxt.render(renderer);
}
