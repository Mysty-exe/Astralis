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

void StateManager::setEvents(vector<string> newEvents)
{
    events = newEvents;
}

void StateManager::run()
{
    SDL_Event windowEvent;
    Vector mouseWheel = Vector(0, 0);

    while (state != "Quit")
    {
        click = false;

        while (SDL_PollEvent(&windowEvent) != 0)
        {
            if (windowEvent.type == SDL_QUIT)
            {
                state = "Quit";
            }
            else if (windowEvent.type == SDL_MOUSEWHEEL)
            {
                mouseWheel.x = windowEvent.wheel.x;
                mouseWheel.y = windowEvent.wheel.y;
            }
            else if (windowEvent.type == SDL_MOUSEBUTTONDOWN)
            {
                events.push_back("click");
                holdClick = true;
                click = true;
            }
            else if (windowEvent.type == SDL_MOUSEBUTTONUP)
            {
                holdClick = false;
            }
            else if (windowEvent.type == SDL_KEYDOWN)
            {
                switch (windowEvent.key.keysym.sym)
                {
                case (SDLK_TAB):
                    events.push_back("tab");
                    break;
                case (SDLK_SPACE):
                    events.push_back("space");
                    break;
                case (SDLK_ESCAPE):
                    events.push_back("escape");
                    break;
                case (SDLK_LCTRL):
                    events.push_back("ctrl");
                    break;
                case (SDLK_UP):
                    events.push_back("up");
                    break;
                case (SDLK_DOWN):
                    events.push_back("down");
                    break;
                case (SDLK_RIGHT):
                    events.push_back("right");
                    break;
                case (SDLK_LEFT):
                    events.push_back("left");
                    break;
                }
            }
        }

        if (state == "Menu")
        {
            state = menu.runMenu(state, click);
            if (state == "Simulation")
            {
                holdClick = false;
            }
        }
        else if (state == "Simulation")
        {
            state = simulation.runSimulation(state, events, click, holdClick, mouseWheel);
            if (state == "Menu")
            {
                menu.transition.transitionState = "In";
                menu.transition.getPossibleLocations();
            }
        }

        events.clear();
    }
}
