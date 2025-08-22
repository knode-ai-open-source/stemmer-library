// SPDX-FileCopyrightText: 2025 Andy Curtis <contactandyc@gmail.com>
// SPDX-FileCopyrightText: 2024–2025 Knode.ai — technical questions: contact Andy (above)
// SPDX-License-Identifier: Apache-2.0

#ifndef STEM_CACHE_H
#define STEM_CACHE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include "a-memory-library/aml_pool.h"

/* Maximum lengths for cache entries */
#define MAX_KEY_LEN   24
#define MAX_VALUE_LEN 24

/* Forward declaration of the cache type */
struct stem_cache_s;
typedef struct stem_cache_s stem_cache_t;

/* Create a new stem cache with the specified capacity */
stem_cache_t *stem_cache_init(size_t capacity);

void stem_cache_destroy(stem_cache_t *cache);

/* Get a value from the cache by key */
const char *stem_cache_get(stem_cache_t *cache, const char *key);

/* Put a value into the cache */
void stem_cache_put(stem_cache_t *cache, const char *key, const char *value);

#ifdef __cplusplus
}
#endif

#endif /* STEM_CACHE_H */ 
