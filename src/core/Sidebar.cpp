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
    editScroll = 0;
    editMaxScroll = 0;
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
    particlesTxt.loadFromRenderedText(renderer, font, "Particle", {255, 255, 255});
    particlesHelperTxt.loadFromRenderedText(renderer, smallFont, "Choose Whether You Want Particles", {211, 211, 211});
    kmTxt.loadFromRenderedText(renderer, smallFont, "km", {255, 255, 255});
    kgTxt.loadFromRenderedText(renderer, smallFont, "kg", {255, 255, 255});
    msTxt.loadFromRenderedText(renderer, smallFont, "m/s", {255, 255, 255});
    degreeTxt.loadFromRenderedText(renderer, smallFont, "degrees", {255, 255, 255});
    circle.loadFromFile(renderer, "assets/circle.png", 0.25);
    check.loadFromFile(renderer, "assets/check.png", 1);

    nameInput = TextInput(renderer, "", {255, 255, 255});
    radiusInput = TextInput(renderer, "", {255, 255, 255});
    massInput = TextInput(renderer, "", {255, 255, 255});
    velocityInput = TextInput(renderer, "", {255, 255, 255});
    degreeInput = TextInput(renderer, "", {255, 255, 255});

    deleteBtn = Button(renderer, "Delete", {255, 255, 255}, 100);
    duplicateBtn = Button(renderer, "Duplicate", {255, 255, 255}, 100);

    CheckBox checkBox = CheckBox(renderer, false, 30);

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
    if (addState || viewState)
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

    for (vector<string> obj : Utilities::getStellarObjects())
    {
        for (int ii = 0; ii < obj.size(); ii++)
        {
            if (ii == 0)
            {
                Image text;
                text.loadFromRenderedText(renderer, font, obj[ii], color);
                text.setCoords(startX, startY);
                headers.push_back(text);
                startX = 0;
                startY += text.getHeight() + 10;
            }
            else
            {
                Frame frame = Frame(renderer, obj[ii], Utilities::getStellarFiles()[i][ii], startX, startY, size, size);
                frames.push_back(frame);
                startX = (startX == size) ? 0 : startX + size;
                startY = (startX == 0) ? startY + size : startY;

                if (ii == obj.size() - 1 && obj.size() % 2 == 0)
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

void Sidebar::viewObjects(Events events, Simulation &sim, Vector &panningOffset, double deltaTime, double timeRate)
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

    for (int i = 0; i < sim.objects.size(); i++)
    {
        SDL_Rect rect = {20 - addTransitionOffset, startY - viewScroll, sidebarWidth - 40, 65};
        (SDL_PointInRect(&point, &rect))
            ? SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50)
            : SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        sim.objects[i].editing = false;
        if (events.leftClick && (SDL_PointInRect(&events.startClickPos, &rect)))
        {
            sim.objects[i].editing = true;
            editingObject = sim.objects.size() - 1;
            setInputs(sim, sim.objects[i], timeRate);
            addActivated = false;
            editActivated = true;
            header = sim.objects[i].name;
            editState = true;
            panningOffset = Vector(-sim.objects[i].position.x, -sim.objects[i].position.y);
            panningOffset.x += width / 2.0;
            panningOffset.y += height / 2.0;
            viewState = false;
        }
        SDL_RenderFillRect(renderer, &rect);
        objectTxt.loadFromRenderedText(renderer, smallFont, sim.objects[i].name, {177, 156, 217});
        objectTypeTxt.loadFromRenderedText(renderer, smallFont, sim.objects[i].objType, {255, 255, 255});
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

void Sidebar::addObject(Events events, Vector spawnLoc, Simulation &sim, double timeRate, double deltaTime)
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
                for (CelestialObject &obj : sim.objects)
                {
                    obj.editing = false;
                }
                string name = "untitled-" + to_string(sim.objectsNum);
                CelestialObject obj = CelestialObject(renderer, width, height, name, frame.getName(), frame.getFile());
                obj.ctrl = true;
                obj.editing = true;
                obj.position = spawnLoc;
                sim.objects.push_back(obj);
                sim.scaleObjects(obj.name);
                editingObject = sim.objects.size() - 1;
                setInputs(sim, sim.objects[sim.objects.size() - 1], timeRate);
                addActivated = false;
                editActivated = true;
                editState = true;
                header = frame.getName();
                sim.objectsNum++;
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

void Sidebar::editObject(Events events, Vector spawnLoc, Simulation &sim, double timeRate, double deltaTime)
{
    float scrollableHeight;
    SDL_Point point = events.getPoint();

    if (SDL_PointInRect(&point, &editRect))
    {
        editScroll -= events.mouseWheel.y * 100 * deltaTime;
        editScroll = (editScroll < 0) ? 0 : editScroll;
        editScroll = (editScroll > editMaxScroll) ? editMaxScroll : editScroll;
    }

    headerTxt.loadFromRenderedText(renderer, bigFont, header, {177, 156, 217});
    headerTxt.setCoords(editRect.x + 10, 10 - editScroll);
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

    double d = sim.objects[editingObject].velocity.getAngle();
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

    particlesTxt.setCoords(editRect.x + 15, circle.y + circle.getHeight() + 20);
    particlesHelperTxt.setCoords(editRect.x + 15, particlesTxt.y + particlesTxt.getHeight() + 5);
    particlesTxt.render(renderer);
    particlesHelperTxt.render(renderer);

    SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);
    checkBox.setCoords(editRect.x + 20, particlesHelperTxt.y + particlesHelperTxt.getHeight() + 20);
    SDL_RenderDrawRect(renderer, checkBox.getRect());
    if (checkBox.toggle)
    {
        check.setCoords(checkBox.getRect()->x + checkBox.getRect()->w / 2 - check.getWidth() / 2, checkBox.getRect()->y + checkBox.getRect()->h / 2 - check.getHeight() / 2);
        check.render(renderer);
    }
    if (events.leftClick & SDL_PointInRect(&point, checkBox.getRect()) && SDL_PointInRect(&events.startClickPos, checkBox.getRect()))
    {
        checkBox.toggle = !checkBox.toggle;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    double startY = checkBox.getRect()->y + checkBox.getRect()->h + 250;
    scrollableHeight = startY - (headerTxt.getHeight() + 20);
    editMaxScroll = startY - sidebarHeight - (headerTxt.getHeight() + 20);
    editMaxScroll = (editMaxScroll < 0) ? 0 : editMaxScroll;

    if (sliderActivated)
    {
        d += events.mouseOffset.x;
        d = (d > 180) ? 180 : d;
        d = (d < -180) ? -180 : d;
        double radians = d * PI / 180;
        Vector direction = Vector(cos(radians), sin(radians));
        sim.objects[editingObject].velocity = direction.normalize() * sim.objects[editingObject].velocity.magnitude();
        degreeInput.setText(to_string((int)sim.objects[editingObject].velocity.getAngle()), sidebarWidth);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect rect = {editRect.x, duplicateBtn.getRect()->y - 10, sidebarWidth, sidebarHeight - (duplicateBtn.getRect()->y - 10)};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);
    deleteBtn.render();
    duplicateBtn.render();

    if (events.leftClick & SDL_PointInRect(&point, deleteBtn.getRect()) && SDL_PointInRect(&events.startClickPos, deleteBtn.getRect()))
    {
        if (editingObject != -1)
        {
            sim.objects[editingObject].editing = false;
        }
        sim.objects[editingObject].object.freeAll();
        sim.objects.erase(sim.objects.begin() + editingObject);
        editingObject = -1;
        selectedObject = -1;
        editState = false;
        editActivated = false;
        addActivated = false;
    }
    else if (events.leftClick & SDL_PointInRect(&point, duplicateBtn.getRect()) && SDL_PointInRect(&events.startClickPos, duplicateBtn.getRect()))
    {
        if (!nameInput.error && !massInput.error && !radiusInput.error && !velocityInput.error && !degreeInput.error)
        {
            if (editingObject != -1)
            {
                sim.objects[editingObject].editing = false;
            }
            string name = "untitled-" + to_string(sim.objectsNum);
            sim.objects.push_back(sim.objects[editingObject]);
            sim.objects[sim.objects.size() - 1].setName(renderer, name);
            sim.objects[sim.objects.size() - 1].position = spawnLoc;
            editingObject = sim.objects.size() - 1;
            sim.objects[editingObject].editing = true;
            setInputs(sim, sim.objects[sim.objects.size() - 1], timeRate);
            addActivated = false;
            header = sim.objects[sim.objects.size() - 1].objType;
            sim.objectsNum++;
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    float scrollableLength = sidebarHeight - ((float)sidebarHeight / scrollableHeight * sidebarHeight);
    float move = editScroll / (float)editMaxScroll * scrollableLength;
    SDL_RenderDrawLine(renderer, editRect.x + sidebarWidth - 10, 5 + move, editRect.x + sidebarWidth - 10, (sidebarHeight / scrollableHeight * sidebarHeight) + move - 5);
}

void Sidebar::distancesObject(Events events, Simulation &sim, double deltaTime)
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

    for (CelestialObject obj : sim.objects)
    {
        if (obj.name != sim.objects[editingObject].name)
        {
            distanceNameTxt.loadFromRenderedText(renderer, smallFont, obj.name + " - (" + obj.objType + ")", {255, 255, 255});
            distanceTxt.loadFromRenderedText(renderer, smallFont, to_string((int)sim.getRealDistance(obj.position.distance(sim.objects[editingObject].position))) + " km", {255, 255, 255});
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

void Sidebar::runSidebar(Events events, Vector spawnLoc, Simulation &sim, Vector &panningOffset, double timeRate, double timeStep)
{
    SDL_Point point = events.getPoint();
    bool click = events.rightClick;

    if (editingObject != -1 && click)
    {
        SDL_Rect *rect = sim.objects[editingObject].getRect();
        if (SDL_PointInRect(&point, rect))
        {
            if (editingObject != -1)
            {
                sim.objects[editingObject].editing = false;
            }
            editingObject = -1;
            addActivated = false;
            editActivated = false;
            click = false;
        }
    }

    if ((click || events.holdingClick) && selectedObject == -1 && sim.objects.size() > 0)
    {
        for (int i = 0; i < sim.objects.size(); i++)
        {
            SDL_Rect *rect = sim.objects[i].getRect();
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
                        sim.objects[editingObject].editing = false;
                    }
                    editingObject = i;
                    sim.objects[editingObject].editing = true;
                    setInputs(sim, sim.objects[i], timeRate);
                    addActivated = false;
                    editActivated = true;
                    editState = true;
                    header = sim.objects[i].objType;
                }
            }
        }
    }

    if (events.checkSpecialHoldKey(UP))
    {
        sim.objects[editingObject].position.y -= 0.01 * timeStep;
    }
    else if (events.checkSpecialHoldKey(DOWN))
    {
        sim.objects[editingObject].position.y += 0.01 * timeStep;
    }
    if (events.checkSpecialHoldKey(LEFT))
    {
        sim.objects[editingObject].position.x -= 0.01 * timeStep;
    }
    else if (events.checkSpecialHoldKey(RIGHT))
    {
        sim.objects[editingObject].position.x += 0.01 * timeStep;
    }

    if (!events.holdingClick)
    {
        sliderActivated = false;
        selectedObject = -1;
    }
    else if (events.holdingClick)
    {
        if (sim.objects.size() > 0)
        {
            sim.objects[selectedObject].position += events.mouseOffset;
        }
        if (SDL_PointInRect(&point, circle.getRect()))
        {
            sliderActivated = true;
        }
    }

    if (addState || viewState)
    {
        if (events.checkSpecialKey(SHIFT))
        {
            viewState = !viewState;
        }

        if (SDL_PointInRect(&point, inArrow.getRect()) && SDL_PointInRect(&events.startClickPos, inArrow.getRect()) && events.leftClick)
        {
            addActivated = true;
        }
        else if (SDL_PointInRect(&point, outArrow.getRect()) && SDL_PointInRect(&events.startClickPos, outArrow.getRect()) && events.leftClick)
        {
            addActivated = false;
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

        if (viewState)
        {
            viewObjects(events, sim, panningOffset, timeStep, timeRate);
        }
        else
        {
            addObject(events, spawnLoc, sim, timeRate, timeStep);
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
            distancesObject(events, sim, timeStep);
        }
        else
        {
            editObject(events, spawnLoc, sim, timeRate, timeStep);
            updateObject(events, sim, timeRate);
        }
        SDL_SetRenderDrawColor(renderer, 177, 156, 217, 255);
        SDL_RenderDrawRect(renderer, &editRect);
    }
}

bool Sidebar::checkHover(SDL_Point point, Simulation sim)
{
    for (int i = 0; i < sim.objects.size(); i++)
    {
        SDL_Rect *rect = sim.objects[i].getRect();
        if (SDL_PointInRect(&point, rect))
        {
            return true;
        }
    }

    if (viewState)
    {
        int startY = (sidebarTxt.getRect()->y + viewScroll) + sidebarTxt.getRect()->h + 10;
        for (int i = 0; i < sim.objects.size(); i++)
        {
            SDL_Rect rect = {20 - addTransitionOffset, startY - viewScroll, sidebarWidth - 40, 65};
            startY += 70;
            if (SDL_PointInRect(&point, &rect))
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
    else if (addState)
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
        if (SDL_PointInRect(&point, checkBox.getRect()) || SDL_PointInRect(&point, deleteBtn.getRect()) || SDL_PointInRect(&point, duplicateBtn.getRect()) || SDL_PointInRect(&point, circle.getRect()))
        {
            return true;
        }
    }

    return false;
}

void Sidebar::updateObject(Events events, Simulation &sim, double timeRate)
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
        for (int i = 0; i < sim.objects.size(); i++)
        {
            if (i != editingObject)
            {
                if (sim.objects[i].name == nameInput.getText())
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
            sim.objects[editingObject].setName(renderer, nameInput.getText());
        }

        pair<string, string> radiusPair = Utilities::parseInput(radiusInput.getText());
        pair<string, string> massPair = Utilities::parseInput(massInput.getText());
        pair<string, string> velocityPair = Utilities::parseInput(velocityInput.getText());
        string degree = degreeInput.getText();

        if (Utilities::validateRadius(radiusPair.first) && Utilities::validateRadius(radiusPair.second))
        {
            radiusInput.error = false;
            long double r = sim.scaleDistance(stold(radiusPair.first) * pow(10, stold(radiusPair.second)));
            if (r != sim.objects[editingObject].radius)
            {
                (r > 0) ? sim.objects[editingObject].updateRadius = r : radiusInput.error = true;
                (r > 0) ? sim.objects[editingObject].radius = r : radiusInput.error = true;
                sim.objects[editingObject].updateSizeInstant(renderer);
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
            long double m = sim.scaleMass(stold(massPair.first) * pow(10, stold(massPair.second)));
            if (m != sim.objects[editingObject].mass)
            {
                (m > 0) ? sim.objects[editingObject].mass = m : massInput.error = true;
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
            if (direction != sim.objects[editingObject].velocity.normalize())
            {
                sim.objects[editingObject].velocity = direction.normalize() * sim.objects[editingObject].velocity.magnitude();
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
            if (v != sim.objects[editingObject].velocity.magnitude() / Utilities::getTimeRates()[timeRate].second)
            {
                if (sim.objects[editingObject].velocity.magnitude() != 0)
                {
                    sim.objects[editingObject].velocity = sim.objects[editingObject].velocity.normalize() * sim.scaleDistance(v * Utilities::getTimeRates()[timeRate].second);
                }
                else
                {
                    sim.objects[editingObject].velocity = Vector(0, sim.scaleDistance(v * Utilities::getTimeRates()[timeRate].second));
                }
            }
        }
        else
        {
            velocityInput.error = true;
            velocityInput.setErrorText("Invalid Velocity Input.");
        }
    }

    sim.objects[editingObject].toggleParticles = checkBox.toggle;
}

void Sidebar::setInputs(Simulation sim, CelestialObject &object, double timeRate)
{
    std::ostringstream ssRadius, ssMass, ssVelocity, ssDegree;
    ssRadius << sim.getRealDistance(object.radius);
    ssMass << sim.getRealMass(object.mass);
    ssVelocity << sim.getRealDistance(object.velocity.magnitude() / Utilities::getTimeRates()[timeRate].second);
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
    checkBox.setToggle(object.toggleParticles);
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
    editScroll = 0;
    editMaxScroll = 0;
    distanceScroll = 0;
    editingObject = -1;
    selectedObject = -1;
    addActivated = false;
    editActivated = false;
    sliderActivated = false;
    addState = true;
    viewState = false;
    editState = false;
    addTransitionOffset = width / 3;
    editTransitionOffset = -(width / 3);
    addRect = {0 - addTransitionOffset, 0, width / 3, height};
    editRect = {(width - (width / 3)) - editTransitionOffset, 0, width / 3, height};
    headerRect = {0 - addTransitionOffset, 0, sidebarWidth, (int)sidebarTxt.getHeight() + 20};
}
