#include <SimulationHandler.h>

SimulationHandler::SimulationHandler()
{
}

SimulationHandler::SimulationHandler(SDL_Renderer *renderer, int width, int height)
{
    this->renderer = renderer;
    this->width = width;
    this->height = height;

    clickSound = Mix_LoadWAV("assets/Audio/click.wav");
    click2Sound = Mix_LoadWAV("assets/Audio/click2.wav");
    errorSound = Mix_LoadWAV("assets/Audio/errorClick.mp3");

    bigFont = TTF_OpenFont("assets/Fonts/quicksand.otf", 24);
    smallFont = TTF_OpenFont("assets/Fonts/quicksand.otf", 14);

    pause = false;
    editing = false;
    view = false;
    error = false;
    distance = false;

    pointer = false;

    sidebar = Sidebar(renderer, width, height);
    transition = Transition(renderer, width, height, 0.5);

    background = Animation(renderer, "assets/Backgrounds/Game Background", 1, 1);
    arrow.loadFromFile(renderer, "assets/Back Arrow/arrow.png", 3);
    arrowHover.loadFromFile(renderer, "assets/Back Arrow/arrowHover.png", 3);
    arrow.setCoords(20, 25);
    arrowHover.setCoords(20, 25);

    newSimulationTxt.loadFromFile(renderer, "assets/Menu Buttons/newSim.png", 2);
    newTxt.loadFromFile(renderer, "assets/newTxt.png", 1.5);
    controlsHeaderTxt.loadFromFile(renderer, "assets/controlsTxt.png", 1.5);
    controlsHeaderTxt.setCoords(arrow.x + arrow.getWidth() + 40, arrow.x + arrow.getHeight() / 2 - controlsHeaderTxt.getHeight() / 2);
    controlsTxt.loadFromFile(renderer, "assets/controls.png", 1.5);
    templateTxt.loadFromFile(renderer, "assets/templatesTxt.png", 1.5);
    loadSimulationTxt.loadFromFile(renderer, "assets/Menu Buttons/loadSim.png", 2);
    noPastSimsTxt.loadFromFile(renderer, "assets/Edit Simulation/noPastSimsTxt.png", 1);
    newSimulationTxt.setCoords(arrow.x + arrow.getWidth() + 40, arrow.x + arrow.getHeight() / 2 - newSimulationTxt.getHeight() / 2);
    loadSimulationTxt.setCoords(arrow.x + arrow.getWidth() + 40, arrow.x + arrow.getHeight() / 2 - loadSimulationTxt.getHeight() / 2);
    newSimScroll = 0;
    loadSimScroll = 0;
    editIcon.loadFromFile(renderer, "assets/editIcon.png", 0.5);
    editHoverIcon.loadFromFile(renderer, "assets/editHoverIcon.png", 0.5);

    editSimTxt.loadFromFile(renderer, "assets/Edit Simulation/editSimTxt.png", 2);
    editSimTxt.setCoords(arrow.x + arrow.getWidth() + 40, arrow.x + arrow.getHeight() / 2 - editSimTxt.getHeight() / 2);
    nameTxt.loadFromFile(renderer, "assets/Edit Simulation/nameTxt.png", 1);
    nameInput = TextInput(renderer, "", {255, 255, 255});

    distRatioTxt.loadFromFile(renderer, "assets/Edit Simulation/distRatio.png", 1);
    distRatioHelperTxt.loadFromRenderedText(renderer, smallFont, "Ex: [5000] km / pixel", {211, 211, 211});
    distRatioInput = TextInput(renderer, "", {255, 255, 255});

    simRadiusTxt.loadFromFile(renderer, "assets/Edit Simulation/simRadius.png", 1);
    simRadiusHelperTxt.loadFromRenderedText(renderer, smallFont, "Ex: 1000000 km", {211, 211, 211});
    simRadiusInput = TextInput(renderer, "", {255, 255, 255});

    confirmBtn = Button(renderer, "Confirm", {255, 255, 255}, 100);

    handCursor.loadFromFile(renderer, "assets/cursors/hand.png", 2);
    pointerCursor.loadFromFile(renderer, "assets/cursors/pointer.png", 2);

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
    panningOffset = Vector(width / 2, height / 2);
    distanceScroll = 0;
    distanceMaxScroll = 0;

    focusedObject = -1;
    currentTimeRate = 0;
    currentTemplate = -1;

    loadSimulationData();
}

