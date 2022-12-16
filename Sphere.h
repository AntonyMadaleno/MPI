#pragma once

#include "Vec3.h"
#include "Material.h"

#ifndef SPHERE
#define SPHERE
    typedef struct Sphere
    {
        Vec3 * center;
        float radius;
        Material * material;
    } Sphere;
#endif

//SPHERE
void Sphere_set(Vec3 * center, float radius, Material * material, Sphere * output);