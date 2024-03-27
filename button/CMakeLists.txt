# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
set(BOARD stm32f411e_disco)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(
  TestApp 
  VERSION 1.0
  DESCRIPTION "Zephyr test application"
  LANGUAGES C  
)

target_sources(
  app
  PRIVATE 
  src/led.c
  src/encoder.c
  src/button.c
  src/main.c
)

target_include_directories(
  app
  PUBLIC
  inc/
)