#include <Utilities.h>

long double Utilities::g = 6.674 * (1 / pow(10, 20));

vector<pair<Vector, Vector>> Utilities::getZoomScales(float width, float height)
{
    vector<pair<Vector, Vector>> zoomScales;

    for (float scale = 0.4; scale < 2; scale += 0.2)
    {
        zoomScales.push_back(make_pair(Vector(width * scale, height * scale), Vector(((width * 0.4) - (width * 0.2)) / 2, ((height * 0.4) - (height * 0.2)) / 2)));
    }

    return zoomScales;
}

vector<vector<string>> Utilities::getStellarObjects()
{
    vector<string> stars = {"Stars", "Star", "Star", "Star"};
    vector<string> planets = {"Planets", "Planet", "Planet", "Planet", "Planet", "Planet", "Planet", "Planet", "Planet"};
    vector<string> moons = {"Moons", "Moon", "Moon", "Moon"};
    vector<string> asteroids = {"Asteroids", "Asteroid", "Asteroid", "Asteroid"};
    vector<string> comets = {"Comets", "Comet"};
    vector<vector<string>> stellarObjects = {stars, planets, moons, asteroids, comets};
    return stellarObjects;
}

vector<vector<string>> Utilities::getStellarFiles()
{
    vector<string> stars = {"Stars", "assets/Objects/Stars/Red Star", "assets/Objects/Stars/White Star", "assets/Objects/Stars/Blue Star"};
    vector<string> planets = {"Planets", "assets/Objects/Planets/Mercury", "assets/Objects/Planets/Venus", "assets/Objects/Planets/Earth", "assets/Objects/Planets/Mars", "assets/Objects/Planets/Jupiter", "assets/Objects/Planets/Uranus", "assets/Objects/Planets/Neptune", "assets/Objects/Planets/Pluto"};
    vector<string> moons = {"Moons", "assets/Objects/Moons/Moon", "assets/Objects/Moons/Europa", "assets/Objects/Moons/Triton"};
    vector<string> asteroids = {"Asteroids", "assets/Objects/Asteroids/Asteroid 1", "assets/Objects/Asteroids/Asteroid 2", "assets/Objects/Asteroids/Asteroid 3"};
    vector<string> comets = {"Comets", "assets/Objects/Comet"};
    vector<vector<string>> stellarObjects = {stars, planets, moons, asteroids, comets};
    return stellarObjects;
}

vector<float> Utilities::toSecs()
{
    vector<float> toSecs = {};
    toSecs.push_back(1);
    toSecs.push_back(60);
    toSecs.push_back(3600);
    toSecs.push_back(24 * 3600);
    toSecs.push_back(24 * 3600 * 7);
    toSecs.push_back(24 * 3600 * 30);
    toSecs.push_back(24 * 3600 * 365);

    return toSecs;
}

vector<float> Utilities::getSubdividor()
{
    vector<float> subdividors = {};
    subdividors.push_back(1);
    subdividors.push_back(1);
    subdividors.push_back(1);
    subdividors.push_back(24);
    subdividors.push_back(24 * 7);
    subdividors.push_back(24 * 30);
    subdividors.push_back(24 * 365);

    return subdividors;
}

vector<string> Utilities::getUnits()
{
    vector<string> units = {};
    units.push_back("m/s");
    units.push_back("m/min");
    units.push_back("m/h");
    units.push_back("m/h");
    units.push_back("m/h");
    units.push_back("m/h");
    units.push_back("m/h");

    return units;
}

vector<pair<string, double>> Utilities::getTimeRates()
{
    vector<pair<string, double>> timeRates = {};
    timeRates.push_back(make_pair("sec", 1.0));
    timeRates.push_back(make_pair("min", 60.0));
    timeRates.push_back(make_pair("hour", 3600.0));
    timeRates.push_back(make_pair("day", 3600.0));
    timeRates.push_back(make_pair("week", 3600.0));
    timeRates.push_back(make_pair("month", 3600.0));
    timeRates.push_back(make_pair("year", 3600.0));

    return timeRates;
}

