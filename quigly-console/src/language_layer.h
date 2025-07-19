#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include <string.h>

#define PLATFORM_WINDOWS 0
#define PLATFORM_LINUX 1
#define PLATFORM_MACOS 2
#define PLATFORM_IOS 3
#define PLATFORM_ANDROID 4

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
	#define PLATFORM PLATFORM_WINDOWS
#elif defined(__linux__) || defined(__gnu_linux__)
	#define PLATFORM PLATFORM_LINUX
#endif

#if defined(_WIN32) || defined(_WIN64)
	#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
	#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

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

typedef u8 bool;

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

#define F32_MAX __FLT_MAX__

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

#define unreachable\
	printf("Reached unreachable code at %s:%u\n", __FILE__, __LINE__); exit(1)

#ifdef DEBUG
#define assert(expr) if (!(expr)) { printf("Assert failed at %s:%i\n", __FILE__, __LINE__); exit(1); }
#else
#define assert(expr)
#endif

static inline void mem_swap_byte(u8* a, u8* b)
{
	const u8 tmp = *a;
	*a = *b;
	*b = tmp;
}

static inline void mem_swap(void* a, void* b, size_t size)
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

static inline u32 f32_to_bits(f32 f)
{
	u32 n;
	memcpy(&n, &f, sizeof(u32));
	return n;
}

static inline f32 f32_from_bits(u32 bits)
{
	f32 f;
	memcpy(&f, &bits, sizeof(f32));
	return f;
}
