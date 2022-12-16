#include "Sphere.h"

//SPHERE
void Sphere_set(Vec3 * center, float radius, Material * material, Sphere * output)
{
    output->center = center;
    output->material = material;
    output->radius = radius;
}