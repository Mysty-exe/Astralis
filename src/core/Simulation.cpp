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

    bigFont = TTF_OpenFont("assets/Fonts/font.otf", 32);
    smallFont = TTF_OpenFont("assets/Fonts/quicksand.otf", 14);

    simState = MAIN;
    pause = false;
    editing = false;
    view = false;
    error = false;
    zoomFactor = 1;
    distance = false;

    dragState = "";
    pointer = false;

    sidebar = Sidebar(renderer, width, height);
    overlay = Overlay(renderer, width, height);
    transition = Transition(renderer, width, height, 1);
    transition.transitionState = "In";

    pointerCursor.loadFromFile(renderer, "assets/cursors/pointer.png", 2);
    handCursor.loadFromFile(renderer, "assets/cursors/hand.png", 2);

    background = Animation(renderer, "assets/Backgrounds/Game Background", 1, 1);
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

    distancesTxt.loadFromRenderedText(renderer, bigFont, "Distances", {177, 156, 217});
    distanceNameTxt.loadFromRenderedText(renderer, smallFont, "Name", {177, 156, 217});
    distanceTxt.loadFromRenderedText(renderer, smallFont, "100 km", {177, 156, 217});

    viewOffset = 0;
    viewStart = 0;
    panningOffset = Vector(0, 0);
    distanceScroll = 0;
    distanceMaxScroll = 0;

    CelestialObject starA = CelestialObject(renderer, width, height, "Star", "Star", "assets/Objects/Stars/Red Star", 2 * pow(10, 30), 100000, width / 2, height / 2, Vector(0, 0));
    CelestialObject starB = CelestialObject(renderer, width, height, "Comet", "Comet", "assets/Objects/Comet", 1.63 * pow(10, 24), 8000, width / 2 + 600, height / 2, Vector(0, 45));
    objects = {starA, starB};

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
            startX -= background.getWidth();
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
            startY -= background.getHeight();
            if (startY < -panningOffset.y)
            {
                break;
            }
        }
    }

    for (int i = startX; i < (width)-panningOffset.x; i += background.getWidth())
    {
        for (int ii = startY; ii < (height)-panningOffset.y; ii += background.getHeight())
        {
            background.render(i + panningOffset.x, ii + panningOffset.y);
        }
    }
}

void Simulation::info(Events events)
{
    SDL_Rect rect = {width - (width / 4), 0, width / 4, height * 2 / 3};
    overlay.displayObjectInfo(objects[focusedObject], width - (width / 4), 0, currentTimeRate);
}

void Simulation::distances(Events events)
{
    SDL_Point point = events.getPoint();
    SDL_Rect rect = {width - (width / 4), 0, width / 4, height * 2 / 3};

    if (SDL_PointInRect(&point, &rect))
    {
        distanceScroll -= events.mouseWheel.y * 1200 * timeStep;
        distanceScroll = (distanceScroll < 0) ? 0 : distanceScroll;
        distanceScroll = (distanceScroll > distanceMaxScroll) ? distanceMaxScroll : distanceScroll;
    }

    SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);
    distancesTxt.setCoords(rect.x + 10, rect.y + 10 - distanceScroll);
    int startY = (distancesTxt.getRect()->y + distanceScroll) + distancesTxt.getRect()->h + 10;
    distancesTxt.render(renderer);

    for (CelestialObject obj : objects)
    {
        if (obj.name != objects[focusedObject].name)
        {
            distanceNameTxt.loadFromRenderedText(renderer, smallFont, obj.name + " - (" + obj.objType + ")", {255, 255, 255});
            distanceTxt.loadFromRenderedText(renderer, smallFont, to_string((int)Utilities::getRealDistance(obj.position.distance(objects[focusedObject].position))) + " km", {255, 255, 255});
            distanceNameTxt.setCoords(rect.x + 25, rect.y + startY + 10 - distanceScroll);
            distanceTxt.setCoords(rect.x + 25, distanceNameTxt.getRect()->y + distanceNameTxt.getRect()->h + 5);
            distanceNameTxt.render(renderer);
            distanceTxt.render(renderer);
            SDL_RenderDrawLine(renderer, rect.x + 20, rect.y + startY + 65 - distanceScroll, rect.x + rect.w - 20, rect.y + startY + 65 - distanceScroll);
            startY += 65;
        }
    }
    distanceMaxScroll = startY - rect.h - 65;
    distanceMaxScroll = (distanceMaxScroll < 0) ? 0 : distanceMaxScroll;
    float scrollableHeight = startY - 65 - 10;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    float scrollableLength = rect.h - ((float)rect.h / scrollableHeight * rect.h);
    float move = distanceScroll / (float)distanceMaxScroll * scrollableLength;
    SDL_RenderDrawLine(renderer, width - 10, 5 + move, width - 10, 5 + (rect.h / scrollableHeight * rect.h) + move);
}

