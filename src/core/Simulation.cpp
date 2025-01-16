#include <Simulation.h>

Simulation::Simulation()
{
}

Simulation::Simulation(SDL_Renderer *renderer, int width, int height)
{
    this->renderer = renderer;
    this->width = width;
    this->height = height;
    gameWidth = width;
    gameHeight = height;
    simState = MAIN;
    pause = false;
    editing = false;
    zoom = 3;
    zoomFactor = 1;

    dragState = "";
    errorColor = "";
    pointer = false;

    sidebar = Sidebar(renderer, width, height);
    overlay = Overlay(renderer, width, height);
    transition = Transition(renderer, width, height, 1);
    transition.transitionState = "In";

    pointerCursor.loadFromFile(renderer, "assets/cursors/pointer.png", 2);
    handCursor.loadFromFile(renderer, "assets/cursors/hand.png", 2);

    backgroundImg.loadFromFile(renderer, "assets/Backgrounds/background.png", 1);
    editingRect = {0, 0, width, height};

    returnTxt.loadFromFile(renderer, "assets/Escape/mainMenuTxt.png", 2);
    returnTxt.setCoords(width / 2 - returnTxt.getWidth() / 2, height / 8);

    yesTxt.loadFromFile(renderer, "assets/Escape/yesTxt.png", 1.5);
    yesTxt.setCoords(width / 2 - yesTxt.getWidth() / 2, returnTxt.y + height / 3);
    yesHoverTxt.loadFromFile(renderer, "assets/Escape/yesHoverTxt.png", 1.5);
    yesHoverTxt.setCoords(width / 2 - yesHoverTxt.getWidth() / 2, returnTxt.y + height / 3);

    noTxt.loadFromFile(renderer, "assets/Escape/noTxt.png", 1.5);
    noTxt.setCoords(width / 2 - noTxt.getWidth() / 2, yesTxt.y + height / 6);
    noHoverTxt.loadFromFile(renderer, "assets/Escape/noHoverTxt.png", 1.5);
    noHoverTxt.setCoords(width / 2 - noHoverTxt.getWidth() / 2, yesTxt.y + height / 6);

    panningOffset = Vector(0, 0);
    zoomOffset = Vector(0, 0);

    sun = CelestialObject(renderer, "Sun", "Star", 2 * pow(10, 30), 60000, width / 2, height / 2, Vector(0, 0));
    earth = CelestialObject(renderer, "Earth", "Planet", 6 * pow(10, 24), 6000, width / 2 + 100, height / 2, Vector(0, -36.535));
    mars = CelestialObject(renderer, "Mars", "Planet", 5 * pow(10, 23), 3000, width / 2 + 250, height / 2, Vector(0, 23.11));
    objects = {sun, earth, mars};

    timeStep = 0;
    focusedObject = -1;
    currentTimeRate = 0;
}

void Simulation::resetSimulation()
{
    simState = MAIN;
    zoomFactor = 1;
    transition.transitionState = "In";
    transition.getPossibleLocations();
    panningOffset = Vector(0, 0);
    currentTimeRate = 0;
}

void Simulation::drawCursor(Events events)
{
    if (pointer)
    {
        handCursor.setCoords(events.mousePos.x - 4, events.mousePos.y);
        handCursor.render(renderer);
    }
    else
    {
        pointerCursor.setCoords(events.mousePos.x, events.mousePos.y);
        pointerCursor.render(renderer);
    }
}

void Simulation::drawBackground()
{
    int startX = 0;
    if (-panningOffset.x < 0)
    {
        while (true)
        {
            startX -= backgroundImg.getWidth();
            if (startX < -panningOffset.x)
            {
                break;
            }
        }
    }

    int startY = 0;
    if (-panningOffset.y < 0)
    {
        while (true)
        {
            startY -= backgroundImg.getHeight();
            if (startY < -panningOffset.y)
            {
                break;
            }
        }
    }

    for (int i = startX; i < (width)-panningOffset.x; i += backgroundImg.getWidth())
    {
        for (int ii = startY; ii < (height)-panningOffset.y; ii += backgroundImg.getHeight())
        {
            backgroundImg.setCoords(i + panningOffset.x, ii + panningOffset.y);
            backgroundImg.render(renderer);
        }
    }
}

string Simulation::returnScreen(Events events, string state)
{
    SDL_Point point = events.getPoint();

    SDL_Rect rect = {0, 0, width, height};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_RenderFillRect(renderer, &rect);

    returnTxt.render(renderer);

    if (SDL_PointInRect(&point, yesTxt.getRect()))
    {
        pointer = true;
        yesHoverTxt.render(renderer);
        if (events.leftClick)
        {
            resetSimulation();
            state = "Menu";
        }
    }
    else
    {
        yesTxt.render(renderer);
    }

    if (SDL_PointInRect(&point, noTxt.getRect()))
    {
        pointer = true;
        noHoverTxt.render(renderer);
        if (events.leftClick)
        {
            simState = MAIN;
        }
    }
    else
    {
        noTxt.render(renderer);
    }

    return state;
}

