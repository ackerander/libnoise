#ifndef LIBNOISE_H
#define LIBNOISE_H

#include <stddef.h>
#include <stdint.h>

void shuf(void *arr, size_t nmemb, size_t size);
void shuf8(void *arr, size_t nmemb);
void shuf16(void *arr, size_t nmemb);
void shuf32(void *arr, size_t nmemb);
void shuf64(void *arr, size_t nmemb);
void initPerlin(unsigned int seed);
float perlin(float x, float y);
float noise(float x, float y, float offsets[][2], uint8_t octs, float p, float l);
void randCoords(float offsets[][2], float min, float max, size_t nmemb);

#endif
