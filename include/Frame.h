#pragma once
#include <iostream>
#include <string>
#include <SDL.h>
#include <Image.h>

using namespace std;

class Frame
{
private:
    string name;
    SDL_Rect rect;

public:
    Frame(string name, int x, int y, int w, int h);
    SDL_Rect *getRect(int scroll);
    void display(SDL_Renderer *renderer, int scroll);
};