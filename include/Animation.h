#pragma once
#include <iostream>
#include <SDL.h>
#include <Image.h>
#include <string>
#include <vector>
#include <Timer.h>
#include <filesystem>

using namespace std;

class Animation
{
private:
    string folder;

public:
    SDL_Renderer* renderer;
    vector<Image> images;
    int multiplier, currentFrame, frameSecs;
    Timer frameTimer;

    Animation();
    Animation(SDL_Renderer *renderer, string folder, int multiplier, int frameSecs);
    void loadFrames();
    void render(float x, float y);
};