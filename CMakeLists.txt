# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)

# Re-direct the directory where the 'boards' directory is found from
# $ZEPHYR_BASE to this directory.
set(BOARD_ROOT ${CMAKE_CURRENT_LIST_DIR})

# Only compatible with health_hub because it has
# re-directed the 'boards' directory to a directory that only contains
# this board.
set(BOARD health_hub)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(HealthHub)

target_sources(app PRIVATE
  src/main.c
)
