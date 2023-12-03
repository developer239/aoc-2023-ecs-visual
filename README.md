# AoC Day 3 Visual

![master](https://github.com/developer239/cpp-starter/actions/workflows/ci.yml/badge.svg)

## Overview

The cpp-starter project is designed as a starting point for creating C++ applications.

It leverages SDL2 for graphical rendering and input handling and provides a modular architecture for
scalable development.

## Running the Project on Mac

1) Install dependencies:

```bash
$ brew install cmake ninja sdl2 sdl2_ttf sdl2_image sdl2_mixer
```

2) Build:

```bash
$ cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=$(brew --prefix)/bin/ninja -G Ninja -S . -B build
$ cd build
$ ninja
$ ctest
```

3) Run:

```bash
$  cd build/src/apps/pong
$ ./AppPong 
```
