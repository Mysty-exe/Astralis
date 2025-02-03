#include <Sidebar.h>

Sidebar::Sidebar()
{
}

Sidebar::Sidebar(SDL_Renderer *renderer, int width, int height)
{
    this->renderer = renderer;
    this->width = width;
    this->height = height;
    sidebarWidth = width / 3;
    sidebarHeight = height;

    scrollableHeight = height;
    scroll = 0;
    viewScroll = 0;
    distanceScroll = 0;
    maxScroll = 0;
    viewMaxScroll = 0;
    distanceMaxScroll = 0;

    addState = true;
    viewState = false;
    editState = false;
    distanceState = false;
    addActivated = false;
    sliderActivated = false;
    editActivated = true;
    editingObject = -1;
    selectedObject = -1;

    bigFont = TTF_OpenFont("assets/Fonts/font.otf", 32);
    font = TTF_OpenFont("assets/Fonts/font.otf", 24);
    smallFont = TTF_OpenFont("assets/Fonts/quicksand.otf", 14);

    inArrow.loadFromFile(renderer, "assets/Arrows/in.png");
    outArrow.loadFromFile(renderer, "assets/Arrows/out.png");
    sidebarTxt.loadFromRenderedText(renderer, bigFont, "Select Object", {177, 156, 217});
    distancesTxt.loadFromRenderedText(renderer, bigFont, "Distances", {177, 156, 217});
    distanceNameTxt.loadFromRenderedText(renderer, smallFont, "Name", {177, 156, 217});
    distanceTxt.loadFromRenderedText(renderer, smallFont, "100 km", {177, 156, 217});
    nameTxt.loadFromRenderedText(renderer, font, "Name", {255, 255, 255});
    radiusTxt.loadFromRenderedText(renderer, font, "Radius", {255, 255, 255});
    radiusHelperTxt.loadFromRenderedText(renderer, smallFont, "Ex: [60000] or [6e+4] kg", {211, 211, 211});
    massTxt.loadFromRenderedText(renderer, font, "Mass", {255, 255, 255});
    massHelperTxt.loadFromRenderedText(renderer, smallFont, "Ex: [2.5e+24] kg", {211, 211, 211});
    velocityTxt.loadFromRenderedText(renderer, font, "Velocity", {255, 255, 255});
    velocityHelperTxt.loadFromRenderedText(renderer, smallFont, "Ex: [100000] or [1e+5] m/s", {211, 211, 211});
    directionTxt.loadFromRenderedText(renderer, font, "Direction", {255, 255, 255});
    degreeHelperTxt.loadFromRenderedText(renderer, smallFont, "Ex: -180 - 180 degrees", {211, 211, 211});
    kmTxt.loadFromRenderedText(renderer, smallFont, "km", {255, 255, 255});
    kgTxt.loadFromRenderedText(renderer, smallFont, "kg", {255, 255, 255});
    msTxt.loadFromRenderedText(renderer, smallFont, "m/s", {255, 255, 255});
    degreeTxt.loadFromRenderedText(renderer, smallFont, "degrees", {255, 255, 255});
    circle.loadFromFile(renderer, "assets/circle.png", 0.25);

    nameInput = TextInput(renderer, "", {255, 255, 255});
    radiusInput = TextInput(renderer, "", {255, 255, 255});
    massInput = TextInput(renderer, "", {255, 255, 255});
    velocityInput = TextInput(renderer, "", {255, 255, 255});
    degreeInput = TextInput(renderer, "", {255, 255, 255});

    deleteBtn = Button(renderer, "Delete", {255, 255, 255}, 100);
    duplicateBtn = Button(renderer, "Duplicate", {255, 255, 255}, 100);

    addTransitionOffset = (width / 3) + 1;
    editTransitionOffset = -(width / 3);
    addRect = {0 - addTransitionOffset, 0, width / 3, height};
    editRect = {(width - (width / 3)) - editTransitionOffset, 0, width / 3, height};
    headerRect = {0 - addTransitionOffset, 0, sidebarWidth, (int)sidebarTxt.getHeight() + 20};
    color = {255, 255, 255};
    setFrames();
}

void Sidebar::setHeader(string header)
{
    this->header = header;
}

SDL_Rect *Sidebar::getRect()
{
    if (addState)
    {
        return &addRect;
    }
    return &editRect;
}

