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
    void display(SDL_Renderer *renderer, float xscroll, float yscroll);
    string getName();
    SDL_Rect *getRect(float xscroll, float yscroll);
};