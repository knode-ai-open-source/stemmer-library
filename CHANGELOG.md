## 08/22/2025

**Modernize Stemmer Library build system, licensing, and tests**

---

## Summary

This PR refactors the **Stemmer Library** into a modern, variant-aware CMake project with unified build tooling, simplified licensing headers, updated documentation, and a new test/coverage framework.

---

## Key Changes

### 🔧 Build & Tooling

* **Removed** legacy script: `build_install.sh`.
* **Added** `build.sh`:

    * Commands: `build`, `install`, `coverage`, `clean`.
    * Coverage with `llvm-cov`.
* `.gitignore`: added `build-unix_makefiles`, `build-cov`, `build-coverage`.
* **BUILDING.md**:

    * Updated for `Stemmer Library v0.0.1`.
    * Clear local build instructions with `build.sh`.
    * Explicit dependency setup (`a-memory-library`, `the-macro-library`).
    * Optional Dockerfile build.
* **Dockerfile**:

    * Ubuntu base with configurable CMake version.
    * Non-root `dev` user.
    * Builds `a-memory-library`, `the-macro-library`, and this project.

### 📦 CMake

* Raised minimum CMake to **3.20**.
* Project renamed to `stemmer_library` (underscore convention).
* **Multi-variant build**:

    * `debug`, `memory`, `static`, `shared`.
    * Umbrella alias: `stemmer_library::stemmer_library`.
* Coverage toggle (`A_ENABLE_COVERAGE`) and memory profiling define (`_AML_DEBUG_`).
* Dependencies:

    * `a_memory_library`, `the_macro_library`.
* Exported package config:

    * Generates `stemmer_libraryConfig.cmake` + version file.
    * Proper namespace: `stemmer_library::`.

### 📖 Documentation

* **AUTHORS**: updated Andy Curtis entry with GitHub profile.
* **NOTICE**: simplified attribution:

    * Explicitly credits Andy Curtis (2025) and Knode.ai (2024–2025).
    * Snowball code licensing note removed from notice (covered in LICENSE).
* Added GitHub Pages support:

    * `_config.yml` (Jekyll theme + site title).
    * `_layouts/default.html` (basic layout with GitHub edit link).

### 📝 Source & Headers

* SPDX headers standardized:

    * En-dash years (`2024–2025`).
    * Andy Curtis credited explicitly for 2025.
    * Knode.ai marked with “technical questions” contact.
* Removed redundant `Maintainer:` comment lines.
* Cleanups:

    * ANSI-C compliance fixes.
    * Trimmed stray trailing lines in generated code.

### ✅ Tests

* `tests/` refactored into modern CMake:

    * One main executable: `test_stemmer`.
    * Links against `stemmer_library::stemmer_library`.
* `tests/build.sh` added:

    * Supports variants and coverage builds.
* Coverage target generates HTML + console report.
* `test_stemmer.c`:

    * Updated to new API (`stemmer_init(size_t cache_size)` instead of pool-based init).
    * Verifies stemming logic and cache behavior.

---

## Impact

* 🚀 Easier builds: one script, consistent CMake variants, Docker support.
* 🛡️ Clearer and legally consistent SPDX + NOTICE headers.
* 📖 Developer docs modernized.
* ✅ Stronger test coverage and automation readiness.
