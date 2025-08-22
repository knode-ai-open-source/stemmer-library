// SPDX-FileCopyrightText: 2025 Andy Curtis <contactandyc@gmail.com>
// SPDX-FileCopyrightText: 2024–2025 Knode.ai — technical questions: contact Andy (above)
// SPDX-License-Identifier: Apache-2.0

#ifndef STEMMER_H
#define STEMMER_H

/* Make header file work when included from C++ */
#ifdef __cplusplus
extern "C" {
#endif

#include "a-memory-library/aml_pool.h"

/* Opaque stemmer type */
typedef struct stemmer stemmer_t;

stemmer_t * stemmer_init(size_t cache_capacity);
void stemmer_destroy(stemmer_t *stem);

/* The result must be at least 64 bytes long. A word that is longer than 64 bytes will not be stemmed.
	In this case the return value will just be the input word as opposed to result.
*/
const char * stemmer_stem(stemmer_t *stem, const char *word, char *result);

#ifdef __cplusplus
}
#endif

#endif /* STEMMER_H */
