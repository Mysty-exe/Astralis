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
    objectsNum = 1;
    focusedObject = -1;

    font = TTF_OpenFont("../Assets/Fonts/font.otf", 24);
    smallFont = TTF_OpenFont("../Assets/Fonts/font.otf", 16);
    tinyFont = TTF_OpenFont("../Assets/Fonts/font.otf", 12);

    irlSecs = 0;
    simSecs = 0;

    textColor = {255, 255, 255};
    paused = false;
    editing = false;
    overlapping = false;
    outOfBounds = false;
    calculatedTrajectory = false;

    loadImages(Vector(1, 1));
}

void Simulation::reset()
{
    editing = false;
    focusedObject = -1;
    timeRate = 0;
}

void Simulation::loadImages(Vector windowRatio)
{
    focusedTxtImg.loadFromRenderedText(renderer, smallFont, "Focused", textColor);
    modeTxtImg.loadFromRenderedText(renderer, smallFont, "Paused", textColor);
    dateTxtImg.loadFromRenderedText(renderer, smallFont, "Date - Unknown", textColor);
    rateTxtImg.loadFromRenderedText(renderer, smallFont, "1 sec/sec", textColor);

    radiusTxtImg.loadFromRenderedText(renderer, font, "Radius", {177, 156, 217}, windowRatio);
    massTxtImg.loadFromRenderedText(renderer, font, "Mass", {177, 156, 217}, windowRatio);
    velocityTxtImg.loadFromRenderedText(renderer, font, "Velocity", {177, 156, 217}, windowRatio);
    kineticTxtImg.loadFromRenderedText(renderer, font, "Kinetic Energy", {177, 156, 217}, windowRatio);
    potentialTxtImg.loadFromRenderedText(renderer, font, "Potential Energy", {177, 156, 217}, windowRatio);
    trajectoryImg.loadFromFile(renderer, "../Assets/Trajectory/circle.png", Vector(0.5, 0.5) * windowRatio);
}

void Simulation::resizeViewObjects(float width, float height)
{
    for (CelestialObject &obj : objects)
    {
        obj.resizeDimensions(width, height);
    }
}

string Simulation::getName()
{
    return name;
}

double Simulation::getDistRatio()
{
    return distRatio;
}

double Simulation::getSimRadius()
{
    return simRadius;
}

int Simulation::getObjectsNum()
{
    return objectsNum;
}

void Simulation::addObjectsNum()
{
    objectsNum++;
}

double Simulation::getSimSecs()
{
    return simSecs;
}

int Simulation::getTimeRate()
{
    return timeRate;
}

void Simulation::setName(string name)
{
    this->name = name;
}

void Simulation::setDistRatio(double ratio)
{
    this->distRatio = ratio;
}

void Simulation::setSimRadius(double radius)
{
    this->simRadius = radius;
}

void Simulation::setObjectsNum(int objectsNum)
{
    this->objectsNum = objectsNum;
}

void Simulation::setSimSecs(double simSecs)
{
    this->simSecs = simSecs;
}

void Simulation::pause()
{
    paused = true;
    if (!dateTimer.isPaused())
    {
        dateTimer.pause();
    }
}

void Simulation::unPause()
{
    paused = false;
    if (dateTimer.isPaused())
    {
        dateTimer.unpause();
    }
}

bool Simulation::isPaused()
{
    return paused;
}

bool Simulation::isError()
{
    return overlapping || outOfBounds;
}

bool Simulation::isEditing()
{
    return editing;
}

void Simulation::edit(bool e)
{
    editing = e;
}

void Simulation::resetErrors()
{
    overlapping = false;
    outOfBounds = false;
}

bool Simulation::isOverlapping()
{
    return overlapping;
}

void Simulation::toggleOverlapping(bool overlapping)
{
    this->overlapping = overlapping;
}

bool Simulation::isOutOfBounds()
{
    return outOfBounds;
}

void Simulation::toggleOutOfBounds(bool outOfBounds)
{
    this->outOfBounds = outOfBounds;
}

bool Simulation::trajectoryCalculated()
{
    return calculatedTrajectory;
}

void Simulation::setTrajectoryCalculated(bool calculatedTrajectory)
{
    this->calculatedTrajectory = calculatedTrajectory;
}

