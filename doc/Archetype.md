# Archetype

An archetype represents all entities that have the same **component signature**.

```text
ComponentSignature
       │
       ▼
   Archetype
       │
       ├── Chunk
       ├── Chunk
       └── Chunk
```

For example:

```text
[Position, Velocity] → Archetype A

[Position, Velocity, Health] → Archetype B
```

Entities with different component sets belong to different archetypes.

## Signature

The archetype is identified by a `ComponentSignature`.

The signature is a fixed-size bitset where each component type corresponds to one bit.

```text id="l9u5sh"
Position  → bit 0
Velocity  → bit 1
Health    → bit 2

[Position, Velocity]
       ↓
00000011
```

`ArchetypeRegistry` uses this signature to find an existing archetype or create a new one.

## Storage

An archetype owns its chunks.

Each chunk contains the entities and their component data in column-based storage.

```text id="1v8j5w"
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

Chunks target a fixed **16 KB** storage size, with their capacity determined by the component layout.

## Entity Migration

Changing an entity's component set means changing its archetype.

```text
[Position, Velocity]
        │
        │ add Health
        ▼
[Position, Velocity, Health]
```

The entity and all components shared by the two archetypes are copied into the destination archetype.

The `World` then updates the entity's `EntityLocation`.

This is the main cost of structural changes in an archetype ECS.

## Removal

Entities are removed from chunks using **swap-back removal**.

```text
Before:
[A][B][C][D]

Remove B:
[A][D][C]
```

The moved entity's `EntityLocation` must be updated.

This keeps removal O(1) and avoids leaving holes in the chunk.

## Design

The archetype layer separates:

* **Signature** — which components exist
* **Archetype** — which entities share that signature
* **Chunk** — where the actual data is stored

The `World` owns the archetypes through `ArchetypeRegistry`; archetypes do not own or manage the global entity lifecycle.
