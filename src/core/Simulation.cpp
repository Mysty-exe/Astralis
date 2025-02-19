#include <Simulation.h>

Simulation::Simulation()
{
}

Simulation::Simulation(SDL_Renderer *renderer, string name, double distRatio, double simRadius)
{
    this->renderer = renderer;
    this->name = name;
    this->distRatio = distRatio;
    this->simRadius = simRadius;
    timeRate = 0;

    state = SIMULATION;
    objectsNum = 1;

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
    trajectory.loadFromFile(renderer, "assets/circle.png", 0.25);
}

long double Simulation::scaleDistance(long double dist)
{
    return dist / distRatio;
}

long double Simulation::scaleMass(long double mass)
{
    return mass / pow(10, 24);
}

long double Simulation::getRealDistance(long double dist)
{
    return dist * distRatio;
}

long double Simulation::getRealMass(long double mass)
{
    return mass * pow(10, 24);
}

void Simulation::scaleObjects(string n)
{
    for (CelestialObject &obj : objects)
    {
        if (obj.name == n)
        {
            obj.mass = scaleMass(obj.mass);
            obj.radius = scaleDistance(obj.radius);
            obj.updateRadius = scaleDistance(obj.updateRadius);
            obj.velocity.x = scaleDistance(obj.velocity.x);
            obj.velocity.y = scaleDistance(obj.velocity.y);
            obj.updateSizeInstant(renderer);
        }
    }
}

void Simulation::applyForces(double timeStep)
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

                    double force = Utilities::getGravityForce(getRealMass(obj.mass), getRealMass(otherObj.mass), getRealDistance(otherObj.position.distance(obj.position)), rate);
                    forceVector *= (force / getRealMass(obj.mass));
                    forceVector = forceVector.normalize() * scaleDistance(forceVector.magnitude());
                    obj.velocity += forceVector * timeStep;
                }
            }
        }
        applyVelocities(timeStep);
    }
}

void Simulation::applyVelocities(double timeStep)
{
    double third = 1 / 3.0;
    vector<int> indexes;
    for (int i = 0; i < objects.size(); i++)
    {
        objects[i].position += objects[i].velocity * timeStep;
        if (getRealDistance(objects[i].position.x) > simRadius || getRealDistance(objects[i].position.y) > simRadius)
        {
            if (std::find(indexes.begin(), indexes.end(), i) == indexes.end())
            {
                indexes.push_back(i);
                continue;
            }
        }

        for (int ii = 0; ii < objects.size(); ii++)
        {
            if (std::find(indexes.begin(), indexes.end(), ii) != indexes.end())
            {
                continue;
            }
            if (objects[i].name != objects[ii].name && (objects[i].position.distance(objects[ii].position) <= objects[i].radius + objects[ii].radius))
            {
                if (objects[i].mass >= objects[ii].mass || objects[i].objType == "Star" && objects[ii].objType != "Star")
                {
                    objects[i].mass += objects[ii].mass;
                    objects[i].velocity = (objects[i].velocity * objects[i].mass + objects[ii].velocity * objects[ii].mass) / (objects[i].mass + objects[ii].mass);
                    objects[i].updateRadius = pow(pow(objects[i].radius, 3) + pow(objects[ii].radius, 3), third);

                    objects[i].particlePos = (objects[ii].position - objects[i].position).normalize();
                    objects[i].particleExplosion = sqrt(getRealDistance(objects[ii].radius));
                    objects[i].particleDurationTimer.start();
                    objects[ii].object.freeAll();
                    if (std::find(indexes.begin(), indexes.end(), ii) == indexes.end())
                    {
                        indexes.push_back(ii);
                    }
                }
                else if (objects[i].mass < objects[ii].mass || objects[ii].objType == "Star" && objects[i].objType != "Star")
                {
                    objects[ii].mass += objects[i].mass;
                    objects[ii].velocity = (objects[ii].velocity * objects[ii].mass + objects[i].velocity * objects[i].mass) / (objects[ii].mass + objects[i].mass);
                    objects[ii].updateRadius = pow(pow(objects[ii].radius, 3) + pow(objects[i].radius, 3), third);

                    objects[ii].particlePos = (objects[i].position - objects[ii].position).normalize();
                    objects[ii].particleExplosion = sqrt(getRealDistance(objects[i].radius));
                    objects[ii].particleDurationTimer.start();
                    objects[i].object.freeAll();
                    if (std::find(indexes.begin(), indexes.end(), i) == indexes.end())
                    {
                        indexes.push_back(i);
                    }
                }
            }
        }
        objects[i].updateSizeGradually(objects[i].renderer, timeStep);
    }

    for (int i : indexes)
    {
        objects.erase(objects.begin() + i);
    }
}

