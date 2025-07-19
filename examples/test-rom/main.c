#include "qc/qc.h"
#include <stdarg.h>

#define BALL_W 8
#define BALL_H 8
#define DISPLAY_W 160
#define DISPLAY_H 120

typedef struct
{
	i32 pos_x;
	i32 pos_y;
	i32 prev_pos_x;
	i32 prev_pos_y;
	i32 vel_x;
	i32 vel_y;
} ball_t;

typedef struct
{
	i32 pos_x;
	i32 pos_y;
	i32 size_x;
	i32 size_y;
} paddle_t;

typedef struct
{
	i32 x0;
	i32 y0;
	i32 x1;
	i32 y1;
} rect_t;

static paddle_t paddles[2];
static ball_t balls[1];
static u32 seed;
const static u8 sprites[] =
{
	#embed "sprites.bin"
};

static void puts(const char* str)
{
	for (char* c = (char*)str; *c != 0; c += 1)
	{
		putc(*c);
	}
}

static inline void putu(u32 value)
{
	char buffer[10];
	i32 i = 0;

	if (value == 0)
	{
		putc('0');
		return;
	}

	while (value != 0 && i < 10)
	{
		buffer[i] = '0' + (value % 10);
		i += 1;
		value /= 10;
	}

	for (i32 j = i - 1; j >= 0; j -= 1)
	{
		putc(buffer[j]);
	}
}

static inline void puti(i32 value)
{
	if (value < 0)
	{
		putc('-');
		value = -value;
	}

	putu((u32)value);
}

static inline void putsf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	while (*fmt)
	{
		if (*fmt == '%')
		{
			fmt += 1;

			switch (*fmt)
			{
				case 'i':
				{
					puti(va_arg(args, i32));
				} break;

				case 'u':
				{
					putu(va_arg(args, u32));
				} break;

				case 'c':
				{
					putc(va_arg(args, i32));
				} break;

				case 's':
				{
					puts(va_arg(args, const char*));
				} break;

				case '%':
				{
					putc('%');
				} break;

				default:
				{
					putc('%');
					putc(*fmt);
				} break;
			}
		}
		else
		{
			putc(*fmt);
		}
		fmt += 1;
	}

	va_end(args);
}

static u32 rand()
{
	seed = seed * 4398278 + 2348916;
	return seed;
}

static b8 rect_intersects(rect_t a, rect_t b)
{
	if (a.x1 < b.x0) { return false; }
	if (a.y1 < b.y0) { return false; }
	if (a.x0 > b.x1) { return false; }
	if (a.y0 > b.y1) { return false; }

	return true;
}

static b8 check_collision(const ball_t* ball, const paddle_t* paddle)
{
	const rect_t paddle_rect = (rect_t)
	{
		ball->pos_x - (BALL_W / 2),
		ball->pos_y - (BALL_H / 2),
		ball->pos_x + (BALL_W / 2),
		ball->pos_y + (BALL_H / 2)
	};

	const rect_t ball_rect = (rect_t)
	{
		paddle->pos_x - (paddle->size_x / 2),
		paddle->pos_y - (paddle->size_y / 2),
		paddle->pos_x + (paddle->size_x / 2),
		paddle->pos_y + (paddle->size_y / 2)
	};

	return rect_intersects(paddle_rect, ball_rect);
}

void _init()
{
	balls[0].pos_x = 64;
	balls[0].pos_y = 40;
	balls[0].vel_x = -1;
	balls[0].vel_y = 1;

	paddles[0].pos_x = (DISPLAY_W / 2);
	paddles[0].pos_y = DISPLAY_H - 10;
	paddles[0].size_x = 32;
	paddles[0].size_y = 6;
	
	paddles[1].pos_x = (DISPLAY_W / 2);
	paddles[1].pos_y = 10;
	paddles[1].size_x = 32;
	paddles[1].size_y = 6;

	putsf("_heap_start: %u\n", &__heap_start);
	putsf("__stack_top: %u\n", &__stack_top);
}

