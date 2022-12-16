#include "Vec3.h"
#include "stdio.h"
#include "stdlib.h"

void Vec3_set( float x, float y, float z, Vec3 * output)
{
    if (output == NULL)
    {
        fprintf( stderr, "Specified output is NULL !");
        exit(-1);
    }

    output->x = x;
    output->y = y;
    output->z = z;
}

void Vec3_arrayToVec( float arr[3], Vec3 * output)
{
    if (output == NULL)
    {
        fprintf( stderr, "Specified output is NULL !");
        exit(-1);
    }

    output->x = arr[0];
    output->y = arr[1];
    output->z = arr[2];
}

void Vec3_copy( Vec3 * v, Vec3 * output)
{
    if (output == NULL)
    {
        fprintf( stderr, "Specified output is NULL !");
        exit(-1);
    }
    
    if (v == NULL)
    {
        fprintf( stderr, "Specified vector is NULL !");
        exit(-1);
    }
    
    output->x = v->x;
    output->y = v->y;
    output->z = v->z;
}

float Vec3_length2( Vec3 * v )
{
    if (v == NULL)
    {
        fprintf( stderr, "Specified vector is NULL !");
        exit(-1);
    }
    return v->x * v->x + v->y * v->y + v->z * v->z;
}

float Vec3_length( Vec3 * v )
{
    if (v == NULL)
    {
        fprintf( stderr, "Specified vector is NULL !");
        exit(-1);
    }

    float l2 = Vec3_length2(v);

    if (l2 > EPSILON)
        return sqrtf( l2 );
    else
        return 0;
}

void Vec3_normalize( Vec3 * v, Vec3 * output)
{

    if (output == NULL)
    {
        fprintf( stderr, "Specified output is NULL !");
        exit(-1);
    }

    if (v == NULL)
    {
        fprintf( stderr, "Specified vector is NULL !");
        exit(-1);
    }

    float l = Vec3_length(v);

    if (l <= EPSILON)
        return;
    else
    {
        output->x = v->x / l;
        output->y = v->y / l;
        output->z = v->z / l;
    }
}

void Vec3_add( Vec3 * v, Vec3 * u, Vec3 * output)
{

    if (output == NULL)
    {
        fprintf( stderr, "Specified output is NULL !");
        exit(-1);
    }

    if (v == NULL)
    {
        fprintf( stderr, "Specified vector 'v' is NULL !");
        exit(-1);
    }

    if (u == NULL)
    {
        fprintf( stderr, "Specified vector 'u' is NULL !");
        exit(-1);
    }

    output->x = v->x + u->x;
    output->y = v->y + u->y;
    output->z = v->z + u->z;
}

void Vec3_substract( Vec3 * v, Vec3 * u, Vec3 * output)
{
    
    if (output == NULL)
    {
        fprintf( stderr, "Specified output is NULL !");
        exit(-1);
    }

    if (v == NULL)
    {
        fprintf( stderr, "Specified vector 'v' is NULL !");
        exit(-1);
    }

    if (u == NULL)
    {
        fprintf( stderr, "Specified vector 'u' is NULL !");
        exit(-1);
    }

    output->x = v->x - u->x;
    output->y = v->y - u->y;
    output->z = v->z - u->z;
}

void Vec3_multiply( Vec3 * v, Vec3 * u, Vec3 * output)
{
    
    if (output == NULL)
    {
        fprintf( stderr, "Specified output is NULL !");
        exit(-1);
    }

    if (v == NULL)
    {
        fprintf( stderr, "Specified vector 'v' is NULL !");
        exit(-1);
    }

    if (u == NULL)
    {
        fprintf( stderr, "Specified vector 'u' is NULL !");
        exit(-1);
    }

    output->x = v->x * u->x;
    output->y = v->y * u->y;
    output->z = v->z * u->z;
}

void Vec3_divide( Vec3 * v, Vec3 * u, Vec3 * output)
{
    
    if (output == NULL)
    {
        fprintf( stderr, "Specified output is NULL !");
        exit(-1);
    }

    if (v == NULL)
    {
        fprintf( stderr, "Specified vector 'v' is NULL !");
        exit(-1);
    }

    if (u == NULL)
    {
        fprintf( stderr, "Specified vector 'u' is NULL !");
        exit(-1);
    }

    if (u->x != 0)
        output->x = v->x / u->x;
    else
        output->x = INFINITY;
    if (u->y != 0)
        output->y = v->y / u->y;
    else
        output->y = INFINITY;
    if (u->z != 0)
        output->z = v->z / u->z;
    else
        output->z = INFINITY;
}

void Vec3_inverse(Vec3 * v, Vec3 * output)
{
    
    if (output == NULL)
    {
        fprintf( stderr, "Specified output is NULL !");
        exit(-1);
    }

    if (v == NULL)
    {
        fprintf( stderr, "Specified vector 'v' is NULL !");
        exit(-1);
    }

    output->x = -v->x;
    output->y = -v->y;
    output->z = -v->z;
}

float Vec3_scalar( Vec3 * v, Vec3 * u)
{

    if (v == NULL)
    {
        fprintf( stderr, "Specified vector 'v' is NULL !");
        exit(-1);
    }

    if (u == NULL)
    {
        fprintf( stderr, "Specified vector 'u' is NULL !");
        exit(-1);
    }

    return v->x * u->x + v->y * u->y + v->z * u->z;
}

void Vec3_cross( Vec3 * v, Vec3 * u, Vec3 * output)
{

    if (output == NULL)
    {
        fprintf( stderr, "Specified output is NULL !");
        exit(-1);
    }

    if (v == NULL)
    {
        fprintf( stderr, "Specified vector 'v' is NULL !");
        exit(-1);
    }

    if (u == NULL)
    {
        fprintf( stderr, "Specified vector 'u' is NULL !");
        exit(-1);
    }

    output->x = v->y * u->z - v->z * u->y;
    output->y = v->z * u->x - v->x * u->z;
    output->z = v->x * u->y - v->y * u->x;
}

void Bytes_to_Vec3(char * bytes, Vec3 * output)
{
    memcpy( &(output->x), bytes, sizeof( float ) );
    memcpy( &(output->y), bytes + sizeof( float ), sizeof( float ) );
    memcpy( &(output->z), bytes + 2 * sizeof( float ), sizeof( float ) );
}

void Vec3_to_Bytes(Vec3 * input, char * bytes)
{
    memcpy( bytes, &(input->x), sizeof( float ) );
    memcpy( bytes + sizeof( float ), &(input->y), sizeof( float ) );
    memcpy( bytes + sizeof( float ) * 2, &(input->z), sizeof( float ) );
}