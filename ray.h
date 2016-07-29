#ifndef RAY_H
#define RAY_H

#include "la.h"

#include "object.h"

struct ray {
	vec3 origin;
	vec3 direction;
};

struct hit {
	vec3 position;
	vec3 normal;
	struct object *object;
	float distance;
};

int ray_intersect(struct ray *ray, struct hit *hit, object_array_t *objects);

#endif /* RAY_H */
