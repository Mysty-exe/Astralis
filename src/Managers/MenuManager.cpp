#include <MenuManager.h>

MenuManager::MenuManager()
{
}

MenuManager::MenuManager(SDL_Renderer *renderer, int width, int height) : Manager(renderer, width, height)
{
    currentState = MAIN;
    loadImages();

    newScrollbar = Scrollbar(width, height);
    loadScrollbar = Scrollbar(width, height);

    form = Form(renderer, width, height);
    form.setPos(editSimulationTxt.getX() + 10, editSimulationTxt.getEndY() + 50);
    Input *name = new Input(renderer, "Simulation Name", "", "Name of this Simulation", "", 128, 50);
    Input *distRatio = new Input(renderer, "Distance Ratio", "", "Ratio between pixel to kilometres", "", 128, 50);
    Input *simRadius = new Input(renderer, "Simulation Radius", "", "Radius of the Simulation", "", 128, 50);
    Button button = Button(renderer, "Edit");
    vector<Input *> inputs = {name, distRatio, simRadius};
    form.setInputs(inputs);
    form.setSubmitButton(button);
    currentTemplate = -1;
}

void MenuManager::loadSimulationData(vector<Simulation> *simulations)
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
            sim.addObject(o);
            sim.scaleObjects(obj.key());
        }
        templates.push_back(sim);
    }

    for (const auto &d : simulationsData.items())
    {
        Simulation sim = Simulation(renderer, d.key(), simulationsData[d.key()]["distRatio"], simulationsData[d.key()]["simRadius"]);
        sim.setObjectsNum(simulationsData[d.key()]["objectsNum"]);
        sim.setSimSecs(simulationsData[d.key()]["seconds"]);
        for (const auto &obj : simulationsData[d.key()]["objects"].items())
        {
            json objJson = simulationsData[d.key()]["objects"][obj.key()];
            CelestialObject o = CelestialObject(renderer, width, height, obj.key(), objJson["objType"], objJson["folder"], objJson["mass"], objJson["radius"], objJson["position"][0], objJson["position"][1], Vector(objJson["velocity"][0], objJson["velocity"][1]));
            sim.addObject(o);
            sim.scaleObjects(obj.key());
        }
        simulations->push_back(sim);
    }
}

void MenuManager::saveSimulationData(vector<Simulation> *simulations)
{
    json simData;
    std::ofstream o("data/simulations.json");
    for (Simulation sim : *simulations)
    {
        json objectsData;
        for (CelestialObject obj : sim.getObjects())
        {
            obj.setVelocity(obj.getVelocity() / Utilities::getTimeRates()[sim.getTimeRate()].second);

            objectsData[obj.getName()] = {
                {"objType", obj.getObjType(obj.getObjType())},
                {"folder", obj.getFolder()},
                {"mass", sim.getRealMass(obj.getMass())},
                {"radius", sim.getRealDistance(obj.getRadius())},
                {"position", {obj.getPosition().x, obj.getPosition().y}},
                {"velocity", {sim.getRealDistance(obj.getVelocity().x), sim.getRealDistance(obj.getVelocity().y)}}};
        }

        simData[sim.getName()] = {
            {"simRadius", sim.getSimRadius()},
            {"distRatio", sim.getDistRatio()},
            {"objectsNum", sim.getObjectsNum()},
            {"seconds", sim.getSimSecs()}};

        simData[sim.getName()]["objects"] = objectsData;
    }
    o << std::setw(4) << simData << std::endl;
}

