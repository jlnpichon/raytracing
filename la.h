#ifndef LA_H
#define LA_H

/* linear algebra */

#include <math.h>
#include <stdio.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#ifndef M_PI
#define M_PI 3.141592654
#endif
#define PI_OVER_180	 0.017453292519943295769236907684886
#define PI_OVER_360	 0.0087266462599716478846184538424431

enum { VX, VY, VZ, VW }; /* axes */

typedef struct vec2 {
	union {
		struct { float x; float y; };
		struct { float width; float height; };
		struct { float left; float right; };
		struct { float min; float max; };
		struct { float i; float j; };
		float n[2];
	};
} vec2;

typedef struct vec2i {
	union {
		struct { int x; int y; };
		struct { int i; int j; };
		struct { int width; int height; };
		int n[2];
	};
} vec2i;

static inline vec2 vec2f(float x, float y)
{
	vec2 vec2;
	vec2.x = x;
	vec2.y = y;
	return vec2;
}

static inline vec2 vec2_add(vec2 vec1, vec2 vec2)
{
	return vec2f(vec1.x + vec2.x, vec1.y + vec2.y);
}

static inline vec2 vec2_sub(vec2 vec1, vec2 vec2)
{
	return vec2f(vec1.x - vec2.x, vec1.y - vec2.y);
}

static inline vec2 vec2_mulf(vec2 vec, float m)
{
	return vec2f(vec.x * m, vec.y * m);
}

typedef struct vec3 {
	union {
		struct { float x, y, z; };
		struct { float r, g, b; };
		float n[3];
		vec2 v2;
	};
} vec3;

static inline vec3 vec3f(float x, float y, float z)
{
	vec3 v = { .x = x, .y = y, .z = z };
	return v;
}

static inline vec3 vec3_add(vec3 a, const vec3 v)
{
	a.n[VX] += v.n[VX];
	a.n[VY] += v.n[VY];
	a.n[VZ] += v.n[VZ];
	return a;
}

static inline vec3 vec3_sub(vec3 a, const vec3 v)
{
	a.n[VX] -= v.n[VX];
	a.n[VY] -= v.n[VY];
	a.n[VZ] -= v.n[VZ];
	return a;
}

static inline vec3 vec3_divf(vec3 a, float d)
{
	float d_inv = 1.0f / d;
	a.n[VX] *= d_inv;
	a.n[VY] *= d_inv;
	a.n[VZ] *= d_inv;
	return a;
}

static inline float vec3_length2(const vec3 a)
{
	return a.n[VX]*a.n[VX] + a.n[VY]*a.n[VY] + a.n[VZ]*a.n[VZ];
}

static inline float vec3_length(const vec3 a)
{
	return (float) sqrt(vec3_length2(a));
}

static inline vec3 vec3_normalize(const vec3 a)
{
	return vec3_divf(a, vec3_length(a));
}

static inline vec3 vec3_mulf(vec3 a, float d)
{
	a.n[VX] *= d;
	a.n[VY] *= d;
	a.n[VZ] *= d;
	return a;
}

static inline vec3 vec3_div(vec3 a, float d)
{
	float d_inv = 1.f / d;
	a.n[VX] *= d_inv;
	a.n[VY] *= d_inv;
	a.n[VZ] *= d_inv;
	return a;
}

static inline float vec3_dot(const vec3 a, const vec3 b)
{
	return a.n[VX]*b.n[VX] + a.n[VY]*b.n[VY] + a.n[VZ]*b.n[VZ];
}

static inline vec3 vec3_cross(const vec3 a, const vec3 b)
{
	return vec3f(a.n[VY]*b.n[VZ] - a.n[VZ]*b.n[VY],
							 a.n[VZ]*b.n[VX] - a.n[VX]*b.n[VZ],
							 a.n[VX]*b.n[VY] - a.n[VY]*b.n[VX]);
}

static inline vec3 vec3_clamp(vec3 v, float minimum, float maximum)
{
	vec3 r;
	r.x =  min(max(minimum, v.x), maximum);
	r.y =  min(max(minimum, v.y), maximum);
	r.z =  min(max(minimum, v.z), maximum);
	return r;
}

typedef struct vec4 {
	union {
		struct { float x; float y; float z; float w; };
		struct { float left; float right; float width; float height; };
		struct { float _left; float _right; float top; float bottom; };
		float n[4];
		vec3 v3;
	};
} vec4;