SDL_Rect *Sidebar::getHeaderRect()
{
    return &headerRect;
}

vector<Frame> Sidebar::getFrames()
{
    return frames;
}

void Sidebar::setFrames()
{
    sidebarTxt.setCoords(10, 10);

    int startX, startY, size, i;
    startX = 10;
    startY = sidebarTxt.getHeight() + 20;
    size = sidebarWidth / 2;
    i = 0;

    for (vector<string> objects : Utilities::getStellarObjects())
    {
        for (int ii = 0; ii < objects.size(); ii++)
        {
            if (ii == 0)
            {
                Image text;
                text.loadFromRenderedText(renderer, font, objects[ii], color);
                text.setCoords(startX, startY);
                headers.push_back(text);
                startX = 0;
                startY += text.getHeight() + 10;
            }
            else
            {
                Frame frame = Frame(renderer, objects[ii], Utilities::getStellarFiles()[i][ii], startX, startY, size, size);
                frames.push_back(frame);
                startX = (startX == size) ? 0 : startX + size;
                startY = (startX == 0) ? startY + size : startY;

                if (ii == objects.size() - 1 && objects.size() % 2 == 0)
                {
                    startY += size;
                }
            }
        }

        i++;
        startX = 10;
        startY += 10;
    }
    scrollableHeight = startY - 20;
    maxScroll = startY - sidebarHeight;
    maxScroll = (maxScroll < 0) ? 0 : maxScroll;
}

void Sidebar::viewObjects(Events events, vector<CelestialObject> &objects, double deltaTime, double timeRate)
{
    SDL_Point point = events.getPoint();

    if (SDL_PointInRect(&point, &editRect))
    {
        viewScroll -= events.mouseWheel.y * 1200 * deltaTime;
        viewScroll = (viewScroll < 0) ? 0 : viewScroll;
        viewScroll = (viewScroll > viewMaxScroll) ? viewMaxScroll : viewScroll;
    }

    SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);
    sidebarTxt.setCoords(10, editRect.y + 10 - viewScroll);
    int startY = (sidebarTxt.getRect()->y + viewScroll) + sidebarTxt.getRect()->h + 10;
    sidebarTxt.render(renderer, addTransitionOffset, 0);

    for (int i = 0; i < objects.size(); i++)
    {
        SDL_Rect rect = {20 - addTransitionOffset, startY - viewScroll, sidebarWidth - 40, 65};
        (SDL_PointInRect(&point, &rect))
            ? SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50)
            : SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        if (events.leftClick)
        {
            editingObject = objects.size() - 1;
            setInputs(objects[i], timeRate);
            addActivated = false;
            editActivated = true;
            header = objects[i].name;
            editState = true;
        }
        SDL_RenderFillRect(renderer, &rect);
        objectTxt.loadFromRenderedText(renderer, smallFont, objects[i].name, {177, 156, 217});
        objectTypeTxt.loadFromRenderedText(renderer, smallFont, objects[i].objType, {255, 255, 255});
        objectTxt.setCoords(rect.x + 10, rect.y + 10);
        objectTxt.render(renderer, addTransitionOffset, viewScroll);
        objectTypeTxt.setCoords(rect.x + 15, objectTxt.y + objectTxt.getHeight() + 5);
        objectTypeTxt.render(renderer, addTransitionOffset, viewScroll);
        SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);
        SDL_RenderDrawLine(renderer, 20 - addTransitionOffset, startY + 65 - viewScroll, sidebarWidth - 20 - addTransitionOffset, startY + 65 - viewScroll);
        startY += 70;
    }

    viewMaxScroll = startY - sidebarHeight - 65;
    viewMaxScroll = (viewMaxScroll < 0) ? 0 : viewMaxScroll;
    float scrollableHeight = startY - 65 - 10;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    float scrollableLength = sidebarHeight - ((float)sidebarHeight / scrollableHeight * sidebarHeight);
    float move = viewScroll / (float)viewMaxScroll * scrollableLength;
    SDL_RenderDrawLine(renderer, width - 10, 5 + move, width - 10, 5 + (sidebarHeight / scrollableHeight * sidebarHeight) + move);
}

