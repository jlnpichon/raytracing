#include "object.h"
#include "ray.h"

IMPLEMENT_C_ARRAY(object_array);

static inline void swapf(float *a, float *b)
{
	float tmp = *a;
	*a = *b;
	*b = tmp;
}

struct object *object_alloc(size_t bytes)
{
	struct object *object;
	
	object = calloc(1, bytes);
	if (!object)
		return NULL;

	object->to_world = mat4_identity();
	object->to_object = mat4_identity();

	return object;
}

void object_transform_add(struct object *object, mat4 tranform)
{
	object->to_world = mat4_mul(&object->to_world, &tranform);
	object->to_object = mat4_inverse(&object->to_world);
}

static int quadratic_solve(float a, float b, float c, float *x0, float *x1) 
{ 
	float discr = b * b - 4 * a * c; 
	if (discr < 0) {
		return 0; 
	} else if (discr == 0) { 
		*x0 = *x1 = - 0.5 * b / a; 
	} else {
		float q = (b > 0) ? 
			-0.5 * (b + sqrt(discr)) : 
			-0.5 * (b - sqrt(discr)); 
		*x0 = q / a; 
		*x1 = c / q; 
	} 

	return 1; 
} 
static int sphere_intersect(struct object *object, struct ray *ray, struct hit *hit)
{
	struct sphere *sphere = (struct sphere *) object;

	float t0, t1;
	float radius2 = sphere->radius * sphere->radius;

	/* world position center of sphere */
	vec3 center = mat4_mulv4(&object->to_world, vec4_vec3(vec3f(0.f, 0.f, 0.f), 1.f)).v3;
#if 0
	vec3 L = vec3_sub(center, ray->origin); 
	float tca = vec3_dot(L, ray->direction); 
	if (tca < 0)
		return 0; 
	float d2 = vec3_dot(L, L) - tca * tca; 
	if (d2 > radius2)
		return 0; 
	float thc = sqrt(radius2 - d2); 
	t0 = tca - thc; 
	t1 = tca + thc; 
#else
	vec3 L = vec3_sub(ray->origin, center); 
	float a = vec3_dot(ray->direction, ray->direction);
	float b = 2 * vec3_dot(ray->direction, L); 
	float c = vec3_dot(L, L) - radius2; 
	if (!quadratic_solve(a, b, c, &t0, &t1))
		return 0; 
#endif

	if (t0 > t1)
		swapf(&t0, &t1); 

	if (t0 < 0) { 
		t0 = t1;
		if (t0 < 0)
			return 0; /* both t0 and t1 are negative */
	} 

	hit->distance = t0;
	hit->position = vec3_add(ray->origin, vec3_mulf(ray->direction, t0));
	hit->normal = vec3_normalize(vec3_sub(hit->position, center));

	return 1;
}

struct object *sphere_alloc(float radius)
{
	struct object *object;
	struct sphere *sphere;

	object = object_alloc(sizeof(*sphere));
	if (!object)
		return NULL;

	object->intersect = sphere_intersect;

	sphere = (struct sphere *) object;
	sphere->radius = radius;

	return object;
}
