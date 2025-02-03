#pragma once
#include <vector>
#include <Vector.h>
#include <Timer.h>
#include <SDL.h>
#include <Utilities.h>

struct Particle
{
    Vector position;
    Vector velocity;
    float size, r, g, b;
};

class Particles
{
public:
    float r, g, b;
    vector<Particle> particles;
    Timer durationTimer;
    Timer particleTimer;
    Particles();
    Particles(string color);
    void addParticle(string type, double startX, double startY, Vector direction);
    void renderParticles(SDL_Renderer *renderer, string type, double startX, double startY, Vector panningOffset, Vector direction, double timeStep, bool started);
};
