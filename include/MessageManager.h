#pragma once
#include <iostream>
#include <SDL.h>
#include <vector>
#include <Timer.h>
#include <string>
#include <Utilities.h>

using namespace std;

class MessageManager
{
private:
    float width, height;
    vector<string> messages;
    vector<Timer *> messageTimers;

public:
    MessageManager(float width, float height);
    void resizeDimensions(float width, float height);
    void addMessage(string msg);
    void displayMessages(SDL_Renderer *renderer);
};