#include "homework.h"

int num_threads;
int resize_factor;

void readInput(const char * fileName, image *img) {

    FILE *in = fopen(fileName, "rb");
    char buff[255];

    fgets(buff, 10, in); //type line P5/P6
    img->img_type = atoi(buff+1);

    fgets(buff, 255, in); //width height
    img->width = atoi(buff); // up to space
    strcpy(buff, strchr(buff, ' ')); //remove up to space+space
    img->height = atoi(buff);

    fgets(buff, 255, in);
    img->maxval = atoi(buff);

    unsigned char** raster;

    if (img->img_type == 5) { //bw

        raster = malloc(img->height * sizeof(char*));
        int i;

        for (i=0; i<img->height; i++) {
            raster[i] = malloc(sizeof(char) * img->width );
            fread(raster[i], 1, img->width, in);
        }
    }
    if (img->img_type == 6) { //color

        raster = malloc(img->height * sizeof(char*));
        int i;

        for (i=0; i<img->height; i++) {
            raster[i] = malloc(sizeof(char) * img->width *3);
            fread(raster[i], 1, img->width*3, in);
        }
    }

    img->raster=raster;
    fclose(in);
}

void writeData(const char * fileName, image *img) {
    FILE *out = fopen(fileName, "wb");
    fprintf(out, "P%d\n%d %d\n%d\n", img->img_type, img->width, img->height, img->maxval);
    int i=0;
    for(i=0; i<img->height; i++) {
        if (img->img_type == 5)
            fwrite(img->raster[i], 1, img->width, out);
        if (img->img_type == 6)
            fwrite(img->raster[i], 1, img->width*3, out);
        free(img->raster[i]);
    }
    fclose(out);
    free(img->raster);
}

void* threadFunction(void *var) {
    thread_info_t thread_info = *(thread_info_t*)var;
    int thread_id = thread_info.thread_id, sectorSize=thread_info.sectorSize;
    image *in = thread_info.input, *out = thread_info.output;
    int i, j;

    int startLine = thread_id*sectorSize;
    int endLine = (thread_id+1)*sectorSize;
    if (thread_id == num_threads-1) {
        endLine = out->height;
    }

    if (out->img_type == 5) {
        for (i=startLine; i<endLine; i++) {
            for (j=0; j<out->width; j++) {
                //process output pixel out[i][j]
                int sum = 0;
                int inI = i*resize_factor, inJ = j*resize_factor; // start coords
                if (resize_factor%2==0) {
                    int k, l;
                    for (k=inI; k<inI+resize_factor; k++) {
                        for (l=inJ; l<inJ+resize_factor; l++) {
                            sum+=in->raster[k][l];
                        }
                    }
                    sum/=(resize_factor*resize_factor);

                } else if (resize_factor == 3) {
                    sum=sum + in->raster[inI][inJ] + 2* in->raster[inI][inJ+1] + in->raster[inI][inJ+2];
                    sum=sum + 2*in->raster[inI+1][inJ] + 4* in->raster[inI+1][inJ+1] + 2*in->raster[inI+1][inJ+2];
                    sum=sum + in->raster[inI+2][inJ] + 2* in->raster[inI+2][inJ+1] + in->raster[inI+2][inJ+2];
                    sum/=16;
                }
                out->raster[i][j]=sum;
            }
        }
    }

    if (out->img_type == 6) {
        for (i=startLine; i<endLine; i++) {
            for (j=0; j<out->width; j++) {
                //process output pixel out[i][j]
                int sumR = 0, sumG=0, sumB=0;
                int inI = i*resize_factor, inJ = j*resize_factor*3; // start coords

                if (resize_factor%2==0) {
                    int k, l;
                    for (k=inI; k<inI+resize_factor; k++) {
                        for (l=inJ; l<inJ+resize_factor*3; l+=3) {
                            sumR+=in->raster[k][l];
                            sumG+=in->raster[k][l+1];
                            sumB+=in->raster[k][l+2];
                        }
                    }
                    sumR/=(resize_factor*resize_factor);
                    sumG/=(resize_factor*resize_factor);
                    sumB/=(resize_factor*resize_factor);

                } else if (resize_factor == 3) {
                    sumR=sumR + in->raster[inI][inJ] + 2* in->raster[inI][inJ+3] + in->raster[inI][inJ+6];
                    sumR=sumR + 2*in->raster[inI+1][inJ] + 4* in->raster[inI+1][inJ+3] + 2*in->raster[inI+1][inJ+6];
                    sumR=sumR + in->raster[inI+2][inJ] + 2* in->raster[inI+2][inJ+3] + in->raster[inI+2][inJ+6];
                    sumR/=16;

                    sumG=sumG + in->raster[inI][inJ+1] + 2* in->raster[inI][inJ+4] + in->raster[inI][inJ+7];
                    sumG=sumG + 2*in->raster[inI+1][inJ+1] + 4* in->raster[inI+1][inJ+4] + 2*in->raster[inI+1][inJ+7];
                    sumG=sumG + in->raster[inI+2][inJ+1] + 2* in->raster[inI+2][inJ+4] + in->raster[inI+2][inJ+7];
                    sumG/=16;

                    sumB=sumB + in->raster[inI][inJ+2] + 2* in->raster[inI][inJ+5] + in->raster[inI][inJ+8];
                    sumB=sumB + 2*in->raster[inI+1][inJ+2] + 4* in->raster[inI+1][inJ+5] + 2*in->raster[inI+1][inJ+8];
                    sumB=sumB + in->raster[inI+2][inJ+2] + 2* in->raster[inI+2][inJ+5] + in->raster[inI+2][inJ+8];
                    sumB/=16;
                }
                out->raster[i][j*3]=sumR;
                out->raster[i][j*3+1]=sumG;
                out->raster[i][j*3+2]=sumB;
            }
        }
    }
    return NULL;
}

void resize(image *in, image * out) {
    int outHeight, outWidth, i;
    outHeight = in->height/resize_factor;
    outWidth = in->width/resize_factor;

    out->height = outHeight;
    out->width = outWidth;
    out->maxval = in->maxval;
    out->img_type = in->img_type;


    if (out->img_type == 6) {
        outWidth*=3;
    }

    out->raster = malloc(outHeight * sizeof(char*));
    for (i=0; i<outHeight; i++) {
        out->raster[i] = malloc(outWidth*sizeof(char));
    }

    pthread_t tid[num_threads];
    thread_info_t thread_info[num_threads];
    for(i = 0; i < num_threads; i++) {
        thread_info[i].thread_id = i;
        thread_info[i].sectorSize = outHeight/num_threads;
        thread_info[i].input = in;
        thread_info[i].output = out;
    }
    for(i = 0; i < num_threads; i++) {
        pthread_create(&(tid[i]), NULL, threadFunction, &(thread_info[i]));
    }

    for(i = 0; i < num_threads; i++) {
        pthread_join(tid[i], NULL);
    }

    for(i=0; i<in->height; i++){
        free(in->raster[i]);
    }
    free(in->raster);
}
