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
    const static vector<float> &toSecs();
    const static vector<int> &getSubdividor();
    static vector<string> getUnits();
    const static vector<pair<string, double>> &getTimeRates();
    static float getOrbitalVelocity(float orbitingMass, float distance);
    static long double getGravityForce(long double mass1, long double mass2, long double distance);
    static void drawCircle(SDL_Renderer *renderer, int displayX, int displayY, int radius);
    static string removeTrailingZeroes(string number);
    static bool validateNumber(const std::string &radius);
    static bool validateDirection(const std::string &degrees);
    static pair<string, string> parseInput(string input);
    static void displayMessage(SDL_Renderer *renderer, float width, float height, string text, int messageLevel = 1);
};
