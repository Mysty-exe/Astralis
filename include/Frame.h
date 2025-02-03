#pragma once
#include <iostream>
#include <string>
#include <SDL.h>
#include <Image.h>
#include <Animation.h>

using namespace std;

class Frame
{
private:
    SDL_Renderer *renderer;
    string name, file;
    SDL_Rect rect;

public:
    Animation object;
    Frame(SDL_Renderer *renderer, string name, string file, int x, int y, int w, int h);
    void display(float xscroll, float yscroll);
    string getName();
    string getFile();
    SDL_Rect getRect();
};