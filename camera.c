#include <stdlib.h>

#include "camera.h"
#include "ray.h"

static void camera_update(struct camera *camera)
{
	camera->aspect = camera->screen_width / camera->screen_height;
	camera->scale = tan(camera->fov * 0.5 * PI_OVER_180); 

	camera->to_world = mat4_view(camera->eye, camera->lookat, vec3f(0.f, 1.f, 0.f));
	camera->to_world = mat4_inverse(&camera->to_world);
}

struct camera *camera_alloc(int screen_width, int screen_height, float fov,
		vec3 eye, vec3 lookat)
{
	struct camera *camera;

	camera = calloc(1, sizeof(*camera));
	if (!camera)
		return NULL;

	camera->eye = eye;
	camera->lookat = lookat;
	camera->screen_width = (float) screen_width;
	camera->screen_height = (float) screen_height;
	camera->fov = fov;

	camera_update(camera);

	return camera;
}

struct ray camera_ray(struct camera *camera, vec2 pixel)
{
	struct ray ray;
	ray.origin = mat4_mulv4(&camera->to_world, vec4f(0.f, 0.f, 0.f, 1.f)).v3;

	float x = (2 * (pixel.x + 0.5) / (float) camera->screen_width - 1) * camera->aspect * camera->scale; 
	float y = (1 - 2 * (pixel.y + 0.5) / (float) camera->screen_height) * camera->scale;

	ray.direction = mat4_mulv4(&camera->to_world, vec4f(-x, y, -1, 0.f)).v3;
	ray.direction = vec3_normalize(ray.direction);

	return ray;
}
