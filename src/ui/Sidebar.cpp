#include <Sidebar.h>

Sidebar::Sidebar()
{
}

Sidebar::Sidebar(SDL_Renderer *renderer, int width, int height)
{
    this->renderer = renderer;
    this->width = width;
    this->height = height;
    sidebarWidth = width / 3;
    sidebarHeight = height;
    scroll = 0;
    maxScroll = 0;
    state = "Objects";

    font = TTF_OpenFont("assets/fonts/font.otf", 30);

    color = {40, 1, 55};
    rect = {0, 0, width / 3, height};
    setFrames();
}

vector<Frame> Sidebar::getFrames()
{
    return frames;
}

void Sidebar::setFrames()
{
    int startX, startY, size;
    startX = 10;
    startY = 10;
    size = sidebarWidth / 2;

    for (vector<string> objects : Utilities::getStellarObjects())
    {
        for (int i = 0; i < objects.size(); i++)
        {
            if (i == 0)
            {
                Image text;
                text.loadFromRenderedText(renderer, font, objects[i], color);
                text.setCoords(startX, startY);
                headers.push_back(text);
                startX = 0;
                startY += text.getHeight() + 10;
            }
            else
            {
                Frame frame = Frame(objects[i], startX, startY, size, size);
                frames.push_back(frame);
                startX = (startX == size) ? 0 : startX + size;
                startY = (startX == 0) ? startY + size : startY;

                if (i == objects.size() - 1 && objects.size() % 2 == 0)
                {
                    startY += size;
                }
            }
        }

        startX = 10;
        startY += 10;
    }
    maxScroll = startY - sidebarHeight;
}

void Sidebar::drawSidebar()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
    SDL_RenderFillRect(renderer, &rect);

    if (state == "Objects")
    {
        for (Image header : headers)
        {
            header.render(renderer, scroll);
        }

        for (Frame frame : frames)
        {
            frame.display(renderer, scroll);
        }
    }
    else if (state == "Edit")
    {
        Image text;
        text.loadFromRenderedText(renderer, font, "Edit", color);
        text.setCoords(10, 10);
    }
}

SDL_Rect *Sidebar::getRect()
{
    return &rect;
}
