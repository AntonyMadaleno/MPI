#pragma once

#include "Vec3.h"

typedef struct Image
{
    unsigned short height, width;
    float * data;
} Image;

void Image_set(unsigned short height, unsigned short width, Image * img);
void Image_free(Image * img);
void Image_getPixel(Image * img, unsigned short x, unsigned short y, Vec3 * output);
void Image_setPixel(Image * img, unsigned short x, unsigned short y, Vec3 * pixel);

void Image_average(unsigned char size, Image * img, Image * output);

void Image_import(Image * img, char * filepath);
void Image_export(Image * img, char * filepath);