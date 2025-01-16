#include <CelestialObject.h>

CelestialObject::CelestialObject()
{
}

CelestialObject::CelestialObject(SDL_Renderer *renderer, string name, string objType)
{
    this->name = name;
    this->objType = objType;
    if (objType == "Star")
    {
        mass = Utilities::scaleMass(2 * pow(10, 30));
        radius = Utilities::scaleDistance(60000);
    }
    else if (objType == "Planet")
    {
        mass = Utilities::scaleMass(6 * pow(10, 24));
        radius = Utilities::scaleDistance(10000);
    }
    else if (objType == "Moon")
    {
        mass = Utilities::scaleMass(2 * pow(10, 22));
        radius = Utilities::scaleDistance(1000);
    }
    else if (objType == "Asteroid")
    {
        mass = Utilities::scaleMass(pow(10, 15));
        radius = Utilities::scaleDistance(100);
    }
    position = Vector(0, 0);
    velocity.x = Utilities::scaleDistance(0);
    velocity.y = Utilities::scaleDistance(0);
    editing = false;
    rect = {position.x - radius, position.y - radius, radius * 2, radius * 2};

    arrow.loadFromFile(renderer, "assets/arrow.png");
}

CelestialObject::CelestialObject(SDL_Renderer *renderer, string n, string objType, long double m, long double r, float x, float y, Vector vel)
{
    name = n;
    this->objType = objType;
    mass = Utilities::scaleMass(m);
    radius = Utilities::scaleDistance(r);
    position = Vector(x, y);
    velocity = vel;
    velocity.x = Utilities::scaleDistance(velocity.x);
    velocity.y = Utilities::scaleDistance(velocity.y);
    editing = false;
    rect = {position.x - radius, position.y - radius, radius * 2, radius * 2};
    arrow.loadFromFile(renderer, "assets/arrow.png");
}

void CelestialObject::applyForces(vector<CelestialObject> &objects, double timeStep, int timeRate)
{
    int rate = Utilities::getTimeRates()[timeRate].second;
    for (int i = 0; i < Utilities::getSubdividor()[timeRate]; i++)
    {
        for (CelestialObject &obj : objects)
        {
            if (obj.name == "Sun")
            {
                continue;
            }
            for (CelestialObject &otherObj : objects)
            {
                if (obj.name != otherObj.name)
                {
                    Vector forceVector = otherObj.position - obj.position;
                    forceVector.normalize();

                    float force = Utilities::getGravityForce(obj.mass, otherObj.mass, otherObj.position.distance(obj.position), rate, timeStep);
                    forceVector *= (force / obj.mass);
                    obj.velocity += forceVector * timeStep;
                }
            }
        }
        applyVelocities(objects, timeStep);
    }
}

void CelestialObject::applyVelocities(vector<CelestialObject> &objects, double timeStep)
{
    for (CelestialObject &obj : objects)
    {
        obj.position += obj.velocity * timeStep;
    }
}

void CelestialObject::display(SDL_Renderer *renderer, vector<CelestialObject> &objects, Vector panningOffset, Vector zoomOffset)
{
    for (CelestialObject &obj : objects)
    {
        Utilities::drawCircle(renderer, obj.position.x + panningOffset.x + zoomOffset.x, obj.position.y + panningOffset.y + zoomOffset.y, obj.radius);
        obj.rect = {obj.position.x - obj.radius + panningOffset.x, obj.position.y - obj.radius + panningOffset.y, obj.radius * 2, obj.radius * 2};
        if (obj.radius < 8)
        {
            obj.rect = {obj.position.x - 8 + panningOffset.x, obj.position.y - 8 + panningOffset.y, 16, 16};
        }
        // if (obj.editing)
        // {
        //     obj.arrow.setCoords(obj.position.x + panningOffset.x, obj.position.y + panningOffset.y - obj.arrow.getHeight());
        //     obj.arrow.render(renderer, 0, 0, 90);
        // }
    }
}

SDL_Rect *CelestialObject::getRect()
{
    return &rect;
}
