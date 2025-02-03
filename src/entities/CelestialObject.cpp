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
    if (objType == "Star")
    {
        mass = Utilities::scaleMass(2 * pow(10, 30));
        radius = Utilities::scaleDistance(60000);
        updateRadius = Utilities::scaleDistance(60000);
    }
    else if (objType == "Planet")
    {
        mass = Utilities::scaleMass(6 * pow(10, 24));
        radius = Utilities::scaleDistance(10000);
        updateRadius = Utilities::scaleDistance(10000);
    }
    else if (objType == "Moon")
    {
        mass = Utilities::scaleMass(2 * pow(10, 22));
        radius = Utilities::scaleDistance(1000);
        updateRadius = Utilities::scaleDistance(1000);
    }
    else if (objType == "Asteroid")
    {
        mass = Utilities::scaleMass(pow(10, 15));
        radius = Utilities::scaleDistance(100);
        updateRadius = Utilities::scaleDistance(100);
    }
    position = Vector(0, 0);
    particlePos = Vector(0, 0);
    velocity.x = Utilities::scaleDistance(0);
    velocity.y = Utilities::scaleDistance(0);
    ctrl = false;
    editing = false;
    rect = {position.x - radius, position.y - radius, radius * 2, radius * 2};
    realRect = {position.x - radius, position.y - radius, radius * 2, radius * 2};

    nameTxt.loadFromRenderedText(renderer, TTF_OpenFont("assets/Fonts/font.otf", 20), name + " - (" + objType + ")", {255, 255, 255});

    arrow.loadFromFile(renderer, "assets/arrow.png", 0.1);
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
    else
    {
        particles = Particles("Purple");
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
    mass = Utilities::scaleMass(m);
    radius = Utilities::scaleDistance(r);
    updateRadius = Utilities::scaleDistance(r);
    position = Vector(x, y);
    velocity = vel;
    velocity.x = Utilities::scaleDistance(velocity.x);
    velocity.y = Utilities::scaleDistance(velocity.y);
    ctrl = false;
    editing = false;
    rect = {position.x - radius, position.y - radius, radius * 2, radius * 2};
    realRect = {position.x - radius, position.y - radius, radius * 2, radius * 2};
    nameTxt.loadFromRenderedText(renderer, TTF_OpenFont("assets/Fonts/font.otf", 20), name + " - (" + objType + ")", {255, 255, 255});

    arrow.loadFromFile(renderer, "assets/arrow.png", 0.1);
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
    else
    {
        particles = Particles("Purple");
    }
}

void CelestialObject::setName(SDL_Renderer *renderer, string name)
{
    this->name = name;
    nameTxt.loadFromRenderedText(renderer, TTF_OpenFont("assets/Fonts/font.otf", 20), name + " - (" + objType + ")", {255, 255, 255});
}

