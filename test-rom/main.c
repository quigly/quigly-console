#include "language_layer.h"

extern void camera(i32 x, i32 y);
extern u8 pget(i32 x, i32 y);
extern void pset(i32 x, i32 y, u8 color);
extern void pal(u8 c0, u8 c1);
extern void palt(u8 color, u8 transparent);
extern void cls(u8 color);
extern void clip(i32 x, i32 y, i32 w, i32 h);
extern void rect(i32 x, i32 y, i32 w, i32 h, u8 color);
extern void rectfill(i32 x, i32 y, i32 w, i32 h, u8 color);
extern void line(i32 x0, i32 y0, i32 x1, i32 y1, u8 color);
extern void spr(i32 n, i32 x, i32 y, u8 flip_x, u8 flip_y);
extern void print(const char* text, i32 x, i32 y, u8 color);

void _init()
{

}

void _update()
{

}

void _draw()
{
	pset(1, 1, 8);
	pset(2, 2, 9);
	pset(3, 3, 10);

	print("Hello world risc-v :D", 10, 10, 8);
}

