#include <Utilities.h>

float Utilities::g = 6.674 * (1 / pow(10, 8));
int Utilities::numObjects = 1;

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
    vector<string> stars = {"Stars", "Star", "Star"};
    vector<string> planets = {"Planets", "Planet", "Planet", "Planet", "Planet", "Planet"};
    // vector<string> moons = {"Moons", "Moon", "Moon", "Moon"};
    // vector<string> asteroids = {"Asteroids", "Asteroid"};
    vector<vector<string>> stellarObjects = {stars, planets};
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

vector<pair<string, float>> Utilities::getTimeRates()
{
    vector<pair<string, float>> timeRates = {};
    timeRates.push_back(make_pair("sec", 1));
    timeRates.push_back(make_pair("min", 60));
    timeRates.push_back(make_pair("hour", 3600));
    timeRates.push_back(make_pair("day", 3600));
    timeRates.push_back(make_pair("week", 3600));
    timeRates.push_back(make_pair("month", 3600));
    timeRates.push_back(make_pair("year", 3600));

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

double Utilities::getGravityForce(float mass1, float mass2, float distance, float rate, double timeStep)
{
    return (Utilities::g * (pow(rate, 2)) * mass1 * mass2) / pow(distance, 2);
}

void Utilities::drawCircle(SDL_Renderer *renderer, int displayX, int displayY, int radius)
{
    for (int y = -radius; y <= radius; y++)
    {
        int dx = (int)sqrt(radius * radius - y * y);
        SDL_RenderDrawLine(renderer, -dx + displayX, y + displayY, dx + displayX, y + displayY);
    }
};

long double Utilities::scaleDistance(long double dist)
{
    return dist / 2000;
}

long double Utilities::scaleMass(long double mass)
{
    return mass / pow(10, 24);
}

long double Utilities::getRealDistance(long double dist)
{
    return dist * 2000;
}

long double Utilities::getRealMass(long double mass)
{
    return mass * pow(10, 24);
}

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
    int decimal = 0;
    if (radius.length() == 0)
    {
        return false;
    }

    for (int i = 0; i < radius.length(); i++)
    {
        if (radius[i] == '.')
        {
            decimal++;
        }
        if ((!isdigit(radius[i]) && radius[i] != '.') || decimal > 1)
        {
            return false;
        }
    }
    return true;
}

bool Utilities::validateMass(string mass)
{
    int decimal = 0;
    if (mass.length() == 0)
    {
        return false;
    }

    for (int i = 0; i < mass.length(); i++)
    {
        if (!isdigit(mass[i]) || decimal > 1)
        {
            return false;
        }
        if (mass[i] == '.')
        {
            decimal++;
        }
    }
    return true;
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