void CelestialObject::applyForces(vector<CelestialObject> &objects, double timeStep, int timeRate)
{
    int rate = Utilities::getTimeRates()[timeRate].second;
    for (int i = 0; i < Utilities::getSubdividor()[timeRate]; i++)
    {
        for (CelestialObject &obj : objects)
        {
            for (CelestialObject &otherObj : objects)
            {
                if (obj.name != otherObj.name)
                {
                    Vector forceVector = otherObj.position - obj.position;
                    forceVector.normalize();

                    double force = Utilities::getGravityForce(obj.mass, otherObj.mass, otherObj.position.distance(obj.position), rate);
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
    double third = 1 / 3.0;
    for (int i = 0; i < objects.size(); i++)
    {
        objects[i].position += objects[i].velocity * timeStep;

        objects[i].calculateEnergies(objects);
        for (int ii = 0; ii < objects.size(); ii++)
        {
            if (objects[i].name != objects[ii].name && (objects[i].position.distance(objects[ii].position) <= objects[i].radius + objects[ii].radius))
            {
                if (objects[i].mass >= objects[ii].mass || objects[i].objType == "Star" && objects[ii].objType != "Star")
                {
                    objects[i].mass += objects[ii].mass;
                    objects[i].velocity = (objects[i].velocity * objects[i].mass + objects[ii].velocity * objects[ii].mass) / (objects[i].mass + objects[ii].mass);
                    objects[i].updateRadius = pow(pow(objects[i].radius, 3) + pow(objects[ii].radius, 3), third);

                    objects[i].particlePos = (objects[ii].position - objects[i].position).normalize();
                    objects[i].particleDurationTimer.start();
                    objects.erase(objects.begin() + ii);
                }
                else if (objects[i].mass < objects[ii].mass || objects[ii].objType == "Star" && objects[i].objType != "Star")
                {
                    objects[ii].mass += objects[i].mass;
                    objects[ii].velocity = (objects[ii].velocity * objects[ii].mass + objects[i].velocity * objects[i].mass) / (objects[ii].mass + objects[i].mass);
                    objects[ii].updateRadius = pow(pow(objects[ii].radius, 3) + pow(objects[i].radius, 3), third);

                    objects[ii].particlePos = (objects[i].position - objects[ii].position).normalize();
                    objects[ii].particleDurationTimer.start();
                    objects.erase(objects.begin() + i);
                }
            }
        }
        objects[i].updateSizeGradually(objects[i].renderer, timeStep);
    }
}

void CelestialObject::calculateEnergies(vector<CelestialObject> &objects)
{
    kineticEnergy = 0.5 * Utilities::getRealMass(mass) * pow(Utilities::getRealDistance(velocity.magnitude()), 2);
    potentialEnergy = 0;
    for (CelestialObject &obj : objects)
    {
        if (name != obj.name)
        {
            potentialEnergy += (Utilities::g * Utilities::getRealMass(obj.mass) * Utilities::getRealMass(mass)) / Utilities::getRealDistance(obj.position.distance(position));
        }
    }
}

void CelestialObject::display(SDL_Renderer *renderer, vector<CelestialObject> &objects, Vector panningOffset, int timeRate, double timeStep)
{
    for (CelestialObject &obj : objects)
    {
        obj.particles.renderParticles(renderer, obj.objType, obj.position.x + (obj.particlePos.x * obj.radius), obj.position.y + (obj.particlePos.y * obj.radius), panningOffset, obj.particlePos * obj.radius, timeStep, obj.particleDurationTimer.isStarted());
        if (obj.particleDurationTimer.getTicks() >= 500)
        {
            obj.particleDurationTimer.stop();
            obj.particlePos = Vector(0, 0);
        }

        if (obj.objType == "Comet" && !obj.ctrl)
        {
            obj.particles.renderParticles(renderer, obj.objType, obj.position.x, obj.position.y, panningOffset, obj.velocity * -1, timeStep, true);
        }

        if (obj.editing)
        {
            obj.drawTrajectory(renderer, objects, panningOffset, timeRate, timeStep);
        }
        obj.object.render(obj.position.x - obj.radius + panningOffset.x, obj.position.y - obj.radius + panningOffset.y);
        obj.rect = {obj.position.x + panningOffset.x - obj.radius, obj.position.y + panningOffset.y - obj.radius, obj.radius * 2, obj.radius * 2};
        obj.realRect = {obj.position.x + panningOffset.x - obj.radius, obj.position.y + panningOffset.y - obj.radius, obj.radius * 2, obj.radius * 2};
        obj.arrow.setCoords(obj.position.x + panningOffset.x, obj.position.y + panningOffset.y - obj.arrow.getHeight() / 2);
        if (obj.radius < 8)
        {
            obj.rect = {obj.position.x - 8 + panningOffset.x, obj.position.y - 8 + panningOffset.y, 16, 16};
        }
    }
}

void CelestialObject::drawTrajectory(SDL_Renderer *renderer, vector<CelestialObject> objects, Vector panningOffset, int timeRate, double timeStep)
{
    SDL_SetRenderDrawColor(renderer, 203, 195, 227, 100);
    double rate = Utilities::getTimeRates()[timeRate].second;
    Vector pos = position;
    Vector prevPos = pos;
    Vector vel = velocity;
    int counter = 0;
    bool stop = false;

    for (int i = 0; i < 10000; i++)
    {
        SDL_Rect trajectoryRect = {pos.x + panningOffset.x - trajectory.getWidth() / 2, pos.y + panningOffset.y - trajectory.getHeight() / 2, trajectory.getWidth(), trajectory.getHeight()};
        for (CelestialObject obj : objects)
        {
            if (name != obj.name)
            {
                if (SDL_HasIntersection(obj.getRealRect(), &trajectoryRect))
                {
                    stop = true;
                }
                Vector forceVector = obj.position - pos;
                forceVector.normalize();

                double force = Utilities::getGravityForce(obj.mass, mass, pos.distance(obj.position), rate);
                forceVector *= (force / mass);
                vel += forceVector * 0.001;
            }
        }

        pos += vel * 0.001;
        if (!SDL_HasIntersection(&realRect, &trajectoryRect) && !stop)
        {
            if (pos.distance(prevPos) >= 50 || counter == 0)
            {
                prevPos = pos;
                counter++;
                trajectory.setCoords(trajectoryRect.x, trajectoryRect.y);
                trajectory.render(renderer);
            }
        }

        if (counter > 100 || stop)
        {
            break;
        }
    }
}

void CelestialObject::updateAllSizes(SDL_Renderer *renderer, vector<CelestialObject> &objects)
{
    for (CelestialObject &obj : objects)
    {
        obj.object = Animation(renderer, obj.folder, 1, 0.1);
        obj.object.multiplier = (1 / obj.object.getWidth()) * (obj.radius * 2);
        obj.object.loadFrames();
    }
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
