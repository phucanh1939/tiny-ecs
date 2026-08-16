# Query

`Query` provides iteration over entities whose archetypes contain the requested component types.

```text
Query<Position, Velocity>
        │
        ▼
Matching Archetypes
        │
        ├── Chunk
        ├── Chunk
        └── ...
```

## Matching

A query does not own any entities or component data.

The `World` provides it with non-owning pointers to archetypes that match the requested component signature.

For:

```cpp
world.query<Position, Velocity>()
```

an archetype containing:

```text
Position + Velocity
```

matches.

An archetype containing only:

```text
Position
```

does not.

An archetype containing:

```text
Position + Velocity + Health
```

also matches.

```text id="n8v2c1"
Query<Position, Velocity>

[Position]                  ✗
[Velocity]                  ✗
[Position, Velocity]        ✓
[Position, Velocity, Health]✓
```

## Iteration

The query iterator walks through:

```text id="7c1k8p"
Archetype
    ↓
Chunk
    ↓
Entity index
```

It automatically advances across chunk boundaries and skips empty archetypes/chunks.

A query therefore presents the underlying archetype/chunk storage as one continuous sequence.

## Result

Each iteration returns the entity together with references to its requested components:

```cpp
for (auto [entity, position, velocity] :
     world.query<Position, Velocity>())
{
    position.x += velocity.x;
}
```

Conceptually:

```text id="3q6m1z"
(Entity, Position&, Velocity&)
```

The returned component references point directly into chunk storage; the query does not copy component data.

## Ownership

`Query` stores **non-owning** pointers to archetypes.

```text id="v5q2cd"
World
 │
 ├── owns Archetype
 │
 └── creates Query
          │
          └── references Archetype*
```

Therefore the lifetime of the query depends on the referenced archetypes remaining alive.

## Design

The query layer is intentionally thin:

* `World` determines matching archetypes.
* `Archetype` manages chunks.
* `Chunk` provides component storage.
* `Query` provides iteration across the matching storage.

This keeps queries focused on **traversal rather than storage management**.
