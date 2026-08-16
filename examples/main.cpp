#include <tinyecs/World.h>
#include <tinyecs/CommandBuffer.h>

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>

using tinyecs::Entity;

// ============================================================
// Components
// ============================================================

struct Position
{
    float x;
    float y;
};

struct Velocity
{
    float x;
    float y;
};

struct Health
{
    float current;
    float max;
};

struct Team
{
    int id;
};

struct Attack
{
    float damage;
    float range;
    float cooldown;
};

struct Sensor
{
    float range;
};

struct SpawnPosition
{
    float x;
    float y;
};

// ============================================================
// State Components
// ============================================================

struct Idle
{
    float remainingTime;
};

struct Wander
{
    float remainingTime;
    Position targetPosition;
};

struct Chasing
{
    Entity target;
};

struct Returning
{
};

struct Attacking
{
    Entity target;
    float cooldown;
};

// ============================================================
// Helpers
// ============================================================

struct Target
{
    Entity entity;
    float distanceSquared;
};

float distanceSquaredBetween(const Position &a, const Position &b)
{
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;
    return dx * dx + dy * dy;
}

float randomFloat(float min, float max)
{
    const float t = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    return min + (max - min) * t;
}

int randomInt(int min, int max)
{
    return min + std::rand() % (max - min + 1);
}

Position randomPositionAroundSpawn(const SpawnPosition &spawn)
{
    constexpr float radius = 5.0f;

    return {
        spawn.x + randomFloat(-radius, radius),
        spawn.y + randomFloat(-radius, radius)};
}

void moveTowards(const Position &position, Velocity &velocity, const Position &target)
{
    const float dx = target.x - position.x;
    const float dy = target.y - position.y;
    const float length = std::sqrt(dx * dx + dy * dy);

    if (length <= 0.001f)
    {
        velocity.x = 0.0f;
        velocity.y = 0.0f;
        return;
    }

    constexpr float speed = 2.0f;

    velocity.x = dx / length * speed;
    velocity.y = dy / length * speed;
}

Target findClosestEnemy(
    tinyecs::World &world,
    const Position &position,
    const Team &team,
    float range)
{
    Target result{
        .entity = {},
        .distanceSquared = std::numeric_limits<float>::max()};

    const float rangeSquared = range * range;

    for (auto [entity, otherPosition, otherTeam, health] :
         world.query<Position, Team, Health>())
    {
        if (otherTeam.id == team.id)
            continue;

        if (health.current <= 0.0f)
            continue;

        const float distance =
            distanceSquaredBetween(position, otherPosition);

        if (distance > rangeSquared)
            continue;

        if (distance < result.distanceSquared)
        {
            result.entity = entity;
            result.distanceSquared = distance;
        }
    }

    return result;
}

// ============================================================
// Spawn
// ============================================================

Entity spawnUnit(tinyecs::World &world, int team, Position position)
{
    Entity entity = world.createEntity(
        position,
        Velocity{0.0f, 0.0f},
        Health{100.0f, 100.0f},
        Team{team},
        Attack{10.0f, 1.5f, 1.0f},
        Sensor{10.0f},
        SpawnPosition{position.x, position.y},
        Idle{3.0f});

    std::cout << "[Spawn] Entity " << entity.id
              << " | Team " << team
              << " | Position (" << position.x
              << ", " << position.y << ")\n";

    return entity;
}

// ============================================================
// Spawn System
// ============================================================

class SpawnSystem
{
public:
    void update(tinyecs::World &world, float deltaTime)
    {
        _timer -= deltaTime;

        if (_timer > 0.0f)
            return;

        _timer = 2.0f;

        const int team = randomInt(1, 2);

        const Position position{
            randomFloat(-50.0f, 50.0f),
            randomFloat(-50.0f, 50.0f)};

        spawnUnit(world, team, position);
    }

private:
    float _timer = 0.0f;
};

// ============================================================
// Idle System
// ============================================================

