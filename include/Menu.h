#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <cmath>
#include <Events.h>
#include <Image.h>
#include <Vector.h>
#include <Animation.h>
#include <Transition.h>
#include <Particles.h>
#include <Timer.h>

using namespace std;

class Menu
{
private:
    SDL_Renderer *renderer;
    string title;
    int width, height;

    Mix_Chunk *clickSound;

    Animation titleTxtAnimation;
    Image pointerCursor, handCursor, backgroundImg, titleTxt, newTxt, loadTxt, settingsTxt, quitTxt, newHoverTxt, loadHoverTxt, controlsTxt, controlsHoverTxt, quitHoverTxt;
    bool hovering;

    Timer stepTimer;
    double timeStep;

public:
    Transition transition;
    Menu();
    Menu(SDL_Renderer *renderer, string title, int width, int height);
    void drawCursor(int mouseX, int mouseY);
    string runMenu(Events events, string state);
};
