# TinyECS

A small ECS library built from scratch in C++.

The goal is to understand how an Entity Component System works internally by implementing it step by step.

## Goals

* Understand ECS architecture
* Learn archetypes and chunks
* Explore data-oriented memory layout
* Build a reusable engine-independent ECS library
* Improve C++ and systems programming skills

## Project Structure

```text
tiny-ecs/
├── include/     Public headers
├── src/         Library implementation
├── tests/       Tests
├── examples/    Usage examples
└── CMakeLists.txt
```

## Build

```bash
cmake -S . -B build
cmake --build build
```

Run the example:

```bash
./build/examples/tinyecs-example
```

Run tests:

```bash
./build/tests/tinyecs-tests
```

## Current Focus

Start with the fundamental ECS concepts:

```text
Entity
  ↓
World
  ↓
Component
  ↓
System
  ↓
Query
  ↓
Archetype
  ↓
Chunk
```

The implementation will evolve gradually from simple, easy-to-understand storage toward a more data-oriented ECS design.
