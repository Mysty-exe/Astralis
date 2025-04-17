#include <SimulationManager.h>

SimulationManager::SimulationManager()
{
}

SimulationManager::SimulationManager(SDL_Renderer *renderer, int width, int height) : Manager(renderer, width, height)
{
    currentState = SIM;
    currentSimulation = -1;
    viewScroll = 0;
    zoomFactor = 1;
    editingObject = -1;
    dragObject = -1;
    view = false;
    hovering = false;
    isAddViewOverlay = false;
    isEditOverlay = false;
    camera = Camera(width, height);
    camera.centerOnPosition(Vector(0, 0));

    infoOverlay = Overlay(renderer, width / 4, height, width - width / 4, 0);
    infoOverlay.addButton("Info");
    infoOverlay.addButton("Distances");

    addViewOverlay = Overlay(renderer, width / 4, height, 0, 0);
    addViewOverlay.addButton("Add");
    addViewOverlay.addButton("Select");
    editOverlay = Overlay(renderer, width / 4, height, width - (width / 4), 0);
    editOverlay.addButton("Edit");
    editOverlay.addButton("Distances");

    editForm = Form(renderer, width, height);
    editForm.setPos(editOverlay.getPos().x, editOverlay.getPos().y);
    Input *name = new Input(renderer, "Name", "", "Name Of The Celestial Object", "", 128, 50);
    Input *radius = new Input(renderer, "Radius", "", "Radius Of The Celestial Object (in km)", "", 128, 50);
    Input *mass = new Input(renderer, "Mass", "", "Mass Of The Celestial Object (in kg)", "", 128, 50);
    Input *velocity = new Input(renderer, "Velocity", "", "Velocity Of The Celestial Object (in m/s)", "", 128, 50);
    Input *direction = new Input(renderer, "Direction", "", "Velocity Direction (in degrees)", "", 128, 50);
    direction->setSlider(Slider(renderer, width / 4, height, -180, 180, 0));
    Button button = Button(renderer, "Save Changes");
    vector<Input *> inputs = {name, radius, mass, velocity, direction};
    editForm.setInputs(inputs);
    editForm.setSubmitButton(button);

    loadImages();
}

void SimulationManager::resetSimulation()
{
    getSim()->reset();
    camera.centerOnPosition(Vector(0, 0));
    zoomFactor = 1;
    view = false;
}

void SimulationManager::loadImages()
{
    simulationTexture.createRenderableTexture(renderer, width, height, SDL_TEXTUREACCESS_TARGET);
    background.loadAnimation(renderer, "Assets/Backgrounds/Game Background", Vector(1, 1), 1);
    loadingTrajectory.loadAnimation(renderer, "Assets/Trajectory/Trajectory Text", Vector(1, 1), 0.5);

    returnTxt.loadFromFile(renderer, "Assets/Return/mainMenuTxt.png", getWindowRatio());
    returnTxt.setCoords(width / 2 - returnTxt.getWidth() / 2, height / 6);

    yesTxt.loadFromFile(renderer, "Assets/Return/yesTxt.png", getWindowRatio());
    yesTxt.setCoords(width / 2 - yesTxt.getWidth() / 2, returnTxt.getY() + height / 3);
    yesHoverTxt.loadFromFile(renderer, "Assets/Return/yesHoverTxt.png", getWindowRatio());
    yesHoverTxt.setCoords(width / 2 - yesHoverTxt.getWidth() / 2, returnTxt.getY() + height / 3);

    noTxt.loadFromFile(renderer, "Assets/Return/noTxt.png", getWindowRatio());
    noTxt.setCoords(width / 2 - noTxt.getWidth() / 2, yesTxt.getY() + height / 6);
    noHoverTxt.loadFromFile(renderer, "Assets/Return/noHoverTxt.png", getWindowRatio());
    noHoverTxt.setCoords(width / 2 - noHoverTxt.getWidth() / 2, yesTxt.getY() + height / 6);

    inArrowTab.loadFromFile(renderer, "Assets/Arrow Tab/in.png", getWindowRatio());
    inArrowTab.setCoords(0, height / 2 - inArrowTab.getHeight() / 2);
    outArrowTab.loadFromFile(renderer, "Assets/Arrow Tab/out.png", getWindowRatio());
    outArrowTab.setCoords(width / 4, height / 2 - outArrowTab.getHeight() / 2);

    duplicateIcon.loadFromFile(renderer, "Assets/Icons/duplicateIcon.png", getWindowRatio());
    duplicateHoverIcon.loadFromFile(renderer, "Assets/Icons/duplicateHoverIcon.png", getWindowRatio());
    binIcon.loadFromFile(renderer, "Assets/Icons/binIcon.png", getWindowRatio());
    binHoverIcon.loadFromFile(renderer, "Assets/Icons/binHoverIcon.png", getWindowRatio());

    loadFrames();
}

