#include <Menu.h>

Menu::Menu()
{
}

Menu::Menu(SDL_Renderer *renderer, string title, int width, int height)
{
    this->renderer = renderer;
    this->width = width;
    this->height = height;

    clickSound = Mix_LoadWAV("assets/Audio/click.wav");

    backgroundImg.loadFromFile(renderer, "assets/Backgrounds/menuBackground.png", 1);
    pointerCursor.loadFromFile(renderer, "assets/cursors/pointer.png", 2);
    handCursor.loadFromFile(renderer, "assets/cursors/hand.png", 2);

    titleTxtAnimation = Animation(renderer, "assets/Menu Text Animation", 2, 0.1);

    newTxt.loadFromFile(renderer, "assets/Menu Buttons/new.png", 1.5);
    newTxt.setCoords(width / 2 - newTxt.getWidth() / 2, height / 3);
    newHoverTxt.loadFromFile(renderer, "assets/Menu Buttons/newHover.png", 1.5);
    newHoverTxt.setCoords(width / 2 - newTxt.getWidth() / 2, height / 3);

    loadTxt.loadFromFile(renderer, "assets/Menu Buttons/load.png", 1.5);
    loadTxt.setCoords(width / 2 - loadTxt.getWidth() / 2, newTxt.y + height / 6);
    loadHoverTxt.loadFromFile(renderer, "assets/Menu Buttons/loadHover.png", 1.5);
    loadHoverTxt.setCoords(width / 2 - loadTxt.getWidth() / 2, newTxt.y + height / 6);

    controlsTxt.loadFromFile(renderer, "assets/Menu Buttons/controls.png", 1.5);
    controlsTxt.setCoords(width / 2 - controlsTxt.getWidth() / 2, loadTxt.y + height / 6);
    controlsHoverTxt.loadFromFile(renderer, "assets/Menu Buttons/controlsHover.png", 1.5);
    controlsHoverTxt.setCoords(width / 2 - controlsTxt.getWidth() / 2, loadTxt.y + height / 6);

    quitTxt.loadFromFile(renderer, "assets/Menu Buttons/quit.png", 1.5);
    quitTxt.setCoords(width / 2 - quitTxt.getWidth() / 2, controlsTxt.y + height / 6);
    quitHoverTxt.loadFromFile(renderer, "assets/Menu Buttons/quitHover.png", 1.5);
    quitHoverTxt.setCoords(width / 2 - quitTxt.getWidth() / 2, controlsTxt.y + height / 6);

    transition = Transition(renderer, width, height, 1);

    hovering = false;
    timeStep = 0;
    stepTimer.start();
}

void Menu::drawCursor(int mouseX, int mouseY)
{
    pointerCursor.setCoords(mouseX, mouseY);
    handCursor.setCoords(mouseX, mouseY);

    if (hovering)
    {
        handCursor.render(renderer);
    }
    else
    {
        pointerCursor.render(renderer);
    }
}

string Menu::runMenu(Events events, string state)
{
    SDL_Point point = events.getPoint();
    timeStep = stepTimer.getTicks() / 1000.0f;
    stepTimer.start();

    if (transition.transitionState != "In")
    {
        backgroundImg.render(renderer);

        int titleWidth = titleTxtAnimation.images[0].getWidth();
        titleTxtAnimation.render(width / 2 - titleWidth / 2, height / 12);

        if (SDL_PointInRect(&point, newTxt.getRect()))
        {
            newHoverTxt.render(renderer);
            if (events.leftClick && SDL_PointInRect(&events.startClickPos, newTxt.getRect()))
            {
                Mix_PlayChannel(-1, clickSound, 0);
                state = "New";
            }
            hovering = true;
        }
        else
        {
            newTxt.render(renderer);
        }

        if (SDL_PointInRect(&point, loadTxt.getRect()))
        {
            loadHoverTxt.render(renderer);
            if (events.leftClick && SDL_PointInRect(&events.startClickPos, loadTxt.getRect()))
            {
                Mix_PlayChannel(-1, clickSound, 0);
                state = "Load";
            }
            hovering = true;
        }
        else
        {
            loadTxt.render(renderer);
        }

        if (SDL_PointInRect(&point, controlsTxt.getRect()))
        {
            controlsHoverTxt.render(renderer);
            if (events.leftClick && SDL_PointInRect(&events.startClickPos, controlsTxt.getRect()))
            {
                Mix_PlayChannel(-1, clickSound, 0);
                state = "Control";
            }
            hovering = true;
        }
        else
        {
            controlsTxt.render(renderer);
        }

        if (SDL_PointInRect(&point, quitTxt.getRect()))
        {
            quitHoverTxt.render(renderer);
            if (events.leftClick && SDL_PointInRect(&events.startClickPos, quitTxt.getRect()))
            {
                Mix_PlayChannel(-1, clickSound, 0);
                state = "Quit";
            }
            hovering = true;
        }
        else
        {
            quitTxt.render(renderer);
        }

        drawCursor(events.mousePos.x, events.mousePos.y);
        hovering = false;
    }

    transition.runTransition();

    SDL_RenderPresent(renderer);

    return state;
}