int Simulation::getObjectsSize()
{
    return objects.size();
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

Vector Simulation::getRealPosition(Vector position)
{
    return position * distRatio;
}

bool Simulation::isFocusing()
{
    if (focusedObject > -1 && focusedObject <= objects.size() - 1)
    {
        return true;
    }
    return false;
}

CelestialObject Simulation::getObject(int index)
{
    return objects[index];
}

CelestialObject Simulation::getFocusedObject()
{
    if (focusedObject > -1 && focusedObject <= objects.size() - 1)
    {
        return objects[focusedObject];
    }
}

vector<CelestialObject> Simulation::getObjects()
{
    return objects;
}

void Simulation::addObject(CelestialObject obj)
{
    objects.push_back(obj);
}

void Simulation::deleteObject(int index)
{
    if (index >= 0 && index < objects.size())
        objects.erase(objects.begin() + index);
}

void Simulation::scaleObjects(string n)
{
    for (CelestialObject &obj : objects)
    {
        if (obj.getName() == n)
        {
            obj.setMass(scaleMass(obj.getMass()));
            obj.setRadius(scaleDistance(obj.getRadius()));
            obj.setUpdateRadius(scaleDistance(obj.getUpdateRadius()));
            obj.setVelocity(Vector(scaleDistance(obj.getVelocity().x), scaleDistance(obj.getVelocity().y)));
            obj.updateSizeInstant(renderer);
        }
    }
}

void Simulation::updateObject(int index, string name, long double radius, long double mass, Vector velocity)
{
    if (index > -1 && index < objects.size())
    {
        objects[index].setName(renderer, name);
        objects[index].setRadius(scaleDistance(radius));
        objects[index].setUpdateRadius(scaleDistance(radius));
        objects[index].setMass(scaleMass(mass));
        objects[index].setVelocity(Vector(scaleDistance(velocity.x), scaleDistance(velocity.y)));
        objects[index].updateSizeInstant(renderer);
    }
}

void Simulation::updateObjectPosition(int index, Vector position)
{
    if (index > -1 && index < objects.size())
    {
        objects[index].setPosition(position);
    }
}

void Simulation::run(double timeStep)
{
    if (!dateTimer.isStarted())
    {
        dateTimer.start();
    }

    if (!paused)
    {
        applyForces(timeStep);
    }
    calculateEnergy();
}

void Simulation::applyForces(double timeStep)
{
    int steps = Utilities::getSubdividor()[timeRate];
    double rate = Utilities::getTimeRates()[timeRate].second;

    double dt = timeStep * rate / steps;

    int n = objects.size();

    for (int step = 0; step < steps; step++)
    {
        std::vector<Vector> acc(n, Vector(0, 0));

        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                Vector dir = objects[j].getPosition() - objects[i].getPosition();
                double dist = dir.magnitude();

                if (dist <= 0.0)
                    continue;

                Vector unit = dir / dist;

                long double r = getRealDistance(dist) * 1000.0;

                long double m1 = getRealMass(objects[i].getMass());
                long double m2 = getRealMass(objects[j].getMass());

                long double a1 = Utilities::g * m2 / (r * r);
                long double a2 = Utilities::g * m1 / (r * r);

                long double a1_px = a1 / (distRatio * 1000.0);
                long double a2_px = a2 / (distRatio * 1000.0);

                acc[i] += unit * (double)a1_px;
                acc[j] -= unit * (double)a2_px;
            }
        }

        for (int i = 0; i < n; i++)
        {
            objects[i].addVelocity(acc[i] * dt);
        }

        applyVelocities(dt);
    }
}