class IdleSystem
{
public:
    void update(
        tinyecs::World &world,
        tinyecs::CommandBuffer &commands,
        float deltaTime)
    {
        for (auto [entity, idle, position, team, sensor] :
             world.query<Idle, Position, Team, Sensor>())
        {
            idle.remainingTime -= deltaTime;

            const Target target =
                findClosestEnemy(world, position, team, sensor.range);

            if (target.entity.isValid())
            {
                std::cout << "[State] Entity " << entity.id
                          << " Idle -> Chasing Entity "
                          << target.entity.id << '\n';

                commands.removeComponent<Idle>(entity);
                commands.addComponent(entity, Chasing{target.entity});
                continue;
            }

            if (idle.remainingTime <= 0.0f)
            {
                std::cout << "[State] Entity " << entity.id
                          << " Idle -> Wander\n";

                commands.removeComponent<Idle>(entity);
                commands.addComponent(entity, Wander{3.0f, {}});
            }
        }
    }
};

// ============================================================
// Wander System
// ============================================================

class WanderSystem
{
public:
    void update(
        tinyecs::World &world,
        tinyecs::CommandBuffer &commands,
        float deltaTime)
    {
        for (auto [entity, wander, position, velocity, team, sensor, spawn] :
             world.query<Wander, Position, Velocity, Team, Sensor, SpawnPosition>())
        {
            wander.remainingTime -= deltaTime;

            const Target target =
                findClosestEnemy(world, position, team, sensor.range);

            if (target.entity.isValid())
            {
                std::cout << "[State] Entity " << entity.id
                          << " Wander -> Chasing Entity "
                          << target.entity.id << '\n';

                velocity = {};

                commands.removeComponent<Wander>(entity);
                commands.addComponent(entity, Chasing{target.entity});
                continue;
            }

            if (wander.remainingTime <= 0.0f)
            {
                wander.targetPosition =
                    randomPositionAroundSpawn(spawn);

                wander.remainingTime = 3.0f;

                std::cout << "[Wander] Entity " << entity.id
                          << " new target ("
                          << wander.targetPosition.x
                          << ", "
                          << wander.targetPosition.y
                          << ")\n";
            }

            moveTowards(
                position,
                velocity,
                wander.targetPosition);
        }
    }
};

// ============================================================
// Chasing System
// ============================================================

class ChasingSystem
{
public:
    void update(
        tinyecs::World &world,
        tinyecs::CommandBuffer &commands,
        float deltaTime)
    {
        for (auto [entity, chasing, position, velocity, sensor, attack] :
             world.query<Chasing, Position, Velocity, Sensor, Attack>())
        {
            if (!world.isValid(chasing.target))
            {
                startReturning(entity, velocity, commands);
                continue;
            }

            const Position &targetPosition =
                world.getComponent<Position>(chasing.target);

            const float distance =
                distanceSquaredBetween(position, targetPosition);

            const float attackRangeSquared =
                attack.range * attack.range;

            if (distance <= attackRangeSquared)
            {
                std::cout << "[State] Entity " << entity.id
                          << " Chasing -> Attacking Entity "
                          << chasing.target.id << '\n';

                velocity = {};

                commands.removeComponent<Chasing>(entity);
                commands.addComponent(
                    entity,
                    Attacking{chasing.target, 0.0f});

                continue;
            }

            const float chaseRangeSquared =
                sensor.range * sensor.range * 4.0f;

            if (distance > chaseRangeSquared)
            {
                startReturning(entity, velocity, commands);
                continue;
            }

            moveTowards(position, velocity, targetPosition);
        }
    }

private:
    void startReturning(
        Entity entity,
        Velocity &velocity,
        tinyecs::CommandBuffer &commands)
    {
        std::cout << "[State] Entity " << entity.id
                  << " Chasing -> Returning\n";

        velocity = {};

        commands.removeComponent<Chasing>(entity);
        commands.addComponent(entity, Returning{});
    }
};

// ============================================================
// Returning System
// ============================================================

class ReturningSystem
{
public:
    void update(
        tinyecs::World &world,
        tinyecs::CommandBuffer &commands,
        float deltaTime)
    {
        for (auto [entity, returning, position, velocity, team, sensor, spawn] :
             world.query<Returning, Position, Velocity, Team, Sensor, SpawnPosition>())
        {
            const Target target =
                findClosestEnemy(world, position, team, sensor.range);

            if (target.entity.isValid())
            {
                std::cout << "[State] Entity " << entity.id
                          << " Returning -> Chasing Entity "
                          << target.entity.id << '\n';

                commands.removeComponent<Returning>(entity);
                commands.addComponent(entity, Chasing{target.entity});
                continue;
            }

            const Position spawnPosition{
                spawn.x,
                spawn.y};

            const float distance =
                distanceSquaredBetween(position, spawnPosition);

            if (distance <= 0.1f)
            {
                position = spawnPosition;
                velocity = {};

                std::cout << "[State] Entity " << entity.id
                          << " Returning -> Wander\n";

                commands.removeComponent<Returning>(entity);
                commands.addComponent(
                    entity,
                    Wander{3.0f, {}});

                continue;
            }

            moveTowards(
                position,
                velocity,
                spawnPosition);
        }
    }
};