void SimulationManager::loadFrames()
{
    frames.clear();
    vector<vector<string>> files = Utilities::getStellarFiles();

    for (int i = 0; i < files.size(); i++)
    {
        frames.push_back({});
        for (int ii = 0; ii < files[i].size(); ii++)
        {
            Frame f = Frame(renderer, files[i][ii], width / 8);
            frames[i].push_back(f);
        }
    }
}

void SimulationManager::setWindowSize(int width, int height)
{
    this->width = width;
    this->height = height;
    editForm.updateSize(width, height);
    camera.resizeDimensions(width, height);
    infoOverlay.resizeDimensions(width / 4, height, width - width / 4, 0);
    addViewOverlay.resizeDimensions(width / 4, height, 0, 0);
    editOverlay.resizeDimensions(width / 4, height, width - (width / 4), 0);
    loadImages();

    if (currentSimulation != -1)
    {
        getSim()->loadImages(getWindowRatio());
        getSim()->resizeViewObjects(width, height);
    }
}

SimulationState SimulationManager::getCurrentState()
{
    return currentState;
}

vector<Simulation> *SimulationManager::getSimulations()
{
    return &simulations;
}

int SimulationManager::getSimIndex()
{
    return currentSimulation;
}

void SimulationManager::setSimIndex(int currentSim)
{
    currentSimulation = currentSim;
}

Simulation *SimulationManager::getSim()
{
    if (currentSimulation != -1)
    {
        return &(simulations[currentSimulation]);
    }
}

void SimulationManager::drawBackground()
{
    int startX = 0;
    if (-camera.getX() < 0)
    {
        while (true)
        {
            startX -= background.getWidth();
            if (startX < -camera.getX())
            {
                break;
            }
        }
    }

    int startY = 0;
    if (-camera.getY() < 0)
    {
        while (true)
        {
            startY -= background.getHeight();
            if (startY < -camera.getY())
            {
                break;
            }
        }
    }

    for (int i = startX; i < width - camera.getX(); i += background.getWidth())
    {
        for (int ii = startY; ii < height - camera.getY(); ii += background.getHeight())
        {
            background.render(renderer, i + camera.getX(), ii + camera.getY());
        }
    }
}

void SimulationManager::eventHandler(EventsManager eventManager, MessageManager &messageManager)
{
    if (eventManager.checkSpecialKey(SPACE) && currentState == SIM)
    {
        getSim()->isPaused() ? getSim()->unPause() : getSim()->pause();
    }
    else if (eventManager.checkSpecialKey(CTRL) && currentState != RETURN)
    {
        setEditingError(messageManager);
        currentState = (currentState == EDITING && !getSim()->isError()) ? SIM : EDITING;
        (currentState == EDITING) ? getSim()->edit(true) : getSim()->edit(false);
        if (currentState == SIM)
        {
            resetEditor();
        }
    }
    else if (eventManager.checkSpecialKey(TAB) && currentState == SIM)
    {
        viewScroll = 0;
        view = !view;
    }
    else if (eventManager.checkSpecialKey(RIGHTARROW) && currentState == SIM && !view)
    {
        getSim()->speedUp();
    }
    else if (eventManager.checkSpecialKey(LEFTARROW) && currentState == SIM && !view)
    {
        getSim()->slowDown();
    }
    else if (eventManager.checkSpecialKey(ENTER) && currentState == SIM)
    {
        getSim()->focusObject();
    }
    else if (eventManager.checkSpecialKey(BACKSPACE) && currentState == SIM)
    {
        getSim()->defocusObjects();
    }
    else if (eventManager.checkSpecialKey(ESCAPE))
    {
        currentState = (currentState == SIM) ? RETURN : SIM;
        (currentState == SIM) ? getSim()->unPause() : getSim()->pause();
    }
}

void SimulationManager::showInfoDisplay(EventsManager eventManager)
{
    infoOverlay.render(eventManager);
    if (infoOverlay.getCurrentTab() == 0)
    {
        infoOverlay.resizeDimensions(width / 4, height, infoOverlay.getPos().x, 0);
        infoOverlay.setEndY(getSim()->displayObjectInfo(getSim()->getFocusedObject(), infoOverlay, getWindowRatio()));
    }
    else
    {
        infoOverlay.resizeDimensions(width / 4, height, infoOverlay.getPos().x, 0);
        infoOverlay.setEndY(getSim()->displayDistances(getSim()->getFocusedObject(), infoOverlay, Vector(1, 1)));
    }
    hovering = infoOverlay.renderWidgets(eventManager);
}