void Simulation::applyVelocities(double dt)
{
    int n = objects.size();

    for (int i = 0; i < n; i++)
    {
        objects[i].addPosition(objects[i].getVelocity() * dt);

        if (getRealPosition(objects[i].getPosition()).magnitude() > simRadius)
        {
            objects[i].deleteObject();
        }
    }

    for (int i = 0; i < n; i++)
    {
        if (objects[i].isMarkedForDelete())
            continue;

        for (int j = i + 1; j < n; j++)
        {
            if (objects[j].isMarkedForDelete())
                continue;

            double dist = objects[i].getPosition().distance(objects[j].getPosition());
            double rSum = objects[i].getRadius() + objects[j].getRadius();

            if (dist > rSum)
                continue;

            CelestialObject &A = objects[i];
            CelestialObject &B = objects[j];

            bool A_big =
                (A.getMass() >= B.getMass()) ||
                (A.getObjType() == STAR && B.getObjType() != STAR);

            CelestialObject &big = A_big ? A : B;
            CelestialObject &small = A_big ? B : A;

            double m1 = big.getMass();
            double m2 = small.getMass();

            Vector v1 = big.getVelocity();
            Vector v2 = small.getVelocity();

            double totalMass = m1 + m2;

            Vector newVel = (v1 * m1 + v2 * m2) / totalMass;

            big.setVelocity(newVel);
            big.setMass(totalMass);

            double r1 = big.getRadius();
            double r2 = small.getRadius();

            big.setUpdateRadius(pow(r1 * r1 * r1 + r2 * r2 * r2, 1.0 / 3.0));

            big.startParticles(
                (small.getPosition() - big.getPosition()).normalize(),
                log(getRealDistance(r2)));

            small.getObject()->freeAll();
            small.deleteObject();
        }
    }

    for (auto &obj : objects)
    {
        if (!obj.getPosition().isNumber() || !obj.getVelocity().isNumber())
        {
            obj.getObject()->freeAll();
            obj.deleteObject();
        }

        obj.updateSizeGradually(renderer, dt);
    }

    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
                       [](CelestialObject &o)
                       {
                           return o.isMarkedForDelete();
                       }),
        objects.end());
}

void Simulation::dragObject(int obj, Vector dragOffset)
{
    objects[obj].addPosition(dragOffset);
}

void Simulation::calculateEnergy()
{
    int n = objects.size();

    for (int i = 0; i < n; i++)
    {
        CelestialObject &A = objects[i];

        long double m1 = getRealMass(A.getMass());

        long double v = getRealDistance(A.getVelocity().magnitude()) * 1000;

        A.setKineticEnergy(0.5 * m1 * v * v);
        A.setPotentialEnergy(0);
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            CelestialObject &A = objects[i];
            CelestialObject &B = objects[j];

            long double m1 = getRealMass(A.getMass());
            long double m2 = getRealMass(B.getMass());

            long double dist = getRealDistance(
                                   A.getPosition().distance(B.getPosition())) *
                               1000;

            if (dist < 1e-6)
                continue;

            long double U = -Utilities::g * m1 * m2 / dist;

            A.addPotentialEnergy(U);
            B.addPotentialEnergy(U);
        }
    }
}

void Simulation::focusObject()
{
    if ((focusedObject == -1 && objects.size() > 0) || focusedObject == objects.size() - 1)
    {
        focusedObject = 0;
    }
    else if (objects.size() - 1 >= focusedObject + 1)
    {
        focusedObject += 1;
    }
    else
    {
        focusedObject = 0;
    }

    if (objects.size() == 0)
    {
        focusedObject = -1;
    }
}

void Simulation::focusObject(int index)
{
    if (index > -1 && index <= objects.size() - 1)
    {
        focusedObject = index;
    }
}

void Simulation::defocusObjects()
{
    focusedObject = -1;
}

void Simulation::speedUp()
{
    if (timeRate < Utilities::getTimeRates().size() - 1)
    {
        timeRate++;
    }
}

void Simulation::slowDown()
{
    if (timeRate > 0)
    {
        timeRate--;
    }
}

void Simulation::runTrajectoryThread(Vector panningOffset)
{
    calculatedTrajectory = false;
    std::thread worker(&Simulation::calculateTrajectory, this, panningOffset);
    worker.detach();
}

void Simulation::calculateTrajectory(Vector panningOffset)
{
    Vector pos, prevPos, vel;
    double rate;
    int counter;
    bool stop;

    for (CelestialObject &obj : objects)
    {
        rate = Utilities::getTimeRates()[timeRate].second;

        pos = obj.getPosition();
        prevPos = pos;
        vel = obj.getVelocity();

        counter = 0;
        stop = false;

        vector<Vector> positions;

        double dt = 0.01 * rate;

        for (int i = 0; i < 10000; i++)
        {
            Vector acc(0, 0);

            for (CelestialObject &other : objects)
            {
                if (&obj == &other)
                    continue;

                Vector dir = other.getPosition() - pos;
                double dist = dir.magnitude();

                if (dist < 1e-6)
                    continue;

                Vector unit = dir / dist;

                long double r = getRealDistance(dist) * 1000.0;
                long double m2 = getRealMass(other.getMass());

                long double a = Utilities::g * m2 / (r * r);
                long double a_px = a / (distRatio * 1000.0);

                acc += unit * (double)a_px;

                SDL_Point p = {(int)pos.x, (int)pos.y};
                SDL_Rect rect = other.getRect(panningOffset);

                if (SDL_PointInRect(&p, &rect))
                {
                    stop = true;
                }
            }

            vel += acc * dt;
            pos += vel * dt;

            if (!stop && (pos.distance(prevPos) >= 35 || counter == 0))
            {
                prevPos = pos;
                positions.push_back(pos);
                counter++;
            }

            if (counter >= 50 || stop)
                break;
        }

        obj.setTrajectories(positions);
    }

    calculatedTrajectory = true;
}