void MenuManager::loadImages()
{
    titleTxtAnimation.loadAnimation(renderer, "Assets/Title Animation", getWindowRatio(), 0.1);
    backgroundImg.loadFromFile(renderer, "Assets/Backgrounds/Menu Background/menuBackground.png", getWindowRatio());

    newTxt.loadFromFile(renderer, "Assets/Menu Buttons/Regular/new.png", getWindowRatio());
    newTxt.setCoords(width / 2 - newTxt.getWidth() / 2, height / 3);
    newHoverTxt.loadFromFile(renderer, "Assets/Menu Buttons/Hovering/new.png", getWindowRatio());
    newHoverTxt.setCoords(width / 2 - newTxt.getWidth() / 2, height / 3);

    loadTxt.loadFromFile(renderer, "Assets/Menu Buttons/Regular/load.png", getWindowRatio());
    loadTxt.setCoords(width / 2 - loadTxt.getWidth() / 2, newTxt.getY() + height / 6);
    loadHoverTxt.loadFromFile(renderer, "Assets/Menu Buttons/Hovering/load.png", getWindowRatio());
    loadHoverTxt.setCoords(width / 2 - loadTxt.getWidth() / 2, newTxt.getY() + height / 6);

    controlsTxt.loadFromFile(renderer, "Assets/Menu Buttons/Regular/controls.png", getWindowRatio());
    controlsTxt.setCoords(width / 2 - controlsTxt.getWidth() / 2, loadTxt.getY() + height / 6);
    controlsHoverTxt.loadFromFile(renderer, "Assets/Menu Buttons/Hovering/controls.png", getWindowRatio());
    controlsHoverTxt.setCoords(width / 2 - controlsTxt.getWidth() / 2, loadTxt.getY() + height / 6);

    quitTxt.loadFromFile(renderer, "Assets/Menu Buttons/Regular/quit.png", getWindowRatio());
    quitTxt.setCoords(width / 2 - quitTxt.getWidth() / 2, controlsTxt.getY() + height / 6);
    quitHoverTxt.loadFromFile(renderer, "Assets/Menu Buttons/Hovering/quit.png", getWindowRatio());
    quitHoverTxt.setCoords(width / 2 - quitTxt.getWidth() / 2, controlsTxt.getY() + height / 6);

    arrow.loadFromFile(renderer, "Assets/Back Arrow/arrow.png", getWindowRatio());
    arrowHover.loadFromFile(renderer, "Assets/Back Arrow/arrowHover.png", getWindowRatio());
    arrow.setCoords(20, 25);
    arrowHover.setCoords(20, 25);

    newSimulationTxt.loadFromFile(renderer, "Assets/Menu Headers/newSimHeader.png", getWindowRatio());
    newSimulationTxt.setCoords(arrow.getEndX() + 40, arrow.getMiddleY() - newSimulationTxt.getHeight() / 2);

    newHeaderTxt.loadFromRenderedText(renderer, largeFont, "New", {255, 255, 255}, getWindowRatio());
    templateHeaderTxt.loadFromFile(renderer, "assets/Menu Headers/templatesHeader.png", getWindowRatio() * 1.5);

    loadSimulationTxt.loadFromFile(renderer, "Assets/Menu Headers/loadHeader.png", getWindowRatio());
    loadSimulationTxt.setCoords(arrow.getEndX() + 40, arrow.getMiddleY() - loadSimulationTxt.getHeight() / 2);

    editSimulationTxt.loadFromFile(renderer, "Assets/Menu Headers/editSimHeader.png", getWindowRatio());
    editSimulationTxt.setCoords(arrow.getEndX() + 40, arrow.getMiddleY() - editSimulationTxt.getHeight() / 2);

    noPastSimsTxt.loadFromFile(renderer, "Assets/Menu Headers/noPastSimsHeader.png", getWindowRatio());

    controlsHeaderTxt.loadFromFile(renderer, "assets/Menu Headers/controlsHeader.png", getWindowRatio());
    controlsHeaderTxt.setCoords(arrow.getEndX() + 40, arrow.getMiddleY() - controlsHeaderTxt.getHeight() / 2);
    allControlsTxt.loadFromFile(renderer, "assets/Controls Menu/controls.png", getWindowRatio());

    editIcon.loadFromFile(renderer, "Assets/Icons/editIcon.png", getWindowRatio());
    editHoverIcon.loadFromFile(renderer, "Assets/Icons/editHoverIcon.png", getWindowRatio());
    binIcon.loadFromFile(renderer, "Assets/Icons/binIcon.png", getWindowRatio());
    binHoverIcon.loadFromFile(renderer, "Assets/Icons/binHoverIcon.png", getWindowRatio());
}

MenuState MenuManager::getCurrentState()
{
    return currentState;
}

void MenuManager::setWindowSize(int width, int height)
{
    this->width = width;
    this->height = height;
    loadImages();
    form.updateSize(width, height);
}

