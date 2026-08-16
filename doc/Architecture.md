# Tiny ECS Architecture

## Overview

Tiny ECS is a small **archetype-based ECS** designed to understand the internal mechanics of ECS storage and structural changes.

The main relationship is:

```text
World
 │
 ├── Entity management
 │
 └── ArchetypeRegistry
       │
       ├── Archetype
       │    ├── Chunk
       │    ├── Chunk
       │    └── ...
       │
       └── Archetype
            └── ...
```

The core storage path is:

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
Component columns
```

## Core Design

### Entity

An `Entity` is a lightweight `{id, version}` handle.

The `World` maintains:

* entity versions
* reusable entity IDs
* entity locations

The entity itself does not own component data.

See [entity.md](entity.md).

### Component

Component types are registered by `ComponentRegistry` and assigned a `ComponentType`.

A `ComponentSignature` represents a set of component types using a fixed-size bitset.

```text
Position → bit 0
Velocity → bit 1
Health   → bit 2

Position + Velocity
       ↓
     0011
```

### Archetype

An `Archetype` represents one unique component signature.

```text
[Position, Velocity]        → Archetype A
[Position, Velocity, Health] → Archetype B
```

The `ArchetypeRegistry` owns and retrieves archetypes by signature.

See [archetype.md](archetype.md).

### Chunk

An archetype stores its entities in one or more fixed-size chunks.

Chunks target **16 KB** and use column-based storage.

```text
Chunk
├── Entity[]
├── Position[]
├── Velocity[]
└── Health[]
```

This keeps components of the same type contiguous in memory.

See [chunk.md](chunk.md).

## Entity Movement

An entity belongs to exactly one archetype.

Changing its component set therefore requires migration.

```text
[Position, Velocity]
        │
        │ add Health
        ▼
[Position, Velocity, Health]
```

The migration process:

```text
1. Find/create destination archetype
2. Copy entity
3. Copy components shared by both archetypes
4. Update EntityLocation
5. Remove entity from source archetype
6. Update location of any entity moved by swap-back
```

This is the fundamental structural operation in the ECS.

## Queries

`Query<T...>` provides iteration over matching archetypes.

```text
World
 │
 ├── Archetype A [Position]
 ├── Archetype B [Position, Velocity]       ← match
 ├── Archetype C [Position, Velocity, Health]← match
 └── Archetype D [Health]
```

The query keeps non-owning references to matching archetypes and traverses their chunks.

See [query.md](query.md).

## Structural Changes

Structural changes modify an entity's archetype.

Examples:

```text
Add component
Remove component
Create entity
Destroy entity
```

These operations can be deferred through `CommandBuffer`.

```text
System
  │
  │ record
  ▼
CommandBuffer
  │
  │ playback
  ▼
World
  │
  ▼
Archetype migration / entity lifecycle
```

See [command-buffer.md](command-buffer.md).

## Data Ownership

Ownership is intentionally hierarchical:

```text
World
 │
 └── ArchetypeRegistry
       │
       └── Archetype
             │
             └── Chunk
                   │
                   └── Component data
```

Other systems provide access without owning this storage:

```text
Query ───────────────► Archetype
CommandBuffer ───────► World
EntityLocation ──────► Archetype / Chunk / index
```

## Structural vs Data Operations

The architecture separates two kinds of operations.

**Data operations** modify existing component values:

```cpp
position.x += velocity.x;
health.current -= damage;
```

These can happen directly while iterating.

**Structural operations** change the entity's component signature:

```cpp
world.addComponent(...);
world.removeComponent(...);
world.destroyEntity(...);
```

These may move entities between archetypes, so systems can defer them through `CommandBuffer`.

## Main Data Flow

### Entity Creation

```text
World::createEntity(components...)
        │
        ▼
Build ComponentSignature
        │
        ▼
ArchetypeRegistry::getOrCreate()
        │
        ▼
Archetype::addEntity()
        │
        ▼
Initialize component data
        │
        ▼
Store EntityLocation
```

Creating an entity with all its initial components is important because it places the entity directly into its final archetype instead of creating intermediate archetypes.

### Query

```text
World::query<T...>()
        │
        ▼
Find matching archetypes
        │
        ▼
Query
        │
        ▼
Chunks
        │
        ▼
Component references
```

### Structural Change

```text
CommandBuffer
      │
   playback
      ▼
     World
      │
      ▼
Destination Archetype
      │
      ▼
Entity Migration
      │
      ▼
Updated EntityLocation
```

## Design Principles

The v1 implementation follows a few simple principles:

1. **Entities are handles, not storage.**
2. **Component composition determines archetype.**
3. **Archetypes own chunks.**
4. **Chunks own component storage.**
5. **Entity locations provide direct access to storage.**
6. **Queries provide traversal without owning storage.**
7. **Structural changes can be deferred through commands.**
8. **Storage stays dense through swap-back removal.**

The implementation intentionally stays small so the underlying ECS mechanics remain visible.
