#include <TextInput.h>

TextInput::TextInput()
{
}

TextInput::TextInput(SDL_Renderer *renderer, string text, SDL_Color color)
{
    this->renderer = renderer;
    this->text = text;
    this->color = color;
    error = false;
    activated = false;
    showCursor = true;

    font = TTF_OpenFont("assets/Fonts/quicksand.otf", 18);
    smallFont = TTF_OpenFont("assets/Fonts/quicksand.otf", 12);

    if (text != "")
    {
        buttonTxt.loadFromRenderedText(renderer, font, text, color);
        int extraWidth = (buttonTxt.getWidth() - 75 > 0) ? buttonTxt.getWidth() - 75 : 0;
        rect.w = 100 + extraWidth;
    }
    else
    {
        rect.w = 100;
    }
    rect.h = 25;
}

void TextInput::setCoords(float x, float y)
{
    rect.x = x;
    rect.y = y;
}

string TextInput::getText()
{
    return text;
}

void TextInput::setText(string text, int maxWidth)
{
    this->text = text;
    if (this->text != "")
    {
        buttonTxt.loadFromRenderedText(renderer, font, text, color);
        int extraWidth = (buttonTxt.getWidth() - 75 > 0) ? buttonTxt.getWidth() - 75 : 0;
        rect.w = 100 + extraWidth;
        rect.w = (rect.w < maxWidth - 50) ? rect.w : maxWidth - 50;
    }
}

void TextInput::setErrorText(string text)
{
    this->errorText = text;
    if (this->errorText != "")
    {
        errorTxt.loadFromRenderedText(renderer, smallFont, this->errorText, {255, 0, 0});
    }
}

SDL_Rect *TextInput::getRect()
{
    return &rect;
}

void TextInput::render(Events events, int maxWidth, float xscroll, float yscroll)
{
    SDL_Point point = events.getPoint();

    if (events.leftClick && SDL_PointInRect(&point, &rect) && SDL_PointInRect(&events.startClickPos, &rect))
    {
        activated = not activated;
        if (activated)
        {
            cursorTimer.start();
        }
    }
    else if (events.leftClick)
    {
        activated = false;
    }

    if (activated)
    {
        if (events.checkSpecialKey(BACKSPACE))
        {
            if (text.length() != 0)
            {
                text = text.erase(text.length() - 1, 1);
                if (text.length() != 0)
                {
                    buttonTxt.loadFromRenderedText(renderer, font, text, color);
                }
            }
        }
        else if (events.checkSpecialKey(SPACE))
        {
            text += " ";
            buttonTxt.loadFromRenderedText(renderer, font, text, color);
        }
        else if (events.input != "")
        {
            text += events.input;
            buttonTxt.loadFromRenderedText(renderer, font, text, color);
        }

        if (rect.w < maxWidth - 50)
        {
            int extraWidth = (buttonTxt.getWidth() - 75 > 0) ? buttonTxt.getWidth() - 75 : 0;
            rect.w = 100 + extraWidth;
        }

        SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);
        if (cursorTimer.getTicks() >= 500)
        {
            showCursor = not showCursor;
            cursorTimer.start();
        }

        if (showCursor)
        {
            int w = (text.length() > 0) ? buttonTxt.getWidth() : 0;
            SDL_RenderDrawLine(renderer, rect.x + 6 + w, rect.y + 2, rect.x + 6 + w, rect.y + rect.h - 3);
        }
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }

    if (error & !activated)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }

    SDL_Rect renderQuad = {rect.x - xscroll, rect.y - yscroll, rect.w, rect.h};
    SDL_RenderDrawRect(renderer, &renderQuad);
    SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);
    if (text.length() != 0)
    {
        buttonTxt.setCoords(renderQuad.x + 5, renderQuad.y + 2);
        buttonTxt.render(renderer);
    }
    if (error && errorText.length() != 0)
    {
        errorTxt.setCoords(renderQuad.x, renderQuad.y + renderQuad.h + 3);
        errorTxt.render(renderer);
    }
}