// ============================================================
// Attack System
// ============================================================

class AttackSystem
{
public:
    void update(
        tinyecs::World &world,
        tinyecs::CommandBuffer &commands,
        float deltaTime)
    {
        for (auto [entity, attacking, attack, position] :
             world.query<Attacking, Attack, Position>())
        {
            if (!world.isValid(attacking.target))
            {
                std::cout << "[State] Entity " << entity.id
                          << " Attacking -> Returning\n";

                commands.removeComponent<Attacking>(entity);
                commands.addComponent(entity, Returning{});
                continue;
            }

            const Position &targetPosition =
                world.getComponent<Position>(attacking.target);

            const float distance =
                distanceSquaredBetween(position, targetPosition);

            if (distance > attack.range * attack.range)
            {
                std::cout << "[State] Entity " << entity.id
                          << " Attacking -> Chasing Entity "
                          << attacking.target.id << '\n';

                commands.removeComponent<Attacking>(entity);
                commands.addComponent(
                    entity,
                    Chasing{attacking.target});

                continue;
            }

            attacking.cooldown -= deltaTime;

            if (attacking.cooldown <= 0.0f)
            {
                attacking.cooldown = attack.cooldown;

                Health &targetHealth =
                    world.getComponent<Health>(attacking.target);

                targetHealth.current -= attack.damage;

                std::cout << "[Attack] Entity " << entity.id
                          << " -> Entity " << attacking.target.id
                          << " | Damage " << attack.damage
                          << " | Target HP "
                          << targetHealth.current
                          << '\n';
            }
        }
    }
};

// ============================================================
// Death System
// ============================================================

class DeathSystem
{
public:
    void update(
        tinyecs::World &world,
        tinyecs::CommandBuffer &commands)
    {
        for (auto [entity, health] :
             world.query<Health>())
        {
            if (health.current <= 0.0f)
            {
                std::cout << "[Death] Entity "
                          << entity.id << '\n';

                commands.destroyEntity(entity);
            }
        }
    }
};

// ============================================================
// Game
// ============================================================

int main()
{
    std::cout << "Simulation Started!! " << std::endl;
    tinyecs::World world;

    SpawnSystem spawnSystem;
    IdleSystem idleSystem;
    WanderSystem wanderSystem;
    ChasingSystem chasingSystem;
    ReturningSystem returningSystem;
    AttackSystem attackSystem;
    DeathSystem deathSystem;

    constexpr float deltaTime = 1.0f / 60.0f;

    std::size_t frame = 0;

    while (true)
    {
        ++frame;

        spawnSystem.update(world, deltaTime);

        tinyecs::CommandBuffer idleCommands;
        tinyecs::CommandBuffer wanderCommands;
        tinyecs::CommandBuffer chasingCommands;
        tinyecs::CommandBuffer returningCommands;
        tinyecs::CommandBuffer attackCommands;
        tinyecs::CommandBuffer deathCommands;

        idleSystem.update(
            world,
            idleCommands,
            deltaTime);

        wanderSystem.update(
            world,
            wanderCommands,
            deltaTime);

        chasingSystem.update(
            world,
            chasingCommands,
            deltaTime);

        returningSystem.update(
            world,
            returningCommands,
            deltaTime);

        attackSystem.update(
            world,
            attackCommands,
            deltaTime);

        deathSystem.update(
            world,
            deathCommands);

        idleCommands.playback(world);
        wanderCommands.playback(world);
        chasingCommands.playback(world);
        returningCommands.playback(world);
        attackCommands.playback(world);
        deathCommands.playback(world);

        if (frame % 60 == 0)
        {
            std::cout << "[World] Frame "
                      << frame
                      << " | Entities "
                      << world.entityCount()
                      << '\n';
        }
    }

    return 0;
}