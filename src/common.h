#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "../include/raylib.h"

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef bool b32;

typedef float f32;
typedef double f64;

double max(double a, double b);
void int_to_string(int num, char* str);

#define print_vector2(vec) \
	printf("%s = (%f, %f)\n", #vec, vec.x, vec.y);

// Remember that inside this macro `current` is a local copy and changes to it dont actually change the state
// TODO: Experiment with __COUNT__ macro for incremental iterator name, something like _i_ would work.

#define For(item) \
	for (size_t ___ = 0; ___ < (item).size; ___++)	\
		if (auto current = (item).node[___]; true)
