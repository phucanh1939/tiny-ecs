# Entity

An entity is a lightweight handle:

```cpp
Entity{id, version}
```

It does not own component data.

## Identity

The `id` identifies a reusable entity slot.

The `version` identifies the current lifetime of that slot.

This allows IDs to be reused safely:

```text
Entity{5, 1}
    │
  destroy
    │
    ▼
Entity{5, 2}
```

An old `Entity{5, 1}` therefore cannot accidentally refer to the new entity.

## Location

The `World` maintains an `EntityLocation` for each entity ID.

```text
Entity
  │
  ▼
EntityLocation
  │
  ├── Archetype
  ├── Chunk
  └── Index
```

This provides direct access to an entity's storage without searching through archetypes.

When an entity moves between archetypes, or another entity is moved into its slot through swap-back removal, the corresponding location is updated.

## Lifecycle

```text
Create
  ↓
Alive
  ↓
Destroy
  ↓
ID available for reuse
  ↓
Create
  ↓
New version
```

The entity system therefore separates:

* **Identity** — `id + version`
* **Location** — where the entity is currently stored
* **Data** — components stored by archetypes/chunks

For the implementation details, see the corresponding `World`, `Archetype`, and `Chunk` documentation.
