# TinyECS Project Context

## Goal

We are building a small C++ ECS from first principles to understand how ECS works at the memory/layout level.

Current approach:

```text
World
 └── ComponentStorageRegistry
      └── ComponentStorage<TComponent>
           ├── Dense component array
           ├── Dense entity array
           └── Entity ID → component index lookup
```

We are currently testing the **first implementation**. Do not jump to archetypes/chunks yet.

After this implementation is tested and understood, we will redesign toward:

```text
Archetype
 └── Chunk
      └── Component columns
```

with entity locations such as:

```text
Entity → Location
          ├── Archetype
          └── Index
```

---

## Coding Conventions

* `PascalCase`: types and template parameters.
* `camelCase`: functions, methods, variables, and parameters.
* `_camelCase`: private members.
* `lowercase`: namespaces.
* Prefer `const`, early returns, and braces.
* Use four spaces for indentation.
* Do not break lines unnecessarily.
* Use comments to explain **why**, not obvious **what**.
* Use `#pragma once`.

---

# Current ECS Design

## Entity

An `Entity` is a small value-type handle containing an ID and version.

Conceptually:

```cpp
struct Entity
{
    std::uint32_t id;
    std::uint32_t version;
};
```

The `World` stores entity state separately:

```text
Entity ID → World arrays

_versions:
[version0][version1][version2]...

_alive:
[true][true][true]...
```

An entity handle itself does not contain the components.

---

# ComponentStorage<TComponent>

Each component type has its own storage.

Example:

```cpp
struct Position
{
    float x;
    float y;
};

struct Velocity
{
    float x;
    float y;
    float z;
};
```

The storage is:

```text
ComponentStorage<Position>
    ├── std::vector<Position> _components
    ├── std::vector<Entity> _entities
    └── std::vector<std::size_t> _entityToIndex
```

The components are stored densely:

```text
_components:

[Position0][Position1][Position2][Position3]
```

The entity array stores which entity owns each component slot:

```text
_entities:

[Entity0][Entity1][Entity2][Entity3]
```

The lookup array maps:

```text
Entity ID → Component array index
```

Example:

```text
Entity 0 → Position index 0
Entity 3 → Position index 1
Entity 7 → Position index 2
```

---

## ComponentStorage Operations

### Add

```text
1. Check whether entity already has the component.
2. Append component to _components.
3. Append entity to _entities.
4. Resize _entityToIndex if needed.
5. Store entity ID → new component index.
```

### Remove

Uses swap-and-pop to keep storage dense:

Before:

```text
_components:
[P0][P1][P2][P3]
```

Remove `P1`:

```text
_components:
[P0][P3][P2]
```

The moved entity's lookup index is updated.

Then:

```cpp
_components.pop_back();
_entities.pop_back();
```

This means removal is O(1), but component order is not stable.

---

# ComponentStorageRegistry

The registry stores one type-erased storage per component type:

```cpp
std::unordered_map<
    std::type_index,
    std::unique_ptr<IComponentStorage>
> _storages;
```

Conceptually:

```text
typeid(Position)
    ↓
unique_ptr<IComponentStorage>
    ↓
ComponentStorage<Position>

typeid(Velocity)
    ↓
unique_ptr<IComponentStorage>
    ↓
ComponentStorage<Velocity>
```

The registry API:

```cpp
template<typename TComponent>
ComponentStorage<TComponent>& getOrCreate();

template<typename TComponent>
ComponentStorage<TComponent>& get();

template<typename TComponent>
bool contains() const;
```

`getOrCreate<TComponent>()`:

```text
typeid(TComponent)
      ↓
std::type_index
      ↓
unordered_map lookup
      ↓
create ComponentStorage<TComponent> if missing
```

The storage is created with:

```cpp
auto storage =
    std::make_unique<ComponentStorage<TComponent>>();
```

Then ownership is moved into the map:

```cpp
_storages.emplace(type, std::move(storage));
```

The `unique_ptr` local becomes empty after the move. The container now owns the actual storage object.

---

# World

`World` owns the ECS state:

```text
World
├── std::vector<std::uint32_t> _versions
├── std::vector<bool> _alive
└── ComponentStorageRegistry _componentStorages
```

Responsibilities:

* Create entities.
* Destroy entities.
* Validate entities.
* Add components.
* Remove components.
* Own component storage.

The World does not own or update gameplay systems.

Current component API:

```cpp
template<typename TComponent>
void addComponent(Entity entity, TComponent component);

template<typename TComponent>
void removeComponent(Entity entity);
```

The query system is intentionally left simple for now.

We previously started designing an iterator-based `Query<TComponents...>`, but decided not to over-engineer it because the storage architecture will later be replaced by archetypes/chunks.

---

# Current Memory Test

The current test creates multiple entities and adds components:

```cpp
struct Position
{
    float x;
    float y;
};

struct Velocity
{
    float x;
    float y;
    float z;
};
```

Example:

```cpp
for (int i = 0; i < 5; ++i)
{
    tinyecs::Entity entity = world.createEntity();

    world.addComponent(
        entity,
        Position{
            static_cast<float>(i),
            static_cast<float>(i)
        }
    );

    world.addComponent(
        entity,
        Velocity{
            1.0f,
            0.0f,
            0.0f
        }
    );
}
```

A debug function prints component addresses:

```text
8Velocity(size: 12):
[0x1035761f0, 0x1035761fc, 0x103576208, 0x103576214, 0x103576220]

8Position(size: 8):
[0x103576170, 0x103576178, 0x103576180, 0x103576188, 0x103576190]
```

This confirms:

```text
sizeof(Position) == 8
```

and each address differs by:

```text
8 bytes
```

For `Velocity`:

```text
sizeof(Velocity) == 12
```

and each address differs by:

```text
12 bytes
```

Therefore the components are physically stored contiguously:

```text
Position:
[P0][P1][P2][P3][P4]
 8B  8B  8B  8B  8B

Velocity:
[V0][V1][V2][V3][V4]
 12B 12B 12B 12B 12B
```

Different component types have separate arrays:

```text
Position storage:
[P0][P1][P2][P3][P4]

Velocity storage:
[V0][V1][V2][V3][V4]
```

They are not stored as:

```text
Entity 0: [Position][Velocity]
Entity 1: [Position][Velocity]
```

This is the current sparse-set-like storage model.

---

# Current Test Command

Build and run:

```sh
cmake --build build && ./build/tests/tinyecs-tests
```

---

# Important Understanding So Far

The current storage is **not yet an archetype ECS**.

It is closer to:

```text
Entity ID
    ↓
ComponentStorage<T>
    ↓
Dense component array
```

The component type determines the storage.

The entity-to-component relationship is maintained by lookup arrays.

The next architectural step, after testing this implementation, is to move toward:

```text
Entity
  ↓
Location
  ├── Archetype*
  └── index

Archetype
  ↓
Chunks
  ↓
Component columns
```

Example:

```text
Archetype<Position, Velocity>

Chunk:
Position column: [P0][P1][P2][P3]
Velocity column: [V0][V1][V2][V3]
```

Entities with the same component composition live in the same archetype.

Adding/removing a component will move an entity between archetypes.

The current implementation should be completed and tested first so we can compare the simple per-component storage model against the later archetype/chunk model.
