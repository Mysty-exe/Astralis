#include <Button.h>

Button::Button()
{
}

Button::Button(SDL_Renderer *renderer, string text, SDL_Color color, int width)
{
    this->renderer = renderer;
    this->text = text;
    this->color = color;

    font = TTF_OpenFont("assets/Fonts/font.otf", 18);

    buttonTxt.loadFromRenderedText(renderer, font, text, color);
    rect.w = width;
    rect.h = buttonTxt.getHeight() + 10;
}

void Button::setCoords(float x, float y)
{
    rect.x = x;
    rect.y = y;
}

SDL_Rect *Button::getRect()
{
    return &rect;
}

void Button::render()
{
    SDL_RenderDrawRect(renderer, &rect);
    buttonTxt.setCoords(rect.x + (rect.w / 2) - buttonTxt.getRect()->w / 2, rect.y + 5);
    buttonTxt.render(renderer);
}
