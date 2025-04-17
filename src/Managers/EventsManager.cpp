#include <EventsManager.h>

EventsManager::EventsManager()
{
    quit = false;
    keys = SDL_GetKeyboardState(NULL);
    mousePos = Vector(0, 0);
    mouseOffset = Vector(0, 0);
    mouseWheel = Vector(0, 0);
    holdingClick = false;
    changedWidth = 0;
    changedHeight = 0;
    input = "";
    isCTRL = false;
}

std::vector<Event> EventsManager::getKeyEvents()
{
    return keyEvents;
}

bool EventsManager::userQuit()
{
    return quit;
}

void EventsManager::getEvents()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    mouseOffset = Vector(x, y) - mousePos;
    mousePos = Vector(x, y);

    if (keys[SDL_SCANCODE_LCTRL])
    {
        isCTRL = true;
    }

    SDL_StartTextInput();
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            quit = true;
        }
        else if (event.type == SDL_TEXTINPUT)
        {
            input += event.text.text;
        }
        else if (event.type == SDL_MOUSEWHEEL)
        {
            mouseWheel.x = event.wheel.x;
            mouseWheel.y = event.wheel.y;
        }
        if (event.type == SDL_WINDOWEVENT)
        {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                changedWidth = event.window.data1;
                changedHeight = event.window.data2;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                startClickPos = {(int)mousePos.x, (int)mousePos.y};
                leftClick = true;
                holdingClick = true;
            }
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                rightClick = true;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                leftClickUp = true;
                holdingClick = false;
            }
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_SPACE)
            {
                keyEvents.push_back(SPACE);
            }
            if (event.key.keysym.sym == SDLK_TAB)
            {
                keyEvents.push_back(TAB);
            }
            if (event.key.keysym.sym == SDLK_RETURN)
            {
                keyEvents.push_back(ENTER);
            }
            if (event.key.keysym.sym == SDLK_BACKSPACE)
            {
                keyEvents.push_back(BACKSPACE);
            }
            if (event.key.keysym.sym == SDLK_UP)
            {
                keyEvents.push_back(UPARROW);
            }
            if (event.key.keysym.sym == SDLK_DOWN)
            {
                keyEvents.push_back(DOWNARROW);
            }
            if (event.key.keysym.sym == SDLK_LEFT)
            {
                keyEvents.push_back(LEFTARROW);
            }
            if (event.key.keysym.sym == SDLK_RIGHT)
            {
                keyEvents.push_back(RIGHTARROW);
            }
            if (event.key.keysym.sym == SDLK_m)
            {
                keyEvents.push_back(M);
            }
            if (event.key.keysym.sym == SDLK_LCTRL || event.key.keysym.sym == SDLK_RCTRL)
            {
                keyEvents.push_back(CTRL);
            }
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                keyEvents.push_back(ESCAPE);
            }
        }
    }
}

string EventsManager::getInput()
{
    return input;
}

bool EventsManager::isLeftClick()
{
    return leftClick;
}

bool EventsManager::isRightClick()
{
    return rightClick;
}

bool EventsManager::isLeftClickUp()
{
    return leftClickUp;
}

bool EventsManager::isHoldingClick()
{
    return holdingClick;
}

SDL_Point EventsManager::getPoint()
{
    SDL_Point point = {(int)mousePos.x, (int)mousePos.y};
    return point;
}

SDL_Point EventsManager::getStartClickPos()
{
    return startClickPos;
}

Vector EventsManager::getMouseOffset()
{
    return mouseOffset;
}

Vector EventsManager::getMousePos()
{
    return mousePos;
}

Vector EventsManager::getMouseWheel()
{
    return mouseWheel;
}

bool EventsManager::changedWindowSize()
{
    return changedWidth != 0 && changedHeight != 0;
}

int EventsManager::getWidth()
{
    return changedWidth;
}

int EventsManager::getHeight()
{
    return changedHeight;
}

bool EventsManager::checkSpecialKey(Event key)
{
    if (std::find(keyEvents.begin(), keyEvents.end(), key) != keyEvents.end())
    {
        return true;
    }
    return false;
}

bool EventsManager::checkCtrlKey(Event key)
{
    if (isCTRL && std::find(keyEvents.begin(), keyEvents.end(), key) != keyEvents.end())
    {
        return true;
    }
    return false;
}

void EventsManager::clear()
{
    mouseWheel = Vector(0, 0);
    keyEvents.clear();
    rightClick = false;
    leftClick = false;
    leftClickUp = false;
    changedWidth = 0;
    changedHeight = 0;
    input = "";
    isCTRL = false;
}
