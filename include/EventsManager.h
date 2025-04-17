#pragma once
#include <iostream>
#include "SDL.h"
#include <Vector.h>
#include <vector>
#include "algorithm"

enum Event
{
    RIGHTCLICK,
    LEFTCLICK,
    HOLDINGCLICK,
    W,
    A,
    S,
    D,
    M,
    UPARROW,
    DOWNARROW,
    RIGHTARROW,
    LEFTARROW,
    BACKSPACE,
    SPACE,
    ENTER,
    TAB,
    CTRL,
    ESCAPE
};

class EventsManager
{
private:
    int changedWidth, changedHeight;
    SDL_Event event;
    std::vector<Event> keyEvents;
    SDL_Point startClickPos;
    bool holdingClick, leftClick, rightClick, leftClickUp, quit;
    Vector mousePos, mouseOffset, mouseWheel, startingPos;
    const Uint8 *keys;
    string input;
    bool isCTRL;

public:
    EventsManager();
    std::vector<Event> getKeyEvents();
    bool userQuit();
    void getEvents();
    string getInput();
    Vector getMouseOffset();
    Vector getMousePos();
    Vector getMouseWheel();
    SDL_Point getStartClickPos();
    SDL_Point getPoint();
    bool isLeftClick();
    bool isRightClick();
    bool isLeftClickUp();
    bool isHoldingClick();
    bool changedWindowSize();
    int getWidth();
    int getHeight();
    bool checkSpecialKey(Event key);
    bool checkCtrlKey(Event key);
    void clear();
};
