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
    pause = false;
    escape = false;
    addObjectsTab = false;
    zoom = 3;
    zoomFactor = 1;

    dragState = "";
    errorColor = "";

    addSidebar = Sidebar(renderer, width, height);
    overlay = Overlay(renderer, width, height);
    transition = Transition(renderer, width, height, 1, 45);
    transition.transitionState = "In";

    pointerCursor.loadFromFile(renderer, "assets/cursors/pointer.png", 4);
    handCursor.loadFromFile(renderer, "assets/cursors/hand.png", 4);

    backgroundImg.loadFromFile(renderer, "assets/Backgrounds/background.png", 1);

    returnTxt.loadFromFile(renderer, "assets/Escape/mainMenuTxt.png", 4);
    returnTxt.setCoords(width / 2 - returnTxt.getWidth() / 2, height / 8);

    yesTxt.loadFromFile(renderer, "assets/Escape/yesTxt.png", 3);
    yesTxt.setCoords(width / 2 - yesTxt.getWidth() / 2, returnTxt.y + height / 3);
    yesHoverTxt.loadFromFile(renderer, "assets/Escape/yesHoverTxt.png", 3);
    yesHoverTxt.setCoords(width / 2 - yesHoverTxt.getWidth() / 2, returnTxt.y + height / 3);

    noTxt.loadFromFile(renderer, "assets/Escape/noTxt.png", 3);
    noTxt.setCoords(width / 2 - noTxt.getWidth() / 2, yesTxt.y + height / 6);
    noHoverTxt.loadFromFile(renderer, "assets/Escape/noHoverTxt.png", 3);
    noHoverTxt.setCoords(width / 2 - noHoverTxt.getWidth() / 2, yesTxt.y + height / 6);

    mouse = Vector(0, 0);
    panningOffset = Vector(0, 0);
    zoomOffset = Vector(0, 0);

    sun = CelestialObject("Sun", STAR, 2 * pow(10, 30), 60000, width / 2, height / 2, Vector(0, 0));
    earth = CelestialObject("Earth", PLANET, 6 * pow(10, 24), 6000, width / 2 + 100, height / 2, Vector(0, -36.535));
    mars = CelestialObject("Mars", PLANET, 5 * pow(10, 23), 3000, width / 2 + 250, height / 2, Vector(0, 23.11));
    neptune = CelestialObject("Neptune", PLANET, 4 * pow(10, 26), 13000, width - 150, height / 2, Vector(0, -17.223));
    rogue = CelestialObject("Rogue", PLANET, 5 * pow(10, 14), 15000, width * 5, height / 2, Vector(0, -4.97177));
    moon = CelestialObject("Moon", MOON, 2 * pow(10, 9), 1000, (width * 5) + 75, height / 2, Vector(0, -10.671));
    objects = {sun, earth, mars};

    currentTimeRate = 0;
}

void Simulation::resetSimulation()
{
    pause = false;
    escape = false;
    zoomFactor = 1;
    transition.transitionState = "In";
    transition.getPossibleLocations();
    panningOffset = Vector(0, 0);
    currentTimeRate = 0;
}

