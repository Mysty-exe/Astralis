#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>
#include <string>
#include "Vector.h"

using namespace std;

class Texture
{
private:
    int x, y;
    SDL_Texture *texture;
    float width;
    float height;
    SDL_Rect rect;

public:
    Texture();
    void free();
    void createRenderableTexture(SDL_Renderer *renderer, int width, int height, SDL_TextureAccess access);
    void setAsRenderTarget(SDL_Renderer *renderer);
    void loadFromFile(SDL_Renderer *renderer, string path, Vector multiplier = Vector(1, 1));
    void loadFromRenderedText(SDL_Renderer *renderer, TTF_Font *font, string textureText, SDL_Color textColo, Vector multiplier = Vector(1, 1));
    void setCoords(int x, int y);
    void render(SDL_Renderer *renderer, Vector scroll = Vector(0, 0), float zoom = 1, float angle = 0);
    float getWidth();
    float getHeight();
    Vector getSize();
    float getMiddleX();
    float getMiddleY();
    float getX();
    float getY();
    float getEndX();
    float getEndY();
    void setWidth(float width);
    void setHeight(float height);
    SDL_Rect *getRect();
};