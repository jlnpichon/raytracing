#ifndef CAMERA_H
#define CAMERA_H

#include "la.h"

struct ray;

struct camera {
	vec3 eye;
	vec3 lookat;
	mat4 to_world;

	float fov;
	float scale;
	float aspect;
	float screen_width;
	float screen_height;
};

struct camera *camera_alloc(int screen_width, int screen_height, float fov,
		vec3 eye, vec3 lookat);

struct ray camera_ray(struct camera *camera, vec2 pixel);

#endif /* CAMERA_H */
