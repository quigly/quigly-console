#pragma once

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef u8 b8;

typedef u32 size_t;

#define true 1
#define false 0

#define NULL (void*)0

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

#define BUTTON_LEFT 0x0
#define BUTTON_RIGHT 0x1
#define BUTTON_UP 0x2
#define BUTTON_DOWN 0x3
#define BUTTON_A 0x4
#define BUTTON_B 0x5

#define SPR_FLIP_X (1 << 0)
#define SPR_FLIP_Y (1 << 1)

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 120

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
#define clamp(value,low,high) ((value)<(low)?(low):((value)>(high)?(high):(value)))

#define RGB(r, g, b)\
	(\
		((((u16)(r)) & 0x1F)) |\
		((((u16)(g)) & 0x1F) << 5) |\
		((((u16)(b)) & 0x1F) << 10)\
	)

#define assert(x) if (!(x)) { putsf("Assert failed at %s:i\n", __FILE__, __LINE__); shutdown(); }

extern void camera(i32 x, i32 y);
extern u8 pget(i32 x, i32 y);
extern void pset(i32 x, i32 y, u8 color);
extern void cls(u8 color);
extern void clip(i32 x, i32 y, i32 w, i32 h);
extern void rect(i32 x, i32 y, i32 w, i32 h, u8 color);
extern void rectfill(i32 x, i32 y, i32 w, i32 h, u8 color);
extern void line(i32 x0, i32 y0, i32 x1, i32 y1, u8 color);
extern void pal(u16 c0, u16 c1, u16 c2, u16 c3);
extern void spr_data(const void* sprites, u32 num_sprites);
extern void spr(i32 n, i32 x, i32 y, u32 bits);
extern void tile(i32 n, i32 x, i32 y, u32 bits);
extern b8 btn(u8 button, u8 player);
extern b8 btnp(u8 button, u8 player);
extern void putc(char c);
extern void shutdown();

extern u32 __heap_start;
extern u32 __stack_top;

static inline void* memcpy(void* dst, const void* src, size_t count)
{
	u8* dstb = (u8*)dst;
	u8* srcb = (u8*)src;

	while (count > 0)
	{
		*dstb = *srcb;
		dstb += 1;
		srcb += 1;
		count -= 1;
	}

	return dst;
}