void Simulation::calculateEnergy()
{
    for (CelestialObject &obj1 : objects)
    {
        obj1.kineticEnergy = 0.5 * getRealMass(obj1.mass) * pow(getRealDistance(obj1.velocity.magnitude()), 2);
        obj1.potentialEnergy = 0;
        for (CelestialObject &obj2 : objects)
        {
            if (obj1.name != obj2.name)
            {
                obj1.potentialEnergy += (Utilities::g * getRealMass(obj2.mass) * getRealMass(obj1.mass)) / getRealDistance(obj2.position.distance(obj1.position));
            }
        }
    }
}

void Simulation::runTrajectoryThread()
{
    std::thread worker(&Simulation::calculateTrajectory, this);
    worker.detach();
}

void Simulation::calculateTrajectory()
{
    Vector pos, prevPos, vel;
    double rate;
    int counter;
    bool stop;

    while (true)
    {
        for (CelestialObject &obj : objects)
        {
            rate = Utilities::getTimeRates()[timeRate].second;
            pos = obj.position;
            prevPos = pos;
            vel = obj.velocity / rate * 3600.0;
            counter = 0;
            stop = false;
            vector<Vector> positions;

            for (int i = 0; i < 10000; i++)
            {
                SDL_Point trajectoryPoint = {(int)pos.x, (int)pos.y};
                for (CelestialObject obj2 : objects)
                {
                    if (obj.name != obj2.name)
                    {
                        if (SDL_PointInRect(&trajectoryPoint, obj2.getRealRect()))
                        {
                            stop = true;
                        }

                        Vector forceVector = obj2.position - pos;
                        forceVector.normalize();

                        double force = Utilities::getGravityForce(getRealMass(obj.mass), getRealMass(obj2.mass), getRealDistance(obj2.position.distance(pos)), 3600.0);
                        forceVector *= (force / getRealMass(obj.mass));
                        forceVector = forceVector.normalize() * scaleDistance(forceVector.magnitude());
                        vel += forceVector * 0.001;
                    }
                }

                pos += vel * 0.001;
                SDL_Rect *rect = obj.getRealRect();
                if (!SDL_PointInRect(&trajectoryPoint, rect) && !stop)
                {
                    if (pos.distance(prevPos) >= 50 || counter == 0)
                    {
                        prevPos = pos;
                        counter++;
                        positions.push_back(pos);
                    }
                }

                if (counter >= 100 || stop)
                {
                    break;
                }
            }
            obj.storedTrajectories = positions;
        }
        std::this_thread::sleep_for(0.2s);
    }
}

void Simulation::drawTrajectory(CelestialObject obj, Vector panningOffset)
{
    SDL_SetRenderDrawColor(renderer, 203, 195, 227, 100);
    for (Vector v : obj.storedTrajectories)
    {
        trajectory.setCoords(v.x - trajectory.getWidth() / 2 + panningOffset.x, v.y - trajectory.getHeight() / 2 + panningOffset.y);
        trajectory.render(renderer);
    }
}

