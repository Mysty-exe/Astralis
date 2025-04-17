#include "Widget.h"

Widget::Widget()
{
}

Widget::Widget(SDL_Renderer *renderer, string text)
{
    this->renderer = renderer;
    this->text = text;
    isActivated = false;

    headerFont = TTF_OpenFont("assets/Fonts/font.otf", 24);
    regFont = TTF_OpenFont("assets/Fonts/font.otf", 16);
    hintFont = TTF_OpenFont("assets/Fonts/font.otf", 12);
}

bool Widget::getError()
{
    return error;
}

string Widget::getText()
{
    return text;
}

float Widget::getX()
{
    return rect.x;
}

float Widget::getY()
{
    return rect.y;
}

void Widget::deactivate()
{
    isActivated = false;
}

void Widget::activate()
{
    isActivated = true;
}

bool Widget::getActivated()
{
    return isActivated;
}

void Widget::clearError()
{
}

void Widget::setErrorText(string error)
{
}

float Widget::getEndY()
{
    return rect.y + rect.h;
}

void Widget::setPos(float x, float y)
{
    rect.x = x;
    rect.y = y;
}

SDL_Rect *Widget::getRect()
{
    return &rect;
}