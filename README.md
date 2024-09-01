# What is this?

Very basic, simplistic arena allocator for personal use.

Arena struct contains a linked list of regions. Each region is basically an allocation call. 
The memory is divided into regions to support extending the region once capacity is reached.

Supports `mmap` and `malloc`.

# Basic usage example:
#### Single element:
```C
Arena arena = {0};

int* test = arena_create(&a, sizeof(int));
*test = 12345;
```

#### Array:
```C
Arena arena = {0};

int* test = arena_alloc(&a, sizeof(int), 50);
test[2] = 12345;
```

