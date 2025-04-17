#include <Texture.h>

Texture::Texture()
{
    texture = NULL;
    x = 0;
    y = 0;
    width = 0;
    height = 0;
    rect = {x, y, (int)width, (int)height};
}

void Texture::createRenderableTexture(SDL_Renderer *renderer, int width, int height, SDL_TextureAccess access)
{
    free();
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, access, width, height);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    if (texture != NULL)
    {
        this->width = width;
        this->height = height;
    }
}

void Texture::setAsRenderTarget(SDL_Renderer *renderer)
{
    SDL_SetRenderTarget(renderer, texture);
}

void Texture::loadFromFile(SDL_Renderer *renderer, string path, Vector multiplier)
{
    free();
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    width = loadedSurface->w * multiplier.x;
    height = loadedSurface->h * multiplier.y;
    SDL_FreeSurface(loadedSurface);
    rect = {x, y, (int)width, (int)height};
}

void Texture::loadFromRenderedText(SDL_Renderer *renderer, TTF_Font *font, string text, SDL_Color textColor, Vector multiplier)
{
    free();
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    width = textSurface->w * multiplier.x;
    height = textSurface->h * multiplier.y;
    SDL_FreeSurface(textSurface);
    rect = {x, y, (int)width, (int)height};
}

void Texture::free()
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

void Texture::render(SDL_Renderer *renderer, Vector scroll, float zoom, float angle)
{
    SDL_Point point = {0, (int)(height / 2)};
    SDL_Rect renderQuad = {(int)(x - scroll.x), (int)(y - scroll.y), (int)width * zoom, (int)height * zoom};
    SDL_RenderCopyEx(renderer, texture, NULL, &renderQuad, angle, &point, SDL_FLIP_NONE);
}

void Texture::setCoords(int x, int y)
{
    this->x = x;
    this->y = y;
    rect = {x, y, (int)width, (int)height};
}

float Texture::getWidth()
{
    return width;
}

float Texture::getHeight()
{
    return height;
}

Vector Texture::getSize()
{
    return Vector(width, height);
}

float Texture::getX()
{
    return x;
}

float Texture::getY()
{
    return y;
}

float Texture::getMiddleX()
{
    return x + width / 2;
}

float Texture::getMiddleY()
{
    return y + height / 2;
}

float Texture::getEndX()
{
    return x + width;
}

float Texture::getEndY()
{
    return y + height;
}

void Texture::setWidth(float width)
{
    this->width = width;
}

void Texture::setHeight(float height)
{
    this->height = height;
}

SDL_Rect *Texture::getRect()
{
    return &rect;
}
