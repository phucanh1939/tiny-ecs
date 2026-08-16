# Chunk

A chunk is the fixed-size storage unit used by an `Archetype`.

Each chunk stores entities and their component data for one archetype.

```text
Archetype
│
├── Chunk
│   ├── Entity[]
│   ├── Position[]
│   └── Velocity[]
│
├── Chunk
│   ├── Entity[]
│   ├── Position[]
│   └── Velocity[]
│
└── ...
```

## Fixed Size

Each chunk targets **16 KB** of storage.

Its capacity is calculated from the archetype's component layout so the entity data and all component columns fit within that size.

```text
Chunk
┌─────────────────────────────┐
│ Entity column               │
├─────────────────────────────┤
│ Position column             │
├─────────────────────────────┤
│ Velocity column             │
├─────────────────────────────┤
│ ...                         │
└─────────────────────────────┘
          16 KB
```

The chunk therefore has a fixed entity capacity determined by its archetype.

## Column Storage

Component data is stored as separate columns rather than as an array of component structs.

For:

```text
[Position, Velocity, Health]
```

the chunk conceptually contains:

```text
Entity:    [E0][E1][E2][E3]
Position:  [P0][P1][P2][P3]
Velocity:  [V0][V1][V2][V3]
Health:    [H0][H1][H2][H3]
```

Components of the same type are contiguous in memory.

## Raw Storage

The chunk owns one raw memory block:

```cpp
std::unique_ptr<std::byte[]> _data;
```

`ColumnLayout` describes where each component column starts and the size of each element.

This allows the chunk to store arbitrary registered component types without knowing their C++ types directly.

## Entity Operations

### Add

Adding an entity places it at the next available index.

```text
Before:
[A][B][C]

Add D:
[A][B][C][D]
```

The component slots are initially uninitialized/empty storage.

### Remove

Removal uses swap-back:

```text
Before:
[A][B][C][D]

Remove B:

[A][D][C]
```

The moved entity (`D`) is returned so `World` can update its `EntityLocation`.

### Copy

`copyEntity()` copies an entity into another compatible chunk and copies only the component columns shared by the source and destination archetypes.

This is used during archetype migration.

## Key Responsibility

The chunk is responsible only for **dense storage and movement of entities/data**.

It does not manage:

* Entity lifetime
* Entity IDs
* Archetype selection
* Component registration
* Global entity locations

Those responsibilities belong to `World` and `Archetype`.

The result is a simple hierarchy:

```text
World
  ↓
Archetype
  ↓
Chunk
  ↓
Raw component storage
```