void SimulationHandler::loadSimulationData()
{
    ifstream f1("data/templates.json");
    json templatesData = json::parse(f1);
    ifstream f2("data/simulations.json");
    json simulationsData = json::parse(f2);

    for (const auto &d : templatesData.items())
    {
        Simulation sim = Simulation(renderer, d.key(), templatesData[d.key()]["distRatio"], templatesData[d.key()]["simRadius"]);
        for (const auto &obj : templatesData[d.key()]["objects"].items())
        {
            json objJson = templatesData[d.key()]["objects"][obj.key()];
            CelestialObject o = CelestialObject(renderer, width, height, obj.key(), objJson["objType"], objJson["folder"], objJson["mass"], objJson["radius"], objJson["position"][0], objJson["position"][1], Vector(objJson["velocity"][0], objJson["velocity"][1]));
            sim.objects.push_back(o);
            sim.scaleObjects(obj.key());
        }
        templates.push_back(sim);
    }

    for (const auto &d : simulationsData.items())
    {
        Simulation sim = Simulation(renderer, d.key(), simulationsData[d.key()]["distRatio"], simulationsData[d.key()]["simRadius"]);
        sim.objectsNum = simulationsData[d.key()]["objectsNum"];
        sim.simSecs = simulationsData[d.key()]["seconds"];
        for (const auto &obj : simulationsData[d.key()]["objects"].items())
        {
            json objJson = simulationsData[d.key()]["objects"][obj.key()];
            CelestialObject o = CelestialObject(renderer, width, height, obj.key(), objJson["objType"], objJson["folder"], objJson["mass"], objJson["radius"], objJson["position"][0], objJson["position"][1], Vector(objJson["velocity"][0], objJson["velocity"][1]));
            o.toggleParticles = objJson["particles"];
            sim.objects.push_back(o);
            sim.scaleObjects(obj.key());
        }
        simulations.push_back(sim);
    }
}

void SimulationHandler::saveSimulationData()
{
    json simData;
    std::ofstream o("data/simulations.json");
    for (Simulation sim : simulations)
    {
        json objectsData;
        for (CelestialObject obj : sim.objects)
        {
            obj.velocity /= Utilities::getTimeRates()[currentTimeRate].second;

            objectsData[obj.name] = {
                {"objType", obj.objType},
                {"folder", obj.folder},
                {"particles", obj.toggleParticles},
                {"mass", sim.getRealMass(obj.mass)},
                {"radius", sim.getRealDistance(obj.radius)},
                {"position", {obj.position.x, obj.position.y}},
                {"velocity", {sim.getRealDistance(obj.velocity.x), sim.getRealDistance(obj.velocity.y)}}};
        }
        simData[sim.name] = {
            {"simRadius", sim.simRadius},
            {"distRatio", sim.distRatio},
            {"objectsNum", sim.objectsNum},
            {"seconds", sim.simSecs}};
        simData[sim.name]["objects"] = objectsData;
    }
    currentTimeRate = 0;
    o << std::setw(4) << simData << std::endl;
}

string SimulationHandler::controls(Events events, string state, double timeStep)
{
    pointer = false;
    SDL_Point point = events.getPoint();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    controlsHeaderTxt.render(renderer);
    SDL_RenderDrawLine(renderer, 100, controlsHeaderTxt.y + controlsHeaderTxt.getHeight() + 20, width - 100, controlsHeaderTxt.y + controlsHeaderTxt.getHeight() + 20);
    controlsTxt.setCoords(50, controlsHeaderTxt.y + controlsHeaderTxt.getHeight() + 50);
    controlsTxt.render(renderer);

    if (SDL_PointInRect(&point, arrow.getRect()))
    {
        arrowHover.render(renderer);
        pointer = true;
        if (events.leftClick && SDL_PointInRect(&events.startClickPos, arrow.getRect()))
        {
            Mix_PlayChannel(-1, clickSound, 0);
            state = "Menu";
        }
    }
    else
    {
        arrow.render(renderer);
    }

    drawCursor(events);
    SDL_RenderPresent(renderer);

    return state;
}

