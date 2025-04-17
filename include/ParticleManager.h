#pragma once
#include <vector>
#include <Vector.h>
#include <Timer.h>
#include <algorithm>
#include <SDL.h>
#include <Utilities.h>

struct Particle
{
    Vector position;
    Vector velocity;
    float size, r, g, b;
    bool dead;
    Particle(Vector position, Vector vel, int size);
    void applyForces(double timeStep);
    void display(SDL_Renderer *renderer, SDL_Rect renderQuad, Vector panningOffset);
};

class ParticleManager
{
private:
    float explosion;
    vector<Particle> particles;
    Timer durationTimer, particleTimer;

public:
    ParticleManager();
    void setExplosion(float explosion);
    void addParticle(double startX, double startY, Vector direction);
    void renderParticles(SDL_Renderer *renderer, SDL_Rect *renderQuad, double startX, double startY, Vector panningOffset, Vector direction, double timeStep, bool started);
};