void SimulationManager::viewObjects(EventsManager eventManager)
{
    SDL_Rect rect = {(int)(0), (int)(height - width / 5), (int)(width / 5), (int)(width / 5)};

    if (eventManager.checkSpecialKey(RIGHTARROW))
    {
        if (viewScroll < getSim()->getObjectsSize() - 5)
        {
            viewScroll += 1;
        }
    }
    else if (eventManager.checkSpecialKey(LEFTARROW))
    {
        if (viewScroll > 0)
        {
            viewScroll -= 1;
        }
    }

    int ii = 0;
    for (int i = viewScroll; i < getSim()->getObjectsSize(); i++)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
        if (SDL_PointInRect(&point, &rect))
        {
            SDL_SetRenderDrawColor(renderer, 203, 195, 227, 50);
            hovering = true;
            if (eventManager.isLeftClick() && SDL_PointInRect(&startClickPos, &rect))
            {
                playClickSound();
                getSim()->focusObject(i);
                view = false;
            }
        }

        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);
        rect.x += (width / 5);

        getSim()->getObject(i).renderViewDisplay(ii * (width / 5), height - (width / 5), width / 5, width / 5);
        ii++;
    }

    if (eventManager.isHoldingClick() && (startClickPos.y < (height - height / 3) || startClickPos.x > rect.x))
    {
        hovering = true;
        camera.drag(eventManager.getMouseOffset());
    }
}

void SimulationManager::displaySimulation(EventsManager eventManager, double timeStep)
{
    renderQuad = {(int)-camera.getX(), (int)-camera.getY(), width, height};
    SDL_Point point = eventManager.getPoint();

    simulationTexture.setAsRenderTarget(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    drawBackground();
    getSim()->display(&renderQuad, camera.getOffset(), timeStep, dragObject);

    if (currentState == EDITING && editingObject != -1 && getSim()->trajectoryCalculated())
    {
        getSim()->drawTrajectory(getSim()->getObjects()[editingObject], camera.getOffset());
    }

    SDL_SetRenderTarget(renderer, NULL);
    simulationTexture.setCoords(0 - getRenderOffset().x, 0 - getRenderOffset().y);
    simulationTexture.render(renderer, Vector(0, 0), zoomFactor);
}

int SimulationManager::runSimulation(EventsManager eventManager, double timeStep)
{
    getSim()->run(timeStep);

    if (getSim()->isFocusing())
    {
        if (getSim()->getObjectsSize() > 0)
        {
            showInfoDisplay(eventManager);
        }
    }

    if (eventManager.isHoldingClick() && !getSim()->isFocusing() && !view)
    {
        hovering = true;
        camera.drag(eventManager.getMouseOffset() / zoomFactor);
    }
    if (getSim()->isFocusing())
    {
        camera.centerOnPosition(getSim()->getFocusedObject().getPosition());
        getSim()->displayFocusedObject(getWindowRatio());
    }

    if (!getSim()->isPaused() && (!getSim()->isFocusing() || (getSim()->isFocusing() && !SDL_PointInRect(&point, infoOverlay.getRect()))))
    {
        handleZoom(eventManager);
    }

    if (view && getSim()->getObjectsSize() > 0)
    {
        viewObjects(eventManager);
    }
    else
    {
        getSim()->displayPauseStatus(getWindowRatio());
        getSim()->displaySimulationDate(height, getWindowRatio());
        getSim()->displayTimeRate(getWindowRatio());
    }

    return 1;
}

int SimulationManager::editAddObjects(EventsManager eventManager, double timeStep)
{
    Texture headerTxt, objTexture;
    int startY = addViewOverlay.getPos().y + 10;

    vector<string> objects = Utilities::getStellarObjects();
    for (int i = 0; i < frames.size(); i++)
    {
        headerTxt.loadFromRenderedText(renderer, bigFont, objects[i], {255, 255, 255});
        headerTxt.setCoords(10, startY - addViewOverlay.getScroll());
        startY += headerTxt.getHeight() + 10;

        int x = 0;
        for (int ii = 0; ii < frames[i].size(); ii++)
        {
            setColor(WHITE);
            if (SDL_PointInRect(&point, frames[i][ii].getRect()))
            {
                hovering = true;
                setColor(PURPLE);
                if (eventManager.isLeftClick() && (SDL_PointInRect(&startClickPos, frames[i][ii].getRect())) && !SDL_PointInRect(&point, addViewOverlay.getButtonRect()))
                {
                    string name = "Untitled-" + to_string(getSim()->getObjectsNum());
                    CelestialObject obj = CelestialObject(renderer, width, height, name, objects[i], frames[i][ii].getFile());
                    obj.setPosition((camera.getOffset() * -1) + Vector(width / 2, height / 2));
                    getSim()->addObjectsNum();
                    getSim()->addObject(obj);
                    getSim()->scaleObjects(name);
                    int index = getSim()->getObjectsSize() - 1;
                    editingObject = index;
                    isAddViewOverlay = false;
                    isEditOverlay = true;
                    setFormInputs();

                    getSim()->runTrajectoryThread(camera.getOffset());
                }
            }

            frames[i][ii].display(x, startY - addViewOverlay.getScroll());
            if (x == 0)
            {
                x = frames[i][ii].getSize();
            }
            else
            {
                x = 0;
                if (ii != frames[i].size() - 1)
                {
                    startY += frames[i][ii].getSize();
                }
            }
        }

        headerTxt.render(renderer);
        startY += width / 8 + 20;
    }

    return startY;
}

int SimulationManager::editViewObjects(EventsManager eventManager, double timeStep)
{
    Texture objectTxt, objectTypeTxt;
    int startY = addViewOverlay.getPos().y;

    for (int i = 0; i < getSim()->getObjectsSize(); i++)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);
        SDL_Rect rect = {0, startY - addViewOverlay.getScroll(), width / 4, height / 10};
        if (SDL_PointInRect(&point, &rect))
        {
            hovering = true;
            SDL_RenderFillRect(renderer, &rect);
            if (eventManager.isLeftClick() && (SDL_PointInRect(&startClickPos, &rect)))
            {
                camera.centerOnPosition(getSim()->getObjects()[i].getPosition());
                isAddViewOverlay = false;
                isEditOverlay = true;
                editingObject = i;
                setFormInputs();

                getSim()->runTrajectoryThread(camera.getOffset());
            }
        }

        objectTxt.loadFromRenderedText(renderer, smallFont, getSim()->getObjects()[i].getName(), {177, 156, 217});
        objectTypeTxt.loadFromRenderedText(renderer, smallFont, getSim()->getObjects()[i].getObjType(getSim()->getObjects()[i].getObjType()), {255, 255, 255});

        objectTypeTxt.setCoords(rect.x + 10, rect.y + 10);
        objectTxt.setCoords(rect.x + 15, objectTypeTxt.getY() + objectTypeTxt.getHeight() + 5);
        objectTypeTxt.render(renderer);
        objectTxt.render(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 5, startY + height / 10 - addViewOverlay.getScroll(), width / 4 - 10, startY + height / 10 - addViewOverlay.getScroll());
        startY += height / 10;
    }

    return startY;
}

