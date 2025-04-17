#include <Animation.h>

Animation::Animation()
{
}

void Animation::loadAnimation(SDL_Renderer *renderer, string folder, Vector multiplier, double frameSecs)
{
    this->frameSecs = frameSecs;
    this->folder = folder;
    currentFrame = 0;
    images = {};
    loadFrames(renderer, multiplier);
}

void Animation::loadFrames(SDL_Renderer *renderer, Vector multiplier)
{
    images.clear();
    int size = -1;
    for (auto &f : filesystem::directory_iterator(folder))
    {
        size++;
    }

    for (int x = 1; x <= size; x++)
    {
        Texture image;
        image.loadFromFile(renderer, folder + "/" + to_string(x) + ".png", multiplier);
        images.push_back(image);
    }
}

void Animation::freeAll()
{
    for (Texture image : images)
    {
        image.free();
    }
    images = {};
}

void Animation::render(SDL_Renderer *renderer, float x, float y)
{
    if (!frameTimer.isStarted())
    {
        frameTimer.start();
    }

    images[currentFrame].setCoords(x, y);
    images[currentFrame].render(renderer);

    double secs = frameTimer.getTicks() / 1000;
    if (secs >= frameSecs)
    {
        currentFrame += 1;
        if (currentFrame >= images.size())
        {
            currentFrame = 0;
        }
        frameTimer.stop();
    }
}

Vector Animation::getPos()
{
    return Vector(images[currentFrame].getX(), images[currentFrame].getY());
}

float Animation::getWidth()
{
    return images[0].getWidth();
}

float Animation::getHeight()
{
    return images[0].getHeight();
}

Vector Animation::getSize()
{
    return images[0].getSize();
}

int Animation::getCurrentFrame()
{
    return currentFrame;
}