string SimulationHandler::newSimulation(Events events, string state, double timeStep)
{
    float newSimScrollMax;

    pointer = false;
    SDL_Point point = events.getPoint();

    if (transition.transitionState == "Out" || transition.transitionState == "Done")
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect newRect = {100, newSimulationTxt.y + newSimulationTxt.getHeight() + 20, width - 200, 70};
        SDL_SetRenderDrawColor(renderer, 203, 195, 227, 255);
        templateTxt.setCoords(100, newRect.y + 70 + 50);

        int startY = templateTxt.y + templateTxt.getHeight() + 10;
        for (int i = 0; i < templates.size(); i++)
        {
            SDL_SetRenderDrawColor(renderer, 203, 195, 227, 255);
            SDL_Rect templateRect = {100, startY - newSimScroll, width - 200, 70};
            simNameTxt.loadFromRenderedText(renderer, bigFont, templates[i].name, {255, 255, 255});
            if (SDL_PointInRect(&point, &templateRect))
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                pointer = true;
                if (events.leftClick && SDL_PointInRect(&events.startClickPos, &templateRect))
                {
                    Mix_PlayChannel(-1, clickSound, 0);
                    currentTemplate = i;
                    state = "Edit New";
                    setInputs(state);
                    break;
                }
            }
            simNameTxt.setCoords(120, startY + 70 - newSimScroll - simNameTxt.getHeight() - 10);
            SDL_RenderDrawLine(renderer, 100, startY + 70 - newSimScroll, width - 100, startY + 70 - newSimScroll);
            simNameTxt.render(renderer);
            startY += 70;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect blackRect = {0, 0, width, templateTxt.y + templateTxt.getHeight() + 10};
        SDL_RenderFillRect(renderer, &blackRect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        if (SDL_PointInRect(&point, &newRect))
        {
            SDL_SetRenderDrawColor(renderer, 203, 195, 227, 255);
            pointer = true;
            if (events.leftClick && SDL_PointInRect(&events.startClickPos, &newRect))
            {
                Mix_PlayChannel(-1, clickSound, 0);
                state = "Edit New";
            }
        }
        newTxt.setCoords(120, newRect.y + 70 - newTxt.getHeight() - 10);
        newTxt.render(renderer);
        SDL_RenderDrawLine(renderer, 100, newRect.y + 70, width - 100, newRect.y + 70);
        templateTxt.render(renderer);

        if (SDL_PointInRect(&point, arrow.getRect()))
        {
            arrowHover.render(renderer);
            pointer = true;
            if (events.leftClick && SDL_PointInRect(&events.startClickPos, arrow.getRect()))
            {
                Mix_PlayChannel(-1, clickSound, 0);
                state = "Menu";
            }
        }
        else
        {
            arrow.render(renderer);
        }

        newSimulationTxt.render(renderer);

        newSimScrollMax = startY - height - (newSimulationTxt.y + newSimulationTxt.getHeight());
        newSimScrollMax = (newSimScrollMax < 0) ? 0 : newSimScrollMax;
        float scrollableHeight = startY - (newSimulationTxt.y + newSimulationTxt.getHeight());

        newSimScroll -= events.mouseWheel.y * 1000 * timeStep;
        newSimScroll = (newSimScroll < 0) ? 0 : newSimScroll;
        newSimScroll = (newSimScroll > newSimScrollMax) ? newSimScrollMax : newSimScroll;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        float scrollableLength = height - ((float)height / scrollableHeight * height);
        float move = newSimScroll / (float)newSimScrollMax * scrollableLength;
        SDL_RenderDrawLine(renderer, width - 10, 5 + move, width - 10, 5 + (height / scrollableHeight * height) + move);

        drawCursor(events);
    }

    transition.runTransition();
    SDL_RenderPresent(renderer);

    return state;
}

