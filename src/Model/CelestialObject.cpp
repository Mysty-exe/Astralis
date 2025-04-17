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
    this->folder = fileName;
    delObject = false;

    if (objType == "Star" || objType == "Stars")
    {
        this->objType = STAR;
        mass = 2 * pow(10, 30);
        radius = 60000;
        updateRadius = 60000;
    }
    else if (objType == "Planet" || objType == "Planets")
    {
        this->objType = PLANET;
        mass = 6 * pow(10, 24);
        radius = 10000;
        updateRadius = 10000;
    }
    else if (objType == "Moon" || objType == "Moons")
    {
        this->objType = MOON;
        mass = 2 * pow(10, 22);
        radius = 1000;
        updateRadius = 1000;
    }
    else if (objType == "Asteroid" || objType == "Asteroids")
    {
        this->objType = ASTEROID;
        mass = pow(10, 15);
        radius = 100;
        updateRadius = 100;
    }

    position = Vector(0, 0);
    velocity.x = 0;
    velocity.y = 0;

    rect = {(int)position.x - (int)radius, (int)position.y - (int)radius, (int)radius * 2, (int)radius * 2};
    realRect = {(int)position.x - (int)radius, (int)position.y - (int)radius, (int)radius * 2, (int)radius * 2};

    nameTxt.loadFromRenderedText(renderer, TTF_OpenFont("assets/Fonts/quicksand.otf", 20), name + " - (" + getObjType(this->objType) + ")", {255, 255, 255});

    object.loadAnimation(renderer, fileName, Vector(1, 1), 0.1);
    object.loadFrames(renderer, Utilities::getSizeRatio(object.getSize(), Vector(radius, radius)));

    viewObject.loadAnimation(renderer, fileName, Vector(1, 1), 0.1);
    viewObject.loadFrames(renderer, Utilities::getSizeRatio(viewObject.getSize(), Vector((width / 5) - 100, (width / 5) - 100)));
}

CelestialObject::CelestialObject(SDL_Renderer *renderer, int width, int height, string name, string objType, string fileName, long double m, long double r, float x, float y, Vector vel)
{
    this->renderer = renderer;
    this->width = width;
    this->height = height;
    this->name = name;
    this->folder = fileName;
    delObject = false;

    mass = m;
    radius = r;
    updateRadius = r;
    position = Vector(x, y);
    velocity = vel;

    rect = {(int)position.x - (int)radius, (int)position.y - (int)radius, (int)radius * 2, (int)radius * 2};
    realRect = {(int)position.x - (int)radius, (int)position.y - (int)radius, (int)radius * 2, (int)radius * 2};

    if (objType == "Star" || objType == "Stars")
    {
        this->objType = STAR;
    }
    else if (objType == "Planet" || objType == "Planets")
    {
        this->objType = PLANET;
    }
    else if (objType == "Moon" || objType == "Moons")
    {
        this->objType = MOON;
    }
    else if (objType == "Asteroid" || objType == "Asteroids")
    {
        this->objType = ASTEROID;
    }

    nameTxt.loadFromRenderedText(renderer, TTF_OpenFont("assets/Fonts/quicksand.otf", 20), name + " - (" + getObjType(this->objType) + ")", {255, 255, 255});

    object.loadAnimation(renderer, fileName, Vector(1, 1), 0.1);
    object.loadFrames(renderer, Utilities::getSizeRatio(object.getSize(), Vector(radius * 2, radius * 2)));

    viewObject.loadAnimation(renderer, fileName, Vector(1, 1), 0.1);
    viewObject.loadFrames(renderer, Utilities::getSizeRatio(viewObject.getSize(), Vector((width / 5) - 100, (width / 5) - 100)));
}

void CelestialObject::resizeDimensions(float width, float height)
{
    this->width = width;
    this->height = height;
    viewObject.loadAnimation(renderer, folder, Vector(1, 1), 0.1);
    viewObject.loadFrames(renderer, Utilities::getSizeRatio(viewObject.getSize(), Vector((width / 5) - 100, (width / 5) - 100)));
}

bool CelestialObject::isMarkedForDelete()
{
    return delObject;
}

void CelestialObject::deleteObject()
{
    delObject = true;
}

string CelestialObject::getName()
{
    return name;
}

string CelestialObject::getFolder()
{
    return folder;
}

CelestialType CelestialObject::getObjType()
{
    return objType;
}

long double CelestialObject::getMass()
{
    return mass;
}

void CelestialObject::setMass(long double mass)
{
    this->mass = mass;
}

void CelestialObject::addMass(long double mass)
{
    this->mass += mass;
}

long double CelestialObject::getRadius()
{
    return radius;
}

void CelestialObject::setRadius(long double radius)
{
    this->radius = radius;
}