void SimulationManager::addViewObjects(EventsManager eventManager, double timeStep)
{

    if (!isAddViewOverlay)
    {
        if (SDL_PointInRect(&point, inArrowTab.getRect()))
        {
            hovering = true;
            if (eventManager.isLeftClick())
            {
                isAddViewOverlay = true;
            }
        }

        inArrowTab.render(renderer);
        return;
    }

    addViewOverlay.render(eventManager);
    if (addViewOverlay.getCurrentTab() == 0)
    {
        addViewOverlay.resizeDimensions(width / 4, height, 0, 0);
        addViewOverlay.setEndY(editAddObjects(eventManager, timeStep));
    }
    else
    {
        addViewOverlay.resizeDimensions(width / 4, height, 0, 0);
        addViewOverlay.setEndY(editViewObjects(eventManager, timeStep));
    }

    if (hovering)
    {
        addViewOverlay.renderWidgets(eventManager);
    }
    else
    {
        hovering = addViewOverlay.renderWidgets(eventManager);
    }

    if (SDL_PointInRect(&point, outArrowTab.getRect()))
    {
        hovering = true;
        if (eventManager.isLeftClick())
        {
            isAddViewOverlay = false;
        }
    }

    outArrowTab.render(renderer);
}

int SimulationManager::editAnObject(EventsManager eventManager, double timeStep)
{
    int startX = editOverlay.getPos().x;
    int startY = editOverlay.getPos().y;

    binIcon.setCoords(startX + width / 4 - binIcon.getWidth() - 15, duplicateIcon.getY());
    binHoverIcon.setCoords(binIcon.getX(), binIcon.getY());
    duplicateIcon.setCoords(binIcon.getX() - duplicateIcon.getWidth() - 20, startY + 10);
    duplicateHoverIcon.setCoords(duplicateIcon.getX(), duplicateIcon.getY());
    startY = duplicateIcon.getEndY();

    if (SDL_PointInRect(&point, duplicateIcon.getRect()))
    {
        hovering = true;
        duplicateHoverIcon.render(renderer);
        if (eventManager.isLeftClick() && SDL_PointInRect(&startClickPos, duplicateIcon.getRect()))
        {
            string name = "Untitled-" + to_string(getSim()->getObjectsNum());
            CelestialObject copy = getSim()->getObjects()[editingObject];
            CelestialObject obj = CelestialObject(renderer, width, height, name, copy.getObjType(copy.getObjType()), copy.getFolder(), getSim()->getRealMass(copy.getMass()), getSim()->getRealDistance(copy.getRadius()), getSim()->getRealDistance(copy.getPosition().x), getSim()->getRealDistance(copy.getPosition().y), Vector(getSim()->getRealDistance(copy.getVelocity().x), getSim()->getRealDistance(copy.getVelocity().y)));
            obj.setPosition((camera.getOffset() * -1) + Vector(width / 2, height / 2));
            getSim()->addObjectsNum();
            getSim()->addObject(obj);
            getSim()->scaleObjects(name);
            int index = getSim()->getObjectsSize() - 1;
            editingObject = index;
            isEditOverlay = true;
            setFormInputs();

            getSim()->runTrajectoryThread(camera.getOffset());
        }
    }
    else
    {
        duplicateIcon.render(renderer);
    }

    if (SDL_PointInRect(&point, binIcon.getRect()))
    {
        hovering = true;
        binHoverIcon.render(renderer);
        if (eventManager.isLeftClick() && SDL_PointInRect(&startClickPos, binIcon.getRect()))
        {
            getSim()->deleteObject(editingObject);
            isEditOverlay = false;
            editingObject = -1;
            distanceInputs.clear();
        }
    }
    else
    {
        binIcon.render(renderer);
    }

    handleEdits(eventManager);

    editForm.setPos(startX + 10, startY);
    editForm.displayInputs(eventManager, width, height, Vector(width / 4, height));

    if (hovering)
    {
        editForm.hovering(eventManager);
    }
    else
    {
        hovering = editForm.hovering(eventManager);
    }

    return binIcon.getEndY() + 10;
}

