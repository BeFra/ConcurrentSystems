//g++ pthread.c mandel.c -o pthread -lpthread

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include "mandel.h"

void* thread(void*);

uint32_t numberOfThreads = 2;
double startX = -2.0;
double endX = 1.0;
double startY = -1.5;
double endY = 1.5;
double delta = 0.0;
// input
uint32_t width = 800;
uint32_t height = 800;
uint32_t iterations = 281;
uint32_t *data;


void* thread(void* pThreadNumber) {


   uint32_t* number = (uint32_t*) pThreadNumber;
   uint32_t begin = (height / numberOfThreads) * *number;
   uint32_t end = (height / numberOfThreads) * (*number+1);

   //fprintf(stderr,"Thread %d, begin height = %d, end heigh = %d\n", *number, begin , end);
   for(uint32_t h = begin; h < end; ++h) {
      for(uint32_t w = 0; w < height; ++w) {
         uint32_t iter = mandel(startX + w * delta, startY + h * delta, iterations);
         data[width * h + w] = iter;
      }
   }

	return NULL;		
}		


int main(int argc, char *argv[])
{
    if(argc > 3) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
        iterations = atoi(argv[3]);
        numberOfThreads = atoi(argv[4]);      
    }


    // calculate delta increment and recalculate bounds based on width and height

    if(width > height) {
        delta = (endY - startY) / height;
        double midX = (endX + startX) * 0.5;
        startX = midX - (width / 2) * delta;
        endX = midX + (width / 2) * delta;
    } else {
        delta = (endX - startX) / width;
        double midY = (endY + startY) * 0.5;
        startY = midY - (height / 2) * delta;
        endY = midY + (height / 2) * delta;
    }

    // make output buffer
    data = (uint32_t*)malloc(sizeof(uint32_t) * width * height);
    if(data == NULL) {
        return EXIT_FAILURE;
    }


   // pthread
   pthread_t *threads = (pthread_t*) malloc(sizeof(pthread_t) * numberOfThreads);
   if(threads == NULL) {
      printf("Threads can't created (%s).\n", strerror(errno));
      return EXIT_FAILURE;
   }

   uint32_t *threadNumbers = (uint32_t*) malloc(sizeof(uint32_t) * numberOfThreads);
   if(threadNumbers == NULL) {
      printf("Can't allocate memory for thread numbers (%s).\n", strerror(errno));
      return EXIT_FAILURE;
   }

   
   //create threads
   for(uint32_t i = 0; i < numberOfThreads; i++) {
      threadNumbers[i] = i;     
      pthread_create(&threads[i],NULL,thread,(void*)&threadNumbers[i]);
   }
 

   for(uint32_t i = 0; i < numberOfThreads; i++) {
      pthread_join(threads[i],NULL);
   }
   free(threads);
   free(threadNumbers);

    // draw the picture in PPM format
    printf("P3\n%u %u\n255\n", width, height);
    for(uint32_t h = 0; h < height; ++h) {
        for(uint32_t w = 0; w < width; ++w) {
            uint32_t iter = data[width * h + w];
            uint8_t r = 0;
            uint8_t g = 0;
            uint8_t b = 0;
            getRGB(iter, iterations, &r, &g, &b);

            printf("%u %u %u\n", (uint32_t)r, (uint32_t)g, (uint32_t)b);
        }
    }

    // free buffer
    free(data);

    return EXIT_SUCCESS; 
}
