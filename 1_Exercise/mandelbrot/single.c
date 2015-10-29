#include <stdlib.h>
#include <stdio.h>
#include "mandel.h"

int main(int argc, char *argv[])
{
    // input
    uint32_t width = 800;
    uint32_t height = 800;
    uint32_t iterations = 281;

    if(argc > 3) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
        iterations = atoi(argv[3]);
    }

    double startX = -2.0;
    double endX = 1.0;
    double startY = -1.5;
    double endY = 1.5;

    // calculate delta increment and recalculate bounds based on width and height
    double delta = 0.0;
    if(width > height) {
        delta = (endY - startY) / height;
        double midX = (endX + startX) * 0.5;
        startX = midX - (width / 2) * delta;
        endX = midX + (width / 2) * delta;
    }
    else {
        delta = (endX - startX) / width;
        double midY = (endY + startY) * 0.5;
        startY = midY - (height / 2) * delta;
        endY = midY + (height / 2) * delta;
    }

    // make output buffer
    uint32_t *data = (uint32_t*)malloc(sizeof(uint32_t) * width * height);
    if(data == NULL) {
        return EXIT_FAILURE;
    }

    // sample all points
    for(uint32_t h = 0; h < height; ++h) {
        for(uint32_t w = 0; w < width; ++w) {
            uint32_t iter = mandel(startX + w * delta, startY + h * delta, iterations);
            data[width * h + w] = iter;
        }
    }

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
