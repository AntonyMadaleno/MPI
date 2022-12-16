#include "mpi.h"

#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#include "Scene.h"
#include "Quaternion.h"
#include "Camera.h"
#include "Ray.h"
#include "Image.h"

#ifndef M_PI
    #define M_PI (3.14159265358979323846)
#endif
#ifndef M_PIF
    #define M_PIF (3.141592653589793238462643383279502884e+00F)
#endif

#define IM_SIZE 4096

void main( int argc, char * argv[] )
{

    int numtasks , rank , len , rc, msg;
    char hostname[MPI_MAX_PROCESSOR_NAME ];

    // initialize MPI
    MPI_Init (&argc ,& argv);
    // get number of tasks
    MPI_Comm_size(MPI_COMM_WORLD ,& numtasks);
    // get my rank
    MPI_Comm_rank(MPI_COMM_WORLD ,& rank);
    srand(time(NULL) * rank);

    Camera camera;

    //LIGHTS
    Light * lights;
    lights = (Light *) malloc( sizeof( Light ) * 2 );

    Vec3 light0_position, light0_color;
    Vec3_set(-600.0, 600.0, -600.0, &light0_position);
    Vec3_set(1.0, 1.0, 1.0, &light0_color);

    Light_set( light0_position, light0_color, 0.0002f, &lights[0] );

    Vec3 light1_position, light1_color;
    Vec3_set(0.0, -10.0, -10.0, &light1_position);
    Vec3_set(1.0, 0.0, 0.0, &light1_color);

    Light_set( light1_position, light1_color, 0.0002f, &lights[1] );

    //MATERIALS

    //define some basic colors
    Vec3 grey;
    Vec3_set(0.12, 0.12, 0.12, &grey);

    Material earth, mars, mirror, jade;
    
    //earth
    Image earth_texture;

    //mars
    Image mars_texture;

    //mirror
    Image mirror_ref;

    //jade
    Image jade_tex;

    //SPHERES
    Sphere * spheres;
    spheres = (Sphere *) malloc( sizeof( Sphere ) * 9);

    Vec3 sphere0_center = { 0, 0, 4 };
    Sphere_set( &sphere0_center, 1.5f, &mirror, &spheres[0] );

    Vec3 sphere1_center = { 0, 5, -3};
    Sphere_set( &sphere1_center, 1, &jade, &spheres[1] );

    Vec3 sphere2_center = { 0, -5, -3};
    Sphere_set( &sphere2_center, 1, &earth, &spheres[2] );

    Vec3 sphere3_center = { 0, 5, 7};
    Sphere_set( &sphere3_center, 1, &mars, &spheres[3] );

    Vec3 sphere4_center = { 0, -5, 7};
    Sphere_set( &sphere4_center, 1, &jade, &spheres[4] );

    Vec3 sphere5_center = { 5, 0, 7};
    Sphere_set( &sphere5_center, 1, &earth, &spheres[5] );

    Vec3 sphere6_center = { -5, 0, 7};
    Sphere_set( &sphere6_center, 1, &jade, &spheres[6] );

    Vec3 sphere7_center = { 5, 0, -3 };
    Sphere_set( &sphere7_center, 1, &jade, &spheres[7] );

    Vec3 sphere8_center = { -5, 0, -3};
    Sphere_set( &sphere8_center, 1, &mars, &spheres[8] );
    
    Scene scene;
    Image skybox;
    scene.light_count = 1;
    scene.sphere_count = 9;

    float fov[2] = {90, 90};
    float position[3] = {0.0, 0.0, 0.0};
    float dir[3] = {0.0, 0.0, 1.0};

    //rank 0
    if (rank == 0)
    {

        float ntsqr = sqrtf(numtasks - 1);

        Image img;
        short res[2] = {IM_SIZE, IM_SIZE};
        Image_set(res[0], res[1], &img);
        Camera_set( position, dir, res, fov, &camera );

        printf("proc %d ===> %d, %d\n", rank, img.height, img.width);

        //MATERIALS
        Image_import(&earth_texture, "assets/textures/earth.bmp");
        Material_set(&earth, &earth_texture, NULL);

        Image_import(&mars_texture, "assets/textures/mars.bmp");
        Material_set(&mars, &mars_texture, NULL);

        Image_import(&mirror_ref, "assets/textures/copper.bmp");
        Material_set(&mirror, &mirror_ref, &mirror_ref);

        Image_import(&jade_tex, "assets/textures/jade_texture.bmp");
        Material_set(&jade, &jade_tex, &jade_tex);

        Image_import(&skybox, "assets/skybox.bmp");
        Scene_set( &camera, spheres, lights, &skybox, &scene );

        Camera_genDirectionMatrix(&camera);
        //BROADCAST NEEDED INFORMATIONS TO PROCESSES

        //first we broadcast information concernig textures (file wont be duplicated on each drive of each machines we need to send data trough network)

        //SKYBOX TEXTURE INFO
        MPI_Bcast(&skybox.height, 1, MPI_SHORT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&skybox.width, 1, MPI_SHORT, 0, MPI_COMM_WORLD);
        MPI_Bcast(skybox.data, skybox.height * skybox.width * 3, MPI_FLOAT, 0, MPI_COMM_WORLD);

        //EARTH TEXTURE INFO
        MPI_Bcast(&earth_texture.height, 1, MPI_SHORT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&earth_texture.width, 1, MPI_SHORT, 0, MPI_COMM_WORLD);
        MPI_Bcast(earth_texture.data, earth_texture.height * earth_texture.width * 3, MPI_FLOAT, 0, MPI_COMM_WORLD);

        //MARS TEXTURE INFO
        MPI_Bcast(&mars_texture.height, 1, MPI_SHORT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&mars_texture.width, 1, MPI_SHORT, 0, MPI_COMM_WORLD);
        MPI_Bcast(mars_texture.data, mars_texture.height * mars_texture.width * 3, MPI_FLOAT, 0, MPI_COMM_WORLD);

        //JADE TEXTURE INFO
        MPI_Bcast(&jade_tex.height, 1, MPI_SHORT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&jade_tex.width, 1, MPI_SHORT, 0, MPI_COMM_WORLD);
        MPI_Bcast(jade_tex.data, jade_tex.height * jade_tex.width * 3, MPI_FLOAT, 0, MPI_COMM_WORLD);

        //MIRROR(COPPER) TEXTURE INFO
        MPI_Bcast(&mirror_ref.height, 1, MPI_SHORT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&mirror_ref.width, 1, MPI_SHORT, 0, MPI_COMM_WORLD);
        MPI_Bcast(mirror_ref.data, mirror_ref.height * mirror_ref.width * 3, MPI_FLOAT, 0, MPI_COMM_WORLD);

        short local_res[2];
        local_res[0] = res[0] / (short) ntsqr;
        local_res[1] = res[1] / (short) ntsqr;

        //SEND CAMERA PARTIAL INFO (PART OF SCENE THAT EACH WORKERS WILL HAVE TO CALCULATE)
        for (int p = 1; p < numtasks; p++)
        {
            MPI_Send(local_res, 2, MPI_SHORT, p, 0, MPI_COMM_WORLD);
            //send part of this so that we can use it on processes 'camera.directionMatrix'
            MPI_Send(&camera.directionMatrix[(p-1) * local_res[0] * local_res[1] * 3] , local_res[0] * local_res[1] * 3 , MPI_FLOAT, p, 0, MPI_COMM_WORLD);
        }

        //RECEIVE RESULT
        for (unsigned char t = 0; t < 400; t++)
        {

            
            for (int p = 1; p < numtasks; p++)
            {
                MPI_Recv(img.data + (numtasks - (p+1)) * local_res[0] * local_res[1] * 3, local_res[0] * local_res[1] * 3, MPI_FLOAT, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }

            char filename[100];
            sprintf(filename, "output/image%d.bmp", t);

            Image average;
            Image_set(res[0], res[1], &average);
            Image_average(3, &img, &average);
            //Image_export(&average, filename );
            Image_free(&average);

            printf("finished image %d !\n", t);

        }
        

        MPI_Finalize ();        

    }

    else if (rank != 0) //NOT MASTER 

    {

        //BROADCAST NEEDED INFORMATIONS TO PROCESSES

        //receive info from master process

        //SKYBOX TEXTURE INFO
        MPI_Bcast(&skybox.height, 1, MPI_SHORT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&skybox.width, 1, MPI_SHORT, 0, MPI_COMM_WORLD);
        //we dont forget to allocate memory :3
        skybox.data = (float *) malloc( sizeof( float ) * skybox.height * skybox.width * 3);
        MPI_Bcast(skybox.data, skybox.height * skybox.width * 3, MPI_FLOAT, 0, MPI_COMM_WORLD);

        //EARTH TEXTURE INFO
        MPI_Bcast(&earth_texture.height, 1, MPI_SHORT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&earth_texture.width, 1, MPI_SHORT, 0, MPI_COMM_WORLD);
        //we dont forget to allocate memory :3
        earth_texture.data = (float *) malloc( sizeof( float ) * earth_texture.height * earth_texture.width * 3);
        MPI_Bcast(earth_texture.data, earth_texture.height * earth_texture.width * 3, MPI_FLOAT, 0, MPI_COMM_WORLD);

        //MARS TEXTURE INFO
        MPI_Bcast(&mars_texture.height, 1, MPI_SHORT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&mars_texture.width, 1, MPI_SHORT, 0, MPI_COMM_WORLD);
        //we dont forget to allocate memory :3
        mars_texture.data = (float *) malloc( sizeof( float ) * mars_texture.height * mars_texture.width * 3);
        MPI_Bcast(mars_texture.data, mars_texture.height * mars_texture.width * 3, MPI_FLOAT, 0, MPI_COMM_WORLD);

        //JADE TEXTURE INFO
        MPI_Bcast(&jade_tex.height, 1, MPI_SHORT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&jade_tex.width, 1, MPI_SHORT, 0, MPI_COMM_WORLD);
        //we dont forget to allocate memory :3
        jade_tex.data = (float *) malloc( sizeof( float ) * jade_tex.height * jade_tex.width * 3);
        MPI_Bcast(jade_tex.data, jade_tex.height * jade_tex.width * 3, MPI_FLOAT, 0, MPI_COMM_WORLD);

        //MIRROR(COPPER) TEXTURE INFO
        MPI_Bcast(&mirror_ref.height, 1, MPI_SHORT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&mirror_ref.width, 1, MPI_SHORT, 0, MPI_COMM_WORLD);
        //we dont forget to allocate memory :3
        mirror_ref.data = (float *) malloc( sizeof( float ) * mirror_ref.height * mirror_ref.width * 3);
        MPI_Bcast(mirror_ref.data, mirror_ref.height * mirror_ref.width * 3, MPI_FLOAT, 0, MPI_COMM_WORLD);
        
        //MATERIALS
        Material_set(&earth, &earth_texture, NULL);
        Material_set(&mars, &mars_texture, NULL);
        Material_set(&mirror, &mirror_ref, &mirror_ref);
        Material_set(&jade, &jade_tex, &jade_tex);

        //CAMERA/SCENE RECONSTRUCTION (LOCAL)
        short local_res[2];
        MPI_Recv(local_res, 2, MPI_SHORT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        Camera_set( position, dir, local_res, fov, &camera );
        Scene_set( &camera, spheres, lights, &skybox, &scene );

        Image local_img; //partial part of image
        Image_set(local_res[0], local_res[1], &local_img);
        printf("proc %d ===> %d, %d\n", rank, local_res[0], local_res[1]);

        camera.directionMatrix = (float *) malloc( sizeof(float) * local_res[0] * local_res[1] * 3);
        MPI_Recv(camera.directionMatrix, local_res[0] * local_res[1] * 3, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


        for (unsigned char t = 0; t < 120; t++)
        {

            for (int it = 1; it < scene.sphere_count; it++)
            {
                char s = 1;
                if (scene.spheres[it].center->z > 0)
                    s = -1;

                scene.spheres[it].center->x = s * cosf( M_PIF/4 + M_PIF * t / 60 + it%4 * M_PIF/2) * 5;
                scene.spheres[it].center->y = s * sinf( M_PIF/4 + M_PIF * t / 60 + it%4 * M_PIF/2) * 5;
            }

            Scene_set(&camera, spheres, lights, &skybox, &scene);
            Scene_render(&scene, &local_img);

            //send back result to master
            MPI_Send(local_img.data, local_res[0] * local_res[1] * 3, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
        }
        

        MPI_Finalize ();

    }
    
/**
    for (unsigned int t = 0; t < 1; t++)
    {
        clock_t start = clock();

        for (int it = 1; it < scene.sphere_count; it++)
        {
            scene.spheres[it].center->x = cosf( M_PIF/4 + M_PIF * t / 30 + it%4 * M_PIF/2) * 5;
            scene.spheres[it].center->y = sinf( M_PIF/4 + M_PIF * t / 30 + it%4 * M_PIF/2) * 5;
        }

        Scene_render(&scene, &img);

        clock_t end = clock();
        double elapsed = (double) (end - start)/CLOCKS_PER_SEC;
        printf("Ellapsed time (Casting Rays) : %3.4fs\t", elapsed);

        start = clock();r_ref.width * 3, MPI_FLOAT, 0, MPI_COMM_WORLD);
        
        //MATERIALS
        Material_set(&earth, &earth_texture, NULL);
        Material_set(&mars, &mars_texture, NULL);
        Material_set(&mirror, &mirror_ref, &mirror_ref);
        Material_set(&jade, &jade_tex, &jade_tex);

        //CAMERA/SCENE RECONSTRUCTION (LOCAL)
        short local_res[2];
        MPI_Recv(local_res, 2, MPI_SHORT, 0, 0, MPI_COMM_W
        Image_free(&average);

        end = clock();
        elapsed = (double) (end - start)/CLOCKS_PER_SEC;
        printf("Ellapsed time (Exporting image) : %3.4fs\n", elapsed);

    }

    Camera_free( &camera );
    Image_free(&img);
**/

}