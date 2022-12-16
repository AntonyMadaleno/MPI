#include "Image.h"
#include "stdio.h"
#include "stdlib.h"

void Image_set(unsigned short height, unsigned short width, Image * img)
{
    img->height = height;
    img->width = width;

    img->data = (float *) malloc( width * height * 3 * sizeof(float) );
}

void Image_free(Image * img)
{
    free(img->data);
}

void Image_getPixel(Image * img, unsigned short x, unsigned short y, Vec3 * output)
{
    unsigned int i = ( x + (y * img->width) ) * 3;

    output->x = img->data[ i ];
    output->y = img->data[ i + 1 ];
    output->z = img->data[ i + 2 ];
}

void Image_setPixel(Image * img, unsigned short x, unsigned short y, Vec3 * pixel)
{
    unsigned int i = ( x + (y * img->width) ) * 3;

    img->data[i + 0] = pixel->x;
    img->data[i + 1] = pixel->y;
    img->data[i + 2] = pixel->z;
}

void Image_average(unsigned char size, Image * img, Image * output)
{

    for (unsigned short x = 0; x < img->width; x++)
    {
        Vec3 pixel;

        for (unsigned char a = 0; a <= size / 2; a++)
        {
            Image_getPixel(img, x, a, &pixel);
            Image_setPixel(output, x, a, &pixel);

            Image_getPixel(img, x, img->height - 1 - a, &pixel);
            Image_setPixel(output, x, img->height - 1 - a, &pixel);
        }
    }

    for (unsigned short y = 0; y < img->width; y++)
    {
        Vec3 pixel;
        for (unsigned char a = 0; a <= size / 2; a++)
        {
            Image_getPixel(img, a, y, &pixel);
            Image_setPixel(output, a, y, &pixel);

            Image_getPixel(img, img->width -a -1, y, &pixel);
            Image_setPixel(output, img->width -a -1, y, &pixel);
        }
    }

    for (unsigned short x = size / 2; x < img->width - size/2; x++)
    {

        for (unsigned short y = size/2; y < img->height - size/2; y++)
        {

            Vec3 average;
            Vec3_set(0, 0, 0, &average);

            for (unsigned char i = 0; i < size; i++)
            {
                for (unsigned char j = 0; j < size; j++)
                {

                    Vec3 current_pix;
                    Image_getPixel(img, x + i - size/2, y + j - size/2, &current_pix);
                    average.x += current_pix.x / (size * size);
                    average.y += current_pix.y / (size * size);
                    average.z += current_pix.z / (size * size);

                }

                Image_setPixel(output, x, y, &average);
            }


        }

    }
}

void Image_import(Image * img, char * filepath)
{
    //get data from a bmp file
    FILE * file;
    file = fopen(filepath, "rb");

    if( file != NULL)
    {

        unsigned char * header = (unsigned char *) malloc( sizeof(unsigned char) * 14);
        unsigned char * info_header = (unsigned char *) malloc( sizeof(unsigned char) * 40);

        fread(header, sizeof(char), 14, file);
        fread(info_header, sizeof(char), 40, file);

        unsigned int width = 0;
        unsigned int height = 0;

        width += info_header[4]     << 0;
        width += info_header[5]     << 8;
        width += info_header[6]     << 16;
        width += info_header[7]     << 24;

        height += info_header[8]    << 0;
        height += info_header[9]    << 8;
        height += info_header[10]   << 16;
        height += info_header[11]   << 24;

        unsigned char * data = (unsigned char *) malloc( sizeof(unsigned char) * 3);
        img->height = height;
        img->width = width;
        img->data = (float *) malloc( sizeof(float) * height * width * 3);

        free(header);
        free(info_header);

        for (unsigned short y = 0; y < height; y++)
        {
            for (unsigned short x = 0; x < width; x++)
            {
                fread(data, sizeof(unsigned char), 3, file);
                Vec3 * pixel = (Vec3 *) malloc( sizeof(Vec3) );
                Vec3_set(
                    (float) data[2] / 255.0f ,
                    (float) data[1] / 255.0f ,
                    (float) data[0] / 255.0f ,
                    pixel);
                Image_setPixel(img, x, y, pixel);
            }
        }

        free(data);
    }

}

