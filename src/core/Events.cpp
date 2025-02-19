#pragma once
#include <Events.h>

Events::Events()
{
    alphaNums = "abcdefghijklmnopqrstuvwxyz1234567890.";
    quit = false;
    leftClick = false;
    rightClick = false;
    holdingClick = false;
    mousePos = Vector(0, 0);
    mouseOffset = Vector(0, 0);
    mouseWheel = Vector(0, 0);
    startClickPos = {0, 0};
    input = "";
    keys = SDL_GetKeyboardState(NULL);
}

void Events::getEvents(SDL_Event event)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    mouseOffset = Vector(x, y) - mousePos;
    mousePos.x = x;
    mousePos.y = y;

    SDL_StartTextInput();
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            quit = true;
        }
        else if (event.type == SDL_MOUSEWHEEL)
        {
            mouseWheel.x = event.wheel.x;
            mouseWheel.y = event.wheel.y;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                startClickPos = {(int)mousePos.x, (int)mousePos.y};
                holdingClick = true;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                leftClick = true;
            }
            else if (event.button.button == SDL_BUTTON_RIGHT)
            {
                rightClick = true;
            }
            holdingClick = false;
        }
        else if (event.type == SDL_TEXTINPUT)
        {
            input += event.text.text;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_w)
            {
                specialKeys.push_back(W);
            }
            if (event.key.keysym.sym == SDLK_s)
            {
                specialKeys.push_back(S);
            }
            if (event.key.keysym.sym == SDLK_a)
            {
                specialKeys.push_back(A);
            }
            if (event.key.keysym.sym == SDLK_d)
            {
                specialKeys.push_back(D);
            }
            if (event.key.keysym.sym == SDLK_m)
            {
                specialKeys.push_back(M);
            }
            if (event.key.keysym.sym == SDLK_TAB)
            {
                specialKeys.push_back(TAB);
            }
            if (event.key.keysym.sym == SDLK_SPACE)
            {
                specialKeys.push_back(SPACE);
            }
            if (event.key.keysym.sym == SDLK_LCTRL || event.key.keysym.sym == SDLK_RCTRL)
            {
                specialKeys.push_back(CTRL);
            }
            if (event.key.keysym.sym == SDLK_UP)
            {
                specialKeys.push_back(UP);
            }
            if (event.key.keysym.sym == SDLK_DOWN)
            {
                specialKeys.push_back(DOWN);
            }
            if (event.key.keysym.sym == SDLK_LEFT)
            {
                specialKeys.push_back(LEFT);
            }
            if (event.key.keysym.sym == SDLK_RIGHT)
            {
                specialKeys.push_back(RIGHT);
            }
            if (event.key.keysym.sym == SDLK_BACKSPACE)
            {
                specialKeys.push_back(BACKSPACE);
            }
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                specialKeys.push_back(ESCAPE);
            }
            if (event.key.keysym.sym == SDLK_RETURN)
            {
                specialKeys.push_back(ENTER);
            }
            if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT)
            {
                specialKeys.push_back(SHIFT);
            }
        }
    }

    if (keys[SDL_SCANCODE_W])
    {
        specialHoldKeys.push_back(W);
    }
    if (keys[SDL_SCANCODE_S])
    {
        specialHoldKeys.push_back(S);
    }
    if (keys[SDL_SCANCODE_A])
    {
        specialHoldKeys.push_back(A);
    }
    if (keys[SDL_SCANCODE_D])
    {
        specialHoldKeys.push_back(D);
    }
    if (keys[SDL_SCANCODE_M])
    {
        specialHoldKeys.push_back(M);
    }
    if (keys[SDL_SCANCODE_TAB])
    {
        specialHoldKeys.push_back(TAB);
    }
    if (keys[SDL_SCANCODE_SPACE])
    {
        specialHoldKeys.push_back(SPACE);
    }
    if (keys[SDL_SCANCODE_LCTRL] || keys[SDL_SCANCODE_RCTRL])
    {
        specialHoldKeys.push_back(CTRL);
    }
    if (keys[SDL_SCANCODE_UP])
    {
        specialHoldKeys.push_back(UP);
    }
    if (keys[SDL_SCANCODE_DOWN])
    {
        specialHoldKeys.push_back(DOWN);
    }
    if (keys[SDL_SCANCODE_LEFT])
    {
        specialHoldKeys.push_back(LEFT);
    }
    if (keys[SDL_SCANCODE_RIGHT])
    {
        specialHoldKeys.push_back(RIGHT);
    }
    if (keys[SDL_SCANCODE_BACKSPACE])
    {
        specialHoldKeys.push_back(BACKSPACE);
    }
    if (keys[SDL_SCANCODE_ESCAPE])
    {
        specialHoldKeys.push_back(ESCAPE);
    }
    if (keys[SDL_SCANCODE_RETURN])
    {
        specialHoldKeys.push_back(ENTER);
    }
    if (keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT])
    {
        specialHoldKeys.push_back(SHIFT);
    }
}

bool Events::checkSpecialKey(Keys key)
{
    if (std::find(specialKeys.begin(), specialKeys.end(), key) != specialKeys.end())
    {
        return true;
    }
    return false;
}

bool Events::checkSpecialHoldKey(Keys key)
{
    if (std::find(specialHoldKeys.begin(), specialHoldKeys.end(), key) != specialHoldKeys.end())
    {
        return true;
    }
    return false;
}

SDL_Point Events::getPoint()
{
    SDL_Point point = {(int)mousePos.x, (int)mousePos.y};
    return point;
}

void Events::clear()
{
    specialKeys.clear();
    specialHoldKeys.clear();
    input = "";
    leftClick = false;
    rightClick = false;
}
