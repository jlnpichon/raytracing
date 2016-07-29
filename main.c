#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "la.h"

#include "camera.h"
#include "image.h"
#include "light.h"
#include "object.h"
#include "ray.h"
#include "shading.h"

struct scene {
	struct camera *camera;
};

struct material {
	vec3 diffuse;
};

static object_array_t lights;
static object_array_t objects;

static void usage(const char *program)
{
	fprintf(stderr, "usage: %s -o,--output <image-name>\n", program);
}

int main(int argc, char *argv[])
{
	int i;
	int stop_options;
	int screen_width = 800;
	int screen_height = 600;
	struct image *image;
	char *error;
	struct camera *camera;
	struct object *sphere;
	struct object *light;
	char *filename;

	filename = NULL;
	stop_options = 0;
	for (i = 1; i < argc; i++) {
		if (!stop_options) {
			if (!strcmp(argv[i], "--")) {
				stop_options = 1;
				continue;
			}
			if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
				usage(argv[0]);
				return 0;
			}
			if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output")) {
				filename = argv[++i];
				continue;
			}

			fprintf(stderr, "unkown option '%s'\n", argv[i]);
			usage(argv[0]);
			return 1;
		}
	}

	if (!filename) {
		fprintf(stderr, "missing option -o\n");
		usage(argv[0]);
		return 1;
	}

	image = image_alloc(screen_width, screen_height);
	if (!image) {
		fprintf(stderr, "can't allocate image\n");
		return 1;
	}

	light = light_alloc(vec3f(1.f, 1.f, 1.f));
	object_transform_add(light, mat4_translation(vec3f(0.f, 5.f, 2.f)));
	object_array_push(&lights, &light);

	sphere = sphere_alloc(2.f);
	object_transform_add(sphere, mat4_translation(vec3f(0.f, 0.f, 0.f)));
	object_array_push(&objects, &sphere);

	sphere = sphere_alloc(0.5f);
	object_transform_add(sphere, mat4_translation(vec3f(0.f, 2.f, 2.f)));
	object_array_push(&objects, &sphere);

	camera = camera_alloc(screen_width, screen_height, 90.f,
			vec3f(0.f, 0.f, 5.f), vec3f(0.f, 0.f, 0.f));

	size_t j;
	size_t hits_count = 0;
	vec3 view = vec3_normalize(vec3_sub(camera->lookat, camera->eye));

	for (i = 0; i < screen_height; i++) {
		for (j = 0; j < screen_width; j++) {
			struct hit hit;
			struct ray ray = camera_ray(camera, vec2f((float) j, (float) i));

			if (ray_intersect(&ray, &hit, &objects)) {
				struct pixel *pixel = &image->pixels[i * screen_width + j];

#if 0
				mat4 world_to_camera = mat4_inverse(&camera->to_world);
				hit.normal = mat4_mulv4(&world_to_camera, vec4_vec3(hit.normal, 0.f)).v3;
				hit.normal = vec3_add(vec3_mulf(hit.normal, 0.5f), vec3f(0.5f, 0.5f, 0.5f));
				pixel->red = hit.normal.x * 255;
				pixel->green = hit.normal.y * 255;
				pixel->blue = hit.normal.z * 255;
#endif
				vec3 color = shade(&ray, &hit, view, &objects, &lights);
				color = vec3_clamp(color, 0.f, 1.f);
				pixel->red = color.x * 255;
				pixel->green = color.y * 255;
				pixel->blue = color.z * 255;

				hits_count++;
			}
		}
	}

	fprintf(stderr, "hits count: %ld (%.02f)\n",
			hits_count, (float) hits_count / (float) (screen_width * screen_height));

	if (image_save(image, filename, &error) < 0) {
		fprintf(stderr, "can't save image to disk: %s\n", error);
		free(error);
		image_free(image);
		return 1;
	}

	image_free(image);

	return 0;
}
