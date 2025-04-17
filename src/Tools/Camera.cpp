#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(int width, int height)
{
    resizeDimensions(width, height);
    offset = Vector(0, 0);
}

void Camera::resizeDimensions(float width, float height)
{
    this->width = width;
    this->height = height;
}

Vector
Camera::getOffset()
{
    return offset;
}

float Camera::getX()
{
    return offset.x;
}

float Camera::getY()
{
    return offset.y;
}

void Camera::centerOnPosition(Vector pos)
{
    offset = Vector(-pos.x + width / 2, -pos.y + height / 2);
}

void Camera::drag(Vector vec)
{
    offset += vec;
}
