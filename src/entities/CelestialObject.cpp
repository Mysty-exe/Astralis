#include <CelestialObject.h>

CelestialObject::CelestialObject()
{
}

CelestialObject::CelestialObject(SDL_Renderer *renderer, int width, int height, string name, string objType, string fileName)
{
    this->renderer = renderer;
    this->width = width;
    this->height = height;
    this->name = name;
    this->objType = objType;
    this->folder = fileName;
    toggleParticles = true;
    if (objType == "Star")
    {
        mass = 2 * pow(10, 30);
        radius = 60000;
        updateRadius = 60000;
    }
    else if (objType == "Planet")
    {
        mass = 6 * pow(10, 24);
        radius = 10000;
        updateRadius = 10000;
    }
    else if (objType == "Moon")
    {
        mass = 2 * pow(10, 22);
        radius = 1000;
        updateRadius = 1000;
    }
    else if (objType == "Asteroid")
    {
        mass = pow(10, 15);
        radius = 100;
        updateRadius = 100;
    }
    else if (objType == "Comet")
    {
        mass = pow(10, 14);
        radius = 10;
        updateRadius = 10;
    }
    position = Vector(0, 0);
    particlePos = Vector(0, 0);
    velocity.x = 0;
    velocity.y = 0;
    ctrl = false;
    editing = false;
    rect = {position.x - radius, position.y - radius, radius * 2, radius * 2};
    realRect = {position.x - radius, position.y - radius, radius * 2, radius * 2};

    nameTxt.loadFromRenderedText(renderer, TTF_OpenFont("assets/Fonts/quicksand.otf", 20), name + " - (" + objType + ")", {255, 255, 255});

    trajectory.loadFromFile(renderer, "assets/circle.png", 0.25);
    object = Animation(renderer, fileName, 1, 0.1);
    object.multiplier = (1 / object.getWidth()) * (radius * 2);
    object.loadFrames();

    viewObject = Animation(renderer, fileName, 1, 0.1);
    viewObject.multiplier = (1 / viewObject.getWidth()) * (width / 4 - 100);
    viewObject.loadFrames();

    if (fileName.find("White") != std::string::npos)
    {
        particles = Particles("White");
    }
    else if (fileName.find("Red") != std::string::npos)
    {
        particles = Particles("Red");
    }
    else if (fileName.find("Blue") != std::string::npos)
    {
        particles = Particles("Blue");
    }
    else if (objType == "Comet")
    {
        particles = Particles("Purple");
    }
    else
    {
        particles = Particles("Grey");
    }
}

CelestialObject::CelestialObject(SDL_Renderer *renderer, int width, int height, string n, string objType, string fileName, long double m, long double r, float x, float y, Vector vel)
{
    this->renderer = renderer;
    this->width = width;
    this->height = height;
    name = n;
    this->objType = objType;
    this->folder = fileName;
    toggleParticles = true;
    mass = m;
    radius = r;
    updateRadius = r;
    position = Vector(x, y);
    velocity = vel;
    velocity.x = velocity.x;
    velocity.y = velocity.y;
    ctrl = false;
    editing = false;
    rect = {position.x - radius, position.y - radius, radius * 2, radius * 2};
    realRect = {position.x - radius, position.y - radius, radius * 2, radius * 2};
    nameTxt.loadFromRenderedText(renderer, TTF_OpenFont("assets/Fonts/quicksand.otf", 20), name + " - (" + objType + ")", {255, 255, 255});

    trajectory.loadFromFile(renderer, "assets/circle.png", 0.25);
    object = Animation(renderer, fileName, 1, 0.1);
    object.multiplier = (1 / object.getWidth()) * (radius * 2);
    object.loadFrames();

    viewObject = Animation(renderer, fileName, 1, 0.1);
    viewObject.multiplier = (1 / viewObject.getWidth()) * (width / 4 - 100);
    viewObject.loadFrames();

    if (fileName.find("White") != std::string::npos)
    {
        particles = Particles("White");
    }
    else if (fileName.find("Red") != std::string::npos)
    {
        particles = Particles("Red");
    }
    else if (fileName.find("Blue") != std::string::npos)
    {
        particles = Particles("Blue");
    }
    else if (objType == "Comet")
    {
        particles = Particles("Purple");
    }
    else
    {
        particles = Particles("Grey");
    }
}

void CelestialObject::setName(SDL_Renderer *renderer, string name)
{
    this->name = name;
    nameTxt.loadFromRenderedText(renderer, TTF_OpenFont("assets/Fonts/quicksand.otf", 20), name + " - (" + objType + ")", {255, 255, 255});
}

void CelestialObject::drawTrajectory(SDL_Renderer *renderer, vector<CelestialObject> objects, Vector panningOffset, int timeRate, double timeStep)
{
    // SDL_SetRenderDrawColor(renderer, 203, 195, 227, 100);
    // double rate = Utilities::getTimeRates()[timeRate].second;
    // Vector pos = position;
    // Vector prevPos = pos;
    // Vector vel = velocity;
    // int counter = 0;
    // bool stop = false;

    // for (int i = 0; i < 10000; i++)
    // {
    //     SDL_Rect trajectoryRect = {pos.x + panningOffset.x - trajectory.getWidth() / 2, pos.y + panningOffset.y - trajectory.getHeight() / 2, trajectory.getWidth(), trajectory.getHeight()};
    //     for (CelestialObject obj : objects)
    //     {
    //         if (name != obj.name)
    //         {
    //             if (SDL_HasIntersection(obj.getRealRect(), &trajectoryRect))
    //             {
    //                 stop = true;
    //             }
    //             Vector forceVector = obj.position - pos;
    //             forceVector.normalize();

    //             double force = Utilities::getGravityForce(obj.mass, mass, pos.distance(obj.position), rate);
    //             forceVector *= (force / mass);
    //             vel += forceVector * 0.001;
    //         }
    //     }

    //     pos += vel * 0.001;
    //     if (!SDL_HasIntersection(&realRect, &trajectoryRect) && !stop)
    //     {
    //         if (pos.distance(prevPos) >= 50 || counter == 0)
    //         {
    //             prevPos = pos;
    //             counter++;
    //             trajectory.setCoords(trajectoryRect.x, trajectoryRect.y);
    //             trajectory.render(renderer);
    //         }
    //     }

    //     if (counter > 100 || stop)
    //     {
    //         break;
    //     }
    // }
}

void CelestialObject::updateSizeGradually(SDL_Renderer *renderer, double timeStep)
{
    if (radius != updateRadius)
    {
        radius += 100 * timeStep;
        radius = (radius > updateRadius) ? updateRadius : radius;
        object = Animation(renderer, folder, 1, 0.1);
        object.multiplier = (1 / object.getWidth()) * (radius * 2);
        object.loadFrames();
    }
}

void CelestialObject::updateSizeInstant(SDL_Renderer *renderer)
{
    radius = updateRadius;
    object = Animation(renderer, folder, 1, 0.1);
    object.multiplier = (1 / object.getWidth()) * (radius * 2);
    object.loadFrames();
}

SDL_Rect *CelestialObject::getRect()
{
    return &rect;
}

SDL_Rect *CelestialObject::getRealRect()
{
    return &realRect;
}
