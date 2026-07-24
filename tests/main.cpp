#include <cassert>
#include <iostream>

#include <tinyecs/World.h>

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

struct Health
{
    float currentHealth;
    float maxHealth;
};

int main()
{
    constexpr int entityCount = 5;

    tinyecs::World world;
    tinyecs::Entity entities[entityCount];

    for (int i = 0; i < entityCount; ++i)
    {
        entities[i] = world.createEntity();
        world.addComponent(entities[i], Position{1.1f, 2.2f});
        world.addComponent(entities[i], Velocity{1.0f, 0.0f, 2.0f});
    }

    world.print();

    world.removeComponent<Position>(entities[2]);
    world.removeComponent<Velocity>(entities[1]);
    world.addComponent<Health>(entities[4], Health{});
    world.destroyEntity(entities[3]);
    std::cout << "----------------" << std::endl;


    world.print();

    return 0;
}