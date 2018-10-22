#include "homework1.h"

int num_threads;
int resolution;

void initialize(image *img) {
    img->height = resolution;
    img->width = resolution;
    img->img_type = 5;
    img->maxval = 255;
    int i;

    img->raster = malloc(img->height * sizeof(char*));
    for (i=0; i<img->height; i++) {
        img->raster[i] = malloc(sizeof(char) * img->width );
        memset(img->raster[i], img->maxval, img->width); //start with a white image
    }
}

float getDistanceToLine(float x, float y) {
    float divident = abs(-1*x + 2*y); //-x +2y +0 = 0
    return abs(divident*100/resolution) / SQRT5; // sqrt(a*a + b*b)
}


void* threadFunction(void *var) {
    thread_info_t thread_info = *(thread_info_t*)var;
    int thread_id = thread_info.thread_id, sectorSize=thread_info.sectorSize;
    image *out = thread_info.output;
    int i, j;

    int startLine = thread_id*sectorSize;
    int endLine = (thread_id+1)*sectorSize;
    if (thread_id == num_threads-1) {
        endLine = out->height;
    }

    for (i=startLine; i<endLine; i++){
        for (j=0; j<out->width; j++) {
            if (getDistanceToLine(i+0.5, j+0.5)<3){
                out->raster[out->height-j-1][i]=0; // flips the image to align (0,0) to down-left
            }
        }
    }
    return NULL;
}

void render(image *img) {
    int i;
    pthread_t tid[num_threads];
    thread_info_t thread_info[num_threads];

    for(i = 0; i < num_threads; i++) {
        thread_info[i].thread_id = i;
        thread_info[i].sectorSize = resolution/num_threads;
        thread_info[i].output = img;
    }

    for(i = 0; i < num_threads; i++) {
        pthread_create(&(tid[i]), NULL, threadFunction, &(thread_info[i]));
    }

    for(i = 0; i < num_threads; i++) {
        pthread_join(tid[i], NULL);
    }
}

void writeData(const char * fileName, image *img) {
    FILE *out = fopen(fileName, "wb");
    fprintf(out, "P%d\n%d %d\n%d\n", img->img_type, img->width, img->height, img->maxval);
    int i=0;
    for(i=0; i<img->height; i++) {
        fwrite(img->raster[i], 1, img->width, out);
        free(img->raster[i]);
    }
    free(img->raster);
    fclose(out);
}