void Sidebar::addObject(Events events, Vector spawnLoc, vector<CelestialObject> &objects, double timeRate, double deltaTime)
{
    SDL_Point point = events.getPoint();

    if (SDL_PointInRect(&point, &addRect))
    {
        scroll -= events.mouseWheel.y * 1200 * deltaTime;
        scroll = (scroll < 0) ? 0 : scroll;
        scroll = (scroll > maxScroll) ? maxScroll : scroll;
    }

    for (Image header : headers)
    {
        header.render(renderer, addTransitionOffset, scroll);
    }

    for (Frame &frame : frames)
    {
        SDL_Rect rect = frame.getRect();
        rect.x -= addTransitionOffset;
        rect.y -= scroll;

        frame.display(addTransitionOffset, scroll);

        if (SDL_PointInRect(&point, &rect) && !SDL_PointInRect(&point, &headerRect))
        {
            if (events.leftClick && SDL_PointInRect(&events.startClickPos, &rect))
            {
                string name = "untitled-" + to_string(Utilities::numObjects);
                CelestialObject obj = CelestialObject(renderer, width, height, name, frame.getName(), frame.getFile());
                obj.position = spawnLoc;
                objects.push_back(obj);
                editingObject = objects.size() - 1;
                setInputs(objects[objects.size() - 1], timeRate);
                addActivated = false;
                editActivated = true;
                header = frame.getName();
                editState = true;
                Utilities::numObjects++;
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &headerRect);

    SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);
    sidebarTxt.render(renderer, addTransitionOffset, 0);

    SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    float scrollableLength = sidebarHeight - ((float)sidebarHeight / scrollableHeight * sidebarHeight);
    float move = scroll / (float)maxScroll * scrollableLength;
    SDL_RenderDrawLine(renderer, sidebarWidth - 10 - addTransitionOffset, 5 + move, sidebarWidth - 10 - addTransitionOffset, 5 + (sidebarHeight / scrollableHeight * sidebarHeight) + move);
}

void Sidebar::editObject(Events events, Vector spawnLoc, vector<CelestialObject> &objects, double timeRate)
{
    headerTxt.loadFromRenderedText(renderer, bigFont, header, {177, 156, 217});
    headerTxt.setCoords(editRect.x + 10, 10);
    headerTxt.render(renderer);

    SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);

    nameTxt.setCoords(editRect.x + 15, headerTxt.y + headerTxt.getHeight() + 10);
    nameInput.setCoords(editRect.x + 15, nameTxt.y + nameTxt.getHeight() + 7);

    radiusTxt.setCoords(editRect.x + 15, nameInput.getRect()->y + nameInput.getRect()->h + 20);
    radiusHelperTxt.setCoords(editRect.x + 15, radiusTxt.y + radiusTxt.getHeight() + 5);
    radiusInput.setCoords(editRect.x + 15, radiusHelperTxt.y + radiusHelperTxt.getHeight() + 10);

    massTxt.setCoords(editRect.x + 15, radiusInput.getRect()->y + radiusInput.getRect()->h + 20);
    massHelperTxt.setCoords(editRect.x + 15, massTxt.y + massTxt.getHeight() + 5);
    massInput.setCoords(editRect.x + 15, massHelperTxt.y + massHelperTxt.getHeight() + 10);

    velocityTxt.setCoords(editRect.x + 15, massInput.getRect()->y + massInput.getRect()->h + 20);
    velocityHelperTxt.setCoords(editRect.x + 15, velocityTxt.y + velocityTxt.getHeight() + 5);
    velocityInput.setCoords(editRect.x + 15, velocityHelperTxt.y + velocityHelperTxt.getHeight() + 10);

    directionTxt.setCoords(editRect.x + 15, velocityInput.getRect()->y + velocityInput.getRect()->h + 20);
    degreeHelperTxt.setCoords(editRect.x + 15, directionTxt.y + directionTxt.getHeight() + 5);
    degreeInput.setCoords(editRect.x + 15, degreeHelperTxt.y + degreeHelperTxt.getHeight() + 10);

    deleteBtn.setCoords(editRect.x + (editRect.w / 3 - deleteBtn.getRect()->w / 2), sidebarHeight - deleteBtn.getRect()->h - 15);
    duplicateBtn.setCoords(editRect.x + ((editRect.w / 3 * 2)) - duplicateBtn.getRect()->w / 2, sidebarHeight - duplicateBtn.getRect()->h - 15);

    nameTxt.render(renderer);
    nameInput.render(events, sidebarWidth);

    radiusTxt.render(renderer);
    radiusHelperTxt.render(renderer);
    radiusInput.render(events, sidebarWidth);

    massTxt.render(renderer);
    massHelperTxt.render(renderer);
    massInput.render(events, sidebarWidth);

    velocityTxt.render(renderer);
    velocityHelperTxt.render(renderer);
    velocityInput.render(events, sidebarWidth);

    directionTxt.render(renderer);
    degreeHelperTxt.render(renderer);
    degreeInput.render(events, sidebarWidth);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect nameQuad = {nameInput.getRect()->x + nameInput.getRect()->w, nameInput.getRect()->y, sidebarWidth, nameInput.getRect()->h};
    SDL_RenderFillRect(renderer, &nameQuad);
    SDL_Rect radiusQuad = {radiusInput.getRect()->x + radiusInput.getRect()->w, radiusInput.getRect()->y, sidebarWidth, radiusInput.getRect()->h};
    SDL_RenderFillRect(renderer, &radiusQuad);
    SDL_Rect massQuad = {massInput.getRect()->x + massInput.getRect()->w, massInput.getRect()->y, sidebarWidth, massInput.getRect()->h};
    SDL_RenderFillRect(renderer, &massQuad);
    SDL_Rect velocityQuad = {velocityInput.getRect()->x + velocityInput.getRect()->w, velocityInput.getRect()->y, sidebarWidth, velocityInput.getRect()->h};
    SDL_RenderFillRect(renderer, &velocityQuad);
    SDL_Rect directionQuad = {degreeInput.getRect()->x + degreeInput.getRect()->w, degreeInput.getRect()->y, sidebarWidth, degreeInput.getRect()->h};
    SDL_RenderFillRect(renderer, &directionQuad);

    SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);
    kmTxt.setCoords((radiusInput.getRect()->x + radiusInput.getRect()->w + 5), radiusInput.getRect()->y + radiusInput.getRect()->h / 2 - kmTxt.getHeight() / 2);
    kmTxt.render(renderer);
    kgTxt.setCoords((massInput.getRect()->x + massInput.getRect()->w + 5), massInput.getRect()->y + massInput.getRect()->h / 2 - kgTxt.getHeight() / 2);
    kgTxt.render(renderer);
    msTxt.setCoords((velocityInput.getRect()->x + velocityInput.getRect()->w + 5), velocityInput.getRect()->y + velocityInput.getRect()->h / 2 - msTxt.getHeight() / 2);
    msTxt.render(renderer);
    degreeTxt.setCoords((degreeInput.getRect()->x + degreeInput.getRect()->w + 5), degreeInput.getRect()->y + degreeInput.getRect()->h / 2 - degreeTxt.getHeight() / 2);
    degreeTxt.render(renderer);

    double d = objects[editingObject].velocity.getAngle();
    int sliderX = editRect.x + 20 + (sidebarWidth - 40) / 2;
    int sliderY = degreeInput.getRect()->y + degreeInput.getRect()->h + 30;
    circle.setCoords(sliderX + ((sidebarWidth - 40) / 360) * d - circle.getWidth() / 2, sliderY - circle.getHeight() / 2);
    SDL_RenderDrawLine(renderer, sliderX + ((sidebarWidth - 40) / 360) * -180, sliderY, sliderX + ((sidebarWidth - 40) / 360) * 180, sliderY);
    SDL_RenderDrawLine(renderer, sliderX + ((sidebarWidth - 40) / 360) * -180, sliderY - 10, sliderX + ((sidebarWidth - 40) / 360) * -180, sliderY + 10);
    SDL_RenderDrawLine(renderer, sliderX + ((sidebarWidth - 40) / 360) * 180, sliderY - 10, sliderX + ((sidebarWidth - 40) / 360) * 180, sliderY + 10);
    SDL_RenderDrawLine(renderer, sliderX, sliderY - 5, sliderX, sliderY + 5);
    SDL_RenderDrawLine(renderer, sliderX + ((sidebarWidth - 40) / 360) * -90, sliderY - 5, sliderX + ((sidebarWidth - 40) / 360) * -90, sliderY + 5);
    SDL_RenderDrawLine(renderer, sliderX + ((sidebarWidth - 40) / 360) * 90, sliderY - 5, sliderX + ((sidebarWidth - 40) / 360) * 90, sliderY + 5);
    circle.render(renderer);

    if (sliderActivated)
    {
        d += events.mouseOffset.x;
        d = (d > 180) ? 180 : d;
        d = (d < -180) ? -180 : d;
        double radians = d * PI / 180;
        Vector direction = Vector(cos(radians), sin(radians));
        objects[editingObject].velocity = direction.normalize() * objects[editingObject].velocity.magnitude();
        degreeInput.setText(to_string((int)objects[editingObject].velocity.getAngle()), sidebarWidth);
    }

    SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);
    deleteBtn.render();
    duplicateBtn.render();

    SDL_Point point = events.getPoint();
    if (events.leftClick & SDL_PointInRect(&point, deleteBtn.getRect()) && SDL_PointInRect(&events.startClickPos, deleteBtn.getRect()))
    {
        if (editingObject != -1)
        {
            objects[editingObject].editing = false;
        }
        objects.erase(objects.begin() + editingObject);
        editingObject = -1;
        selectedObject = -1;
        editState = false;
        editActivated = false;
        addActivated = false;
    }
    else if (events.leftClick & SDL_PointInRect(&point, duplicateBtn.getRect()) && SDL_PointInRect(&events.startClickPos, duplicateBtn.getRect()))
    {
        if (editingObject != -1)
        {
            objects[editingObject].editing = false;
        }
        string name = "untitled-" + to_string(Utilities::numObjects);
        objects.push_back(objects[editingObject]);
        objects[objects.size() - 1].setName(renderer, name);
        objects[objects.size() - 1].position = spawnLoc;
        editingObject = objects.size() - 1;
        objects[editingObject].editing = true;
        setInputs(objects[objects.size() - 1], timeRate);
        addActivated = false;
        header = objects[objects.size() - 1].objType;
        Utilities::numObjects++;
    }
}