void SimulationManager::setFormInputs()
{
    std::stringstream radius, mass, velocity, direction;
    string name = getSim()->getObjects()[editingObject].getName();
    radius << getSim()->getRealDistance(getSim()->getObjects()[editingObject].getRadius());
    mass << getSim()->getRealMass(getSim()->getObjects()[editingObject].getMass());
    velocity << getSim()->getRealDistance(getSim()->getObjects()[editingObject].getVelocity().magnitude());
    direction << getSim()->getObjects()[editingObject].getVelocity().getAngle();

    editForm.getInputs()[0]->setText(name);
    editForm.getInputs()[1]->setText(radius.str());
    editForm.getInputs()[2]->setText(mass.str());
    editForm.getInputs()[3]->setText(velocity.str());
    editForm.getInputs()[4]->setText(direction.str());
    editForm.getInputs()[4]->setSliderValue(getSim()->getObjects()[editingObject].getVelocity().getAngle());
    setDistanceInputs();
}

void SimulationManager::checkFormChange()
{
    string name = getSim()->getObjects()[editingObject].getName();
    std::stringstream radius, mass, velocity, direction;
    radius << getSim()->getRealDistance(getSim()->getObjects()[editingObject].getRadius());
    mass << getSim()->getRealMass(getSim()->getObjects()[editingObject].getMass());
    velocity << getSim()->getRealDistance(getSim()->getObjects()[editingObject].getVelocity().magnitude());
    direction << getSim()->getObjects()[editingObject].getVelocity().getAngle();

    editForm.enableButton();
    if (editForm.getInputs()[0]->getText() == name && editForm.getInputs()[1]->getText() == radius.str())
    {
        if (editForm.getInputs()[2]->getText() == mass.str() && editForm.getInputs()[3]->getText() == velocity.str())
        {
            if (editForm.getInputs()[4]->getText() == direction.str())
            {
                editForm.disableButton();
            }
        }
    }
}