void _update()
{
	balls[0].prev_pos_x = balls[0].pos_x;
	balls[0].prev_pos_y = balls[0].pos_y;

	balls[0].pos_x += balls[0].vel_x;
	balls[0].pos_y += balls[0].vel_y;

	paddles[1].pos_x = clamp(balls[0].pos_x, paddles[1].size_x / 2, DISPLAY_W - (paddles[1].size_x / 2));

	if (btn(BUTTON_LEFT, 0))
	{
		paddles[0].pos_x -= 1;
		if (paddles[0].pos_x - (paddles[0].size_x / 2) < 0)
		{
			paddles[0].pos_x = (paddles[0].size_x / 2);
		}
	}
	
	if (btn(BUTTON_RIGHT, 0))
	{
		paddles[0].pos_x += 1;
		if (paddles[0].pos_x + (paddles[0].size_x / 2) >= DISPLAY_W)
		{
			paddles[0].pos_x = DISPLAY_W - (paddles[0].size_x / 2);
		}
	}

	if (balls[0].pos_x + (BALL_W / 2) >= DISPLAY_W || balls[0].pos_x - (BALL_W / 2) < 0)
	{
		balls[0].pos_x = clamp(balls[0].pos_x, BALL_W / 2, DISPLAY_W - (BALL_W / 2));
		balls[0].vel_x *= -1;
	}

	if (balls[0].pos_y - (BALL_H / 2) < 0)
	{
		balls[0].pos_x = 64 + ((rand() % 64) - 32);
		balls[0].pos_y = 30;
		balls[0].vel_y = 1;
	}
	
	if (balls[0].pos_y - (BALL_H / 2) >= DISPLAY_H)
	{
		balls[0].pos_x = 64 + ((rand() % 80) - 40);
		balls[0].pos_y = 30;
		balls[0].vel_y = 1;
	}

	if (check_collision(&balls[0], &paddles[0])) // player
	{
		balls[0].pos_x = balls[0].prev_pos_x;
		balls[0].pos_y = balls[0].prev_pos_y;
		balls[0].vel_y = -balls[0].vel_y;
		// puts("bounce\n");
	}
	
	if (check_collision(&balls[0], &paddles[1])) // enemy
	{
		balls[0].pos_x = balls[0].prev_pos_x;
		balls[0].pos_y = balls[0].prev_pos_y;
		balls[0].vel_y *= -1;
	}
}

static void draw_paddle(u32 i)
{
	spr_data(sprites, 256);
	pal(RGB(0, 0, 0), RGB(31, 0, 0), RGB(0, 31, 0), RGB(0, 0, 31));
	spr(1, paddles[i].pos_x - (paddles[i].size_x / 2), paddles[i].pos_y - (paddles[i].size_y / 2), 0);
	spr(2, paddles[i].pos_x - (paddles[i].size_x / 2) + 8, paddles[i].pos_y - (paddles[i].size_y / 2), 0);
	spr(3, paddles[i].pos_x - (paddles[i].size_x / 2) + 16, paddles[i].pos_y - (paddles[i].size_y / 2), 0);
	spr(4, paddles[i].pos_x - (paddles[i].size_x / 2) + 24, paddles[i].pos_y - (paddles[i].size_y / 2), 0);
}

void _draw()
{
	cls(0);

	/*static i32 x = 0;
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
	palt(0, false);*/

	{
		// u8 colors[4] = { 0, 1, 2, 3 };
		//rectfill(paddles[0].pos_x - (paddles[0].size_x / 2), paddles[0].pos_y - (paddles[0].size_y / 2), paddles[0].size_x, paddles[0].size_y, 7);
		//rectfill(paddles[1].pos_x - (paddles[1].size_x / 2), paddles[1].pos_y - (paddles[1].size_y / 2), paddles[1].size_x, paddles[1].size_y, 7);

		draw_paddle(0);
		draw_paddle(1);
	}

	// rectfill(balls[0].pos_x - (BALL_W / 2), balls[0].pos_y - (BALL_H / 2), BALL_W, BALL_H, 7);
	{
		spr_data(sprites, 256);
		pal(RGB(0, 0, 0), RGB(31, 0, 0), RGB(0, 31, 0), RGB(0, 0, 31));
		spr(0, balls[0].pos_x - (BALL_W / 2), balls[0].pos_y - (BALL_H / 2), 0);
	}
}

