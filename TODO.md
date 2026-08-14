# TODO

- Need to support init entity with a set of component instead of add 1 by one. Current implementation make this create a lot of archetype

```cpp
Entity spawnUnit(tinyecs::World& world, int team, Position position) {
    Entity entity = world.createEntity();

    world.addComponent(entity, position);
    world.addComponent(entity, Velocity{0.0f, 0.0f});
    world.addComponent(entity, Health{100.0f, 100.0f});
    world.addComponent(entity, Team{team});
    world.addComponent(entity, Attack{10.0f, 1.5f, 1.0f});
    world.addComponent(entity, Sensor{10.0f});
    world.addComponent(entity, SpawnPosition{position.x, position.y});
    world.addComponent(entity, Idle{3.0f});

    return entity;
}
```