#include <Particles.h>

Particles::Particles()
{
}

Particles::Particles(string color)
{
    particles = {};
    if (color == "Purple")
    {
        r = 60;
        g = 0;
        b = 100;
    }
    if (color == "Red")
    {
        r = 255;
        g = 0;
        b = 0;
    }
    if (color == "White")
    {
        r = 255;
        g = 255;
        b = 255;
    }
    if (color == "Blue")
    {
        r = 0;
        g = 0;
        b = 255;
    }
    if (color == "Grey")
    {
        r = 200;
        g = 200;
        b = 200;
    }
}

void Particles::addParticle(string type, double x, double y, Vector direction, float explosion)
{
    float velX, velY;
    if (type == "Comet" || type == "Asteroid")
    {
        velX = direction.normalize().x * 100;
        velY = direction.normalize().y * 100;
    }
    else
    {
        velX = (direction.normalize().x + (-20 + (rand() % 40)) / 10) * explosion;
        velY = (direction.normalize().y + (-20 + (rand() % 40)) / 10) * explosion;
    }

    Particle particle;
    particle.position = Vector(x, y);
    particle.velocity = Vector(velX, velY);
    particle.size = 10 + (rand() % 5);
    particle.r = r;
    particle.g = g;
    particle.b = b;
    particles.push_back(particle);
}

void Particles::renderParticles(SDL_Renderer *renderer, SDL_Rect *renderQuad, string type, double x, double y, Vector panningOffset, Vector direction, float explosion, double timeStep, bool started)
{
    if (!particleTimer.isStarted())
    {
        particleTimer.start();
    }
    if (particleTimer.getTicks() / 1000 >= 0.01 && started)
    {
        SDL_Point point = {(int)x, (int)y};
        if (SDL_PointInRect(&point, renderQuad))
        {
            addParticle(type, x, y, direction, explosion);
            particleTimer.start();
        }
    }
    vector<int> indexes = {};

    for (int i = 0; i < particles.size(); i++)
    {
        SDL_SetRenderDrawColor(renderer, particles[i].r, particles[i].g, particles[i].b, 255);

        particles[i].r += 100 * timeStep;
        particles[i].g += 100 * timeStep;
        particles[i].b += 100 * timeStep;
        particles[i].r = (particles[i].r > 255) ? 255 : particles[i].r;
        particles[i].g = (particles[i].g > 255) ? 255 : particles[i].g;
        particles[i].b = (particles[i].b > 255) ? 255 : particles[i].b;

        particles[i].position.x += particles[i].velocity.x * timeStep;
        particles[i].position.y += particles[i].velocity.y * timeStep;
        particles[i].size -= 10 * timeStep;
        if (particles[i].size <= 0)
        {
            indexes.push_back(i);
            continue;
        }

        SDL_Rect r = {(int)(particles[i].position.x - particles[i].size / 2), (int)(particles[i].position.y - particles[i].size / 2), (int)particles[i].size, (int)particles[i].size};
        SDL_Rect rect = {(int)(particles[i].position.x - particles[i].size / 2 + panningOffset.x), (int)(particles[i].position.y - particles[i].size / 2 + panningOffset.y), (int)particles[i].size, (int)particles[i].size};
        if (SDL_HasIntersection(renderQuad, &r))
        {
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    for (int i : indexes)
    {
        particles.erase(particles.begin() + i);
    }
}