void Simulation::mainScreen(Events events)
{
    SDL_Point point = events.getPoint();

    timeStep = stepTimer.getTicks() / 1000.0f;
    if (!pause && !editing && simState != RETURN)
    {
        if (dateTimer.isPaused())
        {
            dateTimer.unpause();
        }
        CelestialObject::applyForces(objects, timeStep, currentTimeRate);

        if (events.checkSpecialKey(ENTER) && objects.size() != 0)
        {
            if (focusedObject < objects.size() - 1)
            {
                focusedObject += 1;
            }
            else
            {
                focusedObject = 0;
            }
        }
        if (events.checkSpecialKey(BACKSPACE))
        {
            focusedObject = -1;
        }
    }
    else
    {
        dateTimer.pause();
    }
    stepTimer.start();

    SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);
    CelestialObject::display(renderer, objects, panningOffset, zoomOffset);

    overlay.displaySimulationDate(currentTimeRate, dateTimer.getTicks(), zoom);
    overlay.displayTimeRate(currentTimeRate, zoom);

    if (editing)
    {
        SDL_SetRenderDrawColor(renderer, 203, 195, 227, 50);
        SDL_RenderFillRect(renderer, &editingRect);
    }

    overlay.displaySimulationStatus(pause, editing, zoom);

    if ((simState == MAIN && events.holdingClick) && focusedObject == -1)
    {
        pointer = true;
        panningOffset += events.mouseOffset;
    }
    else if (focusedObject != -1)
    {
        if (!pause)
        {
            overlay.displayFocusedObject(objects[focusedObject].name, objects[focusedObject].objType, zoom);
        }
        panningOffset = Vector(-objects[focusedObject].position.x, -objects[focusedObject].position.y);
        panningOffset.x += width / 2;
        panningOffset.y += height / 2;
    }
}

void Simulation::editScreen(Events events)
{
    SDL_Point point = events.getPoint();

    Vector spawnLoc = Vector(width / 2, height / 2) - panningOffset;
    sidebar.runSidebar(events, spawnLoc, objects, timeStep);
    if (sidebar.editState)
    {
        editing = true;
        simState = EDITING;
    }

    pointer = sidebar.checkHover(point, objects);

    if (events.holdingClick && sidebar.selectedObject == -1)
    {
        pointer = true;
        panningOffset += events.mouseOffset;
    }
}

void Simulation::eventsHandler(Events events)
{
    if (events.checkSpecialKey(CTRL))
    {
        if (!pause)
        {
            focusedObject = -1;
            editing = not editing;
            sidebar.reset();
            simState = (simState != MAIN) ? MAIN : EDITING;
        }
    }
    else if (events.checkSpecialKey(SPACE) && simState != RETURN && simState != EDITING)
    {
        pause = not pause;
    }
    else if (events.checkSpecialKey(ESCAPE))
    {
        simState = (simState != RETURN) ? RETURN : MAIN;
    }
    else if (events.checkSpecialKey(RIGHT) && !pause && !editing && simState != RETURN)
    {
        if (currentTimeRate < Utilities::getTimeRates().size() - 1)
        {
            currentTimeRate++;
            for (CelestialObject &co : objects)
            {
                co.velocity *= Utilities::getTimeMultipliers()[currentTimeRate].second;
            }
        }
    }
    else if (events.checkSpecialKey(LEFT) && !pause && !editing && simState != RETURN)
    {
        if (currentTimeRate > 0)
        {
            for (CelestialObject &co : objects)
            {
                co.velocity /= Utilities::getTimeMultipliers()[currentTimeRate].second;
            }
            currentTimeRate--;
        }
    }
}

string Simulation::runSimulation(Events events, string state)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    SDL_Point point = {mouseX, mouseY};

    Vector currentMouse = Vector(mouseX, mouseY);

    if (transition.transitionState == "Out")
    {
        if (!dateTimer.isStarted())
        {
            dateTimer.start();
            stepTimer.start();
        }
    }

    if (transition.transitionState == "Out" || transition.transitionState == "Done")
    {
        drawBackground();
        mainScreen(events);

        if (transition.transitionState == "Done")
        {
            eventsHandler(events);

            switch (simState)
            {
            case EDITING:
            {
                editScreen(events);
                break;
            };

            case RETURN:
            {
                state = returnScreen(events, state);
                break;
            }
            }
        }
    }

    transition.runTransition();

    drawCursor(events);

    pointer = false;

    SDL_RenderPresent(renderer);

    return state;
}
