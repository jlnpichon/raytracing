#include "ray.h"

int ray_intersect(struct ray *ray, struct hit *hit, object_array_t *objects)
{
	int i;

	hit->distance = INFINITY;

	for (i = 0; i < objects->count; i++) {
		struct object *object = objects->data[i];
		struct hit _hit;

		if (object->intersect(object, ray, &_hit)) {
			if (_hit.distance < hit->distance) {
				memcpy(hit, &_hit, sizeof(_hit));
				hit->object = object;
			}
		}
	}

	if (hit->distance == INFINITY)
		return 0;

	return 1;
}
