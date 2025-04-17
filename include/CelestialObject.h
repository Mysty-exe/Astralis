#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <SDL.h>
#include <Utilities.h>
#include <Vector.h>
#include <Texture.h>
#include <Animation.h>
#include <ParticleManager.h>
#include <Timer.h>

using namespace std;

enum CelestialType
{
    STAR,
    PLANET,
    MOON,
    ASTEROID
};

class CelestialObject
{
private:
    SDL_Renderer *renderer;
    int width, height;
    string name, folder;
    CelestialType objType;
    bool delObject;

    long double mass, radius, updateRadius;
    Vector position, velocity, particlePos;
    long double kineticEnergy, potentialEnergy;
    SDL_Rect rect, realRect;

    Animation object, viewObject;
    ParticleManager particleManager;
    Texture nameTxt;

    vector<Vector> storedTrajectories;
    Timer particleDurationTimer;

public:
    CelestialObject();
    CelestialObject(SDL_Renderer *renderer, int width, int height, string name, string objType, string fileName);
    CelestialObject(SDL_Renderer *renderer, int width, int height, string n, string objType, string fileName, long double mass, long double radius, float x, float y, Vector vel);
    void resizeDimensions(float width, float height);
    bool isMarkedForDelete();
    void deleteObject();
    string getName();
    string getFolder();
    CelestialType getObjType();
    long double getMass();
    void setMass(long double mass);
    void addMass(long double mass);
    long double getRadius();
    void setRadius(long double radius);
    long double getUpdateRadius();
    void setUpdateRadius(long double updateRadius);
    Vector getPosition();
    void setPosition(Vector position);
    Vector getVelocity();
    void setVelocity(Vector velocity);
    void addPosition(Vector vec);
    void addVelocity(Vector vec);
    long double getKineticEnergy();
    void setKineticEnergy(long double energy);
    void addKineticEnergy(long double energy);
    long double getPotentialEnergy();
    void setPotentialEnergy(long double energy);
    void addPotentialEnergy(long double energy);
    void startParticles(Vector position, float explosionSize);
    Animation *getObject();
    void render(SDL_Rect renderQuad, Vector offset, double timeStep);
    vector<Vector> getTrajectories();
    void setTrajectories(vector<Vector> trajectories);
    static string getObjType(CelestialType objType);
    void setName(SDL_Renderer *renderer, string name);
    void renderViewDisplay(float x, float y, float width, float height);
    void updateSizeGradually(SDL_Renderer *renderer, double timeStep);
    void updateSizeInstant(SDL_Renderer *renderer);
    SDL_Rect getRect(Vector scroll = Vector(0, 0), float zoomFactor = 1);
    SDL_Rect *getRealRect();
    void setRect(SDL_Rect rect);
    void setRealRect(SDL_Rect realRect);
};