void SimulationManager::handleEdits(EventsManager eventManager)
{
    string name = editForm.getInputs()[0]->getText();
    long double radius, mass, velocity, direction;

    editForm.getInputs()[0]->clearError();
    if (name.size() == 0)
    {
        editForm.getInputs()[0]->setErrorText("Object name can't be empty");
    }
    else
    {
        editForm.getInputs()[0]->clearError();
    }

    for (int i = 0; i < getSim()->getObjectsSize(); i++)
    {
        if (editingObject != i)
        {
            if (name == getSim()->getObjects()[i].getName())
            {
                editForm.getInputs()[0]->setErrorText("Object name can't be the same as another object");
                break;
            }
        }
    }

    try
    {
        radius = stold(editForm.getInputs()[1]->getText());
        editForm.getInputs()[1]->clearError();

        if (radius <= 0)
        {
            editForm.getInputs()[1]->setErrorText("Object Radius should be > 0.");
        }
        else
        {
            editForm.getInputs()[1]->clearError();
        }
    }
    catch (exception e)
    {
        editForm.getInputs()[1]->setErrorText("Invalid Object Radius");
    }

    try
    {
        mass = stold(editForm.getInputs()[2]->getText());
        editForm.getInputs()[2]->clearError();

        if (mass <= 0)
        {
            editForm.getInputs()[2]->setErrorText("Object Mass should be > 0.");
        }
        else
        {
            editForm.getInputs()[2]->clearError();
        }
    }
    catch (exception e)
    {
        editForm.getInputs()[2]->setErrorText("Invalid Object Mass");
    }

    try
    {
        velocity = stold(editForm.getInputs()[3]->getText());
        editForm.getInputs()[3]->clearError();
    }
    catch (exception e)
    {
        editForm.getInputs()[3]->setErrorText("Invalid Object Velocity");
    }

    try
    {
        direction = stold(editForm.getInputs()[4]->getText());
        editForm.getInputs()[4]->clearError();

        if (direction < -180 || direction > 180)
        {
            editForm.getInputs()[4]->setErrorText("Object Direction should be between -180 and 180.");
        }
        else
        {
            editForm.getInputs()[4]->clearError();
            if (editForm.getInputs()[4]->getActivated())
            {
                editForm.getInputs()[4]->setSliderValue(direction);
            }
            if (editForm.getInputs()[4]->getSliderActivated())
            {
                editForm.getInputs()[4]->setText(to_string(editForm.getInputs()[4]->getSliderValue()));
            }
        }
    }
    catch (exception e)
    {
        editForm.getInputs()[4]->setErrorText("Invalid Object Direction");
    }

    if (!editForm.getInputs()[0]->getError() && !editForm.getInputs()[1]->getError() && !editForm.getInputs()[2]->getError() && !editForm.getInputs()[3]->getError() && !editForm.getInputs()[4]->getError())
    {
        if (editForm.submit(eventManager))
        {
            double radians = direction * PI / 180;
            Vector direction = Vector(cos(radians), sin(radians));
            Vector vel = direction.normalize() * velocity;
            getSim()->updateObject(editingObject, name, radius, mass, vel);
            getSim()->runTrajectoryThread(camera.getOffset());
        }
    }
}

void SimulationManager::setDistanceInputs()
{
    if (editingObject != -1)
    {
        distanceInputs.clear();
        CelestialObject obj = getSim()->getObjects()[editingObject];
        for (CelestialObject o : getSim()->getObjects())
        {
            if (obj.getName() != o.getName())
            {
                Input distance = Input(renderer, "", to_string(getSim()->getRealDistance(obj.getPosition().distance(o.getPosition()))), "", "", 100, 20);
                distanceInputs.push_back(distance);
            }
        }
    }
}

void SimulationManager::updateDistances()
{
    long double distance;

    if (editingObject != -1)
    {
        int i = 0;
        CelestialObject obj = getSim()->getObjects()[editingObject];
        for (CelestialObject o : getSim()->getObjects())
        {
            if (obj.getName() != o.getName())
            {
                if (distanceInputs[i].getActivated())
                {
                    try
                    {
                        distance = (distanceInputs[i].getText().empty()) ? 0 : stold(distanceInputs[i].getText());
                    }
                    catch (exception e)
                    {
                        distance = 0;
                    }

                    Vector position = (obj.getPosition() - o.getPosition()).normalize() * getSim()->scaleDistance(distance);
                    getSim()->updateObjectPosition(editingObject, o.getPosition() + position);
                }
                i++;
            }
        }
    }
}

