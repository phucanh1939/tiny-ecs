# Command Buffer

`CommandBuffer` defers structural changes to the ECS until a controlled playback point.

This is mainly needed because systems may iterate over entities while also needing to:

* Create entities
* Destroy entities
* Add components
* Remove components

Applying those changes immediately could invalidate the query being iterated.

## Basic Flow

```text
System
  │
  │ record commands
  ▼
CommandBuffer
  │
  │ playback()
  ▼
World
  │
  ├── create entity
  ├── destroy entity
  ├── add component
  └── remove component
```

For example:

```cpp
for (auto [entity, idle] : world.query<Idle>())
{
    if (idle.remainingTime <= 0.0f)
    {
        commands.removeComponent<Idle>(entity);
        commands.addComponent(entity, Wander{3.0f, {}});
    }
}
```

The entity is not structurally changed during the query.

## Command Representation

Commands share a small polymorphic interface:

```cpp
struct ICommand
{
    virtual ~ICommand() = default;
    virtual void execute(World& world) = 0;
};
```

Each operation has its own command type:

```text
ICommand
   │
   ├── CreateEntityCommand
   ├── DestroyEntityCommand
   ├── AddComponentCommand<T>
   └── RemoveComponentCommand<T>
```

Commands store the data required to perform the operation later.

## Playback

`playback()` executes commands in the order they were recorded.

```text
Record:

1. Remove Idle
2. Add Wander
3. Destroy Entity

Playback:

1. Remove Idle
2. Add Wander
3. Destroy Entity
```

This preserves command ordering.

After playback, the buffer can be cleared and reused.

## Create Entity

The buffer also supports creating an entity with multiple components:

```cpp
commands.createEntity(
    Position{10.0f, 20.0f},
    Velocity{1.0f, 0.0f},
    Health{100}
);
```

The components are stored inside the command until playback.

At playback:

```text
CreateEntityCommand
        │
        ▼
world.createEntity(Position, Velocity, Health)
        │
        ▼
One archetype
```

This uses the same multi-component creation API as direct entity creation and avoids creating intermediate archetypes.

## System Boundary

The important design rule is:

> **Systems can request structural changes without performing them during iteration.**

This gives the ECS a clear separation:

```text
System
   │
   ├── reads/modifies component data directly
   │
   └── records structural changes
             │
             ▼
       CommandBuffer
             │
          playback
             │
             ▼
           World
```

The command buffer does not own ECS state. It is only a temporary queue of deferred operations.
