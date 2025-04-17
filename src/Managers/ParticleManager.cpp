#include <ParticleManager.h>

Particle::Particle(Vector position, Vector vel, int size)
{
    this->position = position;
    this->velocity = vel;
    this->size = size;
    dead = false;
    r = 255;
    g = 0;
    b = 0;
}

void Particle::applyForces(double timeStep)
{
    r += 100 * timeStep;
    g += 100 * timeStep;
    b += 100 * timeStep;
    r = (r > 255) ? 255 : r;
    g = (g > 255) ? 255 : g;
    b = (b > 255) ? 255 : b;

    position.x += velocity.x * timeStep;
    position.y += velocity.y * timeStep;
    position.y += 0.1;
    size -= 10 * timeStep;
    if (size <= 0)
    {
        dead = true;
    }
}

void Particle::display(SDL_Renderer *renderer, SDL_Rect renderQuad, Vector panningOffset)
{
    SDL_Rect r = {(int)(position.x - size / 2), (int)(position.y - size / 2), (int)size, (int)size};
    SDL_Rect rect = {(int)(position.x - size / 2 + panningOffset.x), (int)(position.y - size / 2 + panningOffset.y), (int)size, (int)size};
    if (SDL_HasIntersection(&renderQuad, &r))
    {
        SDL_RenderFillRect(renderer, &rect);
    }
}

ParticleManager::ParticleManager()
{
}

void ParticleManager::setExplosion(float explosion)
{
    this->explosion = explosion;
}

void ParticleManager::addParticle(double x, double y, Vector direction)
{
    float velX, velY;

    int angle = direction.getAngle() - 10 + (rand() % 20);
    double radians = angle * PI / 180;
    direction = Vector(cos(radians), sin(radians)).normalize();

    velX = (direction.x + (-20 + (rand() % 40)) / 10) * explosion;
    velY = (direction.y + (-20 + (rand() % 40)) / 10) * explosion;

    Particle particle = Particle(Vector(x, y), Vector(velX, velY), 10 + (rand() % 5));
    particles.push_back(particle);
}

void ParticleManager::renderParticles(SDL_Renderer *renderer, SDL_Rect *renderQuad, double x, double y, Vector panningOffset, Vector direction, double timeStep, bool started)
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
            addParticle(x, y, direction);
            particleTimer.start();
        }
    }
    for (int i = 0; i < particles.size(); i++)
    {
        SDL_SetRenderDrawColor(renderer, particles[i].r, particles[i].g, particles[i].b, 255);

        particles[i].applyForces(timeStep);
        particles[i].display(renderer, *renderQuad, panningOffset);
    }

    particles.erase(
        remove_if(particles.begin(), particles.end(),
                  [](Particle p)
                  { return p.dead; }),
        particles.end());
}