void Simulation::drawCursor(bool holdClick)
{
    if (holdClick)
    {
        handCursor.setCoords(mouse.x, mouse.y);
        handCursor.render(renderer);
    }
    else
    {
        pointerCursor.setCoords(mouse.x, mouse.y);
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

string Simulation::escapeTab(string state, bool click)
{
    SDL_Point point = {mouse.x, mouse.y};

    SDL_Rect rect = {0, 0, width, height};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_RenderFillRect(renderer, &rect);

    returnTxt.render(renderer);

    if (SDL_PointInRect(&point, yesTxt.getRect()))
    {
        yesHoverTxt.render(renderer);
        if (click)
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
        noHoverTxt.render(renderer);
        if (click)
        {
            escape = false;
        }
    }
    else
    {
        noTxt.render(renderer);
    }

    return state;
}

void Simulation::dragHandler(Vector mouseWheel, bool holdClick)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    SDL_Point point = {mouseX, mouseY};

    Vector currentMouse = Vector(mouseX, mouseY);

    if (holdClick && !SDL_PointInRect(&point, addSidebar.getRect()) && dragState != "Sidebar")
    {
        dragState = "Simulation";
    }
    else if (addObjectsTab && SDL_PointInRect(&point, addSidebar.getRect()) && holdClick && dragState != "Simulation")
    {
        dragState = "Sidebar";
    }
    else if (!holdClick)
    {
        dragState = "";
    }

    if (holdClick)
    {
        if (dragState == "Simulation")
        {
            panningOffset += currentMouse - mouse;
        }
        else if (dragState == "Sidebar")
        {
            addSidebar.scroll -= (currentMouse - mouse).y;
            addSidebar.scroll = (addSidebar.scroll < 0) ? 0 : addSidebar.scroll;
            addSidebar.scroll = (addSidebar.scroll > addSidebar.maxScroll) ? addSidebar.maxScroll : addSidebar.scroll;
        }
    }
    mouse = currentMouse;
}

void Simulation::zoomHandler(Vector mouseWheel)
{
    if (mouseWheel.y < 0)
    {
        if (zoom > 0)
        {
            zoom -= 1;
        }
        else
        {
            return;
        }
    }
    else if (mouseWheel.y > 0)
    {
        if (zoom < 7)
        {
            zoom += 1;
        }
        else
        {
            return;
        }
    }

    if (mouseWheel.y != 0)
    {
        pair<Vector, Vector> zoomInfo = Utilities::getZoomScales(width, height)[zoom];

        SDL_RenderSetLogicalSize(renderer, zoomInfo.first.x, zoomInfo.first.y);
        zoomOffset.x += (mouseWheel.y / abs(mouseWheel.y)) * zoomInfo.second.x;
        zoomOffset.y += (mouseWheel.y / abs(mouseWheel.y)) * zoomInfo.second.y;
    }
}

void Simulation::eventsHandler(vector<string> events)
{
    for (string event : events)
    {
        if (event == "tab")
        {
            addSidebar.state = "Objects";
            addObjectsTab = not addObjectsTab;
        }
        else if (event == "space")
        {
            pause = not pause;
        }
        else if (event == "escape")
        {
            addObjectsTab = false;
            escape = not escape;
        }
        else if (event == "ctrl")
        {
            panningOffset = Vector(0, 0);
        }
        else if (event == "right")
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
        else if (event == "left")
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
}

string Simulation::runSimulation(string state, vector<string> events, bool click, bool holdClick, Vector mouseWheel)
{
    if (transition.transitionState != "In")
    {
        drawBackground();

        // if (errorColor == "Red")
        // {
        //     SDL_Rect rect = {width / 3, 0, width, height};
        //     SDL_SetRenderDrawColor(renderer, 255, 0, 0, 50);
        //     SDL_RenderFillRect(renderer, &rect);
        // }
        // else if (errorColor == "Red")
        // {
        //     SDL_Rect rect = {width / 3, 0, width, height};
        //     SDL_SetRenderDrawColor(renderer, 0, 255, 0, 50);
        //     SDL_RenderFillRect(renderer, &rect);
        // }

        dragHandler(mouseWheel, holdClick);
        // zoomHandler(mouseWheel);
        eventsHandler(events);

        SDL_Point point = {mouse.x, mouse.y};
        for (Frame frame : addSidebar.getFrames())
        {
            if (click && SDL_PointInRect(&point, frame.getRect(addSidebar.scroll)))
            {
                errorColor = "Red";
                addSidebar.state = "Edit";
            }
        }

        SDL_SetRenderDrawColor(renderer, 123, 12, 255, 255);

        double timeStep = stepTimer.getTicks() / 1000.0f;
        if (!pause && !escape)
        {
            if (dateTimer.isPaused())
            {
                dateTimer.unpause();
            }

            CelestialObject::applyForces(objects, timeStep, currentTimeRate);
        }
        else
        {
            dateTimer.pause();
        }
        stepTimer.start();

        CelestialObject::display(renderer, objects, panningOffset, zoomOffset);

        overlay.displaySimulationStatus(pause, zoom);
        overlay.displaySimulationDate(currentTimeRate, dateTimer.getTicks(), zoom);
        overlay.displayTimeRate(currentTimeRate, zoom);

        if (escape)
        {
            state = escapeTab(state, click);
        }

        if (addObjectsTab)
        {
            addSidebar.drawSidebar();
        }

        if (!addObjectsTab || addSidebar.state != "Edit")
        {
            errorColor = "";
        }
    }
    if (transition.transitionState == "Out")
    {
        if (!dateTimer.isStarted())
        {
            dateTimer.start();
            stepTimer.start();
        }
    }

    drawCursor(holdClick);
    transition.runTransition();

    SDL_RenderPresent(renderer);

    return state;
}
