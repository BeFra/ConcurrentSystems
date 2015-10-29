#include "mandel.h"
#include <math.h>

uint32_t mandel(double startX, double startY, uint32_t max)
{
    double x = 0.0;
    double y = 0.0;

    uint32_t iter = 0;
    for(; (x*x + y*y <= 4.0) && (iter < max); ++iter) {
        double tmp = x*x - y*y + startX;
        y = 2.0 * x * y + startY;
        x = tmp;
    }
    return iter;
}

void getRGB(uint32_t iter, uint32_t max, uint8_t *r, uint8_t *g, uint8_t *b)
{
    const uint8_t colormap[][3] = {
        {255,  64,   0},
        {255, 255,   0},
        {  0,   0,   0}
    };

    const uint32_t colors = sizeof(colormap) / 3;

    if(iter > max) {
        *r = (uint8_t)0;
        *g = (uint8_t)0;
        *b = (uint8_t)0;
        return;
    }

    double dindex = (colors - 1) * (log(iter) / log(max));

    double intpart = 0.0;
    double frac = modf(dindex, &intpart);
    uint32_t nearest = lround(dindex);

    uint32_t other = 0;
    if(nearest > dindex) {
        other = dindex;
    }
    else {
        other = dindex + 1.0;
        frac = 1.0 - frac;
    }

    if(other >= colors) {
        other = colors - 1;
    }

    *r = (uint8_t)lround(frac * colormap[nearest][0] + (1.0 - frac) * colormap[other][0]);
    *g = (uint8_t)lround(frac * colormap[nearest][1] + (1.0 - frac) * colormap[other][1]);
    *b = (uint8_t)lround(frac * colormap[nearest][2] + (1.0 - frac) * colormap[other][2]);
}
