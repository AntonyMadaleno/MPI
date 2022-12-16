#pragma once

#include "Vec3.h"
#include "Quaternion.h"

typedef struct Plane
{
    Vec3 point, normal;
} Plane;

void Plane_set(Plane * output, Vec3 * point, Vec3 * normal);

void Plane_getRandomPoint(Plane * plane, Vec3 * point, float dist);