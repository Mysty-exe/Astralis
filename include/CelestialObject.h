#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include <Utilities.h>
#include <Vector.h>
#include <Image.h>

using namespace std;

enum CelestialTypes
{
    STAR,
    PLANET,
    MOON,
    ASTEROID
};

class CelestialObject
{
public:
    string name;
    string objType;
    long double mass;
    long double radius;
    Vector position;
    Vector velocity;
    Vector acceleration;
    SDL_Rect rect;
    bool editing;

    Image arrow;

    CelestialObject();
    CelestialObject(SDL_Renderer *renderer, string name, string objType);
    CelestialObject(SDL_Renderer *renderer, string n, string objType, long double mass, long double radius, float x, float y, Vector vel);
    static void applyForces(vector<CelestialObject> &objects, double timeStep, int timeRate);
    static void applyVelocities(vector<CelestialObject> &objects, double timeStep);
    static void display(SDL_Renderer *renderer, vector<CelestialObject> &objects, Vector panningOffset, Vector zoomOffset);
    SDL_Rect *getRect();
};
