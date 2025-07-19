# SPDX-FileCopyrightText: 2024-2025 Knode.ai
# SPDX-License-Identifier: Apache-2.0
# Maintainer: Andy Curtis <contactandyc@gmail.com>
cmake_minimum_required(VERSION 3.10)

# Project Configuration
project(stemmer-library VERSION 2.2.0)

# Variables
set(INCLUDE_DIR_NAME "stemmer-library")
set(EXTRA_FILES COPYING)

# Source files
file(GLOB SOURCE_FILES src/*.c)

set(CUSTOM_PACKAGES a-memory-library the-macro-library)
find_package(a-cmake-library REQUIRED)

include(LibraryConfig)
include(LibraryBuild)

# Testing
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
