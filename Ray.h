#pragma once
#include "Vec3.h"
#include "Scene.h"
#include "Camera.h"
#include "Plane.h"
#include "Sphere.h"
#include "Image.h"

#define MAX_REBOUND 5
#define SOFT_SHADOW_RAY_COUNT 10

#ifndef LIGHT
#define LIGHT
    typedef struct Light
    {
        Vec3 position, color;
        float radius;
    } Light;
#endif

#ifndef SCENE
#define SCENE
typedef struct Scene
{
    unsigned short sphere_count, light_count;
    Sphere * spheres;
    Light * lights;
    Camera * camera;
    Image * skybox;
} Scene;
#endif

typedef struct Ray
{
    float shortest_distance;
    float origin[3];
    Vec3 * direction;
} Ray;

void Ray_set(float origin[3], Vec3 * direction, Ray * output);

//try intersect with given objects and place color value in given pixel (unsigned char[3])
unsigned char Ray_castSphere(Ray * ray, Sphere * sphere, Scene * scene, Vec3 * output, unsigned char i );
unsigned char Ray_castPlane(Ray * ray, Plane * plane, Scene * scene, Vec3 * output, unsigned char i );
