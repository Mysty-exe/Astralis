#include <Overlay.h>

Overlay::Overlay()
{
}

Overlay::Overlay(SDL_Renderer *renderer, int width, int height)
{
    this->renderer = renderer;
    this->width = width;
    this->height = height;
    smallFont = TTF_OpenFont("assets/fonts/font.otf", 28);

    irlSecs = 0;
    simSecs = 0;

    textColor = {255, 255, 255};
    pausedTxt.loadFromRenderedText(renderer, smallFont, "Paused", textColor);
    pausedTxt.setCoords(10, 0);

    dateTxt.loadFromRenderedText(renderer, smallFont, "Date - Unknown", textColor);
    dateTxt.setCoords(10, height - dateTxt.getHeight() - 10);

    rateTxt.loadFromRenderedText(renderer, smallFont, "1 sec/sec", textColor);
    rateTxt.setCoords(10, dateTxt.y - rateTxt.getHeight() - 5);
}

void Overlay::displayTimeRate(int rate, int zoom)
{
    string text = "1 " + Utilities::getTimeRates()[rate].first + "/sec";
    rateTxt.setCoords(10 * (0.4 + 0.2 * zoom), dateTxt.y - rateTxt.getHeight() - 5 * (0.4 + 0.2 * zoom));
    rateTxt.loadFromRenderedText(renderer, smallFont, text, textColor, 0.4 + 0.2 * zoom);
    rateTxt.render(renderer);
}

void Overlay::displaySimulationDate(int rate, int totalTime, int zoom)
{
    simSecs += ((totalTime - irlSecs) * Utilities::toSecs()[rate]);
    irlSecs += (totalTime - irlSecs);

    double tempSecs = simSecs;
    string year = to_string((int)floor(tempSecs / 31556952000));
    tempSecs = fmod(tempSecs, 31556952000);
    string days = to_string((int)floor(tempSecs / 86400000));
    tempSecs = fmod(tempSecs, 86400000);
    string hours = to_string((int)floor(tempSecs / 3600000));
    tempSecs = fmod(tempSecs, 3600000);
    string mins = to_string((int)floor(tempSecs / 60000));
    tempSecs = fmod(tempSecs, 60000);
    string secs = to_string((int)floor(tempSecs / 1000));
    tempSecs = fmod(tempSecs, 1000);

    hours = (hours.length() == 1) ? "0" + hours : hours;
    mins = (mins.length() == 1) ? "0" + mins : mins;
    secs = (secs.length() == 1) ? "0" + secs : secs;

    string text = "Year " + year + ", Day " + days + ", " + hours + ":" + mins + ":" + secs;
    dateTxt.loadFromRenderedText(renderer, smallFont, text, textColor, 0.4 + 0.2 * zoom);
    dateTxt.setCoords(10 * (0.4 + 0.2 * zoom), height * (0.4 + 0.2 * zoom) - dateTxt.getHeight() - 10 * (0.4 + 0.2 * zoom));
    dateTxt.render(renderer);
}

void Overlay::displaySimulationStatus(bool paused, int zoom)
{
    if (paused)
    {
        pausedTxt.setCoords(10 * (0.4 + 0.2 * zoom), 0);
        pausedTxt.loadFromRenderedText(renderer, smallFont, "Paused", textColor, 0.4 + 0.2 * zoom);
        pausedTxt.render(renderer);
    }
}