static inline vec4 vec4f(float x, float y, float z, float w)
{
	vec4 vec4;
	vec4.x = x;
	vec4.y = y;
	vec4.z = z;
	vec4.w = w;
	return vec4;
}

static inline vec4 vec4_vec3(const vec3 v, int point)
{
	return vec4f(v.n[VX], v.n[VY], v.n[VZ], point ? 1.f : 0.f);
}

static inline vec4 vec4_sub(vec4 a, const vec4 v)
{
	a.n[VX] -= v.n[VX];
	a.n[VY] -= v.n[VY];
	a.n[VZ] -= v.n[VZ];
	a.n[VW] -= v.n[VW];
	return a;
}

static inline vec4 vec4_divf(vec4 a, float d)
{
	float d_inv = 1.0f / d;
	a.n[VX] *= d_inv;
	a.n[VY] *= d_inv;
	a.n[VZ] *= d_inv;
	a.n[VW] *= d_inv;
	return a;
}

static inline vec4 vec4_mulf(vec4 a, float d)
{
	a.n[VX] *= d;
	a.n[VY] *= d;
	a.n[VZ] *= d;
	a.n[VW] *= d;
	return a;
}

typedef struct mat3 {
	union {
		vec3 v[3];
		float n[3 * 3];
	};
} mat3;

static inline mat3 mat3_vec3(const vec3 v0, const vec3 v1, const vec3 v2)
{
	mat3 m = { .v[0] = v0, .v[1] = v1, .v[2] = v2 };
	return m;
}

typedef struct mat4 {
	union {
		vec4 v[4];
		float n[4*4];
	};
} mat4;

static inline void mat4_dump(FILE *stream, mat4 *mat)
{
	int i;

	fprintf(stream, "{\n");
	for (i = 0; i < 16; i++) {
		if (i > 0 && !(i % 4))
			fprintf(stream, "\n");
		fprintf(stream, "%s%.02f", mat->n[i] >= 0 ? " " : "", mat->n[i]);
		if (i < 15)
			fprintf(stream, ", ");
	}
	fprintf(stream, "\n}\n");
}

static inline mat4 mat4_vec4(const vec4 v0, const vec4 v1, const vec4 v2, const vec4 v3)
{
	mat4 m = { .v[0] = v0, .v[1] = v1, .v[2] = v2, .v[3] = v3 };
	return m;
}

static inline mat4 identity3D(void)
{
	return mat4_vec4(
			vec4f(1.0f, 0.0f, 0.0f, 0.0f),
			vec4f(0.0f, 1.0f, 0.0f, 0.0f),
			vec4f(0.0f, 0.0f, 1.0f, 0.0f),
			vec4f(0.0f, 0.0f, 0.0f, 1.0f));
}

static inline mat4 mat4_identity(void)
{
	return identity3D();
}

static inline mat4 mat4_mul(const mat4 *a, const mat4 *b)
{
#define ROWCOL(i, j) \
	a->v[i].n[0]*b->v[0].n[j] + \
	a->v[i].n[1]*b->v[1].n[j] + \
	a->v[i].n[2]*b->v[2].n[j] + \
	a->v[i].n[3]*b->v[3].n[j]

	return mat4_vec4(
			vec4f(ROWCOL(0,0), ROWCOL(0,1), ROWCOL(0,2), ROWCOL(0,3)),
			vec4f(ROWCOL(1,0), ROWCOL(1,1), ROWCOL(1,2), ROWCOL(1,3)),
			vec4f(ROWCOL(2,0), ROWCOL(2,1), ROWCOL(2,2), ROWCOL(2,3)),
			vec4f(ROWCOL(3,0), ROWCOL(3,1), ROWCOL(3,2), ROWCOL(3,3))
			);

#undef ROWCOL
}

static inline vec4 mat4_mulv4(const mat4 *a, const vec4 v)
{
#define ROWCOL(i) \
	a->v[0].n[i] * v.n[VX] + \
	a->v[1].n[i] * v.n[VY] + \
	a->v[2].n[i] * v.n[VZ] + \
	a->v[3].n[i] * v.n[VW]

	return vec4f(ROWCOL(0), ROWCOL(1), ROWCOL(2), ROWCOL(3));

#undef ROWCOL
}

static inline vec3 mat4_mulv3(const mat4 *a, const vec3 v, int point)
{
	vec4 v4 = mat4_mulv4(a, vec4_vec3(v, point));
	return v4.v3;
}

