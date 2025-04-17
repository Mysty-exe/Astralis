#pragma once
#include "Vector.h"

class Camera
{
private:
    int width, height;
    Vector offset;

public:
    Camera();
    Camera(int width, int height);
    void resizeDimensions(float width, float height);
    Vector getOffset();
    float getX();
    float getY();
    void centerOnPosition(Vector pos);
    void drag(Vector vec);
};