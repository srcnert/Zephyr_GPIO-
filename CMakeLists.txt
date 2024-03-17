# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(gpio_prj)

FILE(GLOB app_sources
    src/*.cpp
)

target_sources(app PRIVATE ${app_sources})

# Libraries

# Include
zephyr_include_directories(
    src
)