string SimulationHandler::loadSimulation(Events events, string state, double timeStep)
{
    float loadSimScrollMax;

    pointer = false;
    SDL_Point point = events.getPoint();

    if (transition.transitionState == "Out" || transition.transitionState == "Done")
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        int startY = loadSimulationTxt.y + loadSimulationTxt.getHeight() + 10;
        for (int i = 0; i < simulations.size(); i++)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            simNameTxt.loadFromRenderedText(renderer, bigFont, simulations[i].name, {255, 255, 255});
            simNameTxt.setCoords(120, startY + 70 - loadSimScroll - simNameTxt.getHeight() - 10);
            editIcon.setCoords(width - 100 - editIcon.getWidth() - 10, startY + 70 - loadSimScroll - editIcon.getHeight() - 10);
            editHoverIcon.setCoords(width - 100 - editIcon.getWidth() - 10, startY + 70 - loadSimScroll - editIcon.getHeight() - 10);
            simNameTxt.render(renderer);

            SDL_Rect rect = {100, startY - loadSimScroll, width - 200, 70};

            if (SDL_PointInRect(&point, &rect))
            {
                SDL_SetRenderDrawColor(renderer, 203, 195, 227, 255);
                pointer = true;
                if (SDL_PointInRect(&point, editIcon.getRect()))
                {
                    if (events.leftClick && SDL_PointInRect(&point, editIcon.getRect()))
                    {
                        Mix_PlayChannel(-1, clickSound, 0);
                        editSimulationIndex = i;
                        state = "Edit Load";
                        setInputs(state);
                        break;
                    }
                    editHoverIcon.render(renderer);
                }
                else
                {
                    editIcon.render(renderer);
                }

                if (events.leftClick && SDL_PointInRect(&events.startClickPos, &rect) && SDL_PointInRect(&point, &rect))
                {
                    Mix_PlayChannel(-1, click2Sound, 0);
                    currentSimulation = i;
                    state = "Simulation";
                    break;
                }
            }
            SDL_RenderDrawLine(renderer, 100, startY + 70 - loadSimScroll, width - 100, startY + 70 - loadSimScroll);
            startY += 70;
        }

        if (simulations.size() == 0)
        {
            noPastSimsTxt.setCoords(100, startY + 50);
            noPastSimsTxt.render(renderer);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect rect = {0, 0, width, loadSimulationTxt.y + loadSimulationTxt.getHeight() + 5};
        SDL_RenderFillRect(renderer, &rect);

        if (SDL_PointInRect(&point, arrow.getRect()))
        {
            arrowHover.render(renderer);
            pointer = true;
            if (events.leftClick && SDL_PointInRect(&events.startClickPos, arrow.getRect()))
            {
                Mix_PlayChannel(-1, clickSound, 0);
                state = "Menu";
            }
        }
        else
        {
            arrow.render(renderer);
        }

        loadSimulationTxt.render(renderer);

        loadSimScrollMax = startY - height - (loadSimulationTxt.y + loadSimulationTxt.getHeight());
        loadSimScrollMax = (loadSimScrollMax < 0) ? 0 : loadSimScrollMax;
        float scrollableHeight = startY - (loadSimulationTxt.y + loadSimulationTxt.getHeight());

        loadSimScroll -= events.mouseWheel.y * 1000 * timeStep;
        loadSimScroll = (loadSimScroll < 0) ? 0 : loadSimScroll;
        loadSimScroll = (loadSimScroll > loadSimScrollMax) ? loadSimScrollMax : loadSimScroll;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        float scrollableLength = height - ((float)height / scrollableHeight * height);
        float move = loadSimScroll / (float)loadSimScrollMax * scrollableLength;
        SDL_RenderDrawLine(renderer, width - 10, 5 + move, width - 10, 5 + (height / scrollableHeight * height) + move);

        drawCursor(events);
    }

    transition.runTransition();
    SDL_RenderPresent(renderer);

    return state;
}