int MenuManager::runMainMenu(EventsManager eventManager, double timeStep)
{
    hovering = false;
    SDL_Point point = eventManager.getPoint();
    SDL_Point startClickPoint = eventManager.getStartClickPos();

    backgroundImg.render(renderer);

    titleTxtAnimation.render(renderer, width / 2 - titleTxtAnimation.getWidth() / 2, height / 12);

    if (SDL_PointInRect(&point, newTxt.getRect()))
    {
        newHoverTxt.render(renderer);
        if (eventManager.isLeftClick() && SDL_PointInRect(&startClickPoint, newTxt.getRect()))
        {
            playClickSound();
            currentState = NEW;
        }
        hovering = true;
    }
    else
    {
        newTxt.render(renderer);
    }

    if (SDL_PointInRect(&point, loadTxt.getRect()))
    {
        loadHoverTxt.render(renderer);
        if (eventManager.isLeftClick() && SDL_PointInRect(&startClickPoint, loadTxt.getRect()))
        {
            playClickSound();
            currentState = LOAD;
        }
        hovering = true;
    }
    else
    {
        loadTxt.render(renderer);
    }

    if (SDL_PointInRect(&point, controlsTxt.getRect()))
    {
        controlsHoverTxt.render(renderer);
        if (eventManager.isLeftClick() && SDL_PointInRect(&startClickPoint, controlsTxt.getRect()))
        {
            playClickSound();
            currentState = CONTROLS;
        }
        hovering = true;
    }
    else
    {
        controlsTxt.render(renderer);
    }

    if (SDL_PointInRect(&point, quitTxt.getRect()))
    {
        quitHoverTxt.render(renderer);
        if (eventManager.isLeftClick() && SDL_PointInRect(&startClickPoint, quitTxt.getRect()))
        {
            playClickSound();
            return -1;
        }
        hovering = true;
    }
    else
    {
        quitTxt.render(renderer);
    }

    drawCursor(eventManager.getMousePos(), hovering);
    return 0;
}

int MenuManager::runNewSim(EventsManager eventManager, vector<Simulation> *simulations, double timeStep)
{
    hovering = false;
    SDL_Point point = eventManager.getPoint();
    SDL_Point startClickPos = eventManager.getStartClickPos();

    setColor(PURPLE);
    int y = templateHeaderTxt.getEndY() + 10;
    for (int i = 0; i < templates.size(); i++)
    {
        setColor(PURPLE);
        SDL_Rect templateRect = {100, (int)(y - newScrollbar.getScroll()), (int)width - 200, 70};
        simNameTxt.loadFromRenderedText(renderer, bigFont, templates[i].getName(), {255, 255, 255});
        if (SDL_PointInRect(&point, &templateRect))
        {
            setColor(WHITE);
            hovering = true;
            if (eventManager.isLeftClick() && SDL_PointInRect(&startClickPos, &templateRect))
            {
                playClickSound();
                form.setButtonText("Create");
                currentState = NEWEDIT;
                currentTemplate = i;
                setEditInputs(simulations, currentTemplate);
                break;
            }
        }

        simNameTxt.setCoords(120, y + 70 - newScrollbar.getScroll() - simNameTxt.getHeight() - 10);
        simNameTxt.render(renderer);

        SDL_RenderDrawLine(renderer, 100, y + 70 - newScrollbar.getScroll(), width - 100, y + 70 - newScrollbar.getScroll());
        y += 70;
    }

    setColor(BLACK);
    SDL_Rect blackRect = {0, 0, (int)width, (int)(templateHeaderTxt.getEndY() + 10)};
    SDL_RenderFillRect(renderer, &blackRect);

    SDL_Rect newRect = {100, (int)(newSimulationTxt.getEndY() + 20), (int)width - 200, 70};
    if (SDL_PointInRect(&point, &newRect))
    {
        setColor(WHITE);
        hovering = true;
        if (eventManager.isLeftClick() && SDL_PointInRect(&startClickPos, &newRect))
        {
            playClickSound();
            form.setButtonText("Create");
            currentState = NEWEDIT;
            currentTemplate = -1;
        }
    }
    else
    {
        setColor(PURPLE);
    }

    newHeaderTxt.setCoords(120, newRect.y + 70 - newHeaderTxt.getHeight());
    newHeaderTxt.render(renderer);

    SDL_RenderDrawLine(renderer, 100, newRect.y + 70, width - 100, newRect.y + 70);

    templateHeaderTxt.setCoords(100, newRect.y + 70 + 50);
    templateHeaderTxt.render(renderer);

    if (SDL_PointInRect(&point, arrow.getRect()))
    {
        arrowHover.render(renderer);
        hovering = true;
        if (eventManager.isLeftClick() && SDL_PointInRect(&startClickPos, arrow.getRect()))
        {
            playClickSound();
            currentState = MAIN;
        }
    }
    else
    {
        arrow.render(renderer);
    }

    newSimulationTxt.render(renderer);

    setColor(WHITE);
    newScrollbar.setDimensions(width, height - (templateHeaderTxt.getEndY() + 10), y - (templateHeaderTxt.getEndY() + 10) + 20);
    newScrollbar.scroll(eventManager.getMouseWheel().y * 10);
    newScrollbar.display(renderer, Vector(width - 75, templateHeaderTxt.getEndY() + 10));

    drawCursor(eventManager.getMousePos(), hovering);
    return 0;
}