void Image_export(Image * img, char * filepath)
{
    FILE * file;
    file = fopen(filepath, "wb");

    if( file != NULL)
    {
        unsigned char bmppad[1] = { 0 };

        const unsigned char padding_amount = ( 4 - ( img->width * 3 ) % 4) % 4;
        const unsigned char header_size = 14;
        const unsigned char info_header_size = 40;
        const unsigned int file_size = header_size + info_header_size + img->width * img->height * 3 + padding_amount * img->height;

        unsigned char file_header[ header_size ];

        //FILE TYPE
        file_header[0] = 'B';
        file_header[1] = 'M';

        //FILE SIZE
        file_header[2] = file_size;
        file_header[3] = file_size >> 8;
        file_header[4] = file_size >> 16;
        file_header[5] = file_size >> 24;

        //RESERVED 0 Not Used
        file_header[6] = 0;
        file_header[7] = 0;

        //RESERVED 1 Not Used
        file_header[8] = 0;
        file_header[9] = 0;

        //PIXEL DATA OFFSET
        file_header[10] = header_size + info_header_size;
        file_header[11] = 0;
        file_header[12] = 0;
        file_header[13] = 0;

        unsigned char information_header[ info_header_size ];

        //HEADER SIZE
        information_header[0] = info_header_size;
        information_header[1] = 0;
        information_header[2] = 0;
        information_header[3] = 0;

        //IMAGE WIDTH
        information_header[4] = img->width;
        information_header[5] = img->width >> 8;
        information_header[6] = img->width >> 16;
        information_header[7] = img->width >> 24;

        //IMAGE HEIGHT
        information_header[8]  = img->height;
        information_header[9]  = img->height >> 8;
        information_header[10] = img->height >> 16;
        information_header[11] = img->height >> 24;

        //PLANES
        information_header[12] = 1;
        information_header[13] = 0;

        //BITS PER PIXELS
        information_header[14] = 24;
        information_header[15] = 0;

        //COMPRESSION (NO COMPRESSION)
        information_header[16] = 0;
        information_header[17] = 0;
        information_header[18] = 0;
        information_header[19] = 0;

        //IMAGE SIZE (NO COMPRESSION)
        information_header[20] = 0;
        information_header[21] = 0;
        information_header[22] = 0;
        information_header[23] = 0;

        //X PIXEL PER METER
        information_header[24] = 0;
        information_header[25] = 0;
        information_header[26] = 0;
        information_header[27] = 0;

        //Y PIXEL PER METER
        information_header[28] = 0;
        information_header[29] = 0;
        information_header[30] = 0;
        information_header[31] = 0;

        //TOTAL COLORS
        information_header[32] = 0;
        information_header[33] = 0;
        information_header[34] = 0;
        information_header[35] = 0;

        //IMPORTANT COLORS [IGNORED]
        information_header[36] = 0;
        information_header[37] = 0;
        information_header[38] = 0;
        information_header[39] = 0;

        fwrite((char *) file_header, header_size, 1, file);
        fwrite((char *) information_header, info_header_size, 1, file);

        for (unsigned short y = 0; y < img->height; y++)
        {
            for (unsigned short x = 0; x < img->width; x++)
            {
                Vec3 pixel;
                Image_getPixel(img, (short) x, (short) y, &pixel);
                unsigned char r = (unsigned char) ( pixel.x * 255.0f );
                unsigned char g = (unsigned char) ( pixel.y * 255.0f );
                unsigned char b = (unsigned char) ( pixel.z * 255.0f );

                unsigned char color[3] = { b, g, r };
                fwrite((char *) color, sizeof(char) * 3, 1, file);
            }

            for (unsigned char p = 0; p < padding_amount; p++)
                fwrite( (char *) bmppad, sizeof(char), 1, file);
        }

    }
    else
        printf("can't open file %s", filepath);

    fclose(file);
}