void Simulation::drawTrajectory(CelestialObject obj, Vector panningOffset)
{
    SDL_SetRenderDrawColor(renderer, 203, 195, 227, 100);
    for (Vector v : obj.getTrajectories())
    {
        trajectoryImg.setCoords(v.x - trajectoryImg.getWidth() / 2 + panningOffset.x, v.y - trajectoryImg.getHeight() / 2 + panningOffset.y);
        trajectoryImg.render(renderer);
    }
}

void Simulation::display(SDL_Rect *renderQuad, Vector panningOffset, double timeStep, int displayLast)
{
    vector<int> indexes = {};
    for (int i = 0; i < objects.size(); i++)
    {
        if ((displayLast != -1 && displayLast != i) || displayLast == -1)
        {
            indexes.push_back(i);
        }
    }

    if (displayLast != -1)
    {
        indexes.push_back(displayLast);
    }

    for (int i : indexes)
    {
        objects[i].setRect({(int)objects[i].getPosition().x + (int)panningOffset.x - (int)objects[i].getRadius(), (int)objects[i].getPosition().y + (int)panningOffset.y - (int)objects[i].getRadius(), (int)objects[i].getRadius() * 2, (int)objects[i].getRadius() * 2});
        objects[i].setRealRect({(int)objects[i].getPosition().x + (int)panningOffset.x - (int)objects[i].getRadius(), (int)objects[i].getPosition().y + (int)panningOffset.y - (int)objects[i].getRadius(), (int)objects[i].getRadius() * 2, (int)objects[i].getRadius() * 2});
        if (objects[i].getRadius() < 8)
        {
            objects[i].setRect({(int)objects[i].getPosition().x - 8 + (int)panningOffset.x, (int)objects[i].getPosition().y - 8 + (int)panningOffset.y, 16, 16});
        }

        SDL_Rect r = {(int)(objects[i].getPosition().x - objects[i].getRadius()), (int)(objects[i].getPosition().y - objects[i].getRadius()), (int)(objects[i].getRadius() * 2), (int)(objects[i].getRadius() * 2)};
        if (SDL_HasIntersection(renderQuad, &r))
        {
            objects[i].render(*renderQuad, panningOffset, timeStep);
        }
    }
}

void Simulation::updateAllSizes()
{
    for (CelestialObject &obj : objects)
    {
        obj.updateSizeInstant(renderer);
    }
}

void Simulation::displayTimeRate(Vector windowRatio)
{
    string text;
    if (Utilities::getTimeRates()[timeRate].first == "year")
    {
        text = "1 " + Utilities::getTimeRates()[timeRate].first + "/5 secs";
    }
    else
    {
        text = "1 " + Utilities::getTimeRates()[timeRate].first + "/sec";
    }
    rateTxtImg.setCoords(10, dateTxtImg.getY() - rateTxtImg.getHeight() - 5);
    rateTxtImg.loadFromRenderedText(renderer, smallFont, text, textColor);
    rateTxtImg.render(renderer);
}

void Simulation::displaySimulationDate(int height, Vector windowRatio)
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
    dateTxtImg.loadFromRenderedText(renderer, smallFont, text, textColor);
    dateTxtImg.setCoords(10, height - dateTxtImg.getHeight() - 10);
    dateTxtImg.render(renderer);
}

void Simulation::displayPauseStatus(Vector windowRatio)
{
    modeTxtImg.setCoords(10, 5);
    if (paused)
    {
        modeTxtImg.loadFromRenderedText(renderer, smallFont, "Paused", textColor);
        modeTxtImg.render(renderer);
    }
}

void Simulation::displayFocusedObject(Vector windowRatio)
{
    focusedTxtImg.setCoords(10, 0);
    focusedTxtImg.loadFromRenderedText(renderer, smallFont, "Focused on " + getFocusedObject().getName() + " (" + getFocusedObject().getObjType(getFocusedObject().getObjType()) + ")", textColor);
    focusedTxtImg.render(renderer);
}

