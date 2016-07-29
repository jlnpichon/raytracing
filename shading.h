#ifndef SHADE_H
#define SHADE_H

#include "la.h"

#include "object.h"

vec3 shade(struct ray *ray, struct hit *hit, vec3 view,
		object_array_t *objects, object_array_t *lights);

#endif /* SHADE_H */