int MenuManager::runLoadSim(EventsManager eventManager, vector<Simulation> *simulations, int &simIndex, double timeStep)
{
    hovering = false;
    SDL_Point point = eventManager.getPoint();
    SDL_Point startClickPos = eventManager.getStartClickPos();

    int y = loadSimulationTxt.getY() + loadSimulationTxt.getHeight() + 10;
    int deleteIndex = -1;
    for (int i = 0; i < simulations->size(); i++)
    {
        SDL_Rect rect = {100, (int)(y - loadScrollbar.getScroll()), (int)width - 200, 70};

        if (SDL_PointInRect(&point, &rect))
        {
            setColor(PURPLE);
            hovering = true;
            if (SDL_PointInRect(&point, editIcon.getRect()))
            {
                if (eventManager.isLeftClick())
                {
                    form.setButtonText("Edit");
                    playClickSound();
                    currentTemplate = -1;
                    currentState = LOADEDIT;
                    simIndex = i;
                    setEditInputs(simulations, simIndex);
                    break;
                }
                editHoverIcon.render(renderer);
            }
            else
            {
                editIcon.render(renderer);
            }

            if (SDL_PointInRect(&point, binIcon.getRect()))
            {
                if (eventManager.isLeftClick())
                {
                    deleteIndex = i;
                    playClickSound();
                    continue;
                }
                binHoverIcon.render(renderer);
            }
            else
            {
                binIcon.render(renderer);
            }

            if (eventManager.isLeftClick() && SDL_PointInRect(&startClickPos, &rect) && SDL_PointInRect(&point, &rect) && !SDL_PointInRect(&point, binIcon.getRect()))
            {
                currentTemplate = -1;
                playClickSound();
                currentState = MAIN;
                simIndex = i;
                return 1;
            }
        }
        else
        {
            setColor(WHITE);
        }

        simNameTxt.loadFromRenderedText(renderer, bigFont, simulations->at(i).getName(), {255, 255, 255});
        simNameTxt.setCoords(120, y + 70 - loadScrollbar.getScroll() - simNameTxt.getHeight() - 10);
        simNameTxt.render(renderer);

        binIcon.setCoords(width - 100 - binIcon.getWidth() - 10, y + 70 - loadScrollbar.getScroll() - binIcon.getHeight() - 10);
        binHoverIcon.setCoords(width - 100 - binIcon.getWidth() - 10, y + 70 - loadScrollbar.getScroll() - binIcon.getHeight() - 10);

        editIcon.setCoords(binIcon.getX() - editIcon.getWidth() - 10, y + 70 - loadScrollbar.getScroll() - editIcon.getHeight() - 10);
        editHoverIcon.setCoords(binIcon.getX() - editIcon.getWidth() - 10, y + 70 - loadScrollbar.getScroll() - editIcon.getHeight() - 10);

        SDL_RenderDrawLine(renderer, 100, y + 70 - loadScrollbar.getScroll(), width - 100, y + 70 - loadScrollbar.getScroll());
        y += 70;
    }

    noPastSimsTxt.setCoords(100, y + 50);

    if (deleteIndex != -1)
    {
        simulations->erase(simulations->begin() + deleteIndex);
        deleteIndex = -1;
    }

    if (simulations->size() == 0)
    {
        noPastSimsTxt.render(renderer);
    }

    setColor(BLACK);
    SDL_Rect rect = {0, 0, (int)width, (int)(loadSimulationTxt.getEndY() + 10)};
    SDL_RenderFillRect(renderer, &rect);

    if (SDL_PointInRect(&point, arrow.getRect()))
    {
        arrowHover.render(renderer);
        hovering = true;
        if (eventManager.isLeftClick() && SDL_PointInRect(&startClickPos, arrow.getRect()))
        {
            playClickSound();
            currentState = MAIN;
        }
    }
    else
    {
        arrow.render(renderer);
    }

    loadSimulationTxt.render(renderer);

    setColor(WHITE);
    loadScrollbar.setDimensions(width, height - (loadSimulationTxt.getEndY() + 10), y - (loadSimulationTxt.getEndY() + 10) + 20);
    loadScrollbar.scroll(eventManager.getMouseWheel().y * 10);
    loadScrollbar.display(renderer, Vector(width - 5, loadSimulationTxt.getEndY() + 10));

    drawCursor(eventManager.getMousePos(), hovering);
    return 0;
}

