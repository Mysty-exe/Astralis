#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <cmath>
#include <vector>
#include <string>
#include <Vector.h>
#include <Texture.h>

using namespace std;

class Utilities
{
public:
    static Texture message;
    static long double g;
    static Vector getSizeRatio(Vector startSize, Vector endSize);
    static vector<pair<Vector, Vector>> getZoomScales(float width, float height);
    static vector<string> getStellarObjects();
    static vector<vector<string>> getStellarFiles();
    static vector<float> toSecs();
    static vector<float> getSubdividor();
    static vector<string> getUnits();
    static vector<pair<string, double>> getTimeRates();
    static vector<pair<string, float>> getTimeMultipliers();
    static float getOrbitalVelocity(float orbitingMass, float distance);
    static long double getGravityForce(long double mass1, long double mass2, long double distance, float rate);
    static void drawCircle(SDL_Renderer *renderer, int displayX, int displayY, int radius);
    static string removeTrailingZeroes(string number);
    static bool validateRadius(string radius);
    static bool validateMass(string mass);
    static bool validateVelocity(string vel);
    static bool validateDirection(string degrees);
    static string getExponentForm(string num);
    static pair<string, string> parseInput(string input);
    static void displayMessage(SDL_Renderer *renderer, float width, float height, string text, int messageLevel = 1);
};
