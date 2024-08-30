#ifndef a_arena
#define a_arena

#include <stdint.h>
#include <sys/mman.h>

// TODO: remove
#include <stdio.h>

// TODO: linked list of mmap allocated regions for expansion?

typedef struct Arena
{
	// TODO: Can I reach start from position + size on free?
	// That way I can avoid storing start.
	void* start;
	uintptr_t position;
	size_t capacity;
	size_t size;
} Arena;

#define DEFAULT_CAP 1024 * 8

void arena_ensure_capacity(Arena* arena, const size_t alloc_size)
{
	const size_t arena_size    = arena->size;
	const size_t next_position = arena_size + alloc_size;

	if (next_position > arena_size) {
		// TODO
	}
}

Arena arena_new()
{
	void* ptr = mmap(NULL, DEFAULT_CAP, PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	if (ptr == MAP_FAILED) {
		// TODO
	}

	Arena arena = {
		.size = 0,
		.capacity = DEFAULT_CAP,
		.start = ptr,
		.position = (uintptr_t) ptr,
	};

	return arena;
}

void* arena_alloc(Arena* arena, size_t size, size_t count)
{
	// TODO
	return NULL;
}

void* arena_create(Arena* arena, size_t size) 
{
	arena_ensure_capacity(arena, size);
	
	arena->size += size;
	return (void*) arena->position;
}


void arena_free(Arena* arena)
{
	munmap(arena->start, arena->capacity);
}

#endif

