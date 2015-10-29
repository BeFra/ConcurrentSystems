#ifndef MANDELBROT_SET_INCLUDE
#define MANDELBROT_SET_INCLUDE

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t mandel(double startX, double startY, uint32_t max);
void getRGB(uint32_t iter, uint32_t max, uint8_t *r, uint8_t *g, uint8_t *b);

#ifdef __cplusplus
}
#endif

#endif
