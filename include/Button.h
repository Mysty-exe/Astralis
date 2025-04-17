#pragma once
#include <Widget.h>

class Button : public Widget
{
private:
    bool hovering;

public:
    Button();
    Button(SDL_Renderer *renderer, string text);
    bool getPressed(EventsManager eventManager);
    void clearError();
    void setErrorText();
    float getX();
    float getY();
    float getEndY();
    void setPos(float x, float y);
    void setText(string text);
    void process(EventsManager eventManager);
    void render(SDL_Renderer *renderer, float width, float height, float xscroll = 0, float yscroll = 0);
};