#pragma once

#include "Vec3.h"
#include "Image.h"
#include <stdlib.h>

typedef struct Material
{
    Image * texture;
    Image * reflection_map;
    Vec3 * default_color;
    float default_reflection;
} Material;

void Material_set(Material * material, Image * texture, Image * reflection_map);
void Material_free(Material * material);