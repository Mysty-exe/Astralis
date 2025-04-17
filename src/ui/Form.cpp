#include "Form.h"

Form::Form()
{
}

Form::Form(SDL_Renderer *renderer, float width, float height)
{
    this->renderer = renderer;
    this->width = width;
    this->height = height;
    enabledButton = true;

    rect = {0, 0, (int)width, (int)height};

    scrollbar = Scrollbar(width, height);
}

void Form::updateSize(float width, float height)
{
    this->width = width;
    this->height = height;
}

void Form::setPos(float x, float y)
{
    this->x = x;
    this->y = y;
    rect = {0, 0, (int)width, (int)height};
}

void Form::enableButton()
{
    enabledButton = true;
}

void Form::disableButton()
{
    enabledButton = false;
}

void Form::setSubmitButton(Button button)
{
    this->submitButton = button;
}

void Form::setButtonText(string text)
{
    submitButton.setText(text);
}

vector<Input *> Form::getInputs()
{
    return inputs;
}

void Form::setInputs(vector<Input *> inputs)
{
    this->inputs = inputs;
}

void Form::clear()
{
    for (Input *input : inputs)
    {
        input->setText("");
        input->deactivate();
        input->clearError();
    }
}

void Form ::displayInputs(EventsManager eventManager, float w, float h, Vector size)
{
    SDL_Point point = eventManager.getPoint();

    int tempY = y - scrollbar.getScroll();
    int index = 0;
    bool tab = eventManager.checkSpecialKey(TAB);
    for (Input *input : inputs)
    {
        if (tab && input->getActivated())
        {
            if (index == inputs.size() - 1)
            {
                inputs[0]->activate();
            }
            else
            {
                inputs[index + 1]->activate();
            }
            input->deactivate();
            tab = false;
        }

        input->setPos(x, tempY);
        input->process(eventManager);
        input->render(renderer, w, h);
        tempY = input->getEndY() + 30;
        index++;
    }

    submitButton.setPos(x, tempY);
    if (enabledButton)
    {
        submitButton.process(eventManager);
        submitButton.render(renderer, w, h);
    }

    tempY = submitButton.getEndY() + 50 + scrollbar.getScroll();

    if (size != Vector(0, 0))
    {
        rect = {(int)x, (int)y, (int)size.x, (int)size.y};
        if (SDL_PointInRect(&point, &rect))
        {
            scrollbar.setDimensions(width, height - (y + 10), tempY - (y + 10) + 20);
            scrollbar.scroll(eventManager.getMouseWheel().y * 10);
        }
    }

    scrollbar.display(renderer, Vector(width - 5, y + 10));
}

bool Form::hovering(EventsManager eventManager)
{
    SDL_Point point = eventManager.getPoint();
    for (Input *input : inputs)
    {
        if (SDL_PointInRect(&point, input->getRect()) || (SDL_PointInRect(&point, submitButton.getRect()) && enabledButton))
        {
            return true;
        }
        if (input->getSlider().isReal())
        {
            if (input->getSlider().isHovering(eventManager))
            {
                return true;
            }
        }
    }
    return false;
}

bool Form::submit(EventsManager eventManager)
{
    if (enabledButton)
    {
        SDL_Point point = eventManager.getPoint();
        SDL_Point startClickPos = eventManager.getPoint();
        if (eventManager.checkSpecialKey(ENTER) || (SDL_PointInRect(&point, submitButton.getRect()) && SDL_PointInRect(&startClickPos, submitButton.getRect()) && eventManager.isLeftClick()))
        {
            return true;
        }
    }
    return false;
}
