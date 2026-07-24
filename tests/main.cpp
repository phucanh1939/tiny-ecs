#include <cassert>

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

int main()
{
    tinyecs::World world;

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
                0.0f
            }
        );
    }

    world.debugPrintMemoryLayout();

    return 0;
}