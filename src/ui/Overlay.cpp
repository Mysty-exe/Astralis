#include <Overlay.h>

Overlay::Overlay()
{
}

Overlay::Overlay(SDL_Renderer *renderer, int width, int height)
{
    this->renderer = renderer;
    this->width = width;
    this->height = height;
    font = TTF_OpenFont("assets/Fonts/font.otf", 24);
    smallFont = TTF_OpenFont("assets/Fonts/font.otf", 16);

    irlSecs = 0;
    simSecs = 0;

    textColor = {255, 255, 255};
    focusedTxt.loadFromRenderedText(renderer, smallFont, "Focused", textColor);
    modeTxt.loadFromRenderedText(renderer, smallFont, "Paused", textColor);
    dateTxt.loadFromRenderedText(renderer, smallFont, "Date - Unknown", textColor);
    rateTxt.loadFromRenderedText(renderer, smallFont, "1 sec/sec", textColor);

    radiusTxt.loadFromRenderedText(renderer, font, "Radius", {177, 156, 217});
    massTxt.loadFromRenderedText(renderer, font, "Mass", {177, 156, 217});
    velocityTxt.loadFromRenderedText(renderer, font, "Velocity", {177, 156, 217});
    kineticTxt.loadFromRenderedText(renderer, font, "Kinetic Energy", {177, 156, 217});
    potentialTxt.loadFromRenderedText(renderer, font, "Potential Energy", {177, 156, 217});
}

void Overlay::displayTimeRate(int rate)
{
    string text = "1 " + Utilities::getTimeRates()[rate].first + "/sec";
    rateTxt.setCoords(10, dateTxt.y - rateTxt.getHeight() - 5);
    rateTxt.loadFromRenderedText(renderer, smallFont, text, textColor, 1);
    rateTxt.render(renderer);
}

void Overlay::displaySimulationDate(int rate, int totalTime)
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
    dateTxt.loadFromRenderedText(renderer, smallFont, text, textColor, 1);
    dateTxt.setCoords(10, height - dateTxt.getHeight() - 10);
    dateTxt.render(renderer);
}

void Overlay::displaySimulationStatus(bool paused, bool editing)
{
    modeTxt.setCoords(10, 0);
    if (paused)
    {
        modeTxt.loadFromRenderedText(renderer, smallFont, "Paused", textColor, 1);
        modeTxt.render(renderer);
    }
    else if (editing)
    {
        modeTxt.loadFromRenderedText(renderer, smallFont, "Editing Mode", textColor, 1);
        modeTxt.render(renderer);
    }
}

void Overlay::displayFocusedObject(string name, string objType)
{
    focusedTxt.setCoords(10, 0);
    focusedTxt.loadFromRenderedText(renderer, smallFont, "Focused on " + name + " (" + objType + ")", textColor, 1.5);
    focusedTxt.render(renderer);
}

void Overlay::displayObjectInfo(CelestialObject obj, int x, int y, int timeRate)
{
    std::stringstream radius;
    std::stringstream mass;
    std::stringstream velocity;
    std::stringstream kinetic;
    std::stringstream potential;
    radius << Utilities::getRealDistance(obj.radius);
    mass << Utilities::getRealMass(obj.mass);
    velocity << Utilities::getRealDistance(obj.velocity.magnitude()) / Utilities::getTimeRates()[timeRate].second;
    kinetic << obj.kineticEnergy;
    potential << obj.potentialEnergy;

    rTxt.loadFromRenderedText(renderer, smallFont, radius.str() + " km", textColor);
    mTxt.loadFromRenderedText(renderer, smallFont, mass.str() + " km", textColor);
    vTxt.loadFromRenderedText(renderer, smallFont, velocity.str() + " m/s", textColor);
    kTxt.loadFromRenderedText(renderer, smallFont, kinetic.str() + " J", textColor);
    pTxt.loadFromRenderedText(renderer, smallFont, potential.str() + " J", textColor);

    radiusTxt.setCoords(x + 10, y + 10);
    rTxt.setCoords(x + 10, radiusTxt.y + radiusTxt.getHeight() + 5);
    massTxt.setCoords(x + 10, rTxt.y + rTxt.getHeight() + 20);
    mTxt.setCoords(x + 10, massTxt.y + massTxt.getHeight() + 5);
    velocityTxt.setCoords(x + 10, mTxt.y + mTxt.getHeight() + 20);
    vTxt.setCoords(x + 10, velocityTxt.y + velocityTxt.getHeight() + 5);
    kineticTxt.setCoords(x + 10, vTxt.y + vTxt.getHeight() + 20);
    kTxt.setCoords(x + 10, kineticTxt.y + kineticTxt.getHeight() + 5);
    potentialTxt.setCoords(x + 10, kTxt.y + kTxt.getHeight() + 20);
    pTxt.setCoords(x + 10, potentialTxt.y + potentialTxt.getHeight() + 5);

    radiusTxt.render(renderer);
    rTxt.render(renderer);
    massTxt.render(renderer);
    mTxt.render(renderer);
    velocityTxt.render(renderer);
    vTxt.render(renderer);
    kineticTxt.render(renderer);
    kTxt.render(renderer);
    potentialTxt.render(renderer);
    pTxt.render(renderer);
}
