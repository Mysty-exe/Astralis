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
    simulationHandler = SimulationHandler(renderer, width, height);
    stepTimer.start();

    bgMusic = Mix_LoadMUS("assets/Audio/spaceBgSound.mp3");
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
    bool mute = false;
    Mix_VolumeMusic(5);

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
        }
        else if (state == "New")
        {
            state = simulationHandler.newSimulation(events, state, timeStep);
        }
        else if (state == "Load")
        {
            state = simulationHandler.loadSimulation(events, state, timeStep);
            if (state == "Simulation")
            {
                simulationHandler.transition.transitionState = "In";
                simulationHandler.transition.getPossibleLocations();
            }
        }
        else if (state == "Edit New" || state == "Edit Load")
        {
            state = simulationHandler.editSimulation(events, state, timeStep);
            if (state == "Simulation")
            {
                simulationHandler.transition.transitionState = "In";
                simulationHandler.transition.getPossibleLocations();
            }
        }
        else if (state == "Control")
        {
            state = simulationHandler.controls(events, state, timeStep);
        }
        else if (state == "Simulation")
        {
            state = simulationHandler.runSimulation(events, state, timeStep);
            if (state == "Menu")
            {
                menu.transition.transitionState = "In";
                menu.transition.getPossibleLocations();
            }
        }

        if (events.quit || state == "Quit")
        {
            simulationHandler.saveSimulationData();
            state = "Quit";
        }

        events.clear();
    }
}