int SimulationManager::editDistances(EventsManager eventManager, double timeStep)
{
    Texture distanceNameTxt, kmTxt;
    SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);

    int i = 0;
    int startY = editOverlay.getPos().y;
    CelestialObject obj = getSim()->getObjects()[editingObject];
    for (CelestialObject o : getSim()->getObjects())
    {
        if (obj.getName() != o.getName())
        {
            distanceNameTxt.loadFromRenderedText(renderer, smallFont, o.getName() + " - (" + o.getObjType(o.getObjType()) + ")", {255, 255, 255});
            distanceNameTxt.setCoords(editOverlay.getPos().x + 25, startY + 10 - editOverlay.getScroll());
            distanceNameTxt.render(renderer);

            distanceInputs[i].setPos(editOverlay.getPos().x + 30, distanceNameTxt.getRect()->y + distanceNameTxt.getRect()->h);
            distanceInputs[i].process(eventManager);
            distanceInputs[i].render(renderer, width, height);

            if (SDL_PointInRect(&point, distanceInputs[i].getRect()))
            {
                hovering = true;
            }

            if (distanceInputs[i].getActivated())
            {
                updateDistances();
            }

            kmTxt.loadFromRenderedText(renderer, smallFont, "km", {255, 255, 255});
            kmTxt.setCoords(distanceInputs[i].getEndX() + 5, distanceInputs[i].getY() + distanceInputs[i].getHeight() / 2);
            kmTxt.render(renderer);

            SDL_RenderDrawLine(renderer, editOverlay.getPos().x + 20, startY + 75 - editOverlay.getScroll(), editOverlay.getPos().x + editOverlay.getWidth() - 20, startY + 75 - editOverlay.getScroll());
            startY += 75;
            i++;
        }
    }

    return startY;
}

void SimulationManager::editObjects(EventsManager eventManager, double timeStep)
{
    if (!isEditOverlay)
    {
        return;
    }

    editOverlay.render(eventManager);
    if (editOverlay.getCurrentTab() == 0)
    {
        checkFormChange();
        editOverlay.resizeDimensions(width / 4, height, width - (width / 4), 0);
        editOverlay.setEndY(editAnObject(eventManager, timeStep));
    }
    else
    {
        editOverlay.resizeDimensions(width / 4, height, width - (width / 4), 0);
        editOverlay.setEndY(editDistances(eventManager, timeStep));
    }

    if (hovering)
    {
        editOverlay.renderWidgets(eventManager);
    }
    else
    {
        hovering = editOverlay.renderWidgets(eventManager);
    }
}

void SimulationManager::resetEditor()
{
    getSim()->toggleOverlapping(false);
    getSim()->toggleOutOfBounds(false);
    addViewOverlay.reset();
    editOverlay.reset();
    isAddViewOverlay = false;
    isEditOverlay = false;
    editingObject = -1;
    dragObject = -1;
    distanceInputs.clear();
    editForm.clear();
}

int SimulationManager::runEditor(EventsManager eventManager, MessageManager &messageManager, double timeStep)
{
    SDL_Rect rect = {0, 0, width, height};
    (!getSim()->isError()) ? SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50) : SDL_SetRenderDrawColor(renderer, 255, 0, 0, 50);
    SDL_RenderFillRect(renderer, &rect);
    getSim()->toggleOverlapping(false);
    getSim()->toggleOutOfBounds(false);

    for (int i = 0; i < getSim()->getObjectsSize(); i++)
    {
        CelestialObject obj = getSim()->getObjects()[i];
        if (dragObject == -1)
        {
            SDL_Rect rect = obj.getRect(getRenderOffset(), zoomFactor);
            if (SDL_PointInRect(&point, &rect))
            {
                hovering = true;
                if (eventManager.isLeftClick())
                {
                    dragObject = i;
                }
                if (eventManager.isRightClick())
                {
                    editingObject = (editingObject == i) ? -1 : i;
                    isEditOverlay = false;
                    distanceInputs.clear();
                    if (editingObject != -1)
                    {
                        getSim()->runTrajectoryThread(camera.getOffset());
                        isEditOverlay = true;
                        setFormInputs();
                    }
                }
            }
        }

        if (getSim()->getRealPosition(obj.getPosition()).magnitude() > getSim()->getSimRadius())
        {
            getSim()->toggleOutOfBounds(true);
            continue;
        }

        for (int ii = 0; ii < getSim()->getObjectsSize(); ii++)
        {
            if (i != ii)
            {
                CelestialObject obj2 = getSim()->getObjects()[ii];
                if (obj.getPosition().distance(obj2.getPosition()) <= obj.getRadius() + obj2.getRadius())
                {
                    getSim()->toggleOverlapping(true);
                    break;
                }
            }
        }
    }

    if (dragObject != -1)
    {
        if (eventManager.isHoldingClick())
        {
            hovering = true;
            Vector drag = (Vector(width / 2, height / 2) - eventManager.getMousePos()) / 100;
            drag = Vector(drag.x * abs(drag.x), drag.y * abs(drag.y));
            camera.drag(drag);
            getSim()->dragObject(dragObject, (eventManager.getMouseOffset() / zoomFactor) - drag);
            setDistanceInputs();
            getSim()->setTrajectoryCalculated(false);
        }
        else
        {
            if (eventManager.isLeftClickUp())
            {
                getSim()->runTrajectoryThread(camera.getOffset());
            }
            dragObject = -1;
        }
    }

    addViewObjects(eventManager, timeStep);
    editObjects(eventManager, timeStep);

    if (editingObject != -1 && !getSim()->trajectoryCalculated())
    {
        if (isAddViewOverlay)
        {
            loadingTrajectory.render(renderer, addViewOverlay.getWidth() + 10, height - loadingTrajectory.getHeight() - 5);
        }
        else
        {
            loadingTrajectory.render(renderer, 10, height - loadingTrajectory.getHeight() - 5);
        }
    }

    if (!getSim()->isPaused())
    {
        if (!isAddViewOverlay || (isAddViewOverlay && !SDL_PointInRect(&point, addViewOverlay.getRect())))
        {
            if (!isEditOverlay || (isEditOverlay && !SDL_PointInRect(&point, editOverlay.getRect())))
            {
                handleZoom(eventManager);
            }
        }
    }

    if (eventManager.isHoldingClick() && dragObject == -1)
    {
        if (!isAddViewOverlay || (!SDL_PointInRect(&startClickPos, addViewOverlay.getRect()) && isAddViewOverlay))
        {
            if (!isEditOverlay || (!SDL_PointInRect(&startClickPos, editOverlay.getRect()) && isEditOverlay))
            {
                hovering = true;
                camera.drag(eventManager.getMouseOffset() / zoomFactor);
            }
        }
    }

    return 1;
}