void Sidebar::distancesObject(Events events, vector<CelestialObject> &objects, double deltaTime)
{
    SDL_Point point = events.getPoint();

    if (SDL_PointInRect(&point, &editRect))
    {
        distanceScroll -= events.mouseWheel.y * 1200 * deltaTime;
        distanceScroll = (distanceScroll < 0) ? 0 : distanceScroll;
        distanceScroll = (distanceScroll > distanceMaxScroll) ? distanceMaxScroll : distanceScroll;
    }

    SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);
    distancesTxt.setCoords(editRect.x + 10, editRect.y + 10 - distanceScroll);
    int startY = (distancesTxt.getRect()->y + distanceScroll) + distancesTxt.getRect()->h + 10;
    distancesTxt.render(renderer);

    for (CelestialObject obj : objects)
    {
        if (obj.name != objects[editingObject].name)
        {
            distanceNameTxt.loadFromRenderedText(renderer, smallFont, obj.name + " - (" + obj.objType + ")", {255, 255, 255});
            distanceTxt.loadFromRenderedText(renderer, smallFont, to_string((int)Utilities::getRealDistance(obj.position.distance(objects[editingObject].position))) + " km", {255, 255, 255});
            distanceNameTxt.setCoords(editRect.x + 25, editRect.y + startY + 10 - distanceScroll);
            distanceTxt.setCoords(editRect.x + 25, distanceNameTxt.getRect()->y + distanceNameTxt.getRect()->h + 5);
            distanceNameTxt.render(renderer);
            distanceTxt.render(renderer);
            SDL_RenderDrawLine(renderer, editRect.x + 20, editRect.y + startY + 65 - distanceScroll, editRect.x + sidebarWidth - 20, editRect.y + startY + 65 - distanceScroll);
            startY += 65;
        }
    }
    distanceMaxScroll = startY - sidebarHeight - 65;
    distanceMaxScroll = (distanceMaxScroll < 0) ? 0 : distanceMaxScroll;
    float scrollableHeight = startY - 65 - 10;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    float scrollableLength = sidebarHeight - ((float)sidebarHeight / scrollableHeight * sidebarHeight);
    float move = distanceScroll / (float)distanceMaxScroll * scrollableLength;
    SDL_RenderDrawLine(renderer, width - 10, 5 + move, width - 10, 5 + (sidebarHeight / scrollableHeight * sidebarHeight) + move);
}

