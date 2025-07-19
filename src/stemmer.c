// SPDX-FileCopyrightText: 2024-2025 Knode.ai
// SPDX-License-Identifier: Apache-2.0
// Maintainer: Andy Curtis <contactandyc@gmail.com>
#include <string.h>
#include <stdlib.h>
#include "stemmer-library/stemmer.h"
#include "stemmer-library/stem_cache.h"
#include "stemmer-library/impl/libstemmer.h"
#include "stemmer-library/impl/header.h"
#include "a-memory-library/aml_alloc.h"

/* Internal stemmer structure */
struct stemmer {
    stem_cache_t *cache;
    struct sb_stemmer *env;    /* Stemmer environment */
    char buffer[64];     /* Fixed-size buffer for stemming operations */
};

stemmer_t *stemmer_init(size_t cache_capacity) {
    stemmer_t *stem = (stemmer_t *)aml_zalloc(sizeof(*stem));
    if (!stem) {
        return NULL;
    }

    /* Initialize the stemmer environment */
    stem->env = sb_stemmer_new("english", NULL);
    if (!stem->env) {
        return NULL;
    }

    /* Initialize the cache if capacity > 0 */
    if (cache_capacity > 0) {
        stem->cache = stem_cache_init(cache_capacity);
        if (!stem->cache) {
            sb_stemmer_delete(stem->env);
            return NULL;
        }
    } else {
        stem->cache = NULL;
    }
    
    return stem;
}

void stemmer_destroy(stemmer_t *stem) {
    if (!stem) return;

    if (stem->env) {
        sb_stemmer_delete(stem->env);
    }
    stem_cache_destroy(stem->cache);
}

const char *stemmer_stem(stemmer_t *stem, const char *word, char *result) {
    if (!stem || !word || !result) {
        return NULL;
    }

    size_t word_len = strlen(word);
    if (word_len >= 64) {
        /* If word is too long, return the original word */
        return word;
    }

    /* Check cache first if enabled */
    if (stem->cache && word_len < MAX_KEY_LEN) {
        const char *cached_result = stem_cache_get(stem->cache, word);
        if (cached_result)
			return cached_result;
    }

	const sb_symbol * stemmed = sb_stemmer_stem(stem->env, (const sb_symbol *)word, word_len);
	if (stemmed == NULL)
		return word;

	/* Copy result */
	const char *stemmed_word = (const char *)stemmed;
	size_t stem_len = strlen(stemmed_word);
	if (stem_len >= 64)
		return word;

	strcpy(result, stemmed_word);
	if(stem->cache && stem_len < MAX_VALUE_LEN)
		stem_cache_put(stem->cache, word, result);
	return result;
}
