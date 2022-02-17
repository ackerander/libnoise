#include "noise.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint8_t pArr[256];

inline float
fade(float t)
{
	return ((6 * t - 15) * t + 10) * t * t * t;
}

inline float
lerp(float t, float a, float b)
{
	return a + t * (b - a);
}

float
powfi(float base, unsigned int exp)
{
	float result = 1;
	while (exp) {
		if (exp & 1)
			result *= base;
		exp /= 2;
		base *= base;
	}
	return result;
}

void
shuf(void *arr, size_t nmemb, size_t size)
{
	void *tmp = malloc(size);
	char *dest, *ptr = arr;
	for (size_t i = 0; i < nmemb - 1; ++i) {
		dest = ptr + size * (rand() % (nmemb - i));
		memcpy(tmp, ptr, size);
		memcpy(ptr, dest, size);
		memcpy(dest, tmp, size);
		ptr += size;
	}
	free(tmp);
}

void
shuf8(void *arr, size_t nmemb)
{
	int8_t tmp;
	size_t dest;
	for (size_t i = 0; i < nmemb - 1; ++i) {
		dest = i + rand() % (nmemb - i);
		tmp = ((int8_t*)arr)[i];
		((int8_t*)arr)[i] = ((int8_t*)arr)[dest];
		((int8_t*)arr)[dest] = tmp;
	}
}

void
shuf16(void *arr, size_t nmemb)
{
	int16_t tmp;
	size_t dest;
	for (size_t i = 0; i < nmemb - 1; ++i) {
		dest = i + rand() % (nmemb - i);
		tmp = ((int16_t*)arr)[i];
		((int16_t*)arr)[i] = ((int16_t*)arr)[dest];
		((int16_t*)arr)[dest] = tmp;
	}
}

void
shuf32(void *arr, size_t nmemb)
{
	int32_t tmp;
	size_t dest;
	for (size_t i = 0; i < nmemb - 1; ++i) {
		dest = i + rand() % (nmemb - i);
		tmp = ((int32_t*)arr)[i];
		((int32_t*)arr)[i] = ((int32_t*)arr)[dest];
		((int32_t*)arr)[dest] = tmp;
	}
}

void
shuf64(void *arr, size_t nmemb)
{
	int64_t tmp;
	size_t dest;
	for (size_t i = 0; i < nmemb - 1; ++i) {
		dest = i + rand() % (nmemb - i);
		tmp = ((int64_t*)arr)[i];
		((int64_t*)arr)[i] = ((int64_t*)arr)[dest];
		((int64_t*)arr)[dest] = tmp;
	}
}

void
initPerlin(unsigned int seed)
{
	srand(seed);
	for (int i = 0; i < 256; ++i)
		pArr[i] = i;
	shuf8(pArr, 256);
}

float
perlin(float x, float y)
{
	static const int8_t constVecs[4][2] = {{1, 1}, {-1, 1}, {-1, -1}, {1, -1}};
	long lx = (long)(x - (x < 0)), ly = (long)(y - (y < 0));
	uint8_t X = (uint8_t)lx, Y = (uint8_t)ly,
		idx0 = pArr[(uint8_t)(pArr[X] + Y)] & 3,
		idx1 = pArr[(uint8_t)(pArr[X] + Y + 1)] & 3,
		idx2 = pArr[(uint8_t)(pArr[(uint8_t)(X + 1)] + Y)] & 3,
		idx3 = pArr[(uint8_t)(pArr[(uint8_t)(X + 1)] + Y + 1)] & 3;
	float xf = x - lx, yf = y - ly, u = fade(xf), v = fade(yf),
		dot0 = xf * constVecs[idx0][0] + yf * constVecs[idx0][1],
		dot1 = xf * constVecs[idx1][0] + (yf - 1) * constVecs[idx1][1],
		dot2 = (xf - 1) * constVecs[idx2][0] + yf * constVecs[idx2][1],
		dot3 = (xf - 1) * constVecs[idx3][0] + (yf - 1) * constVecs[idx3][1];
	return 0.5 * (lerp(u, lerp(v, dot0, dot1), lerp(v, dot2, dot3)) + 1);
}

float
noise(float x, float y, float offsets[const][2], uint8_t octs, float p, float l)
{
	float amp = (1 - p) / (1 - powfi(p, octs)), freq = 1, noiseH = 0;
	for (uint8_t i = 0; i < octs; ++i) {
		noiseH += amp * perlin(x*freq + offsets[i][0], y*freq + offsets[i][1]);
		amp *= p;
		freq *= l;
	}
	return noiseH;
}

void
randCoords(float offsets[][2], float min, float max, size_t nmemb)
{
	float diff = max - min;
	for (size_t i = 0; i < nmemb; ++i) {
		offsets[i][0] = diff * (float)rand() / (float)RAND_MAX + min;
		offsets[i][1] = diff * (float)rand() / (float)RAND_MAX + min;
	}
}