void Simulation::display(SDL_Rect *renderQuad, Vector panningOffset, double timeStep)
{
    for (CelestialObject &obj : objects)
    {
        if (obj.toggleParticles)
        {
            obj.particles.renderParticles(renderer, renderQuad, obj.objType, obj.position.x + (obj.particlePos.x * obj.radius), obj.position.y + (obj.particlePos.y * obj.radius), panningOffset, obj.particlePos * obj.radius, obj.particleExplosion, timeStep, obj.particleDurationTimer.isStarted());
            if (obj.particleDurationTimer.getTicks() >= 500)
            {
                obj.particleDurationTimer.stop();
                obj.particlePos = Vector(0, 0);
            }
        }

        if ((obj.objType == "Comet" || obj.objType == "Asteroid") && !obj.ctrl && obj.toggleParticles)
        {
            obj.particles.renderParticles(renderer, renderQuad, obj.objType, obj.position.x, obj.position.y, panningOffset, obj.velocity * -1, obj.particleExplosion, timeStep, true);
        }

        if (obj.editing)
        {
            drawTrajectory(obj, panningOffset);
        }
        obj.rect = {(int)obj.position.x + (int)panningOffset.x - (int)obj.radius, (int)obj.position.y + (int)panningOffset.y - (int)obj.radius, (int)obj.radius * 2, (int)obj.radius * 2};
        obj.realRect = {(int)obj.position.x + (int)panningOffset.x - (int)obj.radius, (int)obj.position.y + (int)panningOffset.y - (int)obj.radius, (int)obj.radius * 2, (int)obj.radius * 2};
        if (obj.radius < 8)
        {
            obj.rect = {(int)obj.position.x - 8 + (int)panningOffset.x, (int)obj.position.y - 8 + (int)panningOffset.y, 16, 16};
        }

        SDL_Rect r = {(int)(obj.position.x - obj.radius), (int)(obj.position.y - obj.radius), (int)(obj.radius * 2), (int)(obj.radius * 2)};
        if (SDL_HasIntersection(renderQuad, &r))
        {
            obj.object.render(obj.position.x - obj.radius + panningOffset.x, obj.position.y - obj.radius + panningOffset.y);
        }
    }
}

void Simulation::updateAllSizes()
{
    for (CelestialObject &obj : objects)
    {
        obj.object = Animation(renderer, obj.folder, 1, 0.1);
        obj.object.multiplier = (1 / obj.object.getWidth()) * (obj.radius * 2);
        obj.object.loadFrames();
    }
}

void Simulation::displayTimeRate()
{
    string text = "1 " + Utilities::getTimeRates()[timeRate].first + "/sec";
    rateTxt.setCoords(10, dateTxt.y - rateTxt.getHeight() - 5);
    rateTxt.loadFromRenderedText(renderer, smallFont, text, textColor, 1);
    rateTxt.render(renderer);
}

void Simulation::displaySimulationDate(int height)
{
    int totalTime = dateTimer.getTicks();
    simSecs += ((totalTime - irlSecs) * Utilities::toSecs()[timeRate]);
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

void Simulation::displaySimulationStatus(bool paused, bool editing)
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

void Simulation::displayFocusedObject(string name, string objType)
{
    focusedTxt.setCoords(10, 0);
    focusedTxt.loadFromRenderedText(renderer, smallFont, "Focused on " + name + " (" + objType + ")", textColor, 1.5);
    focusedTxt.render(renderer);
}

void Simulation::displayObjectInfo(CelestialObject obj, int x, int y)
{
    std::stringstream radius;
    std::stringstream mass;
    std::stringstream velocity;
    std::stringstream kinetic;
    std::stringstream potential;
    radius << getRealDistance(obj.radius);
    mass << getRealMass(obj.mass);
    velocity << getRealDistance(obj.velocity.magnitude()) / Utilities::getTimeRates()[timeRate].second;
    kinetic << obj.kineticEnergy;
    potential << obj.potentialEnergy;

    rTxt.loadFromRenderedText(renderer, smallFont, radius.str() + " km", textColor);
    mTxt.loadFromRenderedText(renderer, smallFont, mass.str() + " km", textColor);
    vTxt.loadFromRenderedText(renderer, smallFont, velocity.str() + " km/s", textColor);
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
