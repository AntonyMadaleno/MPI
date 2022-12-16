#pragma once

#include <math.h>
#include "string.h"

#define EPSILON 0.0001f

typedef struct Vec3
{
    float x,y,z;
} Vec3;

void Vec3_set( float x, float y, float z, Vec3 * output);
void Vec3_arrayToVec( float v[3], Vec3 * output);
void Vec3_copy( Vec3 * v, Vec3 * output);

void Vec3_normalize( Vec3 * v, Vec3 * output);
float Vec3_length2( Vec3 * v);
float Vec3_length( Vec3 * v );

void Vec3_add( Vec3 * v, Vec3 * u, Vec3 * output);
void Vec3_substract( Vec3 * v, Vec3 * u, Vec3 * output);
void Vec3_multiply( Vec3 * v, Vec3 * u, Vec3 * output);
void Vec3_divide( Vec3 * v, Vec3 * u, Vec3 * output);
void Vec3_inverse(Vec3 * v, Vec3 * output);

float Vec3_scalar( Vec3 * v, Vec3 * u);
void Vec3_cross( Vec3 * v, Vec3 * u, Vec3 * output);

void Bytes_to_Vec3(char * bytes, Vec3 * output);
void Vec3_to_Bytes(Vec3 * input, char * bytes);


