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
}

void Particles::addParticle(string type, double x, double y, Vector direction)
{
    float velX, velY;
    if (type == "Comet")
    {
        velX = direction.normalize().x * 100;
        velY = direction.normalize().y * 100;
    }
    else
    {
        velX = (direction.normalize().x + (-20 + (rand() % 40)) / 10) * 50;
        velY = (direction.normalize().y + (-20 + (rand() % 40)) / 10) * 50;
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

void Particles::renderParticles(SDL_Renderer *renderer, string type, double x, double y, Vector panningOffset, Vector direction, double timeStep, bool started)
{
    if (!particleTimer.isStarted())
    {
        particleTimer.start();
    }
    if (particleTimer.getTicks() / 1000 >= 0.01 && started)
    {
        addParticle(type, x, y, direction);
        particleTimer.start();
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
        SDL_Rect rect = {particles[i].position.x - particles[i].size / 2 + panningOffset.x, particles[i].position.y - particles[i].size / 2 + panningOffset.y, particles[i].size, particles[i].size};
        SDL_RenderFillRect(renderer, &rect);
    }

    for (int i : indexes)
    {
        particles.erase(particles.begin() + i);
    }
}
