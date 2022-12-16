#pragma once

#include "Vec3.h"
#include "Ray.h"
#include "Camera.h"
#include "Image.h"
#include "string.h"
#include "Sphere.h"

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

//LIGHT
void Light_set(Vec3 position, Vec3 color, float radius, Light * output);

//SCENE
void Scene_set(Camera * camera, Sphere * spheres, Light * lights, Image * skybox ,Scene * output);

void Scene_add_Light(Light * light, Scene * output);
void Scene_remove_Light(unsigned long id, Scene * output);

void Scene_add_Sphere(Sphere * sphere, Scene * output);
void Scene_remove_Sphere(unsigned long id, Scene * output);

void Scene_render(Scene * scene, Image * image);