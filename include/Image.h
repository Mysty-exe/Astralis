#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

using namespace std;

class Image
{
public:
    int x, y;
    Image();
    ~Image();
    void loadFromFile(SDL_Renderer *renderer, string path, float multiplier = 1);
    void loadFromRenderedText(SDL_Renderer *renderer, TTF_Font *font, string textureText, SDL_Color textColo, float multiplier = 1);
    void free();
    void render(SDL_Renderer *renderer, float xscroll = 0, float yscroll = 0, float angle = 0);
    void setCoords(int x, int y);
    float getWidth();
    float getHeight();
    SDL_Rect *getRect();

private:
    SDL_Texture *texture;
    float width;
    float height;
    SDL_Rect rect;
};