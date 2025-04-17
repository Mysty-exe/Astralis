#include <Slider.h>

Slider::Slider()
{
    real = false;
}

Slider::Slider(SDL_Renderer *renderer, float width, float height, float min, float max, float value)
{
    updateSize(width, height);
    this->min = min;
    this->max = max;
    this->value = value;
    real = true;
    activated = false;
    circle.loadFromFile(renderer, "Assets/Trajectory/circle.png", Vector(0.3, 0.3));
}

bool Slider::isReal()
{
    return real;
}

void Slider::updateSize(float width, float height)
{
    this->width = width;
    this->height = height;
}

Vector Slider::getPos()
{
    return Vector(x, y);
}

float Slider::getX()
{
    return x;
}

float Slider::getY()
{
    return y;
}

float Slider::getWidth()
{
    return width;
}

float Slider::getHeight()
{
    return height;
}

bool Slider::isActivated()
{
    return activated;
}

bool Slider::isHovering(EventsManager eventManager)
{
    SDL_Point point = eventManager.getPoint();

    if (SDL_PointInRect(&point, circle.getRect()))
    {
        return true;
    }
    return false;
}

void Slider::setPos(float x, float y)
{
    this->x = x;
    this->y = y;
}

float Slider::getValue()
{
    return value;
}

void Slider::setValue(float value)
{
    this->value = value;
}

void Slider::process(EventsManager eventManager)
{
    SDL_Point point = eventManager.getPoint();

    if (!eventManager.isHoldingClick())
    {
        activated = false;
    }

    if (SDL_PointInRect(&point, circle.getRect()) && eventManager.isHoldingClick())
    {
        activated = true;
    }

    if (activated)
    {
        value += eventManager.getMouseOffset().x;
        value = (value < min) ? min : value;
        value = (value > max) ? max : value;
    }
}

void Slider::display(SDL_Renderer *renderer)
{
    SDL_RenderDrawLine(renderer, x, y, x + width - 20, y);
    SDL_RenderDrawLine(renderer, x, y - 10, x, y + 10);
    SDL_RenderDrawLine(renderer, x + width - 20, y - 10, x + width - 20, y + 10);
    SDL_RenderDrawLine(renderer, (x) + (width - 20) / 2, y - 10, (x) + (width - 20) / 2, y + 10);
    SDL_RenderDrawLine(renderer, (x) + (width - 20) / 4, y - 5, (x) + (width - 20) / 4, y + 5);
    SDL_RenderDrawLine(renderer, (x) + (width - 20) * 0.75, y - 5, (x) + (width - 20) * 0.75, y + 5);
    circle.setCoords(((x - 10) + (width / 2)) + ((width - 20) / 360) * value - circle.getWidth() / 2 + 1, y - circle.getHeight() / 2 + 1);
    circle.render(renderer);
}
