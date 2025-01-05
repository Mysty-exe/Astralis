#pragma once
#include <iostream>
#include <SDL.h>
#include <string>
#include <Animation.h>
#include <Timer.h>
#include <Image.h>
#include <vector>
#include <Vector.h>
#include <Timer.h>

using namespace std;

class Transition
{
private:
    int width, height;
    int holdSpeed, pixelatedSpeed;
    SDL_Renderer *renderer;
    Timer holdTimer;
    vector<Vector> locations;
    vector<Vector> drawLocations;

public:
    string transitionState;
    Transition();
    Transition(SDL_Renderer *renderer, int width, int height, int holdSpeed, int pixelatedSpeed);
    void getPossibleLocations();
    string runTransition();
};