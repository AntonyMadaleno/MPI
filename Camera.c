#include "Camera.h"

#ifndef M_PI
    #define M_PI (3.14159265358979323846)
#endif
#ifndef M_PIF
    #define M_PIF (3.141592653589793238462643383279502884e+00F)
#endif

void Camera_set(float position[3], float direction[3], unsigned short resolution[2], float fov[2], Camera * output)
{
    output->position[0] = position[0];
    output->position[1] = position[1];
    output->position[2] = position[2];

    output->direction[0] = direction[0];
    output->direction[1] = direction[1];
    output->direction[2] = direction[2];

    output->resolution[0] = resolution[0];  
    output->resolution[1] = resolution[1];

    output->fov[0] = fov[0] * M_PIF/180.0;                
    output->fov[1] = fov[1] * M_PIF/180.0;
}

void Camera_genDirectionMatrix(Camera * output)
{

    output->directionMatrix = (float *) malloc( 3 * output->resolution[0] * output->resolution[1] * sizeof(float) );

    if (output->directionMatrix == NULL)
        output->directionMatrix = (float *) malloc( 3 * output->resolution[0] * output->resolution[1] * sizeof(float) );

    float tmp0[3];
    float tmp1[3];

    Quaternion identity, rotate_X, rotate_Y, quaternion_tmp;
    Quaternion_setIdentity( &identity );

    //for each cols
    for (unsigned int i = 0; i < output->resolution[1]; i++)
    {

        Quaternion_fromYRotation( (float) -output->fov[1]/2 + i*output->fov[1]/(output->resolution[1]-1) , &rotate_Y );
        Quaternion_multiply(&rotate_Y, &identity, &quaternion_tmp);
        Quaternion_rotate(&quaternion_tmp, output->direction, tmp0);

        //for each rows
        for (unsigned int j = 0; j < output->resolution[0]; j++)
        {
            Quaternion_fromXRotation( (float) -output->fov[0]/2 + j*output->fov[0]/(output->resolution[0]-1) , &rotate_X );
            Quaternion_multiply(&rotate_X, &identity, &quaternion_tmp);
            Quaternion_rotate(&quaternion_tmp, tmp0, tmp1);

            for (unsigned char d = 0; d < 3; d++)
                output->directionMatrix[(unsigned int) j * output->resolution[1] * 3 + i * 3 + d] = tmp1[d];   

        }
    }
}

void Camera_freeDirectionMatrix(Camera * output)
{
    //free memory used to store camera direction matrix
    free(output->directionMatrix);
}

void Camera_free(Camera * output)
{
    free(output->directionMatrix);
}