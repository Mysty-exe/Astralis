#pragma once
#include <iostream>
#include <string>
#include <cmath>
#include <math.h>
#define PI 3.14159265358979323846

using namespace std;

class Vector
{
public:
    long double x, y;
    Vector();
    Vector(long double posX, long double posY);
    bool operator==(const Vector &other) const;
    bool operator!=(const Vector &other) const;
    Vector operator+(const Vector &other) const;
    Vector operator-(const Vector &other) const;
    Vector operator*(const Vector &other) const;
    Vector operator/(const Vector &other) const;
    void operator+=(const Vector &other);
    void operator-=(const Vector &other);
    void operator*=(const Vector &other);
    void operator/=(const Vector &other);
    Vector operator+(const long double &other) const;
    Vector operator-(const long double &other) const;
    Vector operator*(const long double &other) const;
    Vector operator/(const long double &other) const;
    void operator+=(const long double &other);
    void operator-=(const long double &other);
    void operator*=(const long double &other);
    void operator/=(const long double &other);
    Vector absolute();
    Vector roundVec();
    bool isZero();
    long double magnitude();
    long double distance(Vector other);
    Vector normalize();
    double getRadians();
    double getAngle();
    bool isNumber();
    void display();
};