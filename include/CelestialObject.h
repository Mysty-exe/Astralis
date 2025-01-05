#pragma once
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <Utilities.h>
#include <Vector.h>
#include <vector>
#include <cmath>

using namespace std;

enum CelestialTypes
{
    STAR,
    PLANET,
    MOON
};

class CelestialObject
{
public:
    string name;
    CelestialTypes objectType;
    float mass;
    float radius;
    float velScalar;
    Vector position;
    Vector velocity;
    Vector acceleration;

    CelestialObject();
    CelestialObject(string n, CelestialTypes objType, float mass, float radius, float x, float y, Vector vel);
    static void applyForces(vector<CelestialObject> &objects, double timeStep, int timeRate);
    static void applyVelocities(vector<CelestialObject> &objects, double timeStep);
    static void display(SDL_Renderer *renderer, vector<CelestialObject> objects, Vector panningOffset, Vector zoomOffset);
};
