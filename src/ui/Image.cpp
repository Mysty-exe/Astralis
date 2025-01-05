#include <Image.h>

Image::Image()
{
    texture = NULL;
    x = 0;
    y = 0;
    width = 0;
    height = 0;
    rect = {x, y, (int)width, (int)height};
}

Image::~Image()
{
    // free();
}

void Image::loadFromFile(SDL_Renderer *renderer, string path, int multiplier)
{
    free();

    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    width = loadedSurface->w * multiplier;
    height = loadedSurface->h * multiplier;
    SDL_FreeSurface(loadedSurface);
    rect = {x, y, (int)width, (int)height};
}

void Image::loadFromRenderedText(SDL_Renderer *renderer, TTF_Font *font, string text, SDL_Color textColor, float multiplier)
{
    free();
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    width = textSurface->w * multiplier;
    height = textSurface->h * multiplier;
    SDL_FreeSurface(textSurface);
    rect = {x, y, (int)width, (int)height};
}

void Image::free()
{
    if (texture != NULL)
    {
        SDL_DestroyTexture(texture);
        texture = NULL;
        width = 0;
        height = 0;
        rect = {x, y, (int)width, (int)height};
    }
}

void Image::render(SDL_Renderer *renderer, int scroll)
{
    SDL_Rect renderQuad = {x, y - scroll, width, height};
    SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
}

void Image::setCoords(int x, int y)
{
    this->x = x;
    this->y = y;
    rect = {x, y, (int)width, (int)height};
}

float Image::getWidth()
{
    return width;
}

float Image::getHeight()
{
    return height;
}

SDL_Rect *Image::getRect()
{
    return &rect;
}
