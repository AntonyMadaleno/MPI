#include "Ray.h"
#include <math.h>
#include <stdio.h>

#ifndef M_PI
    #define M_PI (3.14159265358979323846)
#endif
#ifndef M_PIF
    #define M_PIF (3.141592653589793238462643383279502884e+00F)
#endif

float minf(float x, float y)
{
    return (x < y) ? x : y;
}

float maxf(float x, float y)
{
    return (x > y) ? x : y;
}

unsigned long Array_minf(float * array, unsigned long size)
{
    unsigned long r = 0;
    float current = array[0];
    for (unsigned long i = 1; i < size; i++)
    {
        current = minf(current, array[i]);
        if (current == array[i])
            r = i;
    }

    return r;

}

void Ray_set(float origin[3], Vec3 * direction, Ray * output)
{
    output->origin[0] = origin[0];
    output->origin[1] = origin[1];
    output->origin[2] = origin[2];

    output->direction = (Vec3 *) malloc( sizeof(Vec3) );
    output->direction->x = direction->x;
    output->direction->y = direction->y;
    output->direction->z = direction->z;

    output->shortest_distance = 1e6;
}

unsigned char Ray_castSphere(Ray * ray, Sphere * sphere, Scene * scene, Vec3 * output, unsigned char i)
{
    if (ray->shortest_distance >= 1e6)
    {
        Vec3 tmp_vec;

        Vec3_set(
            ray->direction->x,
            ray->direction->y,
            ray->direction->z,
            &tmp_vec
        );

        Vec3_normalize(&tmp_vec, &tmp_vec);

        float u, v;

        u = 0.5 + atan2f(tmp_vec.x, tmp_vec.z)/(2 * M_PIF);
        v = 0.5 + asinf(tmp_vec.y)/M_PIF;

        unsigned short x, y;

        x = (unsigned short) (u * ( scene->skybox->width - 1 ) );
        y = (unsigned short) (v * ( scene->skybox->height - 1 ) );

        output->x = scene->skybox->data[y * scene->skybox->height * 3 + x * 3 + 0];
        output->y = scene->skybox->data[y * scene->skybox->height * 3 + x * 3 + 1];
        output->z = scene->skybox->data[y * scene->skybox->height * 3 + x * 3 + 2];

    }

    //VECTOR FROM ORIGIN TO SPHERE CENTER
    Vec3 tmp;

    Vec3_set(
        -ray->origin[0] + sphere->center->x,
        -ray->origin[1] + sphere->center->y,
        -ray->origin[2] + sphere->center->z,
        &tmp
    );

    //SQUARED DISTANCE OF THE VECTOR RAY ORIGIN TO SPHERE CENTER
    float dist2 = Vec3_length2(&tmp);

    float d = Vec3_scalar( ray->direction, &tmp );

    float delta = d*d - dist2 + sphere->radius * sphere->radius;

    if (delta < 0)
    {
        return 0;
    }

    if (delta == 0)
    {
        if (d < 0.0001f)
        {
            return 0;
        }
    }
    else if (delta >= 0)
    {
        float d1 = d - sqrtf( delta );
        float d2 = d + sqrtf( delta );
        if (d1 >= 0.0001f && d2 >= 0.0001f)
            d = minf(d1, d2);
        else if (d1 >= 0.0001f && d2 < 0.0001f)
            d = d1;
        else if (d2 >= 0.0001f && d1 < 0.0001f)
            d = d2;
    }

    if (d >= 0 && d < ray->shortest_distance)
    {

        //INTERSECT WITH DA SPHERE

        ray->shortest_distance = d;
        Vec3 position, origine, move;

        Vec3_set(ray->direction->x * d, ray->direction->y * d, ray->direction->z * d, &move);
        Vec3_arrayToVec(ray->origin, &origine);
        Vec3_add(&move, &(origine), &position );

        Vec3 sphereOrigin_to_position;
        Vec3_substract(sphere->center, &position, &sphereOrigin_to_position);
        Vec3_normalize(&sphereOrigin_to_position, &sphereOrigin_to_position);
        Vec3_normalize(ray->direction, ray->direction);

        float val[3] = { 0, 0, 0 };
        Quaternion * identity = (Quaternion *) malloc( sizeof(Quaternion) );
        Quaternion_setIdentity( identity );
        for ( unsigned char a = 0; a < scene->light_count; a++ )
        {
            Vec3 * light_dir = (Vec3 *) malloc(sizeof(Vec3));
            float len = Vec3_length(light_dir);
            Vec3_substract( &position, &(scene->lights[a].position), light_dir );
            Vec3_normalize(light_dir, light_dir);

            Plane * p = (Plane *) malloc(sizeof(Plane)); 
            Vec3 * punto = (Vec3 *) malloc(sizeof(Vec3));

            Plane_set(p, &(scene->lights[a].position), light_dir);
            Plane_getRandomPoint(p, punto, scene->lights[a].radius);

            Quaternion * rotation = (Quaternion *) malloc( sizeof(Quaternion) );

            Vec3_substract(&scene->lights[a].position, punto, punto);

            for (unsigned char x = 0; x <= SOFT_SHADOW_RAY_COUNT / (i+1) + 1; x++)
            {
                
                Ray light_ray;

                if (x == 0)
                {
                    Ray_set((float *) &position, light_dir, &light_ray);
                    unsigned char ret = 0;

                    for (unsigned short b = 0; b < scene->sphere_count; b++)
                    {
                        Vec3 test;
                        if(&scene->spheres[b] != sphere)
                            ret = Ray_castSphere(&light_ray, &scene->spheres[b], scene, &test, MAX_REBOUND);

                        if (ret != 0)
                            b = scene->sphere_count;
                    }

                    if (ret == 0)
                    {
                        val[0] += Vec3_scalar( light_dir, &sphereOrigin_to_position ) * scene->lights[a].color.x;
                        val[1] += Vec3_scalar( light_dir, &sphereOrigin_to_position ) * scene->lights[a].color.y;
                        val[2] += Vec3_scalar( light_dir, &sphereOrigin_to_position ) * scene->lights[a].color.z;
                    }

                }
                else
                {
                    x--;
                    Vec3 * tmp0 = malloc( sizeof(Vec3) );
                    Quaternion_fromAxisAngle((float *) light_dir, x * M_PI / ( (float) SOFT_SHADOW_RAY_COUNT / (2*i + 2)), rotation );
                    Quaternion_rotate(rotation, (float *) punto, (float *) tmp0);

                    Vec3 current_dir;
                    Vec3_substract(&position, tmp0, &current_dir);
                    Vec3_normalize(&current_dir, &current_dir);

                    for (unsigned char y = 0; y < 1; y++)
                    {
                        Ray_set((float *) &position, &current_dir, &light_ray);
                        unsigned char ret = 0;

                        for (unsigned short b = 0; b < scene->sphere_count; b++)
                        {
                            Vec3 test;
                            if(&scene->spheres[b] != sphere)
                                ret = Ray_castSphere(&light_ray, &scene->spheres[b], scene, &test, MAX_REBOUND);

                            if (ret != 0)
                                b = scene->sphere_count;
                        }

                        if (ret == 0)
                        {
                            val[0] += Vec3_scalar( &current_dir, &sphereOrigin_to_position ) * scene->lights[a].color.x;
                            val[1] += Vec3_scalar( &current_dir, &sphereOrigin_to_position ) * scene->lights[a].color.y;
                            val[2] += Vec3_scalar( &current_dir, &sphereOrigin_to_position ) * scene->lights[a].color.z;
                        }

                    }
                    free(tmp0);
                    x++;
                }
            }  

            free( rotation );
            free( punto );
            free( p );
            free( light_dir );         

        }

        free(identity);

        val[0] /= SOFT_SHADOW_RAY_COUNT / (i+1) + 2;
        val[1] /= SOFT_SHADOW_RAY_COUNT / (i+1) + 2;
        val[2] /= SOFT_SHADOW_RAY_COUNT / (i+1) + 2;

        val[0] = maxf( val[0], 0 );
        val[0] = minf( val[0], 1 );
        val[1] = maxf( val[1], 0 );
        val[1] = minf( val[1], 1 );
        val[2] = maxf( val[2], 0 );
        val[2] = minf( val[2], 1 );

        float u,v;

        if (sphere->material->texture != NULL || sphere->material->reflection_map != NULL)
        {
            u = 0.5 + atan2f(sphereOrigin_to_position.x, sphereOrigin_to_position.z)/(2 * M_PIF);
            v = 0.5 + asinf(sphereOrigin_to_position.y)/M_PIF;   
        }

        if(sphere->material->texture != NULL)
        {

            unsigned short x, y;

            x = (unsigned short) (u * ( sphere->material->texture->width - 1 ) );
            y = (unsigned short) (v * ( sphere->material->texture->height - 1 ) ); 

            output->x = sphere->material->texture->data[y * sphere->material->texture->height * 3 + x*3 + 0] * val[0];
            output->y = sphere->material->texture->data[y * sphere->material->texture->height * 3 + x*3 + 1] * val[1];
            output->z = sphere->material->texture->data[y * sphere->material->texture->height * 3 + x*3 + 2] * val[2];

        }
        else
        {
            output->x = sphere->material->default_color->x * val[0];
            output->y = sphere->material->default_color->y * val[1];
            output->z = sphere->material->default_color->z * val[2];
        }

        if ( i < MAX_REBOUND )
        {
            Ray rebound_ray;

            float rebound_origin[3];
            rebound_origin[0] = position.x;
            rebound_origin[1] = position.y;
            rebound_origin[2] = position.z;

            float alpha = acosf(Vec3_scalar(ray->direction, &sphereOrigin_to_position) / Vec3_length(&sphereOrigin_to_position));

            Vec3 normal;
            Vec3_cross(ray->direction, &sphereOrigin_to_position, &normal);
            Vec3_normalize(&normal, &normal);
            float normal_array[3] = { normal.x, normal.y, normal.z };
            Quaternion q;
            Quaternion_fromAxisAngle(normal_array, alpha + M_PIF, &q);
            float tmp_dir[3] = { ray->direction->x , ray->direction->y, ray->direction->z };
            float tmp_dir2[3];
            Quaternion_rotate(&q, tmp_dir, tmp_dir2);

            Vec3 rebound_direction;
            Vec3_arrayToVec(tmp_dir2, &rebound_direction);
            Ray_set(rebound_origin, &rebound_direction, &rebound_ray); 

            Vec3 rebound_pix;
            for (unsigned int j = 0; j < scene->sphere_count; j++)
            {
                if (scene->spheres + j * sizeof(Sphere) != sphere)
                {
                    Ray_castSphere(&rebound_ray, &scene->spheres[j], scene, &rebound_pix, i+1);
                }
            }

            if(sphere->material->reflection_map != NULL)
            {

                unsigned short x,y;
                x = (unsigned short) (u * ( sphere->material->reflection_map->width - 1 ) );
                y = (unsigned short) (v * ( sphere->material->reflection_map->height - 1 ) );

                output->x = minf( 
                    1.0 , 
                    output->x + rebound_pix.x * 
                    sphere->material->reflection_map->data[y * sphere->material->reflection_map->height * 3 + x*3 + 0]
                );
                output->y = minf( 
                    1.0 , 
                    output->y + rebound_pix.y * 
                    sphere->material->reflection_map->data[y * sphere->material->reflection_map->height * 3 + x*3 + 1] 
                );
                output->z = minf( 
                    1.0 , 
                    output->z + rebound_pix.z * 
                    sphere->material->reflection_map->data[y * sphere->material->reflection_map->height * 3 + x*3 + 2] 
                );
            }
            else
            {
                output->x = minf( 1.0 , output->x + rebound_pix.x * sphere->material->default_reflection );
                output->y = minf( 1.0 , output->y + rebound_pix.y * sphere->material->default_reflection );
                output->z = minf( 1.0 , output->z + rebound_pix.z * sphere->material->default_reflection  );    
            }
        }
        return 1;

    }

    return 0;

}