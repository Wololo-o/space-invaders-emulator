cmake_minimum_required(VERSION 3.10)

list(APPEND CompileOptionsList "-Wall" "-Wextra" "-Wpedantic")

project(space-invaders-emulator
    VERSION 1.0
    LANGUAGES C)

add_executable(invaders src/cpu.c src/main.c)
target_include_directories(invaders PUBLIC include)
target_compile_options(invaders PUBLIC ${CompileOptionsList})

add_executable(cpu-tests tests/cpu_tests.c src/cpu.c)
target_include_directories(cpu-tests PUBLIC include)
target_compile_options(cpu-tests PUBLIC ${CompileOptionsList})
