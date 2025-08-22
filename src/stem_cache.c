// SPDX-FileCopyrightText: 2025 Andy Curtis <contactandyc@gmail.com>
// SPDX-FileCopyrightText: 2024–2025 Knode.ai — technical questions: contact Andy (above)
// SPDX-License-Identifier: Apache-2.0

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "the-macro-library/macro_map.h"
#include "stemmer-library/stem_cache.h"

// Define the cache node type with embedded link fields.
typedef struct stem_cache_node_s {
    macro_map_t key_map;  // Link for insertion/find by key.
    macro_map_t time_map; // Link for insertion/find by time.
    char key[MAX_KEY_LEN];       // Fixed array for the key.
    char value[MAX_VALUE_LEN];   // Fixed array for the stem (value).
    time_t last_used;
} stem_cache_node_t;

// Define the cache container using pointers for the maps.
struct stem_cache_s {
    macro_map_t *key_map;  // Map keyed by string key.
    macro_map_t *time_map; // Map keyed by last_used timestamp.
    size_t capacity;
    size_t count;
    aml_pool_t *pool;      // Memory pool for allocations
};

// --- Comparator Functions ---

// Compare nodes by key string.
static inline int stem_key_insert_compare(const stem_cache_node_t *a,
                                            const stem_cache_node_t *b) {
    return strcmp(a->key, b->key);
}

static inline int stem_key_find_compare(const char *key,
                                        const stem_cache_node_t *node) {
    return strcmp(key, node->key);
}

// Compare nodes by last_used timestamp.
static inline int stem_time_insert_compare(const stem_cache_node_t *a,
                                           const stem_cache_node_t *b) {
    if (a->last_used < b->last_used) return -1;
    if (a->last_used > b->last_used) return 1;
    return 0;
}

static inline int stem_time_find_compare(const time_t *time_ptr,
                                         const stem_cache_node_t *node) {
    if (*time_ptr < node->last_used) return -1;
    if (*time_ptr > node->last_used) return 1;
    return 0;
}

// --- Macro Map Wrappers ---
// These wrappers assume that your macro_map library defines similar macros as before.

static inline
macro_map_find_kv(stem_cache_find_key, char, stem_cache_node_t, stem_key_find_compare);

static inline
macro_map_insert(stem_cache_insert_key, stem_cache_node_t, stem_key_insert_compare);

static inline
macro_map_find_kv_with_field(stem_cache_find_time, time_map, time_t, stem_cache_node_t, stem_time_find_compare);

static inline
macro_map_insert_with_field(stem_cache_insert_time, time_map, stem_cache_node_t, stem_time_insert_compare);

// --- Stem Cache Functions ---

// Create a stem cache with a given capacity.
// Allocates the cache structure and its maps from the provided memory pool.
stem_cache_t *stem_cache_init(size_t capacity) {
	if(capacity < 4096)
		capacity = 4096;

	aml_pool_t *pool = aml_pool_init(((capacity / 8) * sizeof(stem_cache_node_t)) + sizeof(stem_cache_t));
    stem_cache_t *cache = aml_pool_zalloc(pool, sizeof(stem_cache_t));
    /* Other members are zeroed or NULLed by aml_pool_zalloc */
    cache->capacity = capacity;
    cache->pool = pool;
    return cache;
}

// Destroy a stem cache and free all its memory.
void stem_cache_destroy(stem_cache_t *cache) {
	if (!cache) return;
	aml_pool_t *pool = cache->pool;
	aml_pool_destroy(pool);
}

// Insert a key-value pair into the cache.
// For a stemming library, words (keys) that are too long are not cached.
// If the key already exists, only its last_used timestamp is updated.
void stem_cache_put(stem_cache_t *cache, const char *key, const char *value) {
    // Only cache words shorter than MAX_KEY_LEN and MAX_VALUE_LEN.
    if (strlen(key) >= MAX_KEY_LEN || strlen(value) >= MAX_VALUE_LEN) {
        return;
    }
    time_t now = time(NULL);
    stem_cache_node_t *node = stem_cache_find_key(cache->key_map, key);
    if (node) {
        // Node exists: update its timestamp only.
        macro_map_erase(&cache->time_map, &node->time_map);
        node->last_used = now;
        stem_cache_insert_time(&cache->time_map, node);
        return;
    }
    // If the cache is full, reuse the least recently used node.
    if (cache->count >= cache->capacity) {
        // Assume macro_map_first returns a pointer to the first node in the time map.
        macro_map_t *first = macro_map_first(cache->time_map);
        if (first) {
            node = macro_parent_object(first, stem_cache_node_t, time_map);
            // Erase from both maps.
            macro_map_erase(&cache->key_map, (macro_map_t *)node);
            macro_map_erase(&cache->time_map, first);
            // Reuse the node by updating its key, value, and timestamp.
            strcpy(node->key, key);
            strcpy(node->value, value);
            node->last_used = now;
            // Reinsert into both maps.
            stem_cache_insert_key(&cache->key_map, node);
            stem_cache_insert_time(&cache->time_map, node);
            return;
        }
    }
    // Otherwise, create a new node.
    node = aml_pool_zalloc(cache->pool, sizeof(stem_cache_node_t));
    /* Initialize embedded map fields if required by your macro_map library */
    // For example: macro_map_node_init(&node->key_map); macro_map_node_init(&node->time_map);
    strcpy(node->key, key);   // Already checked for MAX_KEY_LEN.
    strcpy(node->value, value); // Already checked for MAX_VALUE_LEN.
    node->last_used = now;
    stem_cache_insert_key(&cache->key_map, node);
    stem_cache_insert_time(&cache->time_map, node);
    cache->count++;
}

// Retrieve a cached value by key and update its timestamp.
// Returns the stored value if found, or NULL if not present.
const char *stem_cache_get(stem_cache_t *cache, const char *key) {
    stem_cache_node_t *node = stem_cache_find_key(cache->key_map, key);
    if (!node) return NULL;
    // Update the timestamp.
    macro_map_erase(&cache->time_map, &node->time_map);
    node->last_used = time(NULL);
    stem_cache_insert_time(&cache->time_map, node);
    return node->value;
}