void SimulationManager::setEditingError(MessageManager &messageManager)
{
    if (getSim()->isOutOfBounds())
    {
        messageManager.addMessage("An Object is Out Of Bounds");
    }
    if (getSim()->isOverlapping())
    {
        messageManager.addMessage("An Object is Overlapping with another Object");
    }
}

int SimulationManager::runReturn(EventsManager eventManager)
{
    simulations[currentSimulation].pause();

    SDL_Rect rect = {0, 0, width, height};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(renderer, &rect);

    returnTxt.render(renderer);

    if (SDL_PointInRect(&point, yesTxt.getRect()))
    {
        hovering = true;
        yesHoverTxt.render(renderer);
        if (eventManager.isLeftClick() && SDL_PointInRect(&startClickPos, yesTxt.getRect()))
        {
            getSim()->unPause();
            playClickSound();
            resetSimulation();
            currentState = SIM;
            return 0;
        }
    }
    else
    {
        yesTxt.render(renderer);
    }

    if (SDL_PointInRect(&point, noTxt.getRect()))
    {
        hovering = true;
        noHoverTxt.render(renderer);
        if (eventManager.isLeftClick() && SDL_PointInRect(&startClickPos, noTxt.getRect()))
        {
            playClickSound();
            currentState = SIM;
            getSim()->unPause();
        }
    }
    else
    {
        noTxt.render(renderer);
    }

    return 1;
}

Vector SimulationManager::getRenderOffset()
{
    return Vector(((zoomFactor * width - width) / 2), ((zoomFactor * height - height) / 2));
}

void SimulationManager::handleZoom(EventsManager eventManager)
{
    if (eventManager.getMouseWheel().y > 0)
    {
        zoomFactor += 0.1;
    }
    else if (eventManager.getMouseWheel().y < 0)
    {
        zoomFactor -= 0.1;
    }

    zoomFactor = (zoomFactor > 10) ? 10 : zoomFactor;
    zoomFactor = (zoomFactor < 1) ? 1 : zoomFactor;
}

int SimulationManager::run(EventsManager eventManager, MessageManager &messageManager, Transition *transition, double timeStep)
{
    int code;

    point = eventManager.getPoint();
    startClickPos = eventManager.getStartClickPos();

    if (transition->getTransitionState() != "In")
    {
        displaySimulation(eventManager, timeStep);
    }

    if (transition->getTransitionState() == "Out" || transition->getTransitionState() == "Done")
    {
        eventHandler(eventManager, messageManager);

        switch (currentState)
        {
        case SIM:
            code = runSimulation(eventManager, timeStep);
            break;

        case RETURN:
            code = runReturn(eventManager);
            break;

        case EDITING:
            code = runEditor(eventManager, messageManager, timeStep);
            break;
        }
    }

    drawCursor(eventManager.getMousePos(), hovering);
    hovering = false;

    return code;
}
