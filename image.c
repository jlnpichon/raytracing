#include <stdio.h>
#include <stdlib.h>

#include <png.h>

#include "image.h"

struct image *image_alloc(size_t width, size_t height)
{
	struct image *image;

	image = calloc(1, sizeof(struct image));
	if (!image)
		return NULL;

	image->pixels = calloc(width * height, sizeof(struct pixel));
	if (!image->pixels) {
		free(image);
		return NULL;
	}

	image->width = width;
	image->height = height;

	return image;
}

void image_free(struct image *image)
{
	free(image);
	free(image->pixels);
}

int image_save(struct image *image, const char *filename,
		char **error)
{
	FILE *fp;
	png_structp png;
	png_infop info;

	*error = NULL;
	fp = fopen(filename, "wb");
	if (!fp) {
		asprintf(error, "fopen fail: %m");
		return -1;
	}

	png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
	if (!png) {
		asprintf(error, "png_create_write_struct fail");
		fclose(fp);
		return -1;
	}
	info = png_create_info_struct(png);
	if (!info) {
		asprintf(error, "png_create_write_struct fail");
		png_destroy_write_struct(&png, NULL);
		fclose(fp);
		return -1;
	}

	if (setjmp(png_jmpbuf(png))) {
		asprintf(error, "png_jmpbuf fail");
		png_destroy_write_struct(&png, &info);
		fclose(fp);
		return -1;
	}

	png_init_io(png, fp);

	png_set_IHDR(png, info, image->width, image->height, 8,
			PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png, info);

	size_t i, j;
	png_bytep row = (png_bytep) malloc(3 * image->width * sizeof(png_byte));
	for (i = 0; i < image->height; i++) {
		for (j = 0; j < image->width; j++) {
			row[j * 3 + 0] = image->pixels[i * image->width + j].red;
			row[j * 3 + 1] = image->pixels[i * image->width + j].green;
			row[j * 3 + 2] = image->pixels[i * image->width + j].blue;
		}
		png_write_row(png, row);
	}
	png_write_end(png, NULL);

	png_free_data(png, info, PNG_FREE_ALL, -1);
	png_destroy_write_struct(&png, &info);
	free(row);
	fclose(fp);

	return 0;
}

#if 0
	size_t i, j;
	for (i = 0; i < image->height; i++) {
		for (j = 0; j < image->width; j++) {
			struct pixel *pixel = &image->pixels[i * image->width + j];
			pixel->red = i % 255;
			pixel->green = i % 255;
			pixel->blue = i % 255;
		}
	}
#endif
