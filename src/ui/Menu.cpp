#include <Menu.h>

Menu::Menu()
{
}

Menu::Menu(SDL_Renderer *renderer, string title, int width, int height)
{
    this->renderer = renderer;
    this->width = width;
    this->height = height;

    backgroundImg.loadFromFile(renderer, "assets/Backgrounds/menuBackground.png", 2);
    pointerCursor.loadFromFile(renderer, "assets/cursors/pointer.png", 4);
    handCursor.loadFromFile(renderer, "assets/cursors/hand.png", 4);

    titleTxtAnimation = Animation(renderer, "assets/Menu Text Animation", 4, 50);

    newTxt.loadFromFile(renderer, "assets/Menu Buttons/new.png", 3);
    newTxt.setCoords(width / 2 - newTxt.getWidth() / 2, height / 3);
    newHoverTxt.loadFromFile(renderer, "assets/Menu Buttons/newHover.png", 3);
    newHoverTxt.setCoords(width / 2 - newTxt.getWidth() / 2, height / 3);

    loadTxt.loadFromFile(renderer, "assets/Menu Buttons/load.png", 3);
    loadTxt.setCoords(width / 2 - loadTxt.getWidth() / 2, newTxt.y + height / 6);
    loadHoverTxt.loadFromFile(renderer, "assets/Menu Buttons/loadHover.png", 3);
    loadHoverTxt.setCoords(width / 2 - loadTxt.getWidth() / 2, newTxt.y + height / 6);

    settingsTxt.loadFromFile(renderer, "assets/Menu Buttons/settings.png", 3);
    settingsTxt.setCoords(width / 2 - settingsTxt.getWidth() / 2, loadTxt.y + height / 6);
    settingsHoverTxt.loadFromFile(renderer, "assets/Menu Buttons/settingsHover.png", 3);
    settingsHoverTxt.setCoords(width / 2 - settingsTxt.getWidth() / 2, loadTxt.y + height / 6);

    quitTxt.loadFromFile(renderer, "assets/Menu Buttons/quit.png", 3);
    quitTxt.setCoords(width / 2 - quitTxt.getWidth() / 2, settingsTxt.y + height / 6);
    quitHoverTxt.loadFromFile(renderer, "assets/Menu Buttons/quitHover.png", 3);
    quitHoverTxt.setCoords(width / 2 - quitTxt.getWidth() / 2, settingsTxt.y + height / 6);

    transition = Transition(renderer, width, height, 1, 45);

    hovering = false;
}

void Menu::drawCursor(int mouseX, int mouseY)
{
    pointerCursor.setCoords(mouseX, mouseY);
    handCursor.setCoords(mouseX, mouseY);
}

string Menu::runMenu(string state, bool click)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    drawCursor(mouseX, mouseY);

    SDL_Point point = {mouseX, mouseY};

    if (transition.transitionState != "In")
    {
        backgroundImg.render(renderer);

        int titleWidth = titleTxtAnimation.images[0].getWidth();
        titleTxtAnimation.render(width / 2 - titleWidth / 2, height / 10);

        if (SDL_PointInRect(&point, newTxt.getRect()))
        {
            newHoverTxt.render(renderer);
            if (click)
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
            if (click)
            {
                state = "Quit";
            }
            hovering = true;
        }
        else
        {
            quitTxt.render(renderer);
        }

        if (hovering)
        {
            handCursor.render(renderer);
        }
        else
        {
            pointerCursor.render(renderer);
        }
    }
    else
    {
        drawCursor(mouseX, mouseY);
    }

    transition.runTransition();

    hovering = false;
    SDL_RenderPresent(renderer);

    return state;
}