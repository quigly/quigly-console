#pragma once

#include "language_layer.h"
#include <SDL3/SDL.h>

#define OPCODE_MASK 0x7F
#define RS_MASK 0x1F
#define FUNCT3_MASK 0x7
#define FUNCT7_MASK 0x7F
#define SHAMT_MASK 0x1F

#define BUS_MAX_DEVICES 2

#define PPU_SCREEN_WIDTH 128
#define PPU_SCREEN_HEIGHT 128
#define PPU_SCREEN_TOTAL_PIXELS (PPU_SCREEN_WIDTH * PPU_SCREEN_HEIGHT)
#define PPU_SPRITE_SHEET_WIDTH 256
#define PPU_SPRITE_SHEET_SIZE (PPU_SPRITE_SHEET_WIDTH * PPU_SPRITE_SHEET_WIDTH)

typedef struct
{
	u32 begin;
	u32 end;
} memory_region_t;

typedef enum
{
	BUS_RESULT_SUCCESS,
	BUS_RESULT_INVALID_ADDRESS
} bus_result_e;

typedef struct
{
	char label[128];
	u8* memory;
	u32 size;
	memory_region_t region;
} bus_device_t;

typedef union
{
	struct
	{
		bus_device_t dram;
		bus_device_t sram;
	};
	bus_device_t devices[BUS_MAX_DEVICES];
} bus_t;

typedef enum
{
    INSTRUCTION_TYPE_UNKNOWN,
    INSTRUCTION_TYPE_R,
    INSTRUCTION_TYPE_I,
    INSTRUCTION_TYPE_S,
    INSTRUCTION_TYPE_B,
    INSTRUCTION_TYPE_U,
    INSTRUCTION_TYPE_J
} instruction_type_e;

typedef enum
{
	#define INSTRUCTION(id_lower, id_upper) INSTRUCTION_##id_upper,
	#include "instructions.inc"
	#undef INSTRUCTION
} instruction_id_e;

typedef struct
{
	u32 x[32];
	u32 x_last[32];
	u32 csrs[4096];
	u32 pc;
	u32 inst;
	instruction_type_e inst_type;
	instruction_id_e inst_id;
	u32 opcode;
	u32 rd;
	u32 rs1;
	u32 rs2;
	u32 rs3;
	u32 funct3;
	u32 funct7;
	i32 imm;
	u32 shamt;
	u32 shiftop;
	u32 csr;
} cpu_t;

typedef struct
{
	u32* pixels;
	u32 palette[16];
	u8 palette_pixels[PPU_SCREEN_TOTAL_PIXELS];
	bool palette_transparent[16];
	u8 sprite_pixels[PPU_SPRITE_SHEET_SIZE];
	u8 font_pixels[PPU_SPRITE_SHEET_SIZE];
	SDL_Rect clip_rect;
	i32 camera_x;
	i32 camera_y;
} ppu_t;

typedef struct
{
	u32* pixels;
	SDL_Window* window;
	i32 window_width;
	i32 window_height;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
} video_t;

typedef struct
{
	cpu_t cpu;
	bus_t bus;
	ppu_t ppu;
	video_t video;

	bool running;
	bool paused;
	bool finished_frame;
	bool init_finished;

	struct
	{
		u64 last_time;
		u32 fps;
		u64 frame_timer;
		f64 delta_time;
		f64 time_step;
		u64 now;
		u64 ticks;
	} time;
} vm_t;

void vm_execute(vm_t* vm);

void cpu_decode(cpu_t* cpu, u32 inst);

void* bus_get_pointer(bus_t* bus, bus_device_t** pdevice, u32 address, u32 size);
u8 bus_read_8(bus_t* bus, cpu_t* cpu, u32 address);
u16 bus_read_16(bus_t* bus, cpu_t* cpu, u32 address);
u32 bus_read_32(bus_t* bus, cpu_t* cpu, u32 address);
void bus_write_8(bus_t* bus, cpu_t* cpu, u32 address, u8 value);
void bus_write_16(bus_t* bus, cpu_t* cpu, u32 address, u16 value);
void bus_write_32(bus_t* bus, cpu_t* cpu, u32 address, u32 value);

void ppu_import(ppu_t* ppu, const char* path, u8* dst, i32 dst_width, i32 dst_height);
void ppu_init(ppu_t* ppu);
void ppu_camera(ppu_t* ppu, i32 x, i32 y); // ecall 100
u8 ppu_pget(ppu_t* ppu, i32 x, i32 y);
void ppu_pset(ppu_t* ppu, i32 x, i32 y, u8 color);
void ppu_pal(ppu_t* ppu, u8 c0, u8 c1);
void ppu_palt(ppu_t* ppu, u8 color, bool transparent);
void ppu_cls(ppu_t* ppu, u8 color);
void ppu_clip(ppu_t* ppu, i32 x, i32 y, i32 w, i32 h);
void ppu_rect(ppu_t* ppu, i32 x, i32 y, i32 w, i32 h, u8 color);
void ppu_rectfill(ppu_t* ppu, i32 x, i32 y, i32 w, i32 h, u8 color);
void ppu_line(ppu_t* ppu, i32 x0, i32 y0, i32 x1, i32 y1, u8 color);
void ppu_spr(ppu_t* ppu, i32 n, i32 x, i32 y, bool flip_x, bool flip_y);
void ppu_print(ppu_t* ppu, const char* text, i32 x, i32 y, u8 color);

/*
csr 0x7C0 frame ready (ready for guest to begin frame)
	0x7C1 render ready (guest finished frame)
	0x7C2 frame start address
*/

#if 0
Disassembly of section .text:

00100000 <_start>:
  100000: 054000ef     	jal	0x100054 <_init>
  100004: 00000297     	auipc	t0, 0x0
  100008: 01828293     	addi	t0, t0, 0x18
  10000c: 7c229073     	csrw	0x7c2, t0
  100010: 00c000ef     	jal	0x10001c <_gameloop>
  100014: 06200893     	li	a7, 0x62
  100018: 00000073     	ecall

0010001c <_gameloop>:
  10001c: 03c000ef     	jal	0x100058 <_update>
  100020: 03c000ef     	jal	0x10005c <_draw>
  100024: 06300893     	li	a7, 0x63
  100028: 00000073     	ecall
  10002c: 00008067     	ret

00100030 <camera>:
  100030: 06400893     	li	a7, 0x64
  100034: 00000073     	ecall
  100038: 00008067     	ret

0010003c <pget>:
  10003c: 06500893     	li	a7, 0x65
  100040: 00000073     	ecall
  100044: 00008067     	ret

00100048 <pset>:
  100048: 06600893     	li	a7, 0x66
  10004c: 00000073     	ecall
  100050: 00008067     	ret

00100054 <_init>:
  100054: 00008067     	ret

00100058 <_update>:
  100058: 00008067     	ret

0010005c <_draw>:
  10005c: 00008067     	ret
#endif
