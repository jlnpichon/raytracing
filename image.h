#ifndef IMAGE_H
#define IMAGE_H

#include <inttypes.h>

struct pixel {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

struct image {
	struct pixel *pixels;
	size_t width;
	size_t height;
};

struct image *image_alloc(size_t width, size_t height);
void image_free(struct image *image);

int image_save(struct image *image, const char *filename,
		char **error);

#endif /* IMAGE_H */
