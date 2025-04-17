#pragma once
#include "SDL.h"
#include "Input.h"
#include "Button.h"
#include "Scrollbar.h"
#include "vector"
#include "Vector.h"

class Form
{
private:
    SDL_Renderer *renderer;
    vector<Input *> inputs;
    Button submitButton;
    float width, height, x, y;
    bool enabledButton;
    SDL_Rect rect;

    Scrollbar scrollbar;

public:
    Form();
    Form(SDL_Renderer *renderer, float width, float height);
    vector<Input *> getInputs();
    void updateSize(float width, float height);
    void setPos(float x, float y);
    void enableButton();
    void disableButton();
    void setSubmitButton(Button button);
    void setButtonText(string text);
    void setInputs(vector<Input *> inputs);
    void clear();
    void displayInputs(EventsManager eventManager, float width, float height, Vector size = Vector(0, 0));
    bool hovering(EventsManager eventManager);
    bool submit(EventsManager eventManager);
};
