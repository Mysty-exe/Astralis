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
    static int numObjects;
    static vector<pair<Vector, Vector>> getZoomScales(float width, float height);
    static vector<vector<string>> getStellarObjects();
    static vector<float> toSecs();
    static vector<float> getSubdividor();
    static vector<pair<string, float>> getTimeRates();
    static vector<pair<string, float>> getTimeMultipliers();
    static float getOrbitalVelocity(float orbitingMass, float distance);
    static double getGravityForce(float mass1, float mass2, float distance, float rate, double timeStep);
    static void drawCircle(SDL_Renderer *renderer, int displayX, int displayY, int radius);
    static long double scaleDistance(long double dist);
    static long double scaleMass(long double mass);
    static long double getRealDistance(long double dist);
    static long double getRealMass(long double mass);
    static string removeTrailingZeroes(string number);
    static bool validateRadius(string radius);
    static bool validateMass(string mass);
    static string getExponentForm(string num);
};