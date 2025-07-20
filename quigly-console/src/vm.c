#include "vm.h"

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define VM_DEBUG_INSTRUCTIONS 0

static instruction_type_e opcode_lookup_table[128] =
{
	/* 0000000 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0000001 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0000010 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0000011 */ INSTRUCTION_TYPE_I,
    /* 0000100 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0000101 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0000110 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0000111 */ INSTRUCTION_TYPE_I,
    /* 0001000 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0001001 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0001010 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0001011 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0001100 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0001101 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0001110 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0001111 */ INSTRUCTION_TYPE_I,
    /* 0010000 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0010001 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0010010 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0010011 */ INSTRUCTION_TYPE_I,
    /* 0010100 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0010101 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0010110 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0010111 */ INSTRUCTION_TYPE_U,
    /* 0011000 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0011001 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0011010 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0011011 */ INSTRUCTION_TYPE_I,
    /* 0011100 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0011101 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0011110 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0011111 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0100000 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0100001 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0100010 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0100011 */ INSTRUCTION_TYPE_S,
    /* 0100100 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0100101 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0100110 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0100111 */ INSTRUCTION_TYPE_S,
    /* 0101000 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0101001 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0101010 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0101011 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0101100 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0101101 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0101110 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0101111 */ INSTRUCTION_TYPE_R,
    /* 0110000 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0110001 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0110010 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0110011 */ INSTRUCTION_TYPE_R,
    /* 0110100 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0110101 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0110110 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0110111 */ INSTRUCTION_TYPE_U,
    /* 0111000 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0111001 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0111010 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0111011 */ INSTRUCTION_TYPE_R,
    /* 0111100 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0111101 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0111110 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 0111111 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1000000 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1000001 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1000010 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1000011 */ INSTRUCTION_TYPE_R,
    /* 1000100 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1000101 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1000110 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1000111 */ INSTRUCTION_TYPE_R,
    /* 1001000 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1001001 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1001010 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1001011 */ INSTRUCTION_TYPE_R,
    /* 1001100 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1001101 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1001110 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1001111 */ INSTRUCTION_TYPE_R,
    /* 1010000 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1010001 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1010010 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1010011 */ INSTRUCTION_TYPE_R,
    /* 1010100 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1010101 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1010110 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1010111 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1011000 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1011001 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1011010 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1011011 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1011100 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1011101 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1011110 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1011111 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1100000 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1100001 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1100010 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1100011 */ INSTRUCTION_TYPE_B,
    /* 1100100 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1100101 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1100110 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1100111 */ INSTRUCTION_TYPE_I,
    /* 1101000 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1101001 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1101010 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1101011 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1101100 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1101101 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1101110 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1101111 */ INSTRUCTION_TYPE_J,
    /* 1110000 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1110001 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1110010 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1110011 */ INSTRUCTION_TYPE_I,
    /* 1110100 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1110101 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1110110 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1110111 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1111000 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1111001 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1111010 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1111011 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1111100 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1111101 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1111110 */ INSTRUCTION_TYPE_UNKNOWN,
    /* 1111111 */ INSTRUCTION_TYPE_UNKNOWN
};

static const char* register_names[32] =
{
	/* x0 */ "zero",
	/* x1 */ "ra",
	/* x2 */ "sp",
	/* x3 */ "gp",
	/* x4 */ "tp",
	/* x5 */ "t0",
	/* x6 */ "t1",
	/* x7 */ "t2",
	/* x8 */ "s0",
	/* x9 */ "s1",
	/* x10 */ "a0",
	/* x11 */ "a1",
	/* x12 */ "a2",
	/* x13 */ "a3",
	/* x14 */ "a4",
	/* x15 */ "a5",
	/* x16 */ "a6",
	/* x17 */ "a7",
	/* x18 */ "s2",
	/* x19 */ "s3",
	/* x20 */ "s4",
	/* x21 */ "s5",
	/* x22 */ "s6",
	/* x23 */ "s7",
	/* x24 */ "s8",
	/* x25 */ "s9",
	/* x26 */ "s10",
	/* x27 */ "s11",
	/* x28 */ "t3",
	/* x29 */ "t4",
	/* x30 */ "t5",
	/* x31 */ "t6"
};

static const char* instruction_names[] =
{
	#define INSTRUCTION(id_lower, id_upper) #id_upper,
	#include "instructions.inc"
	#undef INSTRUCTION
};

static bool used_instructions[1024];