void Sidebar::runSidebar(Events events, Vector spawnLoc, vector<CelestialObject> &objects, double timeRate, double timeStep)
{
    SDL_Point point = events.getPoint();
    bool click = events.rightClick;

    if (editingObject != -1 && click)
    {
        SDL_Rect *rect = objects[editingObject].getRect();
        if (SDL_PointInRect(&point, rect))
        {
            if (editingObject != -1)
            {
                objects[editingObject].editing = false;
            }
            editingObject = -1;
            addActivated = false;
            editActivated = false;
            click = false;
        }
    }

    if ((click || events.holdingClick) && selectedObject == -1 && objects.size() > 0)
    {
        for (int i = 0; i < objects.size(); i++)
        {
            SDL_Rect *rect = objects[i].getRect();
            if (SDL_PointInRect(&point, rect))
            {
                if (events.holdingClick)
                {
                    selectedObject = i;
                }
                else if (click)
                {
                    if (editingObject != -1)
                    {
                        objects[editingObject].editing = false;
                    }
                    editingObject = i;
                    objects[editingObject].editing = true;
                    setInputs(objects[i], timeRate);
                    addActivated = false;
                    editActivated = true;
                    editState = true;
                    header = objects[i].objType;
                }
            }
        }
    }

    if (events.checkSpecialHoldKey(UP))
    {
        objects[editingObject].position.y -= 0.01 * timeStep;
    }
    else if (events.checkSpecialHoldKey(DOWN))
    {
        objects[editingObject].position.y += 0.01 * timeStep;
    }
    if (events.checkSpecialHoldKey(LEFT))
    {
        objects[editingObject].position.x -= 0.01 * timeStep;
    }
    else if (events.checkSpecialHoldKey(RIGHT))
    {
        objects[editingObject].position.x += 0.01 * timeStep;
    }

    if (!events.holdingClick)
    {
        sliderActivated = false;
        selectedObject = -1;
    }
    else if (events.holdingClick)
    {
        if (objects.size() > 0)
        {
            objects[selectedObject].position += events.mouseOffset;
        }
        if (SDL_PointInRect(&point, circle.getRect()))
        {
            sliderActivated = true;
        }
    }

    if (addState)
    {
        if (events.checkSpecialKey(SHIFT))
        {
            viewState = not viewState;
        }

        if (addActivated)
        {
            addTransitionOffset -= 1500 * timeStep;
            if (addTransitionOffset <= 0)
            {
                addTransitionOffset = 0;
            }
            headerRect.x = 0 - addTransitionOffset;
            addRect.x = 0 - addTransitionOffset;
        }
        else
        {
            addTransitionOffset += 1500 * timeStep;
            if (addTransitionOffset >= (width / 3 + 1))
            {
                addTransitionOffset = (width / 3 + 1);
            }
            headerRect.x = 0 - addTransitionOffset;
            addRect.x = 0 - addTransitionOffset;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &addRect);
        SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);

        if (SDL_PointInRect(&point, inArrow.getRect()) && SDL_PointInRect(&events.startClickPos, inArrow.getRect()) && events.leftClick)
        {
            addActivated = true;
        }
        if (SDL_PointInRect(&point, outArrow.getRect()) && SDL_PointInRect(&events.startClickPos, outArrow.getRect()) && events.leftClick)
        {
            addActivated = false;
        }

        if (viewState)
        {
            viewObjects(events, objects, timeStep, timeRate);
        }
        else
        {
            addObject(events, spawnLoc, objects, timeRate, timeStep);
        }

        if (!addActivated)
        {
            inArrow.setCoords(addRect.x + addRect.w, height / 2 - inArrow.getHeight() / 2);
            inArrow.render(renderer, 0, 0);
        }
        else
        {
            outArrow.setCoords(addRect.x + addRect.w, height / 2 - outArrow.getHeight() / 2);
            outArrow.render(renderer, 0, 0);
        }

        SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);
        SDL_RenderDrawRect(renderer, &addRect);
    }
    if (editState)
    {
        if (events.checkSpecialKey(TAB))
        {
            distanceState = not distanceState;
        }

        if (editActivated)
        {
            editTransitionOffset += 1500 * timeStep;
            if (editTransitionOffset >= 0)
            {
                editTransitionOffset = 0;
            }
            editRect.x = (width - (width / 3)) - editTransitionOffset;
        }
        else
        {
            editTransitionOffset -= 1500 * timeStep;
            if (editTransitionOffset <= -(width / 3))
            {
                editTransitionOffset = -(width / 3);
                editState = false;
            }
            editRect.x = (width - (width / 3)) - editTransitionOffset;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &editRect);
        SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);

        if (distanceState)
        {
            distancesObject(events, objects, timeStep);
        }
        else
        {
            editObject(events, spawnLoc, objects, timeRate);
            updateObject(events, objects, timeRate);
        }
        SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);
        SDL_RenderDrawRect(renderer, &editRect);
    }
}

