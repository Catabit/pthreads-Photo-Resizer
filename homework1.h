#ifndef HOMEWORK_H1
#define HOMEWORK_H1

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define SQRT5 2.2360679

typedef struct
{
    int img_type; //5/6
    int maxval;
    int width, height;
    unsigned char** raster;
} image;

typedef struct {
    int thread_id, sectorSize;
    image *output;
} thread_info_t;


void initialize(image *im);
void render(image *im);
void writeData(const char * fileName, image *img);

#endif /* HOMEWORK_H1 */
