#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <cmath>
#include <Image.h>
#include <Vector.h>
#include <Animation.h>
#include <Transition.h>

using namespace std;

class Menu
{
private:
    SDL_Renderer *renderer;
    string title;
    int width, height;

    Animation titleTxtAnimation;
    Image pointerCursor, handCursor, backgroundImg, titleTxt, newTxt, loadTxt, settingsTxt, quitTxt, newHoverTxt, loadHoverTxt, settingsHoverTxt, quitHoverTxt;
    bool hovering;

public:
    Transition transition;
    Menu();
    Menu(SDL_Renderer *renderer, string title, int width, int height);
    void drawCursor(int mouseX, int mouseY);
    string runMenu(string state, bool click);
};
