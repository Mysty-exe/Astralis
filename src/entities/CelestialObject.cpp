#include <CelestialObject.h>

CelestialObject::CelestialObject()
{
}

CelestialObject::CelestialObject(string n, CelestialTypes objType, float m, float r, float x, float y, Vector vel)
{
    name = n;
    objectType = objType;
    mass = Utilities::scaleMass(m);
    radius = Utilities::scaleDistance(r);
    position = Vector(x, y);
    velocity = vel;
    velocity.x = Utilities::scaleDistance(velocity.x);
    velocity.y = Utilities::scaleDistance(velocity.y);
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

void CelestialObject::display(SDL_Renderer *renderer, vector<CelestialObject> objects, Vector panningOffset, Vector zoomOffset)
{
    for (CelestialObject obj : objects)
    {
        Utilities::drawCircle(renderer, obj.position.x + panningOffset.x + zoomOffset.x, obj.position.y + panningOffset.y + zoomOffset.y, obj.radius);
    }
}
