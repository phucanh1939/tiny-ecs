# Tiny ECS

A small archetype-based ECS written in C++.

Tiny ECS is a learning project built to understand how an ECS works internally, with a focus on **archetypes, chunk-based storage, entity migration, and queries**.

## Architecture

```text
World
 │
 ├── Entity Management
 │
 └── ArchetypeRegistry
       │
       ├── Archetype
       │    └── Chunk
       │         └── Component Columns
       │
       └── Archetype
            └── Chunk
```

An entity's component signature determines its archetype.

```text
Entity
  │
  ▼
EntityLocation
  │
  ▼
Archetype
  │
  ▼
Chunk
  │
  ▼
Component data
```

See **[Architecture](docs/Architecture.md)** for the full design.

## Example

```cpp
World world;

Entity player = world.createEntity(
    Position{10.0f, 20.0f},
    Velocity{1.0f, 0.0f},
    Health{100.0f}
);

for (auto [entity, position, velocity] :
     world.query<Position, Velocity>())
{
    position.x += velocity.x;
}
```

Structural changes can be deferred:

```cpp
CommandBuffer commands;

commands.removeComponent<Idle>(entity);
commands.addComponent(entity, Chasing{target});

commands.playback(world);
```

## v1 Features

* Entity IDs and versions
* Entity ID reuse
* Component registration
* Component signatures
* Archetype-based storage
* 16 KB chunks
* Column-based component storage
* Entity migration
* Swap-back removal
* Multi-chunk archetypes
* Typed queries
* Multi-component entity creation
* Deferred structural changes

## Documentation

* **[Architecture](docs/Architecture.md)** — Overall design and data flow
* **[Entities](docs/Entity.md)** — Entity IDs, versions, locations, and lifecycle
* **[Archetypes](docs/Archetype.md)** — Signatures, migration, and archetype registry
* **[Chunks](docs/Chunk.md)** — Memory layout, capacity, columns, and swap-back removal
* **[Queries](docs/Query.md)** — Query matching and iteration
* **[Command Buffer](docs/Command-buffer.md)** — Deferred structural changes

## Project Goal

This is primarily a **learning implementation**, not a production ECS.

The goal is to understand the mechanics behind archetype ECS well enough to reason about the trade-offs and eventually build more advanced systems on top of that knowledge.

That is a much better division: **README tells you where to look; docs explain how it works.**
