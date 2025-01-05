#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
#include <string>
#include <Vector.h>

using namespace std;

class Utilities
{
public:
    static float g;
    static vector<pair<Vector, Vector>> getZoomScales(float width, float height);
    static vector<vector<string>> getStellarObjects();
    static vector<float> toSecs();
    static vector<float> getSubdividor();
    static vector<pair<string, float>> getTimeRates();
    static vector<pair<string, float>> getTimeMultipliers();
    static float getOrbitalVelocity(float orbitingMass, float distance);
    static double getGravityForce(float mass1, float mass2, float distance, float rate, double timeStep);
    static void drawCircle(SDL_Renderer *renderer, int displayX, int displayY, int radius);
    static float scaleDistance(float dist);
    static float scaleMass(float mass);
};