static inline mat4 mat4_view(vec3 eye, vec3 lookat, vec3 up)
{
	vec3 zaxis = vec3_normalize(vec3_sub(lookat, eye));
	vec3 xaxis = vec3_normalize(vec3_cross(up, zaxis));
	vec3 yaxis = vec3_cross(zaxis, xaxis);

	mat4 view;
	view.n[0] = xaxis.x;
	view.n[1] = yaxis.x;
	view.n[2] = -zaxis.x;
	view.n[3] = 0.f;

	view.n[4] = xaxis.y;
	view.n[5] = yaxis.y;
	view.n[6] = -zaxis.y;
	view.n[7] = 0.f;

	view.n[8] = xaxis.z;
	view.n[9] = yaxis.z;
	view.n[10] = -zaxis.z;
	view.n[11] = 0.f;

	view.n[12] = -vec3_dot(xaxis, eye);
	view.n[13] = -vec3_dot(yaxis, eye);
	view.n[14] = vec3_dot(zaxis, eye);
	view.n[15] = 1.f;

	return view;
}

static inline mat4 mat4_scaling(const vec3 scale)
{
    return mat4_vec4(
				vec4f(scale.n[VX], 0.f,         0.f,         0.f),
				vec4f(0.f,         scale.n[VY], 0.f,         0.f),
				vec4f(0.f,         0.f,         scale.n[VZ], 0.f),
				vec4f(0.f,         0.f,         0.f,         1.f));
}

static inline mat4 mat4_translation(const vec3 v)
{
    return mat4_vec4(
				vec4f(1.f, 0.f, 0.f, 0.f),
				vec4f(0.f, 1.f, 0.f, 0.f),
        vec4f(0.f, 0.f, 1.f, 0.f),
        vec4f(v.n[VX], v.n[VY], v.n[VZ], 1.f));
}

static inline mat4 mat4_transpose(const mat4 *a)
{
    return mat4_vec4(
        vec4f(a->v[0].n[0], a->v[1].n[0], a->v[2].n[0], a->v[3].n[0]),
        vec4f(a->v[0].n[1], a->v[1].n[1], a->v[2].n[1], a->v[3].n[1]),
        vec4f(a->v[0].n[2], a->v[1].n[2], a->v[2].n[2], a->v[3].n[2]),
        vec4f(a->v[0].n[3], a->v[1].n[3], a->v[2].n[3], a->v[3].n[3])
				);
}

static inline mat4 mat4_inverse(const mat4 *m)
{
	int i, j, i1;
	mat4 a = *m;
	mat4 b = identity3D();

	for (j = 0; j < 4; j++) {
		i1 = j;
		for (i = j + 1; i < 4; i++)
			if (fabs(a.v[i].n[j]) > fabs(a.v[i1].n[j]))
				i1 = i;

		vec4 _s;
		_s = a.v[i1]; a.v[i1] = a.v[j]; a.v[j] = _s;
		_s = b.v[i1]; b.v[i1] = b.v[j]; b.v[j] = _s;

		if (a.v[j].n[j] == 0.)
			return a;
		b.v[j] = vec4_divf(b.v[j], a.v[j].n[j]);
		a.v[j] = vec4_divf(a.v[j], a.v[j].n[j]);

		for (i = 0; i < 4; i++)
			if (i != j) {
				b.v[i] = vec4_sub(b.v[i], vec4_mulf(b.v[j], a.v[i].n[j]));
				a.v[i] = vec4_sub(a.v[i], vec4_mulf(a.v[j], a.v[i].n[j]));
			}
	}

	return b;
}

static inline mat4 mat4_perspective_projection(float fov, float aspect,
		float znear, float zfar)
{
	float r = (fov / 2) * PI_OVER_180;
	float deltaz = zfar - znear;
	float s = sin(r);
	float cotangent;
	mat4 projection = mat4_identity();

	if (deltaz == 0 || s == 0 || aspect == 0)
		return mat4_identity();

	cotangent = cos(r) / s;

	projection.n[0] = cotangent / aspect;
	projection.n[5] = cotangent;
	projection.n[10] = -(zfar + znear) / deltaz;
	projection.n[11] = -1;
	projection.n[14] = -2 * znear * zfar / deltaz;
	projection.n[15] = 0;

	return projection;
}

#endif /* LA_H */
