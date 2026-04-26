#include <Utilities.h>

long double Utilities::g = 6.674e-11L;
Texture Utilities::message = Texture();

const vector<float> &Utilities::toSecs()
{
    static std::vector<float> v = {
        1.0f,       // sec
        60.0f,      // min
        3600.0f,    // hour
        86400.0f,   // day
        604800.0f,  // week
        2592000.0f, // month
        31536000.0f // year
    };
    return v;
}

const std::vector<int> &Utilities::getSubdividor()
{
    static std::vector<int> v = {
        1,
        1,
        1,
        24,
        24 * 7,
        24 * 30,
        24 * 73};
    return v;
}

const std::vector<std::pair<std::string, double>> &Utilities::getTimeRates()
{
    static std::vector<std::pair<std::string, double>> v = {
        {"sec", 1.0},
        {"min", 60.0},
        {"hour", 3600.0},
        {"day", 86400.0},
        {"week", 604800.0},
        {"month", 2592000.0},
        {"year", 31536000.0}};
    return v;
}

long double Utilities::getGravityForce(long double m1, long double m2, long double distance)
{
    if (distance <= 0.0L)
        return 0.0L;
    return (g * m1 * m2) / (distance * distance);
}

float Utilities::getOrbitalVelocity(float centralMass, float distance)
{
    if (distance <= 0.0f)
        return 0.0f;
    return sqrt((float)(g * centralMass / distance));
}

// ---- Math helpers ----
Vector Utilities::getSizeRatio(Vector startSize, Vector endSize)
{
    return Vector(endSize.x / startSize.x, endSize.y / startSize.y);
}

// ---- UI / Assets ----
std::vector<std::pair<Vector, Vector>> Utilities::getZoomScales(float width, float height)
{
    std::vector<std::pair<Vector, Vector>> zoomScales;

    for (float scale = 0.4f; scale < 2.0f; scale += 0.2f)
    {
        zoomScales.emplace_back(
            Vector(width * scale, height * scale),
            Vector((width * (scale - 0.2f)) / 2.0f,
                   (height * (scale - 0.2f)) / 2.0f));
    }

    return zoomScales;
}

std::vector<std::string> Utilities::getStellarObjects()
{
    return {"Stars", "Planets", "Moons", "Asteroid"};
}

std::vector<std::vector<std::string>> Utilities::getStellarFiles()
{
    return {
        {"../Assets/Objects/Stars/Red Star", "../Assets/Objects/Stars/White Star",
         "../Assets/Objects/Stars/Blue Star", "../Assets/Objects/Stars/Green Star",
         "../Assets/Objects/Stars/Purple Star"},

        {"../Assets/Objects/Planets/Mercury", "../Assets/Objects/Planets/Venus",
         "../Assets/Objects/Planets/Earth", "../Assets/Objects/Planets/Mars",
         "../Assets/Objects/Planets/Jupiter", "../Assets/Objects/Planets/Uranus",
         "../Assets/Objects/Planets/Neptune", "../Assets/Objects/Planets/Pluto"},

        {"../Assets/Objects/Moons/Moon", "../Assets/Objects/Moons/Europa",
         "../Assets/Objects/Moons/Triton"},

        {"../Assets/Objects/Asteroid"}};
}

std::vector<std::string> Utilities::getUnits()
{
    return {"m/s", "m/min", "m/h", "m/day", "m/week", "m/month", "m/year"};
}

// ---- Rendering ----
void Utilities::drawCircle(SDL_Renderer *renderer, int x, int y, int radius)
{
    for (int dy = -radius; dy <= radius; dy++)
    {
        int dx = (int)sqrt(radius * radius - dy * dy);
        SDL_RenderDrawLine(renderer, x - dx, y + dy, x + dx, y + dy);
    }
}

bool Utilities::validateNumber(const std::string &s)
{
    try
    {
        std::stold(s);
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool Utilities::validateDirection(const std::string &s)
{
    try
    {
        long double d = std::stold(s);
        return d >= -180.0 && d <= 180.0;
    }
    catch (...)
    {
        return false;
    }
}

// ---- Formatting ----
std::string Utilities::removeTrailingZeroes(std::string number)
{
    size_t dot = number.find('.');
    if (dot == std::string::npos)
        return number;

    while (!number.empty() && number.back() == '0')
        number.pop_back();
    if (!number.empty() && number.back() == '.')
        number.pop_back();

    return number;
}

// ---- Messaging ----
void Utilities::displayMessage(SDL_Renderer *renderer, float width, float height, std::string text, int level)
{
    message.loadFromRenderedText(renderer, TTF_OpenFont("../Assets/Fonts/font.otf", 18), text, {255, 255, 255});

    int y = height - ((message.getHeight() + 30) * level);
    message.setCoords(width / 2 - message.getWidth() / 2, y);

    SDL_Rect rect = {
        message.getX() - 10,
        message.getY() - 10,
        message.getWidth() + 20,
        message.getHeight() + 20};

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_RenderFillRect(renderer, &rect);
    message.render(renderer);
}