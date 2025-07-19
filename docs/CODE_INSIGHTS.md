# libstemmer_c Code Insights

## Overview
libstemmer_c is a C library that provides word stemming functionality, specifically for English text. It uses the Snowball stemming algorithm and includes a caching mechanism for improved performance.

## Required Files for English Stemming
The library can be reduced to English-only functionality by keeping only these files:

### Include Files
- `stemmer.h` - Main API for stemming operations
- `stem_cache.h` - Cache interface
- `stem_UTF_8_english.h` - English stemmer interface
- `header.h` - Core Snowball definitions

### Source Files
- `stemmer.c` - Main implementation
- `stem_cache.c` - Cache implementation
- `stem_UTF_8_english.c` - English stemmer implementation

## Key Components

### Stemmer (`stemmer.h`, `stemmer.c`)
- Opaque type `stemmer_t` that encapsulates the stemming functionality
- Uses a fixed-size buffer (64 bytes) for stemming operations
- Words longer than 64 bytes are not stemmed and returned as-is
- Integrates with a-memory-library for memory management
- Supports optional caching of stemmed results

### Stem Cache (`stem_cache.h`, `stem_cache.c`)
- LRU (Least Recently Used) cache implementation
- Uses the-macro-library for map operations
- Fixed-size entries:
  - Keys: max 24 bytes
  - Values: max 24 bytes
- Memory managed through a-memory-library pool
- Thread-safe design with atomic operations

## Memory Management
- Uses a-memory-library for memory allocation
- Pool-based memory management
- No explicit memory freeing - memory is managed by the pool
- Cache nodes are allocated from the pool
- Stemmer structure and buffer are allocated from the pool

## Key Design Decisions
1. **Fixed Buffer Size**
   - 64-byte buffer for stemming operations
   - Words longer than 64 bytes are not stemmed
   - Prevents buffer overflows and simplifies memory management

2. **Caching Strategy**
   - LRU cache for frequently stemmed words
   - Fixed-size entries to prevent memory fragmentation
   - Optional caching (capacity = 0 disables cache)

3. **Memory Management**
   - Pool-based allocation for predictable memory usage
   - No explicit freeing - memory managed by pool
   - Cache maintains its own pool reference

4. **Error Handling**
   - Returns original word on stemming failures
   - Returns NULL for invalid inputs
   - Graceful handling of long words

## Usage Example
```c
aml_pool_t *pool = aml_pool_init(1024);  // Initialize memory pool
stemmer_t *stem = stemmer_init(pool, 1000);  // Initialize stemmer with 1000-entry cache
char result[64];
const char *stemmed = stemmer_stem(stem, "running", result);  // Returns "run"
stemmer_destroy(stem);
```

## Dependencies
- a-memory-library: Memory management
- the-macro-library: Map operations for cache
- Snowball stemming algorithm implementation

## Performance Considerations
- Fixed-size buffer prevents dynamic allocation during stemming
- LRU cache reduces repeated stemming operations
- Pool-based memory management reduces fragmentation
- Thread-safe design allows concurrent access

## Limitations
- Maximum word length: 64 bytes
- Maximum cache entry size: 24 bytes for key and value
- English language only
- No support for stemming customization 