#include <Input.h>

Input::Input()
{
}

Input::Input(SDL_Renderer *renderer, string header, string text, string hint, string error, int startWidth, int maxChars) : Widget(renderer, text)
{
    this->headerTxt = header;
    this->hintTxt = hint;
    this->errorTxt = error;
    this->startWidth = startWidth;
    this->maxChars = maxChars;

    showCursor = true;

    if (header != "")
    {
        headerTxtImg.loadFromRenderedText(renderer, headerFont, header, {255, 255, 255, 255});
    }

    setText(text);

    if (hint != "")
    {
        hintTxtImg.loadFromRenderedText(renderer, hintFont, hint, {255, 255, 255, 255});
    }

    if (error != "")
    {
        errorTxtImg.loadFromRenderedText(renderer, hintFont, error, {255, 255, 255, 255});
    }

    rect.h = 25;
}

float Input::getX()
{
    return headerTxtImg.getX();
}

float Input::getEndX()
{
    return headerTxtImg.getX() + rect.w;
}

float Input::getY()
{
    return headerTxtImg.getY();
}

float Input::getEndY()
{
    if (!slider.isReal())
    {
        return hintTxtImg.getEndY();
    }

    return hintTxtImg.getEndY() + 40;
}

float Input::getWidth()
{
    return rect.w;
}

float Input::getHeight()
{
    return rect.h;
}

bool Input::getSliderActivated()
{
    return slider.isActivated();
}

void Input::setPos(float x, float y)
{
    headerTxtImg.setCoords(x, y);
    rect.x = headerTxtImg.getX();
    rect.y = headerTxtImg.getEndY() + 10;
}

string Input::getHeader()
{
    return headerTxt;
}

void Input::setText(string text)
{
    this->text = text;
    if (this->text != "")
    {
        rectTxt.loadFromRenderedText(renderer, regFont, text, {203, 195, 227});
        int extraWidth = (rectTxt.getWidth() - startWidth > 0) ? rectTxt.getWidth() - startWidth : 0;
        rect.w = startWidth + extraWidth + 20;
    }
    else
    {
        rect.w = startWidth + 20;
        rectTxt.free();
    }
}

void Input::clearError()
{
    error = false;
}

void Input::setErrorText(string error)
{
    this->error = true;
    errorTxt = error;
    if (errorTxt != "")
    {
        errorTxtImg.loadFromRenderedText(renderer, hintFont, errorTxt, {255, 0, 0});
    }
}

SDL_Rect *Input::getRect()
{
    return &rect;
}

Slider Input::getSlider()
{
    return slider;
}

void Input::setSlider(Slider slider)
{
    this->slider = slider;
}

int Input::getSliderValue()
{
    return slider.getValue();
}

void Input::setSliderValue(float value)
{
    if (this->slider.isReal())
    {
        this->slider.setValue(value);
    }
}

void Input::process(EventsManager eventManager)
{
    SDL_Point point = eventManager.getPoint();
    SDL_Point startClickPos = eventManager.getStartClickPos();

    if (SDL_PointInRect(&point, &rect) && SDL_PointInRect(&startClickPos, &rect) && eventManager.isLeftClick())
    {
        isActivated = true;
    }
    else if (eventManager.isLeftClick())
    {
        isActivated = false;
    }

    if (isActivated)
    {
        if (eventManager.checkSpecialKey(SPACE))
        {
            if (text.length() < maxChars && maxChars != -1)
            {
                text += " ";
                setText(text);
            }
        }
        else if (eventManager.checkSpecialKey(BACKSPACE))
        {
            if (text != "")
            {
                text.erase(text.length() - 1, 1);
                setText(text);
            }
        }
        else
        {
            if (eventManager.getInput() != "")
            {
                if (text.length() < maxChars && maxChars != -1)
                {
                    text += eventManager.getInput();
                    setText(text);
                }
            }
        }
    }

    if (slider.isReal())
    {
        slider.process(eventManager);
    }
}

void Input::render(SDL_Renderer *renderer, float width, float height, float xscroll, float yscroll)
{
    hintTxtImg.setCoords(rect.x + 5, rect.y + rect.h + 3);
    rectTxt.setCoords(rect.x + 5, rect.y + (rect.h / 2) - rectTxt.getHeight() / 2);

    if (text != "")
    {
        rectTxt.render(renderer);
    }

    if (error)
    {
        errorTxtImg.setCoords(rectTxt.getX(), hintTxtImg.getEndY() + 2);
        errorTxtImg.render(renderer);
    }

    headerTxtImg.render(renderer);
    hintTxtImg.render(renderer);

    if (isActivated)
    {
        if (!cursorTimer.isStarted())
        {
            cursorTimer.start();
        }

        if (cursorTimer.getTicks() / 1000 >= 0.5)
        {
            showCursor = !showCursor;
            cursorTimer.start();
        }

        SDL_SetRenderDrawColor(renderer, 203, 195, 227, 255);

        if (showCursor)
        {
            SDL_RenderDrawLine(renderer, rectTxt.getEndX() + 2, rect.y + 5, rectTxt.getEndX() + 2, rect.y + rect.h - 5);
        }
    }
    else
    {
        showCursor = true;
        cursorTimer.reset();
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }

    SDL_RenderDrawRect(renderer, &rect);

    if (slider.isReal())
    {
        slider.updateSize(width / 4, height);
        slider.setPos(rect.x, rect.y + rect.h + 60);
        slider.display(renderer);
    }
}