string SimulationHandler::handleEdits(Events events, pair<string, string> distRatioPair, pair<string, string> simRadiusPair, string state)
{
    if (!nameInput.error && !distRatioInput.error && !simRadiusInput.error)
    {
        Mix_PlayChannel(-1, click2Sound, 0);
        long double dist = stold(distRatioPair.first) * pow(10, stold(distRatioPair.second));
        long double radius = stold(simRadiusPair.first) * pow(10, stold(simRadiusPair.second));
        if (state.find("New") != std::string::npos)
        {
            Simulation newSimulation = Simulation(renderer, nameInput.getText(), dist, radius);
            if (currentTemplate != -1)
            {
                newSimulation.objects = templates[currentTemplate].objects;
            }
            simulations.push_back(newSimulation);
            currentSimulation = simulations.size() - 1;
            state = "Simulation";
        }
        else
        {
            simulations[editSimulationIndex].name = nameInput.getText();
            simulations[editSimulationIndex].distRatio = dist;
            simulations[editSimulationIndex].simRadius = radius;
            state = "Load";
        }
        clearInputs();
        currentTemplate = -1;
    }
    else
    {
        Mix_PlayChannel(-1, errorSound, 0);
    }

    return state;
}

string SimulationHandler::editSimulation(Events events, string state, double timeStep)
{
    float newSimScrollMax;

    pointer = false;
    SDL_Point point = events.getPoint();

    if (transition.transitionState == "Out" || transition.transitionState == "Done")
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        editSimTxt.render(renderer);

        nameTxt.setCoords(40, editSimTxt.y + editSimTxt.getHeight() + 40);
        nameInput.setCoords(55, nameTxt.y + nameTxt.getHeight() + 20);
        distRatioTxt.setCoords(40, nameInput.getRect()->y + nameInput.getRect()->h + 50);
        distRatioInput.setCoords(55, distRatioTxt.y + distRatioTxt.getHeight() + 40);
        distRatioHelperTxt.setCoords(40, distRatioInput.getRect()->y - distRatioInput.getRect()->h - 5);
        simRadiusTxt.setCoords(40, distRatioInput.getRect()->y + distRatioInput.getRect()->h + 50);
        simRadiusInput.setCoords(55, simRadiusTxt.y + simRadiusTxt.getHeight() + 40);
        simRadiusHelperTxt.setCoords(40, simRadiusInput.getRect()->y - simRadiusInput.getRect()->h - 5);
        confirmBtn.setCoords(40, simRadiusInput.getRect()->y + simRadiusInput.getRect()->h + 50);

        pair<string, string> distRatioPair = Utilities::parseInput(distRatioInput.getText());
        pair<string, string> simRadiusPair = Utilities::parseInput(simRadiusInput.getText());

        nameInput.error = false;
        if (nameInput.getText().length() == 0)
        {
            nameInput.setErrorText("Name can't be empty.");
            nameInput.error = true;
        }
        else
        {
            int counter = 0;
            for (Simulation sim : simulations)
            {
                if ((sim.name == nameInput.getText() && state != "Edit Load") || counter > 1)
                {
                    counter++;
                    nameInput.setErrorText("Name already used.");
                    nameInput.error = true;
                    break;
                }
            }
        }
        distRatioInput.error = false;
        if (!Utilities::validateMass(distRatioPair.first) || !Utilities::validateMass(distRatioPair.second))
        {
            distRatioInput.setErrorText("Invalid Distance Ratio Input.");
            distRatioInput.error = true;
        }
        else
        {
            long double dist = stold(distRatioPair.first) * pow(10, stold(distRatioPair.second));
            if (dist <= 0)
            {
                distRatioInput.setErrorText("Invalid Distance Ratio Input.");
                distRatioInput.error = true;
            }
        }

        simRadiusInput.error = false;
        if (!Utilities::validateMass(simRadiusPair.first) || !Utilities::validateMass(simRadiusPair.second))
        {
            simRadiusInput.setErrorText("Invalid Simulation Radius Input.");
            simRadiusInput.error = true;
        }
        else
        {
            if (!distRatioInput.error)
            {
                long double dist = stold(distRatioPair.first) * pow(10, stold(distRatioPair.second));
                long double radius = stold(simRadiusPair.first) * pow(10, stold(simRadiusPair.second));
                if (radius <= 0)
                {
                    simRadiusInput.setErrorText("Invalid Simulation Radius Input.");
                    simRadiusInput.error = true;
                }
                if (radius / dist > 1000000)
                {
                    simRadiusInput.setErrorText("Simulation Radius is too big.");
                    simRadiusInput.error = true;
                }
            }
        }

        nameTxt.render(renderer);
        distRatioTxt.render(renderer);
        distRatioHelperTxt.render(renderer);
        simRadiusTxt.render(renderer);
        simRadiusHelperTxt.render(renderer);
        nameInput.render(events, width);
        distRatioInput.render(events, width);
        simRadiusInput.render(events, width);
        confirmBtn.render();

        if (SDL_PointInRect(&point, nameInput.getRect()) || SDL_PointInRect(&point, distRatioInput.getRect()) || SDL_PointInRect(&point, simRadiusInput.getRect()) || SDL_PointInRect(&point, confirmBtn.getRect()))
        {
            pointer = true;
            if (events.leftClick && SDL_PointInRect(&events.startClickPos, confirmBtn.getRect()))
            {
                state = handleEdits(events, distRatioPair, simRadiusPair, state);
            }
        }

        if (SDL_PointInRect(&point, arrow.getRect()))
        {
            arrowHover.render(renderer);
            pointer = true;
            if (events.leftClick && SDL_PointInRect(&events.startClickPos, arrow.getRect()))
            {
                Mix_PlayChannel(-1, clickSound, 0);
                if (state == "Edit New")
                {
                    state = "New";
                }
                else if (state == "Edit Load")
                {
                    state = "Load";
                }
                clearInputs();
                currentTemplate = -1;
            }
        }
        else
        {
            arrow.render(renderer);
        }

        drawCursor(events);
    }

    transition.runTransition();
    SDL_RenderPresent(renderer);

    return state;
}

