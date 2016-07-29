#include "light.h"
#include "ray.h"
#include "shading.h"

static const float shininess = 32.f;
static const float material_specular = 1.f;
static const float ambient_intensity = 0.05f;
static const vec3 ambient_color = { .r = 1.f, .g = 1.f, .b = 1.f };

static vec3 reflect(vec3 incident, vec3 normal)
{
	vec3 reflected;

	reflected.x = 2 * vec3_dot(normal, incident) * normal.x - incident.x;
	reflected.y = 2 * vec3_dot(normal, incident) * normal.y - incident.y;
	reflected.z = 2 * vec3_dot(normal, incident) * normal.z - incident.z;

	return vec3_normalize(reflected);
}

vec3 lighting_compute(struct light *light, struct hit *hit,
		vec3 view, vec3 reflected)
{
	vec3 L = light_ray(light, hit->position);
	vec3 N = hit->normal;

	float lambertian = max(vec3_dot(N, L), 0.f);
	float specular = 0.f;

	if (lambertian > 0.f) {
		L = vec3_sub(vec3f(0.f, 0.f, 0.f), L);
		vec3 R = reflect(L, N);
		vec3 V = view;

		float specular_angle = max(vec3_dot(R, V), 0.f);
		specular = pow(specular_angle, shininess) * material_specular;
	}

	vec3 diffuse_color = vec3_mulf(vec3f(1.f, 0.f, 0.f), lambertian);
	vec3 specular_color = vec3_mulf(light->color, specular);

	vec3 color = vec3_add(diffuse_color, specular_color);

	return color;
}

vec3 shade(struct ray *ray, struct hit *hit, vec3 view,
		object_array_t *objects, object_array_t *lights)
{
	int i, j;
	vec3 reflected = reflect(ray->direction, hit->normal);
	vec3 color = vec3_mulf(ambient_color, ambient_intensity);

	for (i = 0; i < lights->count; i++) {
		struct light *light = (struct light *) lights->data[i];
		struct ray shadow_ray; 
		int shadow;

		shadow_ray.origin = hit->position;
		shadow_ray.direction = light_ray(light, hit->position); 
		for (shadow = 0, j = 0; j < objects->count; j++) {
			struct object *object = objects->data[j];
			struct hit _hit;
			
			/* do not intersect with ourself */
			if (object == hit->object)
				continue;

			if (object->intersect(object, &shadow_ray, &_hit)) {
				shadow = 1;
				break;
			}
		}
		if (!shadow)
			color = vec3_add(color, lighting_compute(light, hit, view, reflected));
	}

	return color;
}

