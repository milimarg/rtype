/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sun, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-rtype-noe.tritsch                                *
 * Description     -                                                                    *
 *     SpawnerSystem                                                                    *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄             *
 *      ▐  ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀            *
 *        █▄▄▄▄▄  ▐  █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             *
 *        █    ▌     █          █        █        █    ▌    █         █   █             *
 *       ▄▀▄▄▄▄    ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             *
 *       █    ▐   █         █       █ █          █    ▐   █     ▐   █   █               *
 *       ▐        ▐         ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "SpawnerSystem.hpp"
#include "IndexedZipper.hpp"
#include "Using.hpp"
#include "utils.hpp"
#include <random>

USING(
    ecs::sys::SpawnerSystem,
    ecs::cmp::Timer
);

void SpawnerSystem::operator()(
    Registry &registry,
    Storage<Spawner> &spawners,
    Storage<TimerContainer> &timerContainers,
    Storage<Transformable> &transformables
) const
{
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());
    static thread_local std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (auto &&[i, spawner, timers, transformable] : IndexedZipper(spawners, timerContainers, transformables)) {
        auto &[interval, rate, onSpawn, moveFunc, spawnCondition] = spawner;
        auto &[x, y, _, __, ___, ____] = transformable;

        if (ACTION_READY("spawner move interval"))
            moveFunc(transformable);

        if (timers.contains("spawn interval") && !spawnCondition(registry, i))
            continue;

        if (timers.safeConsume("spawn interval", interval) && dist(gen) <= rate) {
            timers._timers["spawn interval"].resetReached();
            onSpawn(registry, Vec2(x, y));
        }
    }
}
