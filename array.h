#ifndef ARRAY_H
#define ARRAY_H

#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define DECLARE_C_ARRAY(_name, _type) \
typedef _type _name##_element_t; \
typedef struct _name##_t { \
	size_t count; \
	size_t size; \
	_name##_element_t *data; \
} _name##_t; \
inline void _name##_push(_name##_t *array, _name##_element_t *data); \
inline void _name##_push_front(_name##_t *array, _name##_element_t *data); \
inline void _name##_remove(_name##_t *array, _name##_element_t *data); \
inline void _name##_cleanup(_name##_t *array); \

#define IMPLEMENT_C_ARRAY(_name) \
inline int _name##_grow(_name##_t  *array) \
{ \
	void *ptr; \
	size_t new_size; \
\
	if (array->size != 0 && array->count < array->size) \
		return 0; \
\
	new_size = array->size == 0 ? 8 : array->size * 2; \
	ptr = realloc(array->data, sizeof(_name##_element_t) * new_size); \
	if (!ptr) \
		return -1; \
 \
	array->data = ptr; \
	array->size = new_size; \
\
	return 0; \
} \
\
inline void _name##_push(_name##_t *array, _name##_element_t *data) \
{ \
	if (_name##_grow(array) < 0) \
		assert(0); \
\
	memcpy(&array->data[array->count++], data, sizeof(_name##_element_t)); \
} \
\
inline void _name##_push_front(_name##_t *array, _name##_element_t *data) \
{ \
	if (_name##_grow(array) < 0) \
		assert(0); \
\
	if (array->count == 0) { \
		_name##_push(array, data); \
		return; \
	} \
\
	memmove(&array->data[1], &array->data[0], array->count * sizeof(_name##_element_t)); \
	memcpy(&array->data[0], data, sizeof(_name##_element_t)); \
	array->count++; \
} \
\
inline void _name##_remove(_name##_t *array, _name##_element_t *data) \
{ \
	int i; \
	for (i = 0; i < array->count; i++) { \
		if (!memcmp(&array->data[i], data, sizeof(_name##_element_t))) \
			break; \
	} \
	if (i == array->count) \
		return; \
	memmove(&array->data[i], &array->data[i + 1], (array->count - i) * sizeof(_name##_element_t)); \
	array->count--; \
} \
\
inline void _name##_cleanup(_name##_t *array) \
{ \
	if (array->data) \
		free(array->data); \
	array->count = array->size = 0; \
} \

#endif /* ARRAY_H */
