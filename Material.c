#include "Material.h"

void Material_set(Material * material, Image * texture, Image * reflection_map)
{
    material->reflection_map = reflection_map;
    material->texture = texture;

    material->default_color = (Vec3 *) malloc(sizeof(Vec3));
    Vec3_set(0.5f, 0.0f, 0.5f, material->default_color);

    material->default_reflection = 0.0f;
}

void Material_free(Material * material)
{
    Image_free(material->reflection_map);
    Image_free(material->texture);
}