int MenuManager::runEditSim(EventsManager eventManager, vector<Simulation> *simulations, int &simIndex, double timeStep)
{
    SDL_Rect rect;
    hovering = false;
    SDL_Point point = eventManager.getPoint();
    SDL_Point startClickPos = eventManager.getStartClickPos();

    setColor(BLACK);
    rect = {0, 0, (int)width, (int)(editSimulationTxt.getEndY() + 10)};
    SDL_RenderFillRect(renderer, &rect);

    form.displayInputs(eventManager, width, height);
    hovering = form.hovering(eventManager);

    if (form.submit(eventManager) && handleEdits(simulations, simIndex))
    {
        handleSim(simulations, simIndex);
        if (currentState == LOADEDIT)
        {
            currentState = LOAD;
            return 0;
        }
        currentState = MAIN;
        return 1;
    }

    setColor(BLACK);
    rect = {0, 0, (int)width, (int)(editSimulationTxt.getEndY() + 10)};
    SDL_RenderFillRect(renderer, &rect);

    editSimulationTxt.render(renderer);

    if (SDL_PointInRect(&point, arrow.getRect()))
    {
        arrowHover.render(renderer);
        hovering = true;
        if (eventManager.isLeftClick() && SDL_PointInRect(&startClickPos, arrow.getRect()))
        {
            form.clear();
            playClickSound();
            if (currentState == NEWEDIT)
            {
                currentState = NEW;
            }
            else
            {
                currentState = LOAD;
            }
        }
    }
    else
    {
        arrow.render(renderer);
    }

    drawCursor(eventManager.getMousePos(), hovering);
    return 0;
}

int MenuManager::runControls(EventsManager eventManager, double timeStep)
{
    hovering = false;
    SDL_Point point = eventManager.getPoint();
    SDL_Point startClickPos = eventManager.getStartClickPos();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    controlsHeaderTxt.render(renderer);
    SDL_RenderDrawLine(renderer, 100, controlsHeaderTxt.getEndX() + 20, width - 100, controlsHeaderTxt.getEndY() + 20);
    allControlsTxt.setCoords(50, controlsHeaderTxt.getEndY() + 50);
    allControlsTxt.render(renderer);

    if (SDL_PointInRect(&point, arrow.getRect()))
    {
        arrowHover.render(renderer);
        hovering = true;
        if (eventManager.isLeftClick() && SDL_PointInRect(&startClickPos, arrow.getRect()))
        {
            playClickSound();
            currentState = MAIN;
        }
    }
    else
    {
        arrow.render(renderer);
    }

    drawCursor(eventManager.getMousePos(), hovering);

    return 0;
}

