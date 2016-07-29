#ifndef LIGHT_H
#define LIGHT_H

#include "la.h"

#include "object.h"

struct light {
	struct object object;
	vec3 color;
};

struct object *light_alloc(vec3 color);

vec3 light_ray(struct light *light, vec3 position);

#endif /* LIGHT_H */
