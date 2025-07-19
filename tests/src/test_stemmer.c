// SPDX-FileCopyrightText: 2024-2025 Knode.ai
// SPDX-License-Identifier: Apache-2.0
// Maintainer: Andy Curtis <contactandyc@gmail.com>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "a-memory-library/aml_pool.h"
#include "stemmer-library/stemmer.h"

static void test_basic_stemming(void) {
    aml_pool_t *pool = aml_pool_init(1024);
    assert(pool != NULL);

    stemmer_t *stem = stemmer_init(pool, 1000);
    assert(stem != NULL);

    char result[64];
    const char *stemmed;

    // Test basic stemming
    stemmed = stemmer_stem(stem, "running", result);
    assert(stemmed != NULL);
    assert(strcmp(stemmed, "run") == 0);

    // Test word too long
    stemmed = stemmer_stem(stem, "thisisaverylongwordthatshouldnotbestemmedbecauseitislongerthansixtyfourbytes", result);
    assert(stemmed != NULL);
    assert(strcmp(stemmed, "thisisaverylongwordthatshouldnotbestemmedbecauseitislongerthansixtyfourbytes") == 0);

    // Test NULL inputs
    assert(stemmer_stem(NULL, "test", result) == NULL);
    assert(stemmer_stem(stem, NULL, result) == NULL);
    assert(stemmer_stem(stem, "test", NULL) == NULL);

    stemmer_destroy(stem);
    aml_pool_destroy(pool);
}

static void test_cache(void) {
    aml_pool_t *pool = aml_pool_init(1024);
    assert(pool != NULL);

    // Test with cache
    stemmer_t *stem = stemmer_init(pool, 1000);
    assert(stem != NULL);

    char result[64];
    const char *stemmed1, *stemmed2;

    // First stem should compute
    stemmed1 = stemmer_stem(stem, "running", result);
    assert(stemmed1 != NULL);
    assert(strcmp(stemmed1, "run") == 0);

    // Second stem should use cache
    stemmed2 = stemmer_stem(stem, "running", result);
    assert(stemmed2 != NULL);
    assert(strcmp(stemmed2, "run") == 0);

    stemmer_destroy(stem);

    // Test without cache
    stem = stemmer_init(pool, 0);
    assert(stem != NULL);

    stemmed1 = stemmer_stem(stem, "running", result);
    assert(stemmed1 != NULL);
    assert(strcmp(stemmed1, "run") == 0);

    stemmed2 = stemmer_stem(stem, "running", result);
    assert(stemmed2 != NULL);
    assert(strcmp(stemmed2, "run") == 0);

    stemmer_destroy(stem);
    aml_pool_destroy(pool);
}

int main(void) {
    printf("Running stemmer tests...\n");

    test_basic_stemming();
    printf("Basic stemming tests passed\n");

    test_cache();
    printf("Cache tests passed\n");

    printf("All tests passed!\n");
    return 0;
} 