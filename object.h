#ifndef OBJECT_H
#define OBJECT_H

#include "array.h"
#include "la.h"

struct hit;
struct ray;

struct object {
	mat4 to_world;
	mat4 to_object;
	int (*intersect) (struct object *, struct ray *, struct hit *);
};

struct sphere {
	struct object object;
	float radius;
};

DECLARE_C_ARRAY(object_array, struct object *);

struct object *object_alloc(size_t bytes);
void object_transform_add(struct object *object, mat4 tranform);

struct object *sphere_alloc(float radius);

#endif /* OBJECT_H */