int Simulation::displayObjectInfo(CelestialObject obj, Overlay overlay, Vector windowRatio)
{
    std::stringstream radiusTxt;
    std::stringstream massTxt;
    std::stringstream velocityTxt;
    std::stringstream kineticTxt;
    std::stringstream potentialTxt;
    radiusTxt << getRealDistance(obj.getRadius());
    massTxt << getRealMass(obj.getMass());
    velocityTxt << getRealDistance(obj.getVelocity().magnitude());
    kineticTxt << obj.getKineticEnergy();
    potentialTxt << obj.getPotentialEnergy();

    radiusUnitTxt.loadFromRenderedText(renderer, smallFont, radiusTxt.str() + " km", textColor);
    massUnitTxt.loadFromRenderedText(renderer, smallFont, massTxt.str() + " km", textColor);
    velocityUnitTxt.loadFromRenderedText(renderer, smallFont, velocityTxt.str() + " km/s", textColor);
    kineticUnitTxt.loadFromRenderedText(renderer, smallFont, kineticTxt.str() + " J", textColor);
    potentialUnitTxt.loadFromRenderedText(renderer, smallFont, potentialTxt.str() + " J", textColor);

    radiusTxtImg.setCoords(overlay.getPos().x + 10, overlay.getPos().y - overlay.getScroll() + 10);
    radiusUnitTxt.setCoords(overlay.getPos().x + 10, radiusTxtImg.getY() + radiusTxtImg.getHeight() + 5);
    massTxtImg.setCoords(overlay.getPos().x + 10, radiusUnitTxt.getY() + radiusUnitTxt.getHeight() + 20);
    massUnitTxt.setCoords(overlay.getPos().x + 10, massTxtImg.getY() + massTxtImg.getHeight() + 5);
    velocityTxtImg.setCoords(overlay.getPos().x + 10, massUnitTxt.getY() + massUnitTxt.getHeight() + 20);
    velocityUnitTxt.setCoords(overlay.getPos().x + 10, velocityTxtImg.getY() + velocityTxtImg.getHeight() + 5);
    kineticTxtImg.setCoords(overlay.getPos().x + 10, velocityUnitTxt.getY() + velocityUnitTxt.getHeight() + 20);
    kineticUnitTxt.setCoords(overlay.getPos().x + 10, kineticTxtImg.getY() + kineticTxtImg.getHeight() + 5);
    potentialTxtImg.setCoords(overlay.getPos().x + 10, kineticUnitTxt.getY() + kineticUnitTxt.getHeight() + 20);
    potentialUnitTxt.setCoords(overlay.getPos().x + 10, potentialTxtImg.getY() + potentialTxtImg.getHeight() + 5);

    radiusTxtImg.render(renderer);
    radiusUnitTxt.render(renderer);
    massTxtImg.render(renderer);
    massUnitTxt.render(renderer);
    velocityTxtImg.render(renderer);
    velocityUnitTxt.render(renderer);
    kineticTxtImg.render(renderer);
    kineticUnitTxt.render(renderer);
    potentialTxtImg.render(renderer);
    potentialUnitTxt.render(renderer);

    return potentialUnitTxt.getEndY();
}

int Simulation::displayDistances(CelestialObject obj, Overlay overlay, Vector windowRatio)
{
    SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);

    int startY = overlay.getPos().y;
    for (CelestialObject o : objects)
    {
        if (obj.getName() != o.getName())
        {
            distanceNameTxt.loadFromRenderedText(renderer, smallFont, o.getName() + " - (" + o.getObjType(o.getObjType()) + ")", {255, 255, 255});
            distanceTxt.loadFromRenderedText(renderer, tinyFont, to_string((int)getRealDistance(obj.getPosition().distance(o.getPosition()))) + " km", {255, 255, 255});
            distanceNameTxt.setCoords(overlay.getPos().x + 25, startY + 10 - overlay.getScroll());
            distanceTxt.setCoords(overlay.getPos().x + 30, distanceNameTxt.getRect()->y + distanceNameTxt.getRect()->h + 5);

            distanceNameTxt.render(renderer);
            distanceTxt.render(renderer);
            SDL_RenderDrawLine(renderer, overlay.getPos().x + 20, startY + 65 - overlay.getScroll(), overlay.getPos().x + overlay.getWidth() - 20, startY + 65 - overlay.getScroll());
            startY += 65;
        }
    }

    return startY;
}