bool Sidebar::checkHover(SDL_Point point, vector<CelestialObject> objects)
{
    for (int i = 0; i < objects.size(); i++)
    {
        SDL_Rect *rect = objects[i].getRect();
        if (SDL_PointInRect(&point, rect))
        {
            return true;
        }
    }

    if (addState)
    {
        for (Frame frame : frames)
        {
            SDL_Rect rect = frame.getRect();
            rect.x -= addTransitionOffset;
            rect.y -= scroll;

            if (SDL_PointInRect(&point, &rect) && !SDL_PointInRect(&point, &headerRect))
            {
                return true;
            }
        }
        if (!addActivated && SDL_PointInRect(&point, inArrow.getRect()))
        {
            return true;
        }
        else if (addActivated && SDL_PointInRect(&point, outArrow.getRect()))
        {
            return true;
        }
    }

    if (editState && !distanceState)
    {
        if (selectedObject != 0)
        {
            if (SDL_PointInRect(&point, nameInput.getRect()))
            {
                return true;
            }
        }
        if (SDL_PointInRect(&point, radiusInput.getRect()) || SDL_PointInRect(&point, massInput.getRect()) || SDL_PointInRect(&point, velocityInput.getRect()) || SDL_PointInRect(&point, degreeInput.getRect()))
        {
            return true;
        }
        if (SDL_PointInRect(&point, deleteBtn.getRect()) || SDL_PointInRect(&point, duplicateBtn.getRect()) || SDL_PointInRect(&point, circle.getRect()))
        {
            return true;
        }
    }

    return false;
}

