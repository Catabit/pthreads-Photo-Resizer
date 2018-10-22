#ifndef HOMEWORK_H
#define HOMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct
{
    int img_type; //5/6
    int maxval;
    int width, height;
    unsigned char** raster;
} image;

typedef struct {
    int thread_id, sectorSize;
    image *input, *output;
} thread_info_t;

void readInput(const char * fileName, image *img);

void writeData(const char * fileName, image *img);

void resize(image *in, image * out);

#endif /* HOMEWORK_H */
