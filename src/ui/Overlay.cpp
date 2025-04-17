#include <Overlay.h>

Overlay::Overlay()
{
}

Overlay::Overlay(SDL_Renderer *renderer, float width, float height, float x, float y)
{
    this->renderer = renderer;
    smallFont = TTF_OpenFont("assets/Fonts/font.otf", 16);
    resizeDimensions(width, height, x, y);
    currentTab = 0;
    endYs = {0, 0};
}

void Overlay::resizeDimensions(float width, float height, float x, float y)
{
    this->width = width;
    this->height = height;
    this->x = x;
    this->y = y;
    overlay = {(int)x, (int)y, (int)width, (int)height};
}

void Overlay::setEndY(int y)
{
    endYs[currentTab] = y;
}

Vector Overlay::getPos()
{
    return Vector(x, y + (height / 16));
}

float Overlay::getWidth()
{
    return width;
}

float Overlay::getHeight()
{
    return height;
}

int Overlay::getScroll()
{
    return scrollbars[currentTab].getScroll();
}

int Overlay::getCurrentTab()
{
    return currentTab;
}

SDL_Rect *Overlay::getRect()
{
    return &overlay;
}

SDL_Rect *Overlay::getButtonRect()
{
    return &buttonRect;
}

void Overlay::addButton(string name)
{
    scrollbars.push_back(Scrollbar(width, height));
    buttons.push_back(name);
    endYs.push_back(0);
}

void Overlay::render(EventsManager eventManager)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_RenderFillRect(renderer, &overlay);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &overlay);
}

bool Overlay::renderWidgets(EventsManager eventManager)
{
    bool hovering = false;

    SDL_Point point = eventManager.getPoint();
    SDL_Point startClickPos = eventManager.getStartClickPos();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    buttonRect = {(int)(x + 2), 0, (int)width - 4, (int)((height / 16) + 5)};
    SDL_RenderFillRect(renderer, &buttonRect);

    for (int i = 0; i < buttons.size(); i++)
    {
        if (i == currentTab)
        {
            buttonTxt.loadFromRenderedText(renderer, smallFont, buttons[i], {203, 195, 227}, Vector(1, 1));
        }
        else
        {
            buttonTxt.loadFromRenderedText(renderer, smallFont, buttons[i], {255, 255, 255}, Vector(1, 1));
        }

        buttonTxt.setCoords(x + (i * (width / buttons.size()) + (width / buttons.size() / 2) - buttonTxt.getWidth() / 2), height / 32 - buttonTxt.getHeight() / 2);
        buttonRect = {(int)(x + i * (width / buttons.size())), 0, (int)(width / buttons.size()), (int)(height / 16)};
        buttonTxt.render(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        if (SDL_PointInRect(&point, &buttonRect))
        {
            hovering = true;
            SDL_SetRenderDrawColor(renderer, 203, 195, 227, 255);
            if (SDL_PointInRect(&startClickPos, &buttonRect) && eventManager.isLeftClick())
            {
                currentTab = i;
            }
        }

        SDL_RenderDrawRect(renderer, &buttonRect);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    scrollbars[currentTab].setDimensions(width, height - (height / 16 + 10), endYs[currentTab] - (height / 16 + 10) + 30);
    if (SDL_PointInRect(&point, &overlay))
    {
        scrollbars[currentTab].scroll(eventManager.getMouseWheel().y * 10);
    }
    scrollbars[currentTab].display(renderer, Vector(x + width - 10, height / 16 + 10));

    return hovering;
}

void Overlay::reset()
{
    currentTab = 0;
    endYs.clear();
    for (int i = 0; i < buttons.size(); i++)
    {
        endYs.push_back(0);
    }
}