void Sidebar::updateObject(Events events, vector<CelestialObject> &objects, double timeRate)
{
    if (events.input.length() > 0 || events.checkSpecialKey(BACKSPACE) || events.checkSpecialKey(SPACE) || sliderActivated)
    {
        if (nameInput.getText().length() == 0)
        {
            nameInput.error = true;
            nameInput.setErrorText("A name has to be entered.");
        }
        else
        {
            nameInput.error = false;
        }
        for (int i = 0; i < objects.size(); i++)
        {
            if (i != editingObject)
            {
                if (objects[i].name == nameInput.getText())
                {
                    nameInput.error = true;
                    nameInput.setErrorText("Name has been used already.");
                    break;
                }
                else if (nameInput.getText().length() != 0)
                {
                    nameInput.error = false;
                }
            }
        }
        if (!nameInput.error)
        {
            objects[editingObject].setName(renderer, nameInput.getText());
        }

        pair<string, string> radiusPair = parseInput(radiusInput.getText());
        pair<string, string> massPair = parseInput(massInput.getText());
        pair<string, string> velocityPair = parseInput(velocityInput.getText());
        string degree = degreeInput.getText();

        if (Utilities::validateRadius(radiusPair.first) && Utilities::validateRadius(radiusPair.second))
        {
            radiusInput.error = false;
            long double r = Utilities::scaleDistance(stold(radiusPair.first) * pow(10, stold(radiusPair.second)));
            if (r != objects[editingObject].radius)
            {
                (r != 0) ? objects[editingObject].updateRadius = r : radiusInput.error = true;
                objects[editingObject].updateSizeInstant(renderer);
            }
        }
        else
        {
            radiusInput.error = true;
            radiusInput.setErrorText("Invalid Radius Input.");
        }

        if (Utilities::validateMass(massPair.first) && Utilities::validateMass(massPair.second))
        {
            massInput.error = false;
            long double m = Utilities::scaleMass(stold(massPair.first) * pow(10, stold(massPair.second)));
            if (m != objects[editingObject].mass)
            {
                (m != 0) ? objects[editingObject].mass = m : massInput.error = true;
            }
        }
        else
        {
            massInput.error = true;
            massInput.setErrorText("Invalid Mass Input.");
        }

        if (Utilities::validateDirection(degree))
        {
            degreeInput.error = false;
            double radians = stod(degree) * PI / 180;
            Vector direction = Vector(cos(radians), sin(radians));
            if (direction != objects[editingObject].velocity.normalize())
            {
                objects[editingObject].velocity = direction.normalize() * objects[editingObject].velocity.magnitude();
            }
        }
        else
        {
            degreeInput.error = true;
            degreeInput.setErrorText("Invalid Direction Input.");
        }

        if (Utilities::validateVelocity(velocityPair.first) && Utilities::validateVelocity(velocityPair.second))
        {
            velocityInput.error = false;
            long double v = stold(velocityPair.first) * pow(10, stold(velocityPair.second));
            if (v != objects[editingObject].velocity.magnitude() / Utilities::getTimeRates()[timeRate].second)
            {
                if (objects[editingObject].velocity.magnitude() != 0)
                {
                    objects[editingObject].velocity = objects[editingObject].velocity.normalize() * Utilities::scaleDistance(v * Utilities::getTimeRates()[timeRate].second);
                }
                else
                {
                    objects[editingObject].velocity = Vector(0, Utilities::scaleDistance(v * Utilities::getTimeRates()[timeRate].second));
                }
            }
        }
        else
        {
            velocityInput.error = true;
            velocityInput.setErrorText("Invalid Velocity Input.");
        }
    }
}