void SimulationHandler::setInputs(string state)
{
    std::ostringstream ssDistRatio, ssSimRadius;

    if (state == "Edit New")
    {
        ssDistRatio << templates[currentTemplate].distRatio;
        ssSimRadius << templates[currentTemplate].simRadius;
        nameInput.setText(templates[currentTemplate].name, width);
    }
    else if (state == "Edit Load")
    {
        ssDistRatio << simulations[editSimulationIndex].distRatio;
        ssSimRadius << simulations[editSimulationIndex].simRadius;
        nameInput.setText(simulations[editSimulationIndex].name, width);
    }
    distRatioInput.setText(ssDistRatio.str(), width);
    simRadiusInput.setText(ssSimRadius.str(), width);
}

void SimulationHandler::clearInputs()
{
    nameInput.error = false;
    distRatioInput.error = false;
    simRadiusInput.error = false;
    nameInput.setText("", width);
    distRatioInput.setText("", width);
    simRadiusInput.setText("", width);
}

string SimulationHandler::runSimulation(Events events, string state, double timeStep)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    SDL_Point point = {mouseX, mouseY};

    Vector currentMouse = Vector(mouseX, mouseY);

    if (transition.transitionState == "Out")
    {
        if (!simulations[currentSimulation].dateTimer.isStarted())
        {
            simulations[currentSimulation].dateTimer.start();
        }
    }

    if (transition.transitionState == "Out" || transition.transitionState == "Done")
    {
        drawBackground();
        mainScreen(events, timeStep);

        if (transition.transitionState == "Done")
        {
            eventsHandler(events);

            switch (simulations[currentSimulation].state)
            {
            case EDITING:
            {
                editScreen(events, timeStep);
                break;
            };

            case RETURN:
            {
                state = returnScreen(events, state, timeStep);
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

void SimulationHandler::resetSimulation()
{
    for (CelestialObject &obj : simulations[currentSimulation].objects)
    {
        obj.velocity /= Utilities::getTimeRates()[currentTimeRate].second;
    }
    pause = false;
    editing = false;
    view = false;
    error = false;
    distance = false;
    panningOffset = Vector(width / 2, height / 2);
    currentTimeRate = 0;
    simulations[currentSimulation].state = SIMULATION;
}

void SimulationHandler::drawCursor(Events events)
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

void SimulationHandler::drawBackground()
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

void SimulationHandler::info(Events events)
{
    SDL_Rect rect = {width - (width / 4), 0, width / 4, height * 2 / 3};
    simulations[currentSimulation].displayObjectInfo(simulations[currentSimulation].objects[focusedObject], width - (width / 4), 0, currentTimeRate);
}

void SimulationHandler::distances(Events events, double timeStep)
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

    for (CelestialObject obj : simulations[currentSimulation].objects)
    {
        if (obj.name != simulations[currentSimulation].objects[focusedObject].name)
        {
            distanceNameTxt.loadFromRenderedText(renderer, smallFont, obj.name + " - (" + obj.objType + ")", {255, 255, 255});
            distanceTxt.loadFromRenderedText(renderer, smallFont, to_string((int)simulations[currentSimulation].getRealDistance(obj.position.distance(simulations[currentSimulation].objects[focusedObject].position))) + " km", {255, 255, 255});
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

void SimulationHandler::viewState(Events events, double timeStep)
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
        if (simulations[currentSimulation].objects.size() - num >= 4)
        {
            viewStart += (width / 4);
        }
    }

    for (int i = 0; i < simulations[currentSimulation].objects.size(); i++)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);
        if (SDL_PointInRect(&point, &rect))
        {
            SDL_SetRenderDrawColor(renderer, 203, 195, 227, 50);
            pointer = true;
            pan = false;
            if (events.leftClick && SDL_PointInRect(&events.startClickPos, &rect))
            {
                Mix_PlayChannel(-1, clickSound, 0);
                focusedObject = i;
                view = false;
            }
        }

        simulations[currentSimulation].objects[i].nameTxt.setCoords(rect.x + rect.w / 2 - simulations[currentSimulation].objects[i].nameTxt.getWidth() / 2, rect.y + rect.h - simulations[currentSimulation].objects[i].nameTxt.getHeight() - 10);

        SDL_RenderFillRect(renderer, &rect);
        simulations[currentSimulation].objects[i].nameTxt.render(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);
        SDL_RenderDrawRect(renderer, &rect);
        simulations[currentSimulation].objects[i].viewObject.render(rect.x + rect.w / 2 - simulations[currentSimulation].objects[i].viewObject.getWidth() / 2, rect.y + 30);
        rect.x += (width / 4);
    }

    if (events.holdingClick && pan)
    {
        pointer = true;
        panningOffset += events.mouseOffset;
    }
}

string SimulationHandler::returnScreen(Events events, string state, double timeStep)
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
            Mix_PlayChannel(-1, click2Sound, 0);
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
            Mix_PlayChannel(-1, clickSound, 0);
            simulations[currentSimulation].state = SIMULATION;
        }
    }
    else
    {
        noTxt.render(renderer);
    }

    return state;
}

