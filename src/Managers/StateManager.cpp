#include <StateManager.h>

StateManager::StateManager(SDL_Renderer *renderer, int width, int height) : Manager(renderer, width, height)
{
    menuManager = MenuManager(renderer, width, height);
    simulationManager = SimulationManager(renderer, width, height);
    currentState = MENU;
    stepTimer.start();
    saveTimer.start();
    mute = false;

    menuManager.loadSimulationData(simulationManager.getSimulations());
}

ManagerState StateManager::getState()
{
    return currentState;
}

void StateManager::saveData()
{
    if (saveTimer.getTicks() / 1000 >= 1 && simulationManager.getSimIndex() != -1)
    {
        if (!simulationManager.getSim()->isEditing())
        {
            menuManager.saveSimulationData(simulationManager.getSimulations());
            saveTimer.start();
        }
    }
}

void StateManager::handleMusic(EventsManager eventManager, MessageManager &messageManager)
{
    playBgMusic();

    if (eventManager.checkCtrlKey(M))
    {
        mute = !mute;
        if (mute)
        {
            Mix_VolumeMusic(0);
            messageManager.addMessage("Music has been muted.");
        }
        else
        {
            Mix_VolumeMusic(5);
            messageManager.addMessage("Music has been unmuted.");
        }
    }
}

void StateManager::run(EventsManager eventManager, MessageManager messageManager, Transition *transition)
{
    int code = 0;
    Mix_VolumeMusic(5);

    while (currentState != QUIT)
    {
        if (transition->getTransitionState() == "Done")
        {
            setColor(BLACK);
            SDL_RenderClear(renderer);
        }

        timeStep = stepTimer.getTicks() / 1000.0f;
        timeStep = (timeStep <= 0) ? 0.001 : timeStep;
        stepTimer.start();

        saveData();

        eventManager.getEvents();
        currentState = eventManager.userQuit() ? QUIT : currentState;

        handleMusic(eventManager, messageManager);

        if (eventManager.changedWindowSize())
        {
            messageManager.resizeDimensions(eventManager.getWidth(), eventManager.getHeight());
            menuManager.setWindowSize(eventManager.getWidth(), eventManager.getHeight());
            simulationManager.setWindowSize(eventManager.getWidth(), eventManager.getHeight());
            transition->resizeDimensions(eventManager.getWidth(), eventManager.getHeight());
        }

        switch (currentState)
        {
        case MENU:
        {
            int currentSim = simulationManager.getSimIndex();
            code = menuManager.run(eventManager, transition, simulationManager.getSimulations(), currentSim, timeStep);
            simulationManager.setSimIndex(currentSim);
            currentState = (code == 1) ? SIMULATION : currentState;
            if (currentState == SIMULATION)
            {
                transition->setTransitionState("In");
            }
            break;
        }

        case SIMULATION:
            code = simulationManager.run(eventManager, messageManager, transition, timeStep);
            currentState = (code == 0) ? MENU : currentState;
            if (currentState == MENU)
            {
                transition->setTransitionState("In");
            }
            break;
        }

        transition->runTransition();

        messageManager.displayMessages(renderer);

        if (code == -1)
        {
            currentState = QUIT;
        }

        if (currentState == QUIT)
        {
            if (simulationManager.getSimIndex() != -1)
            {
                if (!simulationManager.getSim()->isEditing())
                {
                    menuManager.saveSimulationData(simulationManager.getSimulations());
                }
            }
        }

        eventManager.clear();
        SDL_RenderPresent(renderer);
    }
}
