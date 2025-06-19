#pragma once

#include "language_layer.h"

typedef struct
{
	u8* memory;
	size_t used;
	size_t capacity;
} memory_arena_t;

void memory_arena_init(memory_arena_t* arena, void* memory, size_t size);
void* memory_arena_push(memory_arena_t* arena, size_t size);
void* memory_arena_push_zero(memory_arena_t* arena, size_t size);
void* memory_arena_pop(memory_arena_t* arena, size_t size);
void memory_arena_clear(memory_arena_t* arena);
size_t memory_arena_save(memory_arena_t* arena);
void memory_arena_restore(memory_arena_t* arena, size_t size);
void* memory_arena_peek(memory_arena_t* arena, size_t size);
