#pragma once
#include <iostream>
#include <string>
#include <SDL.h>
#include <Texture.h>
#include <Animation.h>

using namespace std;

class Frame
{
private:
    SDL_Renderer *renderer;
    string file;
    SDL_Rect rect;
    float size;

public:
    Animation object;
    Frame(SDL_Renderer *renderer, string file, float size);
    void display(float x, float y, float xscroll = 0, float yscroll = 0);
    string getFile();
    SDL_Rect *getRect();
    float getSize();
};