void SimulationHandler::mainScreen(Events events, double timeStep)
{
    SDL_Point point = events.getPoint();

    if (!pause && !editing && simulations[currentSimulation].state != RETURN)
    {
        if (simulations[currentSimulation].dateTimer.isPaused())
        {
            simulations[currentSimulation].dateTimer.unpause();
        }
        simulations[currentSimulation].applyForces(timeStep, currentTimeRate);
        simulations[currentSimulation].calculateEnergy();

        if (events.checkSpecialKey(ENTER) && simulations[currentSimulation].objects.size() != 0 && !view)
        {
            if (focusedObject < simulations[currentSimulation].objects.size() - 1)
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
            if (simulations[currentSimulation].objects.size() > 0)
            {
                viewStart = 0;
                viewOffset = 0;
                view = !view;
            }
        }
    }
    else
    {
        simulations[currentSimulation].dateTimer.pause();
    }

    SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);
    simulations[currentSimulation].display(panningOffset, currentTimeRate, timeStep);

    if (!pause && !editing && simulations[currentSimulation].state != RETURN)
    {
        if (view)
        {
            viewState(events, timeStep);
        }
    }

    if (!view)
    {
        simulations[currentSimulation].displaySimulationDate(currentTimeRate, height);
        simulations[currentSimulation].displayTimeRate(currentTimeRate);
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

    simulations[currentSimulation].displaySimulationStatus(pause, editing);

    if ((simulations[currentSimulation].state == SIMULATION && events.holdingClick) && focusedObject == -1 && !view && !sidebar.sliderActivated)
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
            distances(events, timeStep);
        }

        if (!pause)
        {
            simulations[currentSimulation].displayFocusedObject(simulations[currentSimulation].objects[focusedObject].name, simulations[currentSimulation].objects[focusedObject].objType);
        }

        panningOffset = Vector(-simulations[currentSimulation].objects[focusedObject].position.x, -simulations[currentSimulation].objects[focusedObject].position.y);
        panningOffset.x += width / 2;
        panningOffset.y += height / 2;
    }
}

