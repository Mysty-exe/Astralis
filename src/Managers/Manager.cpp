#include <Manager.h>

Manager::Manager()
{
}

Manager::Manager(SDL_Renderer *renderer, int width, int height)
{
    this->renderer = renderer;
    this->eventManager = eventManager;
    this->width = width;
    this->height = height;

    bgMusic = Mix_LoadMUS("assets/Audio/spaceBgSound.mp3");
    clickSound = Mix_LoadWAV("assets/Audio/click.wav");
    pointerCursor.loadFromFile(renderer, "Assets/Cursor/pointer.png", getWindowRatio());
    handCursor.loadFromFile(renderer, "Assets/Cursor/hand.png", getWindowRatio());

    largeFont = TTF_OpenFont("assets/Fonts/font.otf", 30);
    bigFont = TTF_OpenFont("assets/Fonts/font.otf", 24);
    smallFont = TTF_OpenFont("assets/Fonts/font.otf", 14);

    Mix_VolumeChunk(clickSound, 5);
}

void Manager::setWindowSize(int width, int height)
{
    this->width = width;
    this->height = height;
}

Vector Manager::getWindowRatio()
{
    return Vector(width, height) / Vector(1280, 720);
}

void Manager::drawCursor(Vector pos, bool click)
{
    if (!click)
    {
        pointerCursor.setCoords(pos.x, pos.y);
        pointerCursor.render(renderer);
    }
    else
    {
        handCursor.setCoords(pos.x, pos.y);
        handCursor.render(renderer);
    }
}

void Manager::playBgMusic()
{
    if (Mix_PlayingMusic() == 0)
    {
        Mix_PlayMusic(bgMusic, -1);
    }
}

void Manager::playClickSound()
{
    Mix_PlayChannel(-1, clickSound, 0);
}

void Manager::setColor(Color color)
{
    if (color == WHITE)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }
    else if (color == BLACK)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }
    else if (color == PURPLE)
    {
        SDL_SetRenderDrawColor(renderer, 203, 195, 227, 255);
    }
}
