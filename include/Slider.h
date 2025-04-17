#include <SDL.h>
#include <Vector.h>
#include <EventsManager.h>
#include <Texture.h>

class Slider
{
private:
    float x, y;
    float width, height;
    float min, max, value;
    Texture circle;
    bool real, activated;

public:
    Slider();
    Slider(SDL_Renderer *renderer, float width, float height, float min, float max, float value);
    bool isReal();
    void updateSize(float width, float height);
    Vector getPos();
    float getX();
    float getY();
    float getWidth();
    float getHeight();
    bool isActivated();
    bool isHovering(EventsManager eventManager);
    void setPos(float x, float y);
    float getValue();
    void setValue(float value);
    void process(EventsManager eventManager);
    void display(SDL_Renderer *renderer);
};
