#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

#define true 1
#define false 0

#ifndef NULL
#define NULL (void*)0
#endif

#define I8_MIN  (-0x7f - 1)
#define I16_MIN (-0x7fff - 1)
#define I32_MIN (-0x7fffffff - 1)
#define I64_MIN (-0x7fffffffffffffffLL - 1)

#define I8_MAX  0x7f
#define I16_MAX 0x7fff
#define I32_MAX 0x7fffffff
#define I64_MAX 0x7fffffffffffffffLL

#define U8_MIN  0
#define U16_MIN 0
#define U32_MIN 0
#define U64_MIN 0

#define U8_MAX  0xFF
#define U16_MAX 0xFFFF
#define U32_MAX 0xFFFFFFFF
#define U64_MAX 0xFFFFFFFFFFFFFFFFLL

#define BIT(n) (1 << n)
#define KILOBYTES(n) (1024LL * (n))
#define MEGABYTES(n) (1024LL * KILOBYTES(n))
#define GIGABYTES(n) (1024LL * MEGABYTES(n))
#define TERABYTES(n) (1024LL * GIGABYTES(n))

#define array_count(a) (sizeof(a) / sizeof((a)[0]))

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
#define clamp(value,low,high) ((value)<(low)?(low):((value)>(high)?(high):(value)))

#if 0
#define unreachable\
	printf("Reached unreachable code at %s:%u\n", __FILE__, __LINE__); exit(1)

#define assert(expr) if (!(expr)) { printf("Assert failed at %s:%i\n", __FILE__, __LINE__); exit(1); }
#endif

static inline void mem_swap_byte(u8* a, u8* b)
{
	const u8 tmp = *a;
	*a = *b;
	*b = tmp;
}

static inline void mem_swap(void* a, void* b, u32 size)
{
	u8* x = (u8*)a;
	u8* y = (u8*)b;

	while (size > 0)
	{
		mem_swap_byte(x, y);
		x += 1;
		y += 1;
		size -= 1;
	}
}
