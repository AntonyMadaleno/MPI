#pragma once

#include "Quaternion.h"
#include "Vec3.h"
#include <stdlib.h>

/* struct holding info of camera */
typedef struct Camera
{
    float position[3];
    float direction[3];
    unsigned short resolution[2];
    float fov[2];
    float * directionMatrix;
} Camera;

/* Set the value of camera */
void Camera_set(float position[3], float direction[3], unsigned short resolution[2], float fov[2], Camera * output);

/* generate ray direction vectors of the camera */
void Camera_genDirectionMatrix(Camera * output);

/* free memory */
void Camera_freeDirectionMatrix(Camera * output);
void Camera_free(Camera * output);