void Simulation::viewState(Events events)
{
    SDL_Point point = events.getPoint();

    bool pan = true;
    float startY = height;
    startY -= viewOffset;
    viewOffset += 1000 * timeStep;
    if (startY < height - (width / 4))
    {
        startY = height - (width / 4);
    }

    SDL_Rect rect = {0 - viewStart, startY, width / 4, width / 4};

    if (events.checkSpecialKey(LEFT))
    {
        if (viewStart > 0)
        {
            viewStart -= (width / 4);
        }
    }
    else if (events.checkSpecialKey(RIGHT))
    {
        float num = viewStart / (width / 4) + 1;
        if (objects.size() - num >= 4)
        {
            viewStart += (width / 4);
        }
    }

    for (int i = 0; i < objects.size(); i++)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);
        if (SDL_PointInRect(&point, &rect))
        {
            SDL_SetRenderDrawColor(renderer, 203, 195, 227, 50);
            pointer = true;
            pan = false;
            if (events.leftClick && SDL_PointInRect(&events.startClickPos, &rect))
            {
                focusedObject = i;
                view = false;
            }
        }

        objects[i].nameTxt.setCoords(rect.x + rect.w / 2 - objects[i].nameTxt.getWidth() / 2, rect.y + rect.h - objects[i].nameTxt.getHeight() - 10);

        SDL_RenderFillRect(renderer, &rect);
        objects[i].nameTxt.render(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);
        SDL_RenderDrawRect(renderer, &rect);
        objects[i].viewObject.render(rect.x + rect.w / 2 - objects[i].viewObject.getWidth() / 2, rect.y + 30);
        rect.x += (width / 4);
    }

    if (events.holdingClick && pan)
    {
        pointer = true;
        panningOffset += events.mouseOffset;
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
        if (events.leftClick && SDL_PointInRect(&events.startClickPos, yesTxt.getRect()))
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
        if (events.leftClick && SDL_PointInRect(&events.startClickPos, noTxt.getRect()))
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
    timeStep = (timeStep <= 0) ? 0.001 : timeStep;
    if (!pause && !editing && simState != RETURN)
    {
        if (dateTimer.isPaused())
        {
            dateTimer.unpause();
        }
        CelestialObject::applyForces(objects, timeStep, currentTimeRate);

        if (events.checkSpecialKey(ENTER) && objects.size() != 0 && !view)
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
        if (events.checkSpecialKey(TAB))
        {
            if (objects.size() > 0)
            {
                viewStart = 0;
                viewOffset = 0;
                view = !view;
            }
        }
    }
    else
    {
        dateTimer.pause();
    }
    stepTimer.start();

    SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);
    CelestialObject::display(renderer, objects, panningOffset, currentTimeRate, timeStep);

    if (!pause && !editing && simState != RETURN)
    {
        if (view)
        {
            viewState(events);
        }
    }

    if (!view)
    {
        overlay.displaySimulationDate(currentTimeRate, dateTimer.getTicks());
        overlay.displayTimeRate(currentTimeRate);
    }

    if (editing)
    {
        if (error)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 50);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 203, 195, 227, 50);
        }
        SDL_RenderFillRect(renderer, &editingRect);
    }

    overlay.displaySimulationStatus(pause, editing);

    if ((simState == MAIN && events.holdingClick) && focusedObject == -1 && !view && !sidebar.sliderActivated)
    {
        pointer = true;
        panningOffset += events.mouseOffset;
    }
    else if (focusedObject != -1 && !sidebar.sliderActivated)
    {
        SDL_Rect rect = {width - (width / 4), 0, width / 4, height * 2 / 3};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);
        SDL_RenderDrawRect(renderer, &rect);

        if (!distance)
        {
            info(events);
        }
        else
        {
            distances(events);
        }

        if (!pause)
        {
            overlay.displayFocusedObject(objects[focusedObject].name, objects[focusedObject].objType);
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
    sidebar.runSidebar(events, spawnLoc, objects, currentTimeRate, timeStep);
    if (sidebar.editState)
    {
        editing = true;
        simState = EDITING;
    }

    pointer = sidebar.checkHover(point, objects);

    if (events.holdingClick && sidebar.selectedObject == -1 && !sidebar.sliderActivated)
    {
        pointer = true;
        panningOffset += events.mouseOffset;
    }

    error = false;
    for (CelestialObject obj1 : objects)
    {
        for (CelestialObject obj2 : objects)
        {
            if (obj1.name != obj2.name)
            {
                if (SDL_HasIntersection(obj1.getRealRect(), obj2.getRealRect()))
                {
                    error = true;
                }
            }
        }
    }
}

void Simulation::eventsHandler(Events events)
{
    if (events.checkSpecialKey(CTRL))
    {
        if (!pause && !error)
        {
            focusedObject = -1;
            editing = not editing;
            sidebar.reset();
            simState = (simState != MAIN) ? MAIN : EDITING;
            for (CelestialObject &obj : objects)
            {
                if (obj.object.frameTimer.isPaused())
                {
                    obj.object.frameTimer.unpause();
                }
                else
                {
                    obj.object.frameTimer.pause();
                }

                if (obj.ctrl)
                {
                    obj.ctrl = false;
                }
                else
                {
                    obj.ctrl = true;
                }

                obj.editing = (obj.editing) ? false : obj.editing;
            }
        }
    }
    else if (events.checkSpecialKey(SHIFT) && focusedObject != -1)
    {
        distance = !distance;
    }
    else if (events.checkSpecialKey(SPACE) && simState != RETURN && simState != EDITING)
    {
        pause = not pause;
    }
    else if (events.checkSpecialKey(ESCAPE))
    {
        simState = (simState != RETURN) ? RETURN : MAIN;
    }
    else if (events.checkSpecialKey(RIGHT) && !pause && simState != RETURN && !view)
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
    else if (events.checkSpecialKey(LEFT) && !pause && simState != RETURN && !view)
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

        drawCursor(events);
        pointer = false;
    }

    transition.runTransition();

    SDL_RenderPresent(renderer);

    return state;
}
