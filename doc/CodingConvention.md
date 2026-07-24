# C++ Coding Conventions

## Naming

### Types

Use `PascalCase` for classes, structs, enums, and type aliases.

```cpp
class Entity;
struct ComponentStorage;
enum class EntityState;
using EntityId = std::uint32_t;
````

### Functions and Methods

Use `camelCase`.

```cpp
entity.isValid();
world.createEntity();
world.destroyEntity();
```

### Variables and Parameters

Use `camelCase`.

```cpp
std::uint32_t entityId;
std::uint32_t version;
```

### Private Member Variables

Use `_camelCase`.

```cpp
std::uint32_t _id;
std::uint32_t _version;
```

### Namespaces

Use lowercase.

```cpp
namespace tinyecs
{
}
```

### Template Type Parameters

Use `PascalCase`.

```cpp
template<typename TComponent>
class ComponentStorage;
```

---

# Classes and Structs

Use `class` when controlling access to data is important.

Use `struct` for simple data-oriented types where members are naturally public.

```cpp
class World
{
private:
    // Controlled implementation state.
};

struct EntityData
{
    std::uint32_t id;
    std::uint32_t version;
};
```

---

# Access Modifiers

Order members as:

```cpp
class Example
{
public:
    // Public API.

protected:
    // Protected API.

private:
    // Internal implementation.
};
```

Prefer placing the public API first.

---

# Const Correctness

Use `const` whenever a function does not modify the object.

```cpp
bool isValid() const;
std::uint32_t id() const;
```

Pass read-only objects by `const` reference when copying is unnecessary.

```cpp
void process(const Entity& entity);
```

For small value types, pass by value.

```cpp
void process(Entity entity);
```

---

# Initialization

Prefer member initializer lists for constructors.

```cpp
Entity(std::uint32_t id, std::uint32_t version)
    : _id(id),
      _version(version)
{
}
```

Prefer initialization at the declaration when possible.

```cpp
std::uint32_t _id = 0;
std::uint32_t _version = 0;
```

---

# Function Definitions

Keep very small functions in the header.

```cpp
std::uint32_t id() const { return _id; }
```

Put non-trivial implementation in `.cpp` files.

```cpp
// World.h
void destroyEntity(Entity entity);
```

```cpp
// World.cpp
void World::destroyEntity(Entity entity)
{
    // Non-trivial implementation.
}
```

---

# Control Flow

Prefer early returns to deeply nested code.

```cpp
void process(Entity entity)
{
    if (!isAlive(entity))
    {
        return;
    }

    // Main logic.
}
```

Prefer:

```cpp
if (!condition)
{
    return;
}
```

over:

```cpp
if (condition)
{
    // Large block of code.
}
```

---

# Braces

Always use braces, even for single-line statements.

```cpp
if (isValid)
{
    process();
}
```

Avoid:

```cpp
if (isValid)
    process();
```

---

# Formatting

Use four spaces for indentation.

```cpp
class World
{
public:
    Entity createEntity();

private:
    std::vector<Entity> _entities;
};
```

Keep one statement per line.

```cpp
_entityId++;
_version++;
```

Avoid:

```cpp
_entityId++; _version++;
```

---

# Comments

Write comments to explain **why**, not obvious **what**.

Prefer:

```cpp
// Increment the version so previously issued handles become invalid.
++_versions[entity.id()];
```

Avoid:

```cpp
// Increment the version.
++_versions[entity.id()];
```

Use comments to explain:

* Design decisions.
* Non-obvious behavior.
* Important constraints.
* Performance considerations.
* Ownership and lifetime rules.

---

# Ownership

Make ownership explicit.

Use value members when the object owns the data directly.

```cpp
class World
{
private:
    std::vector<Entity> _entities;
};
```

Use smart pointers when dynamic ownership is required.

```cpp
std::unique_ptr<Storage> _storage;
```

Avoid raw owning pointers.

```cpp
Storage* _storage; // Avoid owning resources this way.
```

Raw pointers are acceptable for non-owning references when the lifetime is clear.

---

# Copy and Move

Explicitly define or delete special member functions when ownership is involved.

```cpp
World(const World&) = delete;
World& operator=(const World&) = delete;

World(World&&) noexcept = default;
World& operator=(World&&) noexcept = default;
```

Use `noexcept` for move operations when moving cannot throw.

---

# Header Files

Use `#pragma once`.

```cpp
#pragma once
```

Use angle brackets for library-style includes.

```cpp
#include <tinyecs/Entity.h>
```

Use standard library headers with angle brackets.

```cpp
#include <cstdint>
#include <vector>
```

Keep implementation details out of public headers when possible.

---

# Include Order

Organize includes in this order:

```cpp
// Corresponding header.
#include <tinyecs/World.h>

// Project headers.
#include <tinyecs/Entity.h>

// Standard library.
#include <cstdint>
#include <vector>
```

---

# Error Handling

Validate invalid input at API boundaries.

```cpp
void destroyEntity(Entity entity)
{
    if (!isAlive(entity))
    {
        return;
    }

    // Destroy entity.
}
```

Use assertions for programmer errors that should never happen.

```cpp
assert(entity.isValid());
```

Do not use exceptions for normal gameplay or ECS control flow.

---

# Performance

Prefer data-oriented layouts for performance-critical systems.

Keep frequently accessed data contiguous when possible.

```cpp
std::vector<Position> positions;
std::vector<Velocity> velocities;
```

Avoid unnecessary allocations in hot loops.

Avoid unnecessary copying.

```cpp
for (const Entity& entity : entities)
{
    process(entity);
}
```

Use profiling and measurement before applying complex optimizations.

---

# General Principles

* Keep the public API small.
* Prefer simple designs over premature abstractions.
* Make ownership and lifetime explicit.
* Prefer compile-time guarantees over runtime checks when practical.
* Keep data and behavior separate when it improves the design.
* Avoid unnecessary allocations.
* Avoid unnecessary copying.
* Optimize based on measurement, not assumptions.
* Prefer readable code over clever code.