vector<pair<string, float>> Utilities::getTimeMultipliers()
{
    vector<pair<string, float>> timeRates = {};
    timeRates.push_back(make_pair("Second", 1));
    timeRates.push_back(make_pair("Minute", 60));
    timeRates.push_back(make_pair("Hour", 60));
    timeRates.push_back(make_pair("Day", 1));
    timeRates.push_back(make_pair("Week", 1));
    timeRates.push_back(make_pair("Month", 1));
    timeRates.push_back(make_pair("Year", 1));

    return timeRates;
}

float Utilities::getOrbitalVelocity(float orbitingMass, float distance)
{
    float vel = sqrt(Utilities::g * 3600 * orbitingMass / distance);
    return vel;
}

long double Utilities::getGravityForce(long double mass1, long double mass2, long double distance, float rate)
{
    return (Utilities::g * (pow(rate, 2))*mass1 * mass2) / pow(distance, 2);
}

void Utilities::drawCircle(SDL_Renderer *renderer, int displayX, int displayY, int radius)
{
    for (int y = -radius; y <= radius; y++)
    {
        int dx = (int)sqrt(radius * radius - y * y);
        SDL_RenderDrawLine(renderer, -dx + displayX, y + displayY, dx + displayX, y + displayY);
    }
};

string Utilities::removeTrailingZeroes(string number)
{
    string newStr = "";
    for (int i = number.length() - 1; i > -1; i--)
    {
        if (number[i] == '.' || (number[i] != '0' && number.find('.') != string::npos))
        {
            (number[i] == '.') ? newStr = number.substr(0, i) : newStr = number.substr(0, i + 1);
            break;
        }
        if (number[i] != '0' && number.find('.') == string::npos)
        {
            return number;
        }
    }
    return newStr;
}

bool Utilities::validateRadius(string radius)
{
    try
    {
        long double r = stold(radius);
    }
    catch (const std::exception &e)
    {
        return false;
    }
    return true;
}

bool Utilities::validateMass(string mass)
{
    try
    {
        long double r = stold(mass);
    }
    catch (const std::exception &e)
    {
        return false;
    }
    return true;
}

bool Utilities::validateVelocity(string vel)
{
    try
    {
        long double v = stold(vel);
    }
    catch (const std::exception &e)
    {
        return false;
    }
    return true;
}

bool Utilities::validateDirection(string degrees)
{
    try
    {
        long double d = stold(degrees);
        if (d >= -180 && d <= 180)
        {
            return true;
        }
    }
    catch (const std::exception &e)
    {
    }

    return false;
}

string Utilities::getExponentForm(string num)
{
    string result;
    int numZeroes = 0;
    int decimal = 0;

    if (num == "0")
    {
        return num;
    }

    for (int i = num.length() - 1; i > -1; i--)
    {
        if (num[i] == '0')
        {
            numZeroes++;
        }
        else
        {
            if (numZeroes > 0)
            {
                result = num.substr(0, i + 1);
                return result + "E+" + to_string(numZeroes);
            }
            else
            {
                break;
            }
        }
    }

    numZeroes = 0;
    for (int i = 0; i < num.length(); i++)
    {
        if (num[i] == '0')
        {
            numZeroes++;
        }
        else if (num[i] == '.')
        {
            decimal++;
        }
        else
        {
            if (numZeroes > 0 && decimal == 1)
            {
                result = num.substr(i, num.length() - i);
                return result + "E-" + to_string(numZeroes);
            }
        }
    }

    return num;
}

pair<string, string> Utilities::parseInput(string input)
{
    string currentState = "Base";
    string base = "";
    string exponent = "";

    for (int x = 0; x < input.length(); x++)
    {
        if (isdigit(input[x]) || input[x] == '.' || input[x] == '-' || input[x] == '+')
        {
            (currentState == "Base") ? base += input[x] : exponent += input[x];
        }
        else if (input[x] == 'e' || input[x] == 'E')
        {
            currentState = "Exponent";
        }
    }
    if (exponent.length() == 0)
    {
        exponent = "0";
    }

    return make_pair(base.c_str(), exponent.c_str());
}
