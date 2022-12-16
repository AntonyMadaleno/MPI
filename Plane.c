#include "Plane.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Plane_set(Plane * output, Vec3 * point, Vec3 * normal)
{
    Vec3_copy(point ,&output->point);
    Vec3_copy(normal ,&output->normal);
    Vec3_normalize(&output->normal, &output->normal);
}

//the point given will be at distance 1 from point
void Plane_getRandomPoint(Plane * plane, Vec3 * point, float dist) 
{
    //P = ax + by + cz + d = 0 | n(a,b,c) et p appartient à P

    //on cherche à définir d
    float d = - ( plane->normal.x * plane->point.x + plane->normal.y * plane->point.y + plane->normal.z * plane->point.z );

    //on définie un point random dans l'espace
    srand( time( NULL ) );

    float x = rand() % 256;
    float y = rand() % 256;
    float z = rand() % 256;

    if (plane->normal.x != 0)
        x = - (plane->normal.y * y + plane->normal.z * z + d) / plane->normal.x;

    else if (plane->normal.y != 0)
        y = - (plane->normal.y * y + plane->normal.z * z + d) / plane->normal.y;

    else if (plane->normal.z != 0)
        z = - (plane->normal.y * y + plane->normal.z * z + d) / plane->normal.z;

    point->x = x;
    point->y = y;
    point->z = z;

    Vec3_normalize(point, point);
    point->x *= dist;
    point->y *= dist;
    point->z *= dist;

}