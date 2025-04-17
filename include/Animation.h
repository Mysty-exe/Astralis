#pragma once
#include <iostream>
#include <SDL.h>
#include <Texture.h>
#include <Vector.h>
#include <vector>
#include <Timer.h>
#include <filesystem>

class Animation
{
private:
    string folder;
    std::vector<Texture> images;
    double currentFrame, frameSecs;
    Timer frameTimer;

public:
    Animation();
    void loadAnimation(SDL_Renderer *renderer, string folder, Vector multiplier, double frameSecs);
    void loadFrames(SDL_Renderer *renderer, Vector multiplier);
    void render(SDL_Renderer *renderer, float x, float y);
    void freeAll();
    Vector getPos();
    float getWidth();
    float getHeight();
    Vector getSize();
    int getCurrentFrame();
};
