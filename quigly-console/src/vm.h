#pragma once

#include "language_layer.h"
#include <SDL3/SDL.h>

#define OPCODE_MASK 0x7F
#define RS_MASK 0x1F
#define FUNCT3_MASK 0x7
#define FUNCT7_MASK 0x7F
#define SHAMT_MASK 0x1F

#define BUS_MAX_DEVICES 3

#define ROM_MAX_SIZE MEGABYTES(32)

#define PPU_SCREEN_WIDTH 160
#define PPU_SCREEN_HEIGHT 120
#define PPU_SCREEN_TOTAL_PIXELS (PPU_SCREEN_WIDTH * PPU_SCREEN_HEIGHT)
#define PPU_SPRITE_SHEET_WIDTH 256
#define PPU_SPRITE_SHEET_SIZE (PPU_SPRITE_SHEET_WIDTH * PPU_SPRITE_SHEET_WIDTH)
#define PPU_COLOR_COUNT 32768
#define PPU_MAX_OFFSETS 256

#define PPU_RGB_PACK(r, g, b) ((r) | ((g) << 5) | ((b) << 10))

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

typedef enum
{
	ACCESS_FLAG_NONE = 0,
	ACCESS_FLAG_READ = 1 << 0,
	ACCESS_FLAG_WRITE = 1 << 1
} access_flags_e;

typedef struct
{
	char label[128];
	u8* memory;
	u32 size;
	memory_region_t region;
	u32 read_delay;
	u32 write_delay;
	u32 access_flags;
} bus_device_t;

typedef union
{
	struct
	{
		bus_device_t dram;
		bus_device_t rom;
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
	u32 prev_pc;
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
	u64 cycles;
} cpu_t;

typedef struct
{
	i32 x;
	i32 y;
} offset_t;

typedef struct
{
	u32* pixels;
	u16 palette[4];
	u32 colors[PPU_SCREEN_TOTAL_PIXELS];
	u32 color_lookup[PPU_COLOR_COUNT];
	u8* sprites;
	u32 num_sprites;
	SDL_Rect clip_rect;
	offset_t offsets[PPU_MAX_OFFSETS];
	i32 current_offset;
	offset_t offset;
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
	bool is_down;
	u64 tick_down;
} button_state_t;

typedef struct
{
	button_state_t key_states[SDL_SCANCODE_COUNT];
	button_state_t mouse_button_states[16];
	button_state_t gamepad_button_states[SDL_GAMEPAD_BUTTON_COUNT];
	i32 mouse_x;
	i32 mouse_y;
	i32 mouse_dx;
	i32 mouse_dy;
	bool mouse_captured;
	i32 mouse_wheel_delta;
	bool has_gamepad;
	i32 num_gamepads;
	SDL_JoystickID* joysticks;
	SDL_Gamepad* gamepad;
} input_t;

typedef enum
{
	BUTTON_DPAD_LEFT,
	BUTTON_DPAD_RIGHT,
	BUTTON_DPAD_UP,
	BUTTON_DPAD_DOWN,
	BUTTON_A,
	BUTTON_B,
	BUTTON_X,
	BUTTON_Y,
	BUTTON_L,
	BUTTON_R,
	BUTTON_START,
	BUTTON_SELECT,

	BUTTON_MAX_BUTTONS
} button_e;

typedef enum
{
	SPRITE_FLAG_NONE = 0,
	SPRITE_FLAG_FLIP_X = 1 << 0,
	SPRITE_FLAG_FLIP_Y = 1 << 1,
} sprite_flag_e;

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

	input_t input;

	struct
	{
		u64 last_time;
		u32 frames;
		u64 frame_timer;
		f64 delta_time;
		f64 time_step;
		u64 ticks;
	} time;
} vm_t;

void execute(vm_t* vm);

void cpu_decode(cpu_t* cpu, u32 inst);

void* bus_get_pointer(bus_t* bus, bus_device_t** pdevice, u32 address, u32 size);
u8 bus_read_8(bus_t* bus, cpu_t* cpu, u32 address);
u16 bus_read_16(bus_t* bus, cpu_t* cpu, u32 address);
u32 bus_read_32(bus_t* bus, cpu_t* cpu, u32 address);
void bus_write_8(bus_t* bus, cpu_t* cpu, u32 address, u8 value);
void bus_write_16(bus_t* bus, cpu_t* cpu, u32 address, u16 value);
void bus_write_32(bus_t* bus, cpu_t* cpu, u32 address, u32 value);

void ppu_init(ppu_t* ppu);
// void ppu_camera(ppu_t* ppu, i32 x, i32 y); // ecall 100
u16 ppu_pget(ppu_t* ppu, i32 x, i32 y);
void ppu_pset(ppu_t* ppu, i32 x, i32 y, u16 color);
void ppu_cls(ppu_t* ppu, u16 color);
void ppu_clip(ppu_t* ppu, i32 x, i32 y, i32 w, i32 h);
void ppu_rect(ppu_t* ppu, i32 x, i32 y, i32 w, i32 h, u16 color);
void ppu_rectfill(ppu_t* ppu, i32 x, i32 y, i32 w, i32 h, u16 color);
void ppu_line(ppu_t* ppu, i32 x0, i32 y0, i32 x1, i32 y1, u16 color);
void ppu_pal(ppu_t* ppu, u16 c0, u16 c1, u16 c2, u16 c3);
void ppu_spr_data(ppu_t* ppu, void* data, u32 num_sprites);
void ppu_spr(ppu_t* ppu, i32 n, i32 x, i32 y, u32 bits);
void ppu_tile(ppu_t* ppu, i32 n, i32 x, i32 y, u32 bits);
void ppu_push_offset(ppu_t* ppu, i32 x, i32 y);
void ppu_pop_offset(ppu_t* ppu);
void ppu_get_offset(ppu_t* ppu, i32* x, i32* y);

bool is_key_down(vm_t* vm, SDL_Scancode scancode);
bool is_key_pressed(vm_t* vm, SDL_Scancode scancode);
bool is_gamepad_button_down(vm_t* vm, SDL_GamepadButton button);
bool is_gamepad_button_pressed(vm_t* vm, SDL_GamepadButton button);

/*
csr 0x7C0 frame ready (ready for guest to begin frame)
	0x7C1 render ready (guest finished frame)
	0x7C2 frame start address
*/
