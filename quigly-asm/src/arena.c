#include "arena.h"
#include <string.h>

void memory_arena_init(memory_arena_t* arena, void* memory, size_t size)
{
	assert(arena != NULL);
	assert(memory != NULL);
	assert(size > 0);

	arena->memory = memory;
	arena->capacity = size;
	arena->used = 0;
}

void* memory_arena_push(memory_arena_t* arena, size_t size)
{
	assert(arena != NULL);
	assert(arena->memory != NULL);

	if (arena->used + size <= arena->capacity)
	{
		void* memory = arena->memory + arena->used;
		arena->used += size;
		return memory;
	}

	printf("Failed to allocate %lu bytes in arena with capacity %lu\n", size, arena->capacity);
	return NULL;
}

void* memory_arena_push_zero(memory_arena_t* arena, size_t size)
{
	void* memory = memory_arena_push(arena, size);
	if (memory == NULL) { return NULL; }

	memset(memory, 0, size);

	return memory;
}

void* memory_arena_pop(memory_arena_t* arena, size_t size)
{
	assert(arena != NULL);
	if (arena->used < 1) { return NULL; }

	arena->used -= size;
	
	return arena->memory + arena->used;
}

void memory_arena_clear(memory_arena_t* arena)
{
	assert(arena != NULL);

	arena->used = 0;
}

size_t memory_arena_save(memory_arena_t* arena)
{
	assert(arena != NULL);

	return arena->used;
}

void memory_arena_restore(memory_arena_t* arena, size_t size)
{
	assert(arena != NULL);

	arena->used = size;
}

void* memory_arena_peek(memory_arena_t* arena, size_t size)
{
	assert(arena != NULL);
	assert(arena->memory != NULL);

	if (arena->used < 1) { return NULL; }

	return arena->memory + arena->used - size;
}
