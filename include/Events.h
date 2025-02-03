#pragma once
#include <iostream>
#include <string>
#include <SDL.h>
#include <Vector.h>
#include <bits/stdc++.h>
#include <vector>

using namespace std;

enum Keys
{
    CTRL,
    TAB,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    BACKSPACE,
    SPACE,
    ESCAPE,
    ENTER,
    SHIFT,
    NONE
};

class Events
{
public:
    string alphaNums;
    bool quit;
    bool leftClick, rightClick, holdingClick;
    vector<Keys> specialHoldKeys, specialKeys;
    Vector mousePos, mouseOffset, mouseWheel;
    SDL_Point startClickPos;
    string input;
    const Uint8 *keys;

    Events();
    void getEvents(SDL_Event event);
    bool checkSpecialKey(Keys key);
    bool checkSpecialHoldKey(Keys key);
    SDL_Point getPoint();
    void clear();
};