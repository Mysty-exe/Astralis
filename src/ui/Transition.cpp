#include <Transition.h>

Transition::Transition()
{
}

Transition::Transition(SDL_Renderer *renderer, int width, int height, int holdSpeed, int pixelatedSpeed)
{
    this->renderer = renderer;
    this->width = width;
    this->height = height;
    this->holdSpeed = holdSpeed;
    this->pixelatedSpeed = pixelatedSpeed;
    getPossibleLocations();
}

void Transition::getPossibleLocations()
{
    for (int y = 1; y <= height; y += 64)
    {
        for (int x = 1; x <= width; x += 64)
        {
            locations.push_back(Vector(x, y));
        }
    }
}

string Transition::runTransition()
{
    if (transitionState == "In")
    {
        for (int i = 0; i < pixelatedSpeed; i++)
        {
            if (locations.size() <= 0 && !holdTimer.isStarted())
            {
                transitionState = "Hold";
                holdTimer.start();
                break;
            }
            transitionState = "In";
            int random = rand() % locations.size();
            drawLocations.push_back(locations[random]);
            locations.erase(locations.begin() + random);
        }
    }
    else if (transitionState == "Out")
    {
        for (int i = 0; i < pixelatedSpeed; i++)
        {
            if (drawLocations.size() <= 0)
            {
                transitionState = "Done";
                break;
            }
            int random = rand() % drawLocations.size();
            drawLocations.erase(drawLocations.begin() + random);
        }
    }

    if (holdTimer.getTicks() >= holdSpeed * 1000)
    {
        transitionState = "Out";
        holdTimer.stop();
    }

    for (Vector v : drawLocations)
    {
        SDL_Rect rect = {v.x, v.y, 64, 64};
        SDL_SetRenderDrawColor(renderer, 46, 21, 68, 255);
        SDL_RenderFillRect(renderer, &rect);
    }

    return transitionState;
}