void Sidebar::setInputs(CelestialObject &object, double timeRate)
{
    std::ostringstream ssRadius, ssMass, ssVelocity, ssDegree;
    ssRadius << Utilities::getRealDistance(object.radius);
    ssMass << Utilities::getRealMass(object.mass);
    ssVelocity << Utilities::getRealDistance(object.velocity.magnitude() / Utilities::getTimeRates()[timeRate].second);
    ssDegree << object.velocity.getAngle();

    clearInputs();
    string radiusExpForm = ssRadius.str();
    string massExpForm = ssMass.str();
    string velocityExpForm = ssVelocity.str();
    string degreeForm = ssDegree.str();
    nameInput.setText(object.name, sidebarWidth);
    radiusInput.setText(radiusExpForm, sidebarWidth);
    massInput.setText(massExpForm, sidebarWidth);
    velocityInput.setText(velocityExpForm, sidebarWidth);
    degreeInput.setText(degreeForm, sidebarWidth);
}

pair<string, string> Sidebar::parseInput(string input)
{
    string currentState = "Base";
    string base = "";
    string exponent = "";

    for (int x = 0; x < input.length(); x++)
    {
        if (isdigit(input[x]) || input[x] == '.' || input[x] == '-' || input[x] == '+')
        {
            (currentState == "Base") ? base += input[x] : exponent += input[x];
        }
        else if (input[x] == 'e' || input[x] == 'E')
        {
            currentState = "Exponent";
        }
    }
    if (exponent.length() == 0)
    {
        exponent = "0";
    }

    return make_pair(base.c_str(), exponent.c_str());
}

void Sidebar::clearInputs()
{
    nameInput.error = false;
    nameInput.activated = false;
    radiusInput.error = false;
    radiusInput.activated = false;
    massInput.error = false;
    massInput.activated = false;
    velocityInput.error = false;
    velocityInput.activated = false;
    degreeInput.error = false;
    degreeInput.activated = false;
}

void Sidebar::reset()
{
    scroll = 0;
    distanceScroll = 0;
    editingObject = -1;
    selectedObject = -1;
    addActivated = false;
    editActivated = false;
    sliderActivated = false;
    addState = true;
    editState = false;
    addTransitionOffset = width / 3;
    editTransitionOffset = -(width / 3);
    addRect = {0 - addTransitionOffset, 0, width / 3, height};
    editRect = {(width - (width / 3)) - editTransitionOffset, 0, width / 3, height};
    headerRect = {0 - addTransitionOffset, 0, sidebarWidth, (int)sidebarTxt.getHeight() + 20};
}
