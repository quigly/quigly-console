#include "language_layer.h"

extern void camera(i32 x, i32 y);
extern u8 pget(i32 x, i32 y);
extern void pset(i32 x, i32 y, u8 color);

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
}

