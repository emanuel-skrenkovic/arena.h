#ifndef a_arena
#define a_arena

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>


#define DEFAULT_CAP 1024 * 4

typedef struct Region Region;

// Region contains the allocated region of memory. Each region matches a single
// allocation call.
struct Region
{
	Region* next;
	size_t capacity;
	size_t size;
	uintptr_t data[];
};

// Arena is the collection of memory regions. Each region is separately allocated,
// and serve the purpose of extending the memory once a region is full.
// Arena is a linked list of memory regions.
// The entire arena is deallocated in a single call, regions remain allocated as
// long as the arena is valid.
typedef struct
{
	Region* regions;
	Region* current;
} Arena;

Region* region_new(void);
void region_free(Region* region);

void* arena_create(Arena* arena, size_t size);
void* arena_alloc(Arena* arena, size_t size, size_t count);
void arena_free(Arena* arena);

#define ALLOCATOR_MALLOC 0
#define ALLOCATOR_MMAP 1

#define ALLOCATOR ALLOCATOR_MALLOC



#if ALLOCATOR == ALLOCATOR_MMAP

Region* region_new(void)
{
	size_t bytes = sizeof(Region) + sizeof(uintptr_t) * DEFAULT_CAP;
	Region* region = (Region*) mmap(NULL, bytes, PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);

	if (region == MAP_FAILED) {
		return NULL;
	}

	region->capacity = DEFAULT_CAP;
	region->size     = 0;
	
	return region;
}

void region_free(Region* region)
{
	munmap(region, region->capacity);
}

#elif ALLOCATOR == ALLOCATOR_MALLOC

#include <stdlib.h>

Region* region_new(void)
{
	size_t bytes = sizeof(Region) + sizeof(uintptr_t) * DEFAULT_CAP;
	Region* region = (Region*) malloc(bytes);

	if (region == MAP_FAILED) {
		return NULL;
	}

	region->capacity = DEFAULT_CAP;
	region->size     = 0;
	
	return region;
}

void region_free(Region* region)
{
	free(region);
}

#endif



void* arena_create(Arena* arena, size_t size) 
{
	assert(size > 0);

	if (arena->regions == NULL) {
		Region* region = region_new();

		if (region == NULL) {
			return NULL;
		}

		arena->regions = region;
		arena->current = region;
	}

	if (size > arena->current->capacity) {
		return NULL;
	}

	if (size + arena->current->size >= arena->current->capacity) {
		Region* new_region   = region_new();
		arena->current->next = new_region;
		arena->current       = new_region;
	}

	void* ptr = &arena->current->data[arena->current->size];
	arena->current->size += (size + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);

	return ptr;
}

void* arena_alloc(Arena* arena, size_t size, size_t count)
{
	return arena_create(arena, size * count);
}

void arena_free(Arena* arena)
{
	Region* region = arena->regions;

	while (region != NULL) {
		Region* current = region;
		region          = region->next;

		region_free(current);
	}
}

#endif

