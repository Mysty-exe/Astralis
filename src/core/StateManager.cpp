#include <StateManager.h>

StateManager::StateManager(SDL_Window *window, SDL_Renderer *renderer, string title, int width, int height)
{
    this->window = window;
    this->renderer = renderer;
    this->title = title;
    this->width = width;
    this->height = height;
    state = "Menu";

    click = false;
    holdClick = false;

    menu = Menu(renderer, title, width, height);
    simulation = Simulation(renderer, width, height);
}

string StateManager::getTitle()
{
    return title;
}

string StateManager::getState()
{
    return state;
}

void StateManager::setState(string state)
{
    this->state = state;
}

int StateManager::getWidth()
{
    return width;
}

int StateManager::getHeight()
{
    return height;
}

void StateManager::run()
{
    SDL_Event windowEvent;
    Vector mouseWheel = Vector(0, 0);

    while (state != "Quit")
    {
        events.getEvents(windowEvent);

        if (events.quit)
        {
            state = "Quit";
        }

        if (state == "Menu")
        {
            state = menu.runMenu(events, state);
            if (state == "Simulation")
            {
                holdClick = false;
            }
        }
        else if (state == "Simulation")
        {
            state = simulation.runSimulation(events, state);
            if (state == "Menu")
            {
                menu.transition.transitionState = "In";
                menu.transition.getPossibleLocations();
            }
        }

        events.clear();
    }
}
