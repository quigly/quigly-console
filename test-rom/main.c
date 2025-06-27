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
extern bool btn(u8 button, u8 player);
extern bool btnp(u8 button, u8 player);

typedef enum
{
	BUTTON_DPAD_LEFT,
	BUTTON_DPAD_RIGHT,
	BUTTON_DPAD_UP,
	BUTTON_DPAD_DOWN,
	BUTTON_A,
	BUTTON_B
} button_e;

void _init()
{

}

void _update()
{

}

void _draw()
{
	cls(1);

	static i32 x = 0;
	static i32 y = 0;

	print("Quigly", x, y, 7);

	if (btn(BUTTON_DPAD_UP, 0)) { y -= 1; }
	if (btn(BUTTON_DPAD_DOWN, 0)) { y += 1; }
	if (btn(BUTTON_DPAD_LEFT, 0)) { x -= 1; }
	if (btn(BUTTON_DPAD_RIGHT, 0)) { x += 1; }

	static bool open = false;

	if (btnp(BUTTON_A, 0)) { open = !open; }

	if (open)
	{
		print("Open!", 1, 1, 8);
	}

	palt(0, true);
	spr(0, 50, 50, false, false);
	palt(0, false);
}

