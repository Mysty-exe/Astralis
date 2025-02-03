#include <Animation.h>

Animation::Animation()
{
}

Animation::Animation(SDL_Renderer *renderer, string folder, double multiplier, double frameSecs)
{
    this->renderer = renderer;
    this->folder = folder;
    this->multiplier = multiplier;
    this->frameSecs = frameSecs;
    currentFrame = 0;
    loadFrames();
}

void Animation::loadFrames()
{
    images.clear();
    int size = -1;
    for (auto &f : filesystem::directory_iterator(folder))
    {
        size++;
    }

    for (int x = 1; x <= size; x++)
    {
        Image image;
        image.loadFromFile(renderer, folder + "/" + to_string(x) + ".png", multiplier);
        images.push_back(image);
    }
}

void Animation::render(float x, float y)
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

float Animation::getWidth()
{
    return images[0].getWidth();
}

float Animation::getHeight()
{
    return images[0].getHeight();
}
