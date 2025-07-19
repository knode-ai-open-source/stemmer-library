# stemmer-library

Lightweight C stemming library (Apache-2.0) with an optional fixed-size cache and Snowball-generated English stemmer. Designed for embedding in search / NLP pipelines where minimal dependencies, predictable memory usage, and UTF-8 processing are required.

## Features

* English stemming (aliases: `en`, `eng`, `english`).
* UTF-8 input (`symbol` typedef = `unsigned char`).
* High-level opaque `stemmer_t` API (simple: init → stem → destroy).
* Optional tiny LRU-like string cache (keys & values ≤ 24 bytes) for repeated words.
* Lower-level Snowball environment (`struct SN_env`) and helper routines exposed under `impl/` for adding languages.
* Predictable memory model; you control cache capacity.

## Quick Start

```c
#include "stemmer-library/stemmer.h"

int main(void) {
    stemmer_t *st = stemmer_init(4096); // cache capacity (entries)
    char buf[64];
    const char *v = stem_cache_get(c, "running");
stem_cache_destroy(c);
```

## API Summary

### `stemmer.h`

* `stemmer_t * stemmer_init(size_t cache_capacity);`
* `void stemmer_destroy(stemmer_t *);`
* `const char * stemmer_stem(stemmer_t *, const char *word, char *result);`

    * `result` must provide **≥ 64 bytes**. Words > 63 bytes are returned unchanged (pointer may equal input).

### `stem_cache.h`

* `stem_cache_t * stem_cache_init(size_t capacity);`
* `void stem_cache_destroy(stem_cache_t *);`
* `const char * stem_cache_get(stem_cache_t *, const char *key);`
* `void stem_cache_put(stem_cache_t *, const char *key, const char *value);`
* Limits: `MAX_KEY_LEN = 24`, `MAX_VALUE_LEN = 24` (longer strings must be truncated or avoided by caller).

### `impl/`

Snowball plumbing (`struct SN_env`, grouping / matching helpers, slice/replace utilities, module registration, and the generated English algorithm header. Extend by generating additional Snowball modules and adding them to a new `modules_utf8.h` variant (or regenerating the existing list).

## Limits & Notes

* **Buffer size:** Provide 64-byte buffer for `stemmer_stem`. Oversized tokens are passed through.
* **Cache:** Fixed number of entries (simple strategy—inspect source to adjust policy if needed). Keys/values > 24 bytes are not supported.
* **Encoding:** Only UTF-8 currently enumerated (`ENC_UTF_8`).
* **Thread safety:** A `stemmer_t` is not guaranteed thread-safe; use one instance per thread or add your own locking. Read-only lookups after construction are safe per-instance.
* **Memory:** Deterministic: primary allocations occur at init; per-call uses caller-provided buffer + cache.
* **Error handling:** Functions returning pointers yield `NULL` on failure (e.g., allocation). Caller should check.

## Extending Languages

1. Use the Snowball compiler to generate `<language>_UTF_8_*.c/h`.
2. Add create/close/stem prototypes similar to English module.
3. Add entries to `modules[]` (name + aliases) and `algorithm_names[]`.

## Minimal Build Sketch (example)

```bash
gcc -Iinclude -c src/english_UTF_8.c -o english_UTF_8.o
gcc -Iinclude -c src/stemmer.c -o stemmer.o
gcc english_UTF_8.o stemmer.o -o demo
```

(Integrate with your build system / add other language objects as needed.)

## License

Apache-2.0

SPDX headers are included in source. See `LICENSE` file (add it if missing).

## Maintainer

Andy Curtis [contactandyc@gmail.com](mailto:contactandyc@gmail.com)
