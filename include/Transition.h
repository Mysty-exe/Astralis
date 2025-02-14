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
    double holdSpeed, pixelatedSpeed;
    SDL_Renderer *renderer;
    Timer pixelateTimer, holdTimer;
    vector<Vector> locations;
    vector<Vector> drawLocations;

public:
    string transitionState;
    Transition();
    Transition(SDL_Renderer *renderer, int width, int height, double holdSpeed);
    void getPossibleLocations();
    string runTransition();
};