static bool bus_init_device(bus_t* bus, u32 index, const char* label, size_t start_address, size_t size)
{
	assert(bus != NULL);
	
	bus_device_t* device = &bus->devices[index];

	device->memory = calloc(1, size);
	if (device->memory == NULL)
	{
		return false;
	}

	strncpy(device->label, label, 128);

	device->size = size;

	device->region.begin = start_address;
	device->region.end = start_address + size;

	printf("%s 0x%08X-0x%08X\n", label, device->region.begin, device->region.end);

	return true;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{
	if (argc <= 1)
	{
		printf("Missing path!\n");
		return SDL_APP_FAILURE;
	}

	const char* rom_path = argv[1];

	if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD))
	{
		printf("%s\n", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	vm_t* vm = calloc(1, sizeof(vm_t));
	assert(vm != NULL);

	*appstate = vm;

	vm->video.window_width = PPU_SCREEN_WIDTH * 6;
	vm->video.window_height = PPU_SCREEN_HEIGHT * 6;

	vm->video.window = SDL_CreateWindow(":D", vm->video.window_width, vm->video.window_height, 0);
	if (vm->video.window == NULL)
	{
		printf("%s\n", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	vm->video.renderer = SDL_CreateRenderer(vm->video.window, NULL);
	if (vm->video.renderer == NULL)
	{
		printf("%s\n", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	vm->video.texture = SDL_CreateTexture(vm->video.renderer, SDL_PIXELFORMAT_XBGR8888, SDL_TEXTUREACCESS_STREAMING, PPU_SCREEN_WIDTH, PPU_SCREEN_HEIGHT);
	if (vm->video.texture == NULL)
	{
		printf("%s\n", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	
	SDL_SetTextureScaleMode(vm->video.texture, SDL_SCALEMODE_NEAREST);
	
	if (!bus_init_device(&vm->bus, 0, "dram", MEGABYTES(32), MEGABYTES(32))) { return SDL_APP_FAILURE; }
	if (!bus_init_device(&vm->bus, 1, "rom", MEGABYTES(512), MEGABYTES(128))) { return SDL_APP_FAILURE; }
	if (!bus_init_device(&vm->bus, 2, "sram", MEGABYTES(1024), MEGABYTES(8))) { return SDL_APP_FAILURE; }

	vm->bus.sram.read_delay = 1;
	vm->bus.sram.write_delay = 1;

	vm->bus.dram.access_flags = ACCESS_FLAG_READ | ACCESS_FLAG_WRITE;
	vm->bus.sram.access_flags = ACCESS_FLAG_READ | ACCESS_FLAG_WRITE;
	vm->bus.rom.access_flags = ACCESS_FLAG_READ;

	{
		FILE* fp = fopen("sram.bin", "rb");
		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_END);
			const size_t size = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			if (size != vm->bus.sram.size)
			{
				printf("sram.bin is an incorrect size!\n");
				return SDL_APP_FAILURE;
			}

			fread(vm->bus.sram.memory, size, 1, fp);

			fclose(fp);
		}
	}

	printf("Loading rom file\n");
	{
		FILE* fp = fopen(rom_path, "rb");

		fseek(fp, 0, SEEK_END);
		const size_t rom_size = ftell(fp);
		printf("Rom size: %lu bytes\n", rom_size);
		fseek(fp, 0, SEEK_SET);
	
		fread(vm->bus.rom.memory, rom_size, 1, fp);

		fclose(fp);
	}
	
	ppu_init(&vm->ppu);

	// vm->cpu.x[2] = vm->bus.dram.region.begin + MEGABYTES(1);
	vm->cpu.pc = vm->bus.rom.region.begin;

	vm->running = true;

	printf("Starting init\n");
	while (!vm->init_finished)
	{
		execute(vm);
		if (!vm->running)
		{
			return SDL_APP_FAILURE;
		}
	}
	printf("Init finished\n");
	printf("sp: 0x%08X\n", vm->cpu.x[2]);

	vm->input.has_gamepad = SDL_HasGamepad();
	if (vm->input.has_gamepad)
	{
		vm->input.joysticks = SDL_GetGamepads(&vm->input.num_gamepads);

		for (i32 i = 0; i < vm->input.num_gamepads; i += 1)
		{
			SDL_Gamepad* gamepad = SDL_OpenGamepad(vm->input.joysticks[i]);
			if (gamepad == NULL)
			{
				vm->input.gamepad = gamepad;
				break;
			}
		}
	}

	vm->time.time_step = 1000.0 / 60.0;
	vm->time.last_time = SDL_GetTicks();
	vm->time.frame_timer = vm->time.last_time;

#if 0
	for (size_t i = 0; i < array_count(instruction_names); i += 1)
	{
		const char* name = instruction_names[i];

		printf("case INSTRUCTION_%s:\n{\n} break;\n\n", name);
	}
#endif

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event *event)
{
	vm_t* vm = appstate;
	
	switch (event->type)
	{
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
		{
			printf("Closing window\n");
			return SDL_APP_SUCCESS;
		} break;

		case SDL_EVENT_KEY_DOWN:
		{
			button_state_t* state = &vm->input.key_states[event->key.scancode];

			if (!state->is_down)
			{
				state->tick_down = vm->time.ticks;
			}
			state->is_down = true;
		} break;

		case SDL_EVENT_KEY_UP:
		{
			button_state_t* state = &vm->input.key_states[event->key.scancode];

			state->is_down = false;
		} break;

		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		{
			button_state_t* state = &vm->input.mouse_button_states[event->button.button];

			if (!state->is_down)
			{
				state->tick_down = vm->time.ticks;
			}
			state->is_down = true;
		} break;
		
		case SDL_EVENT_MOUSE_BUTTON_UP:
		{
			button_state_t* state = &vm->input.mouse_button_states[event->button.button];

			state->is_down = false;
		} break;

		case SDL_EVENT_MOUSE_MOTION:
		{
			const f32 scale_x = (f32)vm->video.window_width / (f32)PPU_SCREEN_WIDTH;
			const f32 scale_y = (f32)vm->video.window_height / (f32)PPU_SCREEN_HEIGHT;
			
			vm->input.mouse_x = (f32)event->motion.x / scale_x;
			vm->input.mouse_y = (f32)event->motion.y / scale_y;
			vm->input.mouse_dx = event->motion.xrel;
			vm->input.mouse_dy = event->motion.yrel;
		} break;

		case SDL_EVENT_MOUSE_WHEEL:
		{
			vm->input.mouse_wheel_delta = event->wheel.y;
		} break;

		case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
		{
			// printf("%s\n", SDL_GetGamepadStringForButton(event->gbutton.button));
			
			button_state_t* state = &vm->input.gamepad_button_states[event->gbutton.button];

			if (!state->is_down)
			{
				state->tick_down = vm->time.ticks;
			}
			state->is_down = true;
		} break;
		
		case SDL_EVENT_GAMEPAD_BUTTON_UP:
		{
			button_state_t* state = &vm->input.gamepad_button_states[event->gbutton.button];

			state->is_down = false;
		} break;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	vm_t* vm = appstate;
	
	const u64 now = SDL_GetTicks();
	vm->time.delta_time += (f64)(now - vm->time.last_time) / vm->time.time_step;
	vm->time.last_time = now;

	if (vm->time.delta_time >= 1.0)
	{
		i32 pitch = PPU_SCREEN_WIDTH * 4;
		SDL_LockTexture(vm->video.texture, NULL, (void**)&vm->video.pixels, &pitch);
		vm->ppu.pixels = vm->video.pixels;

		ppu_clip(&vm->ppu, 0, 0, PPU_SCREEN_WIDTH, PPU_SCREEN_HEIGHT);
		vm->ppu.current_offset = 0;
		vm->ppu.offset = (offset_t){ 0, 0 };

		assert(vm->cpu.csrs[0x7C2]);
		vm->cpu.pc = vm->cpu.csrs[0x7C2];

		if (vm->paused)
		{
			if (is_key_pressed(vm, SDL_SCANCODE_BACKSPACE))
			{
				vm->paused = false;
			}
		}
		else
		{
			vm->finished_frame = false;
			while (!vm->finished_frame)
			{
				execute(vm);
				if (!vm->running)
				{
					return SDL_APP_FAILURE;
				}
			}
		}

		SDL_UnlockTexture(vm->video.texture);	
		vm->ppu.pixels = NULL;
		vm->video.pixels = NULL;

		SDL_FRect dst_rect = (SDL_FRect){ 0, 0, vm->video.window_width, vm->video.window_height };
		SDL_RenderTexture(vm->video.renderer, vm->video.texture, NULL, &dst_rect);
		SDL_RenderPresent(vm->video.renderer);

		// vm->input.mouse_dx = 0;
		// vm->input.mouse_dy = 0;
		// vm->input.mouse_wheel_delta = 0;

		vm->time.ticks += 1;
		vm->time.frames += 1;
		vm->time.delta_time -= 1.0;
	}

	if (vm->time.frame_timer - now >= 1000)
	{
		if (vm->time.frames < 60)
		{
			printf("[Warning] fps: %u\n", vm->time.frames);
		}

		vm->time.frames = 0;
		vm->time.frame_timer += 1000;
	}

	if (!vm->running)
	{
		printf("Shutting down vm\n");
		return SDL_APP_SUCCESS;
	}

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	vm_t* vm = appstate;

	{
		FILE* fp = fopen("sram.bin", "wb");
		assert(fp != NULL);

		fwrite(vm->bus.sram.memory, vm->bus.sram.size, 1, fp);

		fclose(fp);
	}
}

void execute(vm_t* vm)
{
	vm->cpu.x[0] = 0;

	if ((vm->cpu.pc % 4) != 0)
	{
		printf("prev_pc: 0x%08X\n", vm->cpu.prev_pc);
		printf("pc: 0x%08X\n", vm->cpu.pc);
		printf("cycles: %lu\n", vm->cpu.cycles);

		for (size_t i = 0; i < 32; i += 1)
		{
			printf("[%s]: 0x%X\n", register_names[i], vm->cpu.x[i]);
		}
	}
	assert((vm->cpu.pc % 4) == 0);

	vm->cpu.prev_pc = vm->cpu.pc;

	const u32 inst = bus_read_32(&vm->bus, &vm->cpu, vm->cpu.pc);
	// printf("[%08X] %08X\n", vm->cpu.pc, inst);
	// printf("inst: 0x%08X\n", inst);
	if (inst == 0)
	{
		printf("prev_pc: 0x%08X\n", vm->cpu.prev_pc);
		printf("pc: 0x%08X\n", vm->cpu.pc);
		printf("cycles: %lu\n", vm->cpu.cycles);
		printf("inst: 0x%08X\n", inst);

		for (size_t i = 0; i < 32; i += 1)
		{
			printf("[%s]: 0x%X\n", register_names[i], vm->cpu.x[i]);
		}

		unreachable;
	}

	cpu_decode(&vm->cpu, inst);

	/*if (!used_instructions[vm->cpu.inst_id] && vm->cpu.inst_id >= INSTRUCTION_FLW)
	{
		printf("%s\n", instruction_names[vm->cpu.inst_id]);
		used_instructions[vm->cpu.inst_id] = true;
	}*/

	u32 next_pc = vm->cpu.pc + 4;

	#if VM_DEBUG_INSTRUCTIONS
	printf("[%08X] %s\n", vm->cpu.pc, instruction_names[vm->cpu.inst_id]);
	#endif

	/*if (vm->cpu.inst_id >= INSTRUCTION_FLW)
	{
		printf("[%08X] %s\n", vm->cpu.pc, instruction_names[vm->cpu.inst_id]);
	}*/

	switch (vm->cpu.inst_id)
	{
		case INSTRUCTION_UNKNOWN:
		{
			printf("Unknown instruction!\n");
			vm->running = false;
		} break;

		case INSTRUCTION_LUI:
		{
			vm->cpu.x[vm->cpu.rd] = vm->cpu.imm;
		} break;

		case INSTRUCTION_AUIPC:
		{
			#if VM_DEBUG_INSTRUCTIONS
			printf("  pc: 0x%08X\n", vm->cpu.pc);
			printf("  imm: i32(%i) u32(%u) 0x%08X\n", (i32)vm->cpu.imm, vm->cpu.imm, vm->cpu.imm);
			printf("  rd: %s\n", register_names[vm->cpu.rd]);
			#endif
			vm->cpu.x[vm->cpu.rd] = vm->cpu.pc + vm->cpu.imm;
		} break;

		case INSTRUCTION_JAL:
		{
			if (vm->cpu.rd != 0)
			{
				vm->cpu.x[vm->cpu.rd] = vm->cpu.pc + 4;
			}
			next_pc = (u32)vm->cpu.imm + vm->cpu.pc;
			
			#if VM_DEBUG_INSTRUCTIONS
			printf("  rd: %s\n", register_names[vm->cpu.rd]);
			printf("  0x%08X\n", next_pc);
			#endif
		} break;

		case INSTRUCTION_JALR:
		{
			const u32 target_addr = ((vm->cpu.x[vm->cpu.rs1] + (u32)vm->cpu.imm) & ~1);
			const u32 return_addr = vm->cpu.pc + 4;

			if (vm->cpu.rd != 0)
			{
				vm->cpu.x[vm->cpu.rd] = return_addr;
			}

			next_pc = target_addr;

			#if VM_DEBUG_INSTRUCTIONS
			printf("  rs1: (i32)%i (u32)%u 0x%X\n", vm->cpu.x[vm->cpu.rs1], vm->cpu.x[vm->cpu.rs1], vm->cpu.x[vm->cpu.rs1]);
			printf("  imm: (i32)%i (u32)%u 0x%X\n", vm->cpu.x[vm->cpu.imm], vm->cpu.x[vm->cpu.imm], vm->cpu.x[vm->cpu.imm]);
			printf("  next_pc: 0x%08X\n", next_pc);

			bus_device_t* device;
			if (bus_get_pointer(&vm->bus, &device, next_pc, 4) == NULL)
			{
				unreachable;
			}
			#endif
		} break;

		case INSTRUCTION_BEQ:
		{
			if (vm->cpu.x[vm->cpu.rs1] == vm->cpu.x[vm->cpu.rs2])
			{
				next_pc = vm->cpu.pc + vm->cpu.imm;
			}
		} break;

		case INSTRUCTION_BNE:
		{
			if (vm->cpu.x[vm->cpu.rs1] != vm->cpu.x[vm->cpu.rs2])
			{
				next_pc = vm->cpu.pc + vm->cpu.imm;
			}
		} break;

		case INSTRUCTION_BLT:
		{
			if ((i32)vm->cpu.x[vm->cpu.rs1] < (i32)vm->cpu.x[vm->cpu.rs2])
			{
				next_pc = vm->cpu.pc + vm->cpu.imm;
			}
		} break;

		case INSTRUCTION_BGE:
		{
			if ((i32)vm->cpu.x[vm->cpu.rs1] >= (i32)vm->cpu.x[vm->cpu.rs2])
			{
				next_pc = vm->cpu.pc + vm->cpu.imm;
			}
		} break;

		case INSTRUCTION_BLTU:
		{
			if (vm->cpu.x[vm->cpu.rs1] < vm->cpu.x[vm->cpu.rs2])
			{
				next_pc = vm->cpu.pc + vm->cpu.imm;
			}
		} break;

		case INSTRUCTION_BGEU:
		{
			if (vm->cpu.x[vm->cpu.rs1] >= vm->cpu.x[vm->cpu.rs2])
			{
				next_pc = vm->cpu.pc + vm->cpu.imm;
			}
		} break;

		case INSTRUCTION_LB:
		{
			if (vm->cpu.rd != 0)
			{
				u32 address = vm->cpu.x[vm->cpu.rs1] + vm->cpu.imm;
				u32 value = (u32)(i32)(i8)bus_read_8(&vm->bus, &vm->cpu, address);
				vm->cpu.x[vm->cpu.rd] = value;
			}
		} break;

		case INSTRUCTION_LH:
		{
			u32 address = vm->cpu.x[vm->cpu.rs1] + vm->cpu.imm;
			u32 value = (u32)(i32)(i16)bus_read_16(&vm->bus, &vm->cpu, address);
			vm->cpu.x[vm->cpu.rd] = value;
		} break;

		case INSTRUCTION_LW:
		{
			u32 address = vm->cpu.x[vm->cpu.rs1] + vm->cpu.imm;
			u32 value = bus_read_32(&vm->bus, &vm->cpu, address);
			vm->cpu.x[vm->cpu.rd] = value;

			if (vm->cpu.rd == 2 && value < MEGABYTES(32))
			{
				unreachable;
			}
		} break;

		case INSTRUCTION_LBU:
		{
			u32 address = vm->cpu.x[vm->cpu.rs1] + vm->cpu.imm;
			u32 value = bus_read_8(&vm->bus, &vm->cpu, address);
			vm->cpu.x[vm->cpu.rd] = value;
		} break;

		case INSTRUCTION_LHU:
		{
			u32 address = vm->cpu.x[vm->cpu.rs1] + vm->cpu.imm;
			u32 value = bus_read_16(&vm->bus, &vm->cpu, address);
			vm->cpu.x[vm->cpu.rd] = value;
		} break;

		case INSTRUCTION_SB:
		{
			u32 address = vm->cpu.x[vm->cpu.rs1] + vm->cpu.imm;
			u8 value = (u8)(vm->cpu.x[vm->cpu.rs2]);
			bus_write_8(&vm->bus, &vm->cpu, address, value);
		} break;

		case INSTRUCTION_SH:
		{
			u32 address = vm->cpu.x[vm->cpu.rs1] + vm->cpu.imm;
			u16 value = (u16)(vm->cpu.x[vm->cpu.rs2]);
			bus_write_16(&vm->bus, &vm->cpu, address, value);
		} break;

		case INSTRUCTION_SW:
		{
			u32 address = vm->cpu.x[vm->cpu.rs1] + vm->cpu.imm;
			u32 value = vm->cpu.x[vm->cpu.rs2];
			bus_write_32(&vm->bus, &vm->cpu, address, value);
		} break;

		case INSTRUCTION_ADDI:
		{
			i32 x = (i32)vm->cpu.x[vm->cpu.rs1];
			i32 y = (i32)vm->cpu.imm;
			#if VM_DEBUG_INSTRUCTIONS
			printf("  rs1.val: i32(%i) u32(%u) 0x%X\n", x, (u32)x, (u32)x);
			printf("  imm.val: i32(%i) u32(%u) 0x%X\n", y, (u32)y, (u32)y);
			printf("  result: i32(%i) u32(%u) 0x%X\n", (x + y), (u32)(x + y), (u32)(x + y));
			#endif
			vm->cpu.x[vm->cpu.rd] = (u32)(x + y);
		} break;

		case INSTRUCTION_SLTI:
		{
			i32 x = (i32)vm->cpu.x[vm->cpu.rs1];
			i32 y = (i32)vm->cpu.imm;
			vm->cpu.x[vm->cpu.rd] = (x < y) ? 1 : 0;
		} break;

		case INSTRUCTION_SLTIU:
		{
			u32 x = vm->cpu.x[vm->cpu.rs1];
			u32 y = vm->cpu.imm;
			vm->cpu.x[vm->cpu.rd] = (x < y) ? 1 : 0;
		} break;

		case INSTRUCTION_XORI:
		{
			u32 x = vm->cpu.x[vm->cpu.rs1];
			u32 y = vm->cpu.imm;
			vm->cpu.x[vm->cpu.rd] = x ^ y;
		} break;

		case INSTRUCTION_ORI:
		{
			u32 x = vm->cpu.x[vm->cpu.rs1];
			u32 y = vm->cpu.imm;
			vm->cpu.x[vm->cpu.rd] = x | y;
		} break;

		case INSTRUCTION_ANDI:
		{
			u32 x = vm->cpu.x[vm->cpu.rs1];
			u32 y = vm->cpu.imm;
			vm->cpu.x[vm->cpu.rd] = x & y;
		} break;

		case INSTRUCTION_SLLI:
		{
			u32 x = vm->cpu.x[vm->cpu.rs1];
			u32 y = vm->cpu.shamt;
			vm->cpu.x[vm->cpu.rd] = x << y;
		} break;

		case INSTRUCTION_SRLI:
		{
			u32 x = vm->cpu.x[vm->cpu.rs1];
			u32 y = vm->cpu.shamt;
			vm->cpu.x[vm->cpu.rd] = x >> y;
		} break;

		case INSTRUCTION_SRAI:
		{
			i32 x = (i32)vm->cpu.x[vm->cpu.rs1];
			u32 y = vm->cpu.shamt;
			vm->cpu.x[vm->cpu.rd] = (u32)(x >> y);
		} break;

		case INSTRUCTION_ADD:
		{
			u32 x = vm->cpu.x[vm->cpu.rs1];
			u32 y = vm->cpu.x[vm->cpu.rs2];
			// printf("  %i %i (0x%X)\n", x, y, x + y);
			vm->cpu.x[vm->cpu.rd] = x + y;
		} break;

		case INSTRUCTION_SUB:
		{
			u32 x = vm->cpu.x[vm->cpu.rs1];
			u32 y = vm->cpu.x[vm->cpu.rs2];
			vm->cpu.x[vm->cpu.rd] = x - y;
		} break;

		case INSTRUCTION_SLL:
		{
			u32 x = vm->cpu.x[vm->cpu.rs1];
			u32 y = vm->cpu.x[vm->cpu.rs2] & SHAMT_MASK;
			vm->cpu.x[vm->cpu.rd] = x << y;
		} break;

		case INSTRUCTION_SLT:
		{
			i32 x = (i32)vm->cpu.x[vm->cpu.rs1];
			i32 y = (i32)vm->cpu.x[vm->cpu.rs2];
			vm->cpu.x[vm->cpu.rd] = (x < y) ? 1 : 0;
		} break;

		case INSTRUCTION_SLTU:
		{
			u32 x = vm->cpu.x[vm->cpu.rs1];
			u32 y = vm->cpu.x[vm->cpu.rs2];
			vm->cpu.x[vm->cpu.rd] = (x < y) ? 1 : 0;
		} break;

		case INSTRUCTION_XOR:
		{
			u32 x = vm->cpu.x[vm->cpu.rs1];
			u32 y = vm->cpu.x[vm->cpu.rs2];
			vm->cpu.x[vm->cpu.rd] = x ^ y;
		} break;

		case INSTRUCTION_SRL:
		{
			u32 x = vm->cpu.x[vm->cpu.rs1];
			u32 y = vm->cpu.x[vm->cpu.rs2] & SHAMT_MASK;
			vm->cpu.x[vm->cpu.rd] = x >> y;
		} break;

		case INSTRUCTION_SRA:
		{
			i32 x = (i32)vm->cpu.x[vm->cpu.rs1];
			u32 y = vm->cpu.x[vm->cpu.rs2] & SHAMT_MASK;
			vm->cpu.x[vm->cpu.rd] = (u32)(x >> y);
		} break;

		case INSTRUCTION_OR:
		{
			u32 x = vm->cpu.x[vm->cpu.rs1];
			u32 y = vm->cpu.x[vm->cpu.rs2];
			vm->cpu.x[vm->cpu.rd] = x | y;
		} break;

		case INSTRUCTION_AND:
		{
			u32 x = vm->cpu.x[vm->cpu.rs1];
			u32 y = vm->cpu.x[vm->cpu.rs2];
			vm->cpu.x[vm->cpu.rd] = x & y;
		} break;

		case INSTRUCTION_FENCE:
		{
			unreachable;
		} break;

		case INSTRUCTION_FENCE_TSO:
		{
			unreachable;
		} break;

		case INSTRUCTION_PAUSE:
		{
			unreachable;
		} break;

		case INSTRUCTION_ECALL:
		{
			const u32 syscall_value = vm->cpu.x[17]; // a7
			#if VM_DEBUG_INSTRUCTIONS
			/*printf("  value: %u\n", syscall_value);*/
			for (u32 i = 0; i < 8; i += 1)
			{
				printf("  a%u: (i32)%i (u32)%u 0x%08X\n",
					i,
					vm->cpu.x[10 + i],
					vm->cpu.x[10 + i],
					vm->cpu.x[10 + i]);
			}
			#endif
			switch (syscall_value)
			{
				case 50: // btn
				{
					const u8 button = vm->cpu.x[10];
					const u8 player = vm->cpu.x[11];

					bool is_down = false;

					switch (button)
					{
						case BUTTON_DPAD_LEFT:
						{
							is_down = is_key_down(vm, SDL_SCANCODE_A) ||
								is_gamepad_button_down(vm, SDL_GAMEPAD_BUTTON_DPAD_LEFT);
						} break;
						
						case BUTTON_DPAD_RIGHT:
						{
							is_down = is_key_down(vm, SDL_SCANCODE_D) ||
								is_gamepad_button_down(vm, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
						} break;

						case BUTTON_DPAD_UP:
						{
							is_down = is_key_down(vm, SDL_SCANCODE_W) ||
								is_gamepad_button_down(vm, SDL_GAMEPAD_BUTTON_DPAD_UP);
						} break;
						
						case BUTTON_DPAD_DOWN:
						{
							is_down = is_key_down(vm, SDL_SCANCODE_S) ||
								is_gamepad_button_down(vm, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
						} break;
						
						case BUTTON_A:
						{
							is_down = is_key_down(vm, SDL_SCANCODE_Z) ||
								is_gamepad_button_down(vm, SDL_GAMEPAD_BUTTON_SOUTH);
						} break;

						case BUTTON_B:
						{
							is_down = is_key_down(vm, SDL_SCANCODE_X) ||
								is_gamepad_button_down(vm, SDL_GAMEPAD_BUTTON_EAST);
						} break;
						
						case BUTTON_X:
						{
							is_down = is_key_down(vm, SDL_SCANCODE_C) ||
								is_gamepad_button_down(vm, SDL_GAMEPAD_BUTTON_WEST);
						} break;

						case BUTTON_Y:
						{
							is_down = is_key_down(vm, SDL_SCANCODE_V) ||
								is_gamepad_button_down(vm, SDL_GAMEPAD_BUTTON_NORTH);
						} break;
						
						case BUTTON_L:
						{
							is_down = is_key_down(vm, SDL_SCANCODE_Q) ||
								is_gamepad_button_down(vm, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER);
						} break;
						
						case BUTTON_R:
						{
							is_down = is_key_down(vm, SDL_SCANCODE_E) ||
								is_gamepad_button_down(vm, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);
						} break;
						
						case BUTTON_SELECT:
						{
							is_down = is_key_down(vm, SDL_SCANCODE_TAB) ||
								is_gamepad_button_down(vm, SDL_GAMEPAD_BUTTON_BACK);
						} break;
						
						case BUTTON_START:
						{
							is_down = is_key_down(vm, SDL_SCANCODE_RETURN) ||
								is_gamepad_button_down(vm, SDL_GAMEPAD_BUTTON_START);
						} break;
					}

					vm->cpu.x[10] = is_down;
				} break;

				case 51: // btnp
				{
					const u8 button = vm->cpu.x[10];
					const u8 player = vm->cpu.x[11];

					bool is_pressed = false;

					switch (button)
					{
						case BUTTON_DPAD_LEFT:
						{
							is_pressed = is_key_pressed(vm, SDL_SCANCODE_A) ||
								is_gamepad_button_pressed(vm, SDL_GAMEPAD_BUTTON_DPAD_LEFT);
						} break;
						
						case BUTTON_DPAD_RIGHT:
						{
							is_pressed = is_key_pressed(vm, SDL_SCANCODE_D) ||
								is_gamepad_button_pressed(vm, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
						} break;

						case BUTTON_DPAD_UP:
						{
							is_pressed = is_key_pressed(vm, SDL_SCANCODE_W) ||
								is_gamepad_button_pressed(vm, SDL_GAMEPAD_BUTTON_DPAD_UP);
						} break;
						
						case BUTTON_DPAD_DOWN:
						{
							is_pressed = is_key_pressed(vm, SDL_SCANCODE_S) ||
								is_gamepad_button_pressed(vm, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
						} break;
						
						case BUTTON_A:
						{
							is_pressed = is_key_pressed(vm, SDL_SCANCODE_Z) ||
								is_gamepad_button_pressed(vm, SDL_GAMEPAD_BUTTON_SOUTH);
						} break;

						case BUTTON_B:
						{
							is_pressed = is_key_pressed(vm, SDL_SCANCODE_X) ||
								is_gamepad_button_pressed(vm, SDL_GAMEPAD_BUTTON_EAST);
						} break;
						
						case BUTTON_X:
						{
							is_pressed = is_key_pressed(vm, SDL_SCANCODE_C) ||
								is_gamepad_button_pressed(vm, SDL_GAMEPAD_BUTTON_WEST);
						} break;

						case BUTTON_Y:
						{
							is_pressed = is_key_pressed(vm, SDL_SCANCODE_V) ||
								is_gamepad_button_pressed(vm, SDL_GAMEPAD_BUTTON_NORTH);
						} break;
						
						case BUTTON_L:
						{
							is_pressed = is_key_pressed(vm, SDL_SCANCODE_Q) ||
								is_gamepad_button_pressed(vm, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER);
						} break;
						
						case BUTTON_R:
						{
							is_pressed = is_key_pressed(vm, SDL_SCANCODE_E) ||
								is_gamepad_button_pressed(vm, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);
						} break;
						
						case BUTTON_SELECT:
						{
							is_pressed = is_key_pressed(vm, SDL_SCANCODE_TAB) ||
								is_gamepad_button_pressed(vm, SDL_GAMEPAD_BUTTON_BACK);
						} break;
						
						case BUTTON_START:
						{
							is_pressed = is_key_pressed(vm, SDL_SCANCODE_RETURN) ||
								is_gamepad_button_pressed(vm, SDL_GAMEPAD_BUTTON_START);
						} break;
					}

					vm->cpu.x[10] = is_pressed;
				} break;

				case 52: // putc
				{
					const char c = vm->cpu.x[10];

					putc(c, stdout);
				} break;

				case 53: // exit
				{
					vm->running = false;
				} break;

				case 98:
				{
					vm->init_finished = true;
				} break;
				
				case 99:
				{
					vm->finished_frame = true;
				} break;

				case 101: // pget
				{
					const i32 x = *(i32*)&vm->cpu.x[10];
					const i32 y = *(i32*)&vm->cpu.x[11];

					vm->cpu.x[10] = ppu_pget(&vm->ppu, x, y);
				} break;
				
				case 102: // pset
				{
					const i32 x = *(i32*)&vm->cpu.x[10];
					const i32 y = *(i32*)&vm->cpu.x[11];
					const u16 color = vm->cpu.x[12];

					ppu_pset(&vm->ppu, x, y, color);
				} break;

				case 105: // cls
				{
					const u16 color = vm->cpu.x[10];

					ppu_cls(&vm->ppu, color);
				} break;

				case 106: // clip
				{
					const i32 x = *(i32*)&vm->cpu.x[10];
					const i32 y = *(i32*)&vm->cpu.x[11];
					const i32 w = *(i32*)&vm->cpu.x[12];
					const i32 h = *(i32*)&vm->cpu.x[13];

					ppu_clip(&vm->ppu, x, y, w, h);
				} break;

				case 107: // rect
				{
					const i32 x = *(i32*)&vm->cpu.x[10];
					const i32 y = *(i32*)&vm->cpu.x[11];
					const i32 w = *(i32*)&vm->cpu.x[12];
					const i32 h = *(i32*)&vm->cpu.x[13];
					const u16 color = vm->cpu.x[14];

					ppu_rect(&vm->ppu, x, y, w, h, color);
				} break;
				
				case 108: // rectfill
				{
					const i32 x = *(i32*)&vm->cpu.x[10];
					const i32 y = *(i32*)&vm->cpu.x[11];
					const i32 w = *(i32*)&vm->cpu.x[12];
					const i32 h = *(i32*)&vm->cpu.x[13];
					const u16 color = vm->cpu.x[14];

					ppu_rectfill(&vm->ppu, x, y, w, h, color);
				} break;
				
				case 109: // line
				{
					const i32 x0 = *(i32*)&vm->cpu.x[10];
					const i32 y0 = *(i32*)&vm->cpu.x[11];
					const i32 x1 = *(i32*)&vm->cpu.x[12];
					const i32 y1 = *(i32*)&vm->cpu.x[13];
					const u16 color = vm->cpu.x[14];

					ppu_line(&vm->ppu, x0, y0, x1, y1, color);
				} break;
				
				case 110: // pal
				{
					const u32 c0 = vm->cpu.x[10];
					const u32 c1 = vm->cpu.x[11];
					const u32 c2 = vm->cpu.x[12];
					const u32 c3 = vm->cpu.x[13];
					assert(c0 < PPU_COLOR_COUNT);
					assert(c1 < PPU_COLOR_COUNT);
					assert(c2 < PPU_COLOR_COUNT);
					assert(c3 < PPU_COLOR_COUNT);

					ppu_pal(&vm->ppu, c0, c1, c2, c3);
				} break;
				
				case 111: // spr_data
				{
					const u32 data_addr = vm->cpu.x[10];
					const i32 num_sprites = vm->cpu.x[11];

					bus_device_t* device;

					void* data = bus_get_pointer(&vm->bus, &device, data_addr, 4);
					if (data == NULL) { unreachable; }
					if (!(device->access_flags & ACCESS_FLAG_READ)) { unreachable; }

					ppu_spr_data(&vm->ppu, data, num_sprites);
				} break;
				
				case 112: // spr
				{
					const i32 n = *(i32*)&vm->cpu.x[10];
					const i32 x = *(i32*)&vm->cpu.x[11];
					const i32 y = *(i32*)&vm->cpu.x[12];
					const bool bits = vm->cpu.x[13];

					ppu_spr(&vm->ppu, n, x, y, bits);
				} break;

				case 113: // tile
				{
					const i32 n = *(i32*)&vm->cpu.x[10];
					const i32 x = *(i32*)&vm->cpu.x[11];
					const i32 y = *(i32*)&vm->cpu.x[12];
					const bool bits = vm->cpu.x[13];

					ppu_tile(&vm->ppu, n, x, y, bits);
				} break;

				case 114: // push_offset
				{
					const i32 x = *(i32*)&vm->cpu.x[10];
					const i32 y = *(i32*)&vm->cpu.x[11];

					ppu_push_offset(&vm->ppu, x, y);
				} break;

				case 115: // pop_offset
				{
					ppu_pop_offset(&vm->ppu);
				} break;

				case 116: // get_offset
				{
					const u32 x_addr = vm->cpu.x[10];
					const u32 y_addr = vm->cpu.x[11];

					i32* x;
					i32* y;

					bus_device_t* device;
					x = bus_get_pointer(&vm->bus, &device, x_addr, 4);
					if (x == NULL) { unreachable; }

					y = bus_get_pointer(&vm->bus, &device, y_addr, 4);
					if (y == NULL) { unreachable; }

					ppu_get_offset(&vm->ppu, x, y);
				} break;
				
				default:
				{
					printf("Unknown syscall %u\n", syscall_value);
					vm->running = false;
				}
			}
		} break;

		case INSTRUCTION_EBREAK:
		{
			printf("Triggered breakpoint at 0x%08X\n", vm->cpu.pc);
			vm->paused = true;
		} break;

		case INSTRUCTION_MUL:
		{
			i32 x = vm->cpu.x[vm->cpu.rs1];
			i32 y = vm->cpu.x[vm->cpu.rs2];
			vm->cpu.x[vm->cpu.rd] = x * y;
		} break;

		case INSTRUCTION_MULH:
		{
			i64 x = (i64)(i32)vm->cpu.x[vm->cpu.rs1];
			i64 y = (i64)(i32)vm->cpu.x[vm->cpu.rs2];
			vm->cpu.x[vm->cpu.rd] = (x * y) >> 32;
		} break;

		case INSTRUCTION_MULHSU:
		{
			i64 x = (i64)vm->cpu.x[vm->cpu.rs1];
			u64 y = vm->cpu.x[vm->cpu.rs2];
			vm->cpu.x[vm->cpu.rd] = (x * y) >> 32;
		} break;

		case INSTRUCTION_MULHU:
		{
			u64 x = (i64)(i32)vm->cpu.x[vm->cpu.rs1];
			u64 y = (u64)vm->cpu.x[vm->cpu.rs2];
			vm->cpu.x[vm->cpu.rd] = (u32)((x * y) >> 32);
		} break;

		case INSTRUCTION_DIV:
		{
			i32 x = vm->cpu.x[vm->cpu.rs1];
			i32 y = vm->cpu.x[vm->cpu.rs2];
			//printf("  div %i %i\n", x, y);

			u32 r;
			if (y == 0)
			{
				r = -1;
			}
			else
			{
				r = x / y;
			}
			vm->cpu.x[vm->cpu.rd] = r;
		} break;

		case INSTRUCTION_DIVU:
		{
			u32 r;
			if (vm->cpu.x[vm->cpu.rs2] == 0)
			{
				r = 0xFFFFFFFF;
			}
			else
			{
				r = vm->cpu.x[vm->cpu.rs1] / vm->cpu.x[vm->cpu.rs2];
			}
			vm->cpu.x[vm->cpu.rd] = r;
		} break;

		case INSTRUCTION_REM:
		{
			i32 x = vm->cpu.x[vm->cpu.rs1];
			i32 y = vm->cpu.x[vm->cpu.rs2];
			// printf("  x: %i, b: %i\n", x, y);
			
			u32 r;
			if (y == 0)
			{
				r = x;
			}
			else
			{
				r = x % y;
			}
			vm->cpu.x[vm->cpu.rd] = r;
		} break;

		case INSTRUCTION_REMU:
		{
			u32 r;
			if (vm->cpu.x[vm->cpu.rs2] == 0)
			{
				r = vm->cpu.x[vm->cpu.rs1];
			}
			else
			{
				r = vm->cpu.x[vm->cpu.rs1] % vm->cpu.x[vm->cpu.rs2];
			}
			vm->cpu.x[vm->cpu.rd] = r;
		} break;

		case INSTRUCTION_CSRRW:
		{
			assert(vm->cpu.csr < 4096);
			// printf("CSRRW 0x%X, 0x%X, 0x%X\n", vm->cpu.rd, vm->cpu.rs1, vm->cpu.csr);

			u32 old_value = vm->cpu.csrs[vm->cpu.csr];
			if (vm->cpu.rs1 != 0)
			{
				vm->cpu.csrs[vm->cpu.csr] = vm->cpu.x[vm->cpu.rs1];
			}
			if (vm->cpu.rd != 0)
			{
				vm->cpu.x[vm->cpu.rd] = old_value;
			}
		} break;

		case INSTRUCTION_CSRRS:
		{
			assert(vm->cpu.csr < 4096);

			u32 old_value = vm->cpu.csrs[vm->cpu.csr];
			if (vm->cpu.rs1 != 0)
			{
				vm->cpu.csrs[vm->cpu.csr] &= ~vm->cpu.x[vm->cpu.rs1];
			}
			if (vm->cpu.rd != 0)
			{
				vm->cpu.x[vm->cpu.rd] = old_value;
			}
		} break;

		case INSTRUCTION_CSRRC:
		{
			assert(vm->cpu.csr < 4096);

			u32 old_value = vm->cpu.csrs[vm->cpu.csr];
			if (vm->cpu.rs1 != 0)
			{
				vm->cpu.csrs[vm->cpu.csr] |= vm->cpu.x[vm->cpu.rs1];
			}
			if (vm->cpu.rd != 0)
			{
				vm->cpu.x[vm->cpu.rd] = old_value;
			}
		} break;

		case INSTRUCTION_CSRRWI:
		{
			assert(vm->cpu.csr < 4096);
			u32 old_value = vm->cpu.csrs[vm->cpu.csr];
			vm->cpu.csrs[vm->cpu.csr] = vm->cpu.imm;
			if (vm->cpu.rd != 0)
			{
				vm->cpu.x[vm->cpu.rd] = old_value;
			}
		} break;

		case INSTRUCTION_CSRRSI:
		{
			assert(vm->cpu.csr < 4096);
			u32 old_value = vm->cpu.csrs[vm->cpu.csr];

			if (vm->cpu.imm != 0)
			{
				vm->cpu.csrs[vm->cpu.csr] |= vm->cpu.imm;
			}
			if (vm->cpu.rd != 0)
			{
				vm->cpu.x[vm->cpu.rd] = old_value;
			}
		} break;

		case INSTRUCTION_CSRRCI:
		{
			assert(vm->cpu.csr < 4096);
			u32 old_value = vm->cpu.csrs[vm->cpu.csr];

			if (vm->cpu.imm != 0)
			{
				vm->cpu.csrs[vm->cpu.csr] &= ~vm->cpu.imm;
			}
			if (vm->cpu.rd != 0)
			{
				vm->cpu.x[vm->cpu.rd] = old_value;
			}
		} break;
	}

	if (next_pc < MEGABYTES(1))
	{
		printf("prev_pc: 0x%08X\n", vm->cpu.prev_pc);
		printf("pc: 0x%08X\n", vm->cpu.pc);
		printf("next_pc: 0x%08X\n", next_pc);
		printf("cycles: %lu\n", vm->cpu.cycles);
		printf("inst: 0x%08X\n", inst);

		for (size_t i = 0; i < 32; i += 1)
		{
			printf("[%s]: 0x%X\n", register_names[i], vm->cpu.x[i]);
		}

		unreachable;
	}

	vm->cpu.pc = next_pc;
	vm->cpu.cycles += 1;

	#if VM_DEBUG_INSTRUCTIONS
	for (i32 i = 0; i < 32; i += 1)
	{
		const u32 x_curr = vm->cpu.x[i];
		const u32 x_last = vm->cpu.x_last[i];

		if (x_curr == x_last)
		{
			continue;
		}

		printf("  [%s] (i32)%i (u32)%u 0x%X\n", register_names[i], (i32)x_curr, x_curr, x_curr);
		printf("       (i32)%i (u32)%u 0x%X\n", (i32)x_last, x_last, x_last);

		vm->cpu.x_last[i] = x_curr;
	}
	#endif

	if (vm->cpu.x[2] == 0)
	{
		unreachable;
	}
}

static inline i32 i_imm(u32 inst)
{
	return ((i32)inst) >> 20;
}

static inline i32 s_imm(u32 inst)
{
	return ((int32_t)(inst & 0xFE000000)) >> 20 | ((int32_t)(inst >> 7) & 0x1F);
}

static inline i32 u_imm(u32 inst)
{
	return inst & 0b11111111111111111111000000000000;
}

static inline i32 j_imm(u32 inst)
{
	u32 dst = 0;
    dst |= (inst & 0b10000000000000000000000000000000);
    dst |= (inst & 0b00000000000011111111000000000000) << 11;
    dst |= (inst & 0b00000000000100000000000000000000) << 2;
    dst |= (inst & 0b01111111111000000000000000000000) >> 9;
    return ((int32_t) dst) >> 11;
}

static inline i32 b_imm(u32 inst)
{
	u32 imm = ((inst >> 31) & 0x1) << 12 |
			  ((inst >> 7)  & 0x1) << 11 |
			  ((inst >> 25) & 0x3F) << 5 |
			  ((inst >> 8)  & 0xF) << 1;
	imm = (i32)(imm << 19) >> 19;  // Sign-extend 13-bit
	
	return imm;
}

void cpu_decode(cpu_t* cpu, u32 inst)
{
	cpu->opcode = inst & OPCODE_MASK;
	cpu->inst_type = opcode_lookup_table[cpu->opcode];
	if (cpu->inst_type == INSTRUCTION_TYPE_UNKNOWN)
	{
		printf("Error: unknown instruction type at 0x%08X\n", cpu->pc);
		printf("Opcode: %u\n", cpu->opcode);
		printf("Inst: 0x%08X\n", inst);
		exit(1);
	}
	cpu->rd = (inst >> 7) & RS_MASK;
	cpu->rs1 = (inst >> 15) & RS_MASK;
	cpu->rs2 = (inst >> 20) & RS_MASK;
	cpu->funct3 = (inst >> 12) & FUNCT3_MASK;
	cpu->funct7 = (inst >> 25) & FUNCT7_MASK;
	cpu->inst_id = INSTRUCTION_UNKNOWN;

	switch (cpu->inst_type)
	{
		case INSTRUCTION_TYPE_UNKNOWN:
		{
			cpu->inst_id = INSTRUCTION_UNKNOWN;
		} break;

		case INSTRUCTION_TYPE_R:
		{
			switch (cpu->opcode)
			{
				case 0b0110011:
				{
					if (cpu->funct7 == 0b0000000 && cpu->funct3 == 0b000) { cpu->inst_id = INSTRUCTION_ADD; return; }
					else if (cpu->funct7 == 0b0100000 && cpu->funct3 == 0b000) { cpu->inst_id = INSTRUCTION_SUB; return; }
					else if (cpu->funct7 == 0b0000000 && cpu->funct3 == 0b001) { cpu->inst_id = INSTRUCTION_SLL; return; }
					else if (cpu->funct7 == 0b0000000 && cpu->funct3 == 0b010) { cpu->inst_id = INSTRUCTION_SLT; return; }
					else if (cpu->funct7 == 0b0000000 && cpu->funct3 == 0b011) { cpu->inst_id = INSTRUCTION_SLTU; return; }
					else if (cpu->funct7 == 0b0000000 && cpu->funct3 == 0b100) { cpu->inst_id = INSTRUCTION_XOR; return; }
					else if (cpu->funct7 == 0b0000000 && cpu->funct3 == 0b101) { cpu->inst_id = INSTRUCTION_SRL; return; }
					else if (cpu->funct7 == 0b0100000 && cpu->funct3 == 0b101) { cpu->inst_id = INSTRUCTION_SRA; return; }
					else if (cpu->funct7 == 0b0000000 && cpu->funct3 == 0b110) { cpu->inst_id = INSTRUCTION_OR; return; }
					else if (cpu->funct7 == 0b0000000 && cpu->funct3 == 0b111) { cpu->inst_id = INSTRUCTION_AND; return; }

					else if (cpu->funct7 == 0b0000001 && cpu->funct3 == 0b000) { cpu->inst_id = INSTRUCTION_MUL; return; }
					else if (cpu->funct7 == 0b0000001 && cpu->funct3 == 0b001) { cpu->inst_id = INSTRUCTION_MULH; return; }
					else if (cpu->funct7 == 0b0000001 && cpu->funct3 == 0b010) { cpu->inst_id = INSTRUCTION_MULHSU; return; }
					else if (cpu->funct7 == 0b0000001 && cpu->funct3 == 0b011) { cpu->inst_id = INSTRUCTION_MULHU; return; }
					else if (cpu->funct7 == 0b0000001 && cpu->funct3 == 0b100) { cpu->inst_id = INSTRUCTION_DIV; return; }
					else if (cpu->funct7 == 0b0000001 && cpu->funct3 == 0b101) { cpu->inst_id = INSTRUCTION_DIVU; return; }
					else if (cpu->funct7 == 0b0000001 && cpu->funct3 == 0b110) { cpu->inst_id = INSTRUCTION_REM; return; }
					else if (cpu->funct7 == 0b0000001 && cpu->funct3 == 0b111) { cpu->inst_id = INSTRUCTION_REMU; return; }
				} break;
				
				default: return;
			}
		} break;
		
		case INSTRUCTION_TYPE_I:
		{
			cpu->imm = i_imm(inst);
			cpu->csr = (inst >> 20) & 0xFFF;
			cpu->shamt = cpu->imm & SHAMT_MASK;
			cpu->funct3 = (inst >> 12) & FUNCT3_MASK;

			switch (cpu->opcode)
			{
				case 0b0010011:
				{
					switch (cpu->funct3)
					{
						case 0b000: cpu->inst_id = INSTRUCTION_ADDI; return;
						case 0b010: cpu->inst_id = INSTRUCTION_SLTI; return;
						case 0b011: cpu->inst_id = INSTRUCTION_SLTIU; return;
						case 0b100: cpu->inst_id = INSTRUCTION_XORI; return;
						case 0b110: cpu->inst_id = INSTRUCTION_ORI; return;
						case 0b111: cpu->inst_id = INSTRUCTION_ANDI; return;
						case 0b001: if (cpu->shiftop == 0b000000) { cpu->inst_id = INSTRUCTION_SLLI; return; }
						case 0b101: if (cpu->shiftop == 0b000000) { cpu->inst_id = INSTRUCTION_SRLI; return; }
									if (cpu->shiftop == 0b010000) { cpu->inst_id = INSTRUCTION_SRAI; return; }

						default: return;
					}
				} break;

				case 0b0000011:
				{
					switch (cpu->funct3)
					{
						case 0b000: cpu->inst_id = INSTRUCTION_LB; return;
						case 0b001: cpu->inst_id = INSTRUCTION_LH; return;
						case 0b010: cpu->inst_id = INSTRUCTION_LW; return;
						case 0b100: cpu->inst_id = INSTRUCTION_LBU; return;
						case 0b101: cpu->inst_id = INSTRUCTION_LHU; return;

						default: return;
					}
				} break;

				case 0b0001111:
				{
					switch (cpu->funct3)
					{
						case 0b000: cpu->inst_id = INSTRUCTION_FENCE; return;

						default: return;
					}
				} break;

				case 0b1100111:
				{
					switch (cpu->funct3)
					{
						case 0b000: cpu->inst_id = INSTRUCTION_JALR; return;

						default: return;
					}
				} break;

				case 0b1110011:
				{
					cpu->csr = (inst >> 20) & 0xFFF;
					cpu->funct7 = (inst >> 25) & 0b1111111;
					cpu->rs2 = (inst >> 20) & 0b11111;
					
					switch (cpu->funct3)
					{
						case 0b000:
						{
							if (cpu->imm == 0 && cpu->rs1 == 0 && cpu->rd == 0) { cpu->inst_id = INSTRUCTION_ECALL; return; }
							if (cpu->imm == 1 && cpu->rs1 == 0 && cpu->rd == 0) { cpu->inst_id = INSTRUCTION_EBREAK; return; }
						}

						case 0b001: cpu->inst_id = INSTRUCTION_CSRRW; return;
						case 0b010: cpu->inst_id = INSTRUCTION_CSRRS; return;
						case 0b011: cpu->inst_id = INSTRUCTION_CSRRC; return;

						case 0b101: cpu->imm = (cpu->inst >> 15) & 0x1F; cpu->inst_id = INSTRUCTION_CSRRWI; return;
						case 0b110: cpu->imm = (cpu->inst >> 15) & 0x1F; cpu->inst_id = INSTRUCTION_CSRRSI; return;
						case 0b111: cpu->imm = (cpu->inst >> 15) & 0x1F; cpu->inst_id = INSTRUCTION_CSRRCI; return;

						default: return;
					}
				} break;

				default: return;
			}
		} break;

		case INSTRUCTION_TYPE_S:
		{
			cpu->imm = s_imm(inst);

			switch (cpu->opcode)
			{
				case 0b0100011:
				{
					switch (cpu->funct3)
					{
						case 0b000: cpu->inst_id = INSTRUCTION_SB; return;
						case 0b001: cpu->inst_id = INSTRUCTION_SH; return;
						case 0b010: cpu->inst_id = INSTRUCTION_SW; return;

						default: return;
					}
				} break;

				default: return;
			}
		} break;

		case INSTRUCTION_TYPE_B:
		{
			cpu->imm = b_imm(inst);

			switch (cpu->opcode)
			{
				case 0b1100011:
				{
					switch (cpu->funct3)
					{
						case 0b000: cpu->inst_id = INSTRUCTION_BEQ; return;
						case 0b001: cpu->inst_id = INSTRUCTION_BNE; return;
						case 0b100: cpu->inst_id = INSTRUCTION_BLT; return;
						case 0b101: cpu->inst_id = INSTRUCTION_BGE; return;
						case 0b110: cpu->inst_id = INSTRUCTION_BLTU; return;
						case 0b111: cpu->inst_id = INSTRUCTION_BGEU; return;

						default: return;
					}
				} break;

				default: return;
			}
		} break;

		case INSTRUCTION_TYPE_U:
		{
			cpu->imm = u_imm(inst);

			switch (cpu->opcode)
			{
				case 0b0010111: cpu->inst_id = INSTRUCTION_AUIPC; return;
				case 0b0110111: cpu->inst_id = INSTRUCTION_LUI; return;

				default: return;
			}
		} break;

		case INSTRUCTION_TYPE_J:
		{
			cpu->imm = j_imm(inst);

			switch (cpu->opcode)
			{
				case 0b1101111: cpu->inst_id = INSTRUCTION_JAL; break;
			}
		} break;
	}
}

void* bus_get_pointer(bus_t* bus, bus_device_t** pdevice, u32 address, u32 size)
{
	for (u32 i = 0; i < BUS_MAX_DEVICES; i += 1)
	{
		bus_device_t* device = &bus->devices[i];

		if (address < device->region.begin ||
			(address + size) >= device->region.end)
		{
			continue;
		}

		*pdevice = device;

		const size_t offset = address - device->region.begin;

		return device->memory + offset;
	}

	printf("Invalid address 0x%08X (size: %u)\n", address, size);
	*pdevice = NULL;
	return NULL;
}

u8 bus_read_8(bus_t* bus, cpu_t* cpu, u32 address)
{
	bus_device_t* device;
	void* ptr = bus_get_pointer(bus, &device, address, 1);
	if (ptr == NULL)
	{
		// TODO: handle invalid address
		unreachable;
		return 0;
	}

	if ((device->access_flags & ACCESS_FLAG_READ) == 0) { unreachable; }
	if (device->read_delay > 0) { SDL_Delay(device->read_delay); }

	return *(u8*)ptr;
}

u16 bus_read_16(bus_t* bus, cpu_t* cpu, u32 address)
{
	bus_device_t* device;
	void* ptr = bus_get_pointer(bus, &device, address, 2);
	if (ptr == NULL)
	{
		// TODO: handle invalid address
		unreachable;
		return 0;
	}
	
	if ((device->access_flags & ACCESS_FLAG_READ) == 0) { unreachable; }
	if (device->read_delay > 0) { SDL_Delay(device->read_delay); }

	return *(u16*)ptr;
}

u32 bus_read_32(bus_t* bus, cpu_t* cpu, u32 address)
{
	#if VM_DEBUG_INSTRUCTIONS
	printf("  bus_read_32(0x%08X)\n", address);
	#endif
	
	bus_device_t* device;
	void* ptr = bus_get_pointer(bus, &device, address, 4);
	if (ptr == NULL)
	{
		// TODO: handle invalid address

		printf("bus_read_32(0x%08X)\n", address);
		printf("pc: 0x%08X\n", cpu->pc);
		
		unreachable;
		return 0;
	}

	if ((device->access_flags & ACCESS_FLAG_READ) == 0) { unreachable; }
	if (device->read_delay > 0) { SDL_Delay(device->read_delay); }

	return *(u32*)ptr;
}

void bus_write_8(bus_t* bus, cpu_t* cpu, u32 address, u8 value)
{
	bus_device_t* device;
	void* ptr = bus_get_pointer(bus, &device, address, 1);
	if (ptr == NULL)
	{
		// TODO: handle invalid address

		unreachable;
		return;
	}
	
	if ((device->access_flags & ACCESS_FLAG_WRITE) == 0) { unreachable; }
	if (device->write_delay > 0) { SDL_Delay(device->read_delay); }

	*(u8*)ptr = value;
}

void bus_write_16(bus_t* bus, cpu_t* cpu, u32 address, u16 value)
{
	bus_device_t* device;
	void* ptr = bus_get_pointer(bus, &device, address, 2);
	if (ptr == NULL)
	{
		// TODO: handle invalid address
		unreachable;
		return;
	}
	
	if ((device->access_flags & ACCESS_FLAG_WRITE) == 0) { unreachable; }
	if (device->write_delay > 0) { SDL_Delay(device->read_delay); }

	*(u16*)ptr = value;
}

void bus_write_32(bus_t* bus, cpu_t* cpu, u32 address, u32 value)
{
	#if VM_DEBUG_INSTRUCTIONS
	printf("  bus_write_32(0x%08X, %i(0x%X))\n", address, (i32)value, value);
	#endif

	bus_device_t* device;
	void* ptr = bus_get_pointer(bus, &device, address, 4);
	if (ptr == NULL)
	{
		// TODO: handle invalid address

		printf("bus_write_32(0x%08X, 0x%X)\n", address, value);
		printf("pc: 0x%08X\n", cpu->pc);

		unreachable;
		return;
	}
	
	if ((device->access_flags & ACCESS_FLAG_WRITE) == 0)
	{
		printf("device: %s\n", device->label);
		printf("sp: 0x%08X\n", cpu->x[2]);
		printf("pc: 0x%08X\n", cpu->pc);
		unreachable;
	}
	if (device->write_delay > 0) { SDL_Delay(device->read_delay); }

	*(u32*)ptr = value;
}

void ppu_init(ppu_t* ppu)
{
	// Create color lookup table
	{
		u8* rgba = (u8*)ppu->color_lookup;

		for (size_t i = 0; i < PPU_COLOR_COUNT; i += 1)
		{
			const u8 r5 = i & 0x1F;
			const u8 g5 = (i >> 5) & 0x1F;
			const u8 b5 = (i >> 10) & 0x1F;

			const u8 r8 = (u8)(((f32)r5 / 31.0) * 255.0);
			const u8 g8 = (u8)(((f32)g5 / 31.0) * 255.0);
			const u8 b8 = (u8)(((f32)b5 / 31.0) * 255.0);

			rgba[0] = r8;
			rgba[1] = g8;
			rgba[2] = b8;
			rgba[3] = 255;

			rgba += 4;
		}
	}

	ppu->clip_rect = (SDL_Rect){ 0, 0, PPU_SCREEN_WIDTH, PPU_SCREEN_HEIGHT };
	ppu->current_offset = 0;
}

u16 ppu_pget(ppu_t* ppu, i32 x, i32 y)
{
	const i32 index = x + y * PPU_SCREEN_WIDTH;
	if (index < 0 || index >= PPU_SCREEN_TOTAL_PIXELS)
	{
		return 0;
	}

	return ppu->colors[index];
}

void ppu_pset(ppu_t* ppu, i32 x, i32 y, u16 color)
{
	assert(ppu->pixels != NULL);

	const i32 index = x + y * PPU_SCREEN_WIDTH;
	if (index < 0 || index >= PPU_SCREEN_TOTAL_PIXELS)
	{
		return;
	}
	
	ppu->colors[index] = color;
	ppu->pixels[index] = ppu->color_lookup[color];
}

static inline void ppu_unpack_color(u16 color5, u32* color24)
{
	const u8 r5 = color5 & 0x1F;
	const u8 g5 = (color5 >> 5) & 0x1F;
	const u8 b5 = (color5 >> 10) & 0x1F;

	((u8*)color24)[0] = ((f32)r5 / 31.0) * 255.0;
	((u8*)color24)[1] = ((f32)g5 / 31.0) * 255.0;
	((u8*)color24)[2] = ((f32)b5 / 31.0) * 255.0;

	printf("(%u, %u, %u) (%u, %u, %u)\n",
		r5, g5, b5,
		((u8*)color24)[0],
		((u8*)color24)[1],
		((u8*)color24)[2]);
}

void ppu_pal(ppu_t* ppu, u16 c0, u16 c1, u16 c2, u16 c3)
{
	ppu->palette[0] = c0;
	ppu->palette[1] = c1;
	ppu->palette[2] = c2;
	ppu->palette[3] = c3;
}

void ppu_cls(ppu_t* ppu, u16 color)
{
	for (size_t i = 0; i < PPU_SCREEN_TOTAL_PIXELS; i += 1)
	{
		ppu->colors[i] = color;
		ppu->pixels[i] = ppu->color_lookup[color];
	}
}

void ppu_clip(ppu_t* ppu, i32 x, i32 y, i32 w, i32 h)
{
	ppu->clip_rect = (SDL_Rect){ x, y, w, h };
}

void ppu_rect(ppu_t* ppu, i32 x, i32 y, i32 w, i32 h, u16 color)
{
	const i32 x0 = x;
	const i32 y0 = y;
	const i32 x1 = x + w;
	const i32 y1 = y + h;

	ppu_line(ppu, x0, y0, x0, y1, color);
	ppu_line(ppu, x0, y1, x1, y1, color);
	ppu_line(ppu, x1, y1, x1, y0, color);
	ppu_line(ppu, x1, y0, x0, y0, color);
}

void ppu_rectfill(ppu_t* ppu, i32 x, i32 y, i32 w, i32 h, u16 color)
{
	x += ppu->offset.x;
	y += ppu->offset.y;

	i32 x0 = clamp(x, 0, PPU_SCREEN_WIDTH);
	i32 y0 = clamp(y, 0, PPU_SCREEN_HEIGHT);
	i32 x1 = clamp(x + w, 0, PPU_SCREEN_WIDTH);
	i32 y1 = clamp(y + h, 0, PPU_SCREEN_HEIGHT);

	for (i32 py = y0; py < y1; py += 1)
	{
		for (i32 px = x0; px < x1; px += 1)
		{
			ppu_pset(ppu, px, py, color);
		}
	}
}

void ppu_line(ppu_t* ppu, i32 x0, i32 y0, i32 x1, i32 y1, u16 color)
{
	x0 += ppu->offset.x;
	y0 += ppu->offset.y;
	x1 += ppu->offset.x;
	y1 += ppu->offset.y;

	i32 dx = abs(x1 - x0);
	i32 dy = abs(y1 - y0);
	i32 sx = x0 < x1 ? 1 : -1;
	i32 sy = y0 < y1 ? 1 : -1;
	i32 err = dx - dy;

	while (1)
	{
		ppu_pset(ppu, x0, y0, color);

		if (x0 == x1 && y0 == y1) { break; }

		i32 e2 = err * 2;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

void ppu_spr_data(ppu_t* ppu, void* data, u32 num_sprites)
{
	ppu->sprites = data;
	ppu->num_sprites = num_sprites;
}

void ppu_spr(ppu_t* ppu, i32 n, i32 x, i32 y, u32 bits)
{
	if (ppu->sprites == NULL) { return; }

	x += ppu->offset.x;
	y += ppu->offset.y;

	assert(n < 256);
	if ((x + 8) < 0) { return; }
	if ((y + 8) < 0) { return; }
	if (x >= PPU_SCREEN_WIDTH) { return; }
	if (y >= PPU_SCREEN_HEIGHT) { return; }

	u8* sprite = &ppu->sprites[n * 16];
	u32 bit_offset = 0;

	for (u32 oy = 0; oy < 8; oy += 1)
	{
		for (u32 ox = 0; ox < 8; ox += 1)
		{
			const u32 byte_index = bit_offset / 8;
			const u32 bit_index = bit_offset % 8;

			const u8 byte = sprite[byte_index];
			const u8 shift = bit_index;
			const u8 value = (byte >> shift) & 0x3;

			bit_offset += 2;

			if (value == 0b00) { continue; }

			const u16 color = ppu->palette[value];

			const u32 dst_x = x + ox;
			const u32 dst_y = y + oy;
			if (dst_x >= PPU_SCREEN_WIDTH) { continue; }
			if (dst_y >= PPU_SCREEN_HEIGHT) { continue; }
			ppu_pset(ppu, dst_x, dst_y, color);
		}
	}
}

void ppu_tile(ppu_t* ppu, i32 n, i32 x, i32 y, u32 bits)
{
	if (ppu->sprites == NULL) { return; }

	x += ppu->offset.x;
	y += ppu->offset.y;

	assert(n < 256);
	if ((x + 8) < 0) { return; }
	if ((y + 8) < 0) { return; }
	if (x >= PPU_SCREEN_WIDTH) { return; }
	if (y >= PPU_SCREEN_HEIGHT) { return; }
	
	for (u32 oy = 0; oy < 8; oy += 1)
	{
		for (u32 ox = 0; ox < 8; ox += 1)
		{
			const u32 bit_offset = n * 2;
			const u32 byte_index = bit_offset / 8;
			const u32 bit_index = bit_offset % 8;

			const u8 byte = ppu->sprites[byte_index];
			const u8 shift = bit_index;
			const u8 value = (byte >> shift) & 0x3;

			const u16 color = ppu->palette[value];

			const u32 dst_x = x + ox;
			const u32 dst_y = y + oy;
			if (dst_x >= PPU_SCREEN_WIDTH) { continue; }
			if (dst_y >= PPU_SCREEN_HEIGHT) { continue; }
			ppu_pset(ppu, dst_x, dst_y, color);
		}
	}
}

void ppu_push_offset(ppu_t* ppu, i32 x, i32 y)
{
	assert(ppu != NULL);

	if (ppu->current_offset >= PPU_MAX_OFFSETS)
	{
		return;
	}
	
	ppu->offset.x += x;
	ppu->offset.y += y;

	ppu->current_offset += 1;
	ppu->offsets[ppu->current_offset] = (offset_t){ x, y };
}

void ppu_pop_offset(ppu_t* ppu)
{
	if (ppu->current_offset <= 0)
	{
		return;
	}

	ppu->offset.x -= ppu->offsets[ppu->current_offset].x;
	ppu->offset.y -= ppu->offsets[ppu->current_offset].y;
	ppu->current_offset -= 1;
}

void ppu_get_offset(ppu_t* ppu, i32* x, i32* y)
{
	*x = ppu->offset.x;
	*y = ppu->offset.y;
}

bool is_key_down(vm_t* vm, SDL_Scancode scancode)
{
	return vm->input.key_states[scancode].is_down;
}

bool is_key_pressed(vm_t* vm, SDL_Scancode scancode)
{
	return vm->input.key_states[scancode].is_down &&
		vm->input.key_states[scancode].tick_down == vm->time.ticks;
}

bool is_gamepad_button_down(vm_t* vm, SDL_GamepadButton button)
{
	return vm->input.gamepad_button_states[button].is_down;
}

bool is_gamepad_button_pressed(vm_t* vm, SDL_GamepadButton button)
{
	return vm->input.gamepad_button_states[button].is_down &&
		vm->input.gamepad_button_states[button].tick_down == vm->time.ticks;
}

