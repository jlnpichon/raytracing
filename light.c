#include "light.h"

struct object *light_alloc(vec3 color)
{
	struct object *object;
	struct light *light;

	object = object_alloc(sizeof(*light));
	if (!object)
		return NULL;

	object->intersect = NULL;

	light = (struct light *) object;
	light->color = color;

	return object;
}

vec3 light_ray(struct light *light, vec3 position)
{
	vec3 light_position = mat4_mulv4(&light->object.to_world, vec4f(0.f, 0.f, 0.f, 1.f)).v3;
	return vec3_normalize(vec3_sub(light_position, position));
}