long double CelestialObject::getUpdateRadius()
{
    return updateRadius;
}

void CelestialObject::setUpdateRadius(long double updateRadius)
{
    this->updateRadius = updateRadius;
}

Vector CelestialObject::getPosition()
{
    return position;
}

void CelestialObject::setPosition(Vector position)
{
    this->position = position;
}

Vector CelestialObject::getVelocity()
{
    return velocity;
}

void CelestialObject::setVelocity(Vector velocity)
{
    this->velocity = velocity;
}

void CelestialObject::addPosition(Vector pos)
{
    this->position += pos;
}

void CelestialObject::addVelocity(Vector vec)
{
    this->velocity += vec;
}

long double CelestialObject::getKineticEnergy()
{
    return kineticEnergy;
}

void CelestialObject::setKineticEnergy(long double energy)
{
    this->kineticEnergy = energy;
}

void CelestialObject::addKineticEnergy(long double energy)
{
    this->kineticEnergy += energy;
}

long double CelestialObject::getPotentialEnergy()
{
    return potentialEnergy;
}

void CelestialObject::setPotentialEnergy(long double energy)
{
    this->potentialEnergy = energy;
}

void CelestialObject::addPotentialEnergy(long double energy)
{
    this->potentialEnergy += energy;
}

void CelestialObject::startParticles(Vector position, float particleExplosion)
{
    particlePos = position;
    particleDurationTimer.start();
    particleManager.setExplosion(particleExplosion);
}

Animation *CelestialObject::getObject()
{
    return &object;
}

void CelestialObject::render(SDL_Rect renderQuad, Vector offset, double timeStep)
{
    if (particleDurationTimer.getTicks() / 1000 >= 1)
    {
        particleDurationTimer.stop();
        particlePos = Vector(0, 0);
    }

    particleManager.renderParticles(renderer, &renderQuad, position.x + (particlePos.x * radius), position.y + (particlePos.y * radius), offset, particlePos * radius, timeStep, particleDurationTimer.isStarted());
    object.render(renderer, position.x - radius + offset.x, position.y - radius + offset.y);
}

vector<Vector> CelestialObject::getTrajectories()
{
    return storedTrajectories;
}

void CelestialObject::setTrajectories(vector<Vector> trajectories)
{
    this->storedTrajectories = trajectories;
}

string CelestialObject::getObjType(CelestialType objType)
{
    if (objType == STAR)
    {
        return "Star";
    }
    else if (objType == PLANET)
    {
        return "Planet";
    }
    else if (objType == MOON)
    {
        return "Moon";
    }
    else if (objType == ASTEROID)
    {
        return "Asteroid";
    }
}

void CelestialObject::setName(SDL_Renderer *renderer, string name)
{
    this->name = name;
    nameTxt.loadFromRenderedText(renderer, TTF_OpenFont("assets/Fonts/quicksand.otf", 20), name + " - (" + getObjType(objType) + ")", {255, 255, 255});
}

void CelestialObject::renderViewDisplay(float x, float y, float width, float height)
{
    nameTxt.setCoords(x + width / 2 - nameTxt.getWidth() / 2, y + height - nameTxt.getHeight() - 10);
    viewObject.render(renderer, x + width / 2 - viewObject.getWidth() / 2, nameTxt.getY() - viewObject.getHeight() - 30);
    nameTxt.render(renderer);
}

void CelestialObject::updateSizeGradually(SDL_Renderer *renderer, double timeStep)
{
    if (radius != updateRadius)
    {
        radius += 100 * timeStep;
        radius = (radius > updateRadius) ? updateRadius : radius;
        object.loadAnimation(renderer, folder, Vector(1, 1), 0.1);
        object.loadFrames(renderer, Utilities::getSizeRatio(object.getSize(), Vector(radius * 2, radius * 2)));
    }
}

void CelestialObject::updateSizeInstant(SDL_Renderer *renderer)
{
    radius = updateRadius;
    object.loadAnimation(renderer, folder, Vector(1, 1), 0.1);
    object.loadFrames(renderer, Utilities::getSizeRatio(object.getSize(), Vector(radius * 2, radius * 2)));
}

SDL_Rect CelestialObject::getRect(Vector scroll, float zoomFactor)
{
    SDL_Rect r = {rect.x * zoomFactor - scroll.x, rect.y * zoomFactor - scroll.y, rect.w * zoomFactor, rect.h * zoomFactor};
    return r;
}

SDL_Rect *CelestialObject::getRealRect()
{
    return &realRect;
}

void CelestialObject::setRect(SDL_Rect rect)
{
    this->rect = rect;
}

void CelestialObject::setRealRect(SDL_Rect realRect)
{
    this->realRect = realRect;
}