void MenuManager::setEditInputs(vector<Simulation> *simulations, int &simIndex)
{
    string name;
    std::stringstream distRatio, simRadius;

    if (currentTemplate == -1)
    {
        name = simulations->at(simIndex).getName();
        distRatio << simulations->at(simIndex).getDistRatio();
        simRadius << simulations->at(simIndex).getSimRadius();
    }
    else
    {
        name = templates[currentTemplate].getName();
        distRatio << templates[currentTemplate].getDistRatio();
        simRadius << templates[currentTemplate].getSimRadius();
    }

    form.getInputs()[0]->setText(name);
    form.getInputs()[1]->setText(distRatio.str());
    form.getInputs()[2]->setText(simRadius.str());
}

bool MenuManager::handleEdits(vector<Simulation> *simulations, int simIndex)
{
    string name = form.getInputs()[0]->getText();
    long double distRatio, simRadius;

    form.getInputs()[0]->clearError();
    if (name.size() == 0)
    {
        form.getInputs()[0]->setErrorText("Simulation name can't be empty");
    }
    else
    {
        form.getInputs()[0]->clearError();
    }

    int count = 0;
    for (int i = 0; i < simulations->size(); i++)
    {
        if (name == simulations->at(i).getName())
        {
            if (simIndex != i)
            {
                form.getInputs()[0]->setErrorText("Simulation name can't be the same as another Simulation");
            }
        }
    }

    try
    {
        distRatio = stold(form.getInputs()[1]->getText());
        form.getInputs()[1]->clearError();

        if (distRatio <= 0)
        {
            form.getInputs()[1]->setErrorText("Distance Ratio should be > 0.");
        }
        else
        {
            form.getInputs()[2]->clearError();
        }
    }
    catch (exception e)
    {
        form.getInputs()[1]->setErrorText("Invalid Distance Ratio");
    }

    try
    {
        simRadius = stold(form.getInputs()[2]->getText());
        form.getInputs()[2]->clearError();

        if (simRadius <= 0)
        {
            form.getInputs()[2]->setErrorText("Simulation Radius should be > 0.");
            if (simRadius / distRatio > 1000000)
            {
                form.getInputs()[2]->setErrorText("The Simulation Radius is too big.");
            }
            else
            {
                form.getInputs()[2]->clearError();
            }
        }
        else
        {
            form.getInputs()[2]->clearError();
        }
    }
    catch (exception e)
    {
        form.getInputs()[2]->setErrorText("Invalid Simulation Radius");
    }

    if (form.getInputs()[0]->getError() || form.getInputs()[1]->getError() || form.getInputs()[2]->getError())
    {
        return false;
    }

    return true;
}

void MenuManager::handleSim(vector<Simulation> *simulations, int &simIndex)
{
    string name = form.getInputs()[0]->getText();
    long double distRatio = stold(form.getInputs()[1]->getText());
    long double simRadius = stold(form.getInputs()[2]->getText());

    if (currentState == NEWEDIT)
    {
        if (currentTemplate == -1)
        {
            simulations->push_back(Simulation(renderer, name, distRatio, simRadius));
            simIndex = simulations->size() - 1;
        }
        else
        {
            Simulation newSim = templates[currentTemplate];
            newSim.setName(name);
            newSim.setDistRatio(distRatio);
            newSim.setSimRadius(simRadius);
            simulations->push_back(newSim);
            simIndex = simulations->size() - 1;
        }
    }
    else if (currentState == LOADEDIT)
    {
        simulations->at(simIndex).setName(name);
        simulations->at(simIndex).setDistRatio(distRatio);
        simulations->at(simIndex).setSimRadius(simRadius);
    }

    form.clear();
}

int MenuManager::run(EventsManager eventManager, Transition *transition, vector<Simulation> *simulations, int &simIndex, double timeStep)
{
    int code = 0;

    if (transition->getTransitionState() == "Out" || transition->getTransitionState() == "Done")
    {
        switch (currentState)
        {
        case MAIN:
            code = runMainMenu(eventManager, timeStep);
            break;

        case NEW:
            code = runNewSim(eventManager, simulations, timeStep);
            break;

        case LOAD:
            code = runLoadSim(eventManager, simulations, simIndex, timeStep);
            break;

        case (NEWEDIT):
            code = runEditSim(eventManager, simulations, simIndex, timeStep);
            break;

        case (LOADEDIT):
            code = runEditSim(eventManager, simulations, simIndex, timeStep);
            break;

        case CONTROLS:
            runControls(eventManager, timeStep);
            break;
        }
    }

    return code;
}
