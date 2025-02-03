#include <Menu.h>

Menu::Menu()
{
}

Menu::Menu(SDL_Renderer *renderer, string title, int width, int height)
{
    this->renderer = renderer;
    this->width = width;
    this->height = height;

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

    settingsTxt.loadFromFile(renderer, "assets/Menu Buttons/settings.png", 1.5);
    settingsTxt.setCoords(width / 2 - settingsTxt.getWidth() / 2, loadTxt.y + height / 6);
    settingsHoverTxt.loadFromFile(renderer, "assets/Menu Buttons/settingsHover.png", 1.5);
    settingsHoverTxt.setCoords(width / 2 - settingsTxt.getWidth() / 2, loadTxt.y + height / 6);

    quitTxt.loadFromFile(renderer, "assets/Menu Buttons/quit.png", 1.5);
    quitTxt.setCoords(width / 2 - quitTxt.getWidth() / 2, settingsTxt.y + height / 6);
    quitHoverTxt.loadFromFile(renderer, "assets/Menu Buttons/quitHover.png", 1.5);
    quitHoverTxt.setCoords(width / 2 - quitTxt.getWidth() / 2, settingsTxt.y + height / 6);

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
                state = "Simulation";
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
            hovering = true;
        }
        else
        {
            loadTxt.render(renderer);
        }

        if (SDL_PointInRect(&point, settingsTxt.getRect()))
        {
            settingsHoverTxt.render(renderer);
            hovering = true;
        }
        else
        {
            settingsTxt.render(renderer);
        }

        if (SDL_PointInRect(&point, quitTxt.getRect()))
        {
            quitHoverTxt.render(renderer);
            if (events.leftClick && SDL_PointInRect(&events.startClickPos, quitTxt.getRect()))
            {
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