void SimulationHandler::editScreen(Events events, double timeStep)
{
    SDL_Point point = events.getPoint();

    Vector spawnLoc = Vector(width / 2, height / 2) - panningOffset;
    sidebar.runSidebar(events, spawnLoc, simulations[currentSimulation], panningOffset, currentTimeRate, timeStep);
    if (sidebar.editState)
    {
        editing = true;
        simulations[currentSimulation].state = EDITING;
    }

    pointer = sidebar.checkHover(point, simulations[currentSimulation]);

    if (events.holdingClick && sidebar.selectedObject == -1 && !sidebar.sliderActivated)
    {
        pointer = true;
        panningOffset += events.mouseOffset;
    }

    error = false;
    for (CelestialObject obj1 : simulations[currentSimulation].objects)
    {
        SDL_Rect obj1Rect = {obj1.position.x + panningOffset.x - obj1.radius, obj1.position.y + panningOffset.y - obj1.radius, obj1.radius * 2, obj1.radius * 2};
        Vector realDist = Vector(simulations[currentSimulation].getRealDistance(obj1.position.x), simulations[currentSimulation].getRealDistance(obj1.position.y));
        if (realDist.x >= simulations[currentSimulation].simRadius || realDist.y >= simulations[currentSimulation].simRadius)
        {
            error = true;
        }
        for (CelestialObject obj2 : simulations[currentSimulation].objects)
        {
            SDL_Rect obj2Rect = {obj2.position.x + panningOffset.x - obj2.radius, obj2.position.y + panningOffset.y - obj2.radius, obj2.radius * 2, obj2.radius * 2};
            if (obj1.name != obj2.name)
            {

                if (SDL_HasIntersection(&obj1Rect, &obj2Rect))
                {
                    error = true;
                }
            }
        }
    }
}

void SimulationHandler::eventsHandler(Events events)
{
    if (events.checkSpecialKey(CTRL))
    {
        if (!pause && !error)
        {
            focusedObject = -1;
            editing = not editing;
            sidebar.reset();
            simulations[currentSimulation].state = (simulations[currentSimulation].state != SIMULATION) ? SIMULATION : EDITING;
            for (CelestialObject &obj : simulations[currentSimulation].objects)
            {
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
    else if (events.checkSpecialKey(SPACE) && simulations[currentSimulation].state != RETURN && simulations[currentSimulation].state != EDITING)
    {
        pause = not pause;
    }
    else if (events.checkSpecialKey(ESCAPE) && !pause && !editing)
    {
        simulations[currentSimulation].state = (simulations[currentSimulation].state != RETURN) ? RETURN : SIMULATION;
    }
    else if (events.checkSpecialKey(RIGHT) && !pause && simulations[currentSimulation].state != RETURN && !view)
    {
        if (currentTimeRate < Utilities::getTimeRates().size() - 1)
        {
            currentTimeRate++;
            for (CelestialObject &co : simulations[currentSimulation].objects)
            {
                co.velocity *= Utilities::getTimeMultipliers()[currentTimeRate].second;
            }
        }
    }
    else if (events.checkSpecialKey(LEFT) && !pause && simulations[currentSimulation].state != RETURN && !view)
    {
        if (currentTimeRate > 0)
        {
            for (CelestialObject &co : simulations[currentSimulation].objects)
            {
                co.velocity /= Utilities::getTimeMultipliers()[currentTimeRate].second;
            }
            currentTimeRate--;
        }
    }
}
