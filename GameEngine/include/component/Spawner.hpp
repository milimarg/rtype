/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-rtype-noe.tritsch                                *
 * Description     -                                                                    *
 *     Spawner                                                                          *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                     *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                     *
 *             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                     *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#ifndef SPAWNER_HPP
    #define SPAWNER_HPP

    #include <iostream>
    #include <functional>
    #include "Transformable.hpp"
    #include "Registry.hpp"
    #include "Vec2.hpp"

namespace ecs
{
    using cmp::Transformable;

    namespace cmp
    {
        struct Spawner {
            using spawnFunc = std::function<void(Registry &, const Vec2 &)>;
            using moveFunc = std::function<void(Transformable &)>;
            using spawnConditionFunc = std::function<bool(Registry &, size_t)>;

            float interval;
            float rate; // ? on 1.0
            spawnFunc onSpawn; // ? function to spawn an entity
            moveFunc onMove; // ? function to move the spawn source
            spawnConditionFunc shouldSpawn; // ? function to check if the spawner is on

            Spawner(
                spawnFunc onSpawn,
                float interval,
                float rate = 1.0f,
                moveFunc moveAction = [](Transformable &){},
                spawnConditionFunc spawnCondition = [](Registry &, size_t){ return true; }
            ) : interval(interval), rate(rate), onSpawn(onSpawn), onMove(moveAction), shouldSpawn(spawnCondition) {}
            Spawner(
                spawnFunc onSpawn,
                float interval,
                moveFunc moveAction,
                spawnConditionFunc spawnCondition = [](Registry &, size_t){ return true; }
            ) : interval(interval), rate(100), onSpawn(onSpawn), onMove(moveAction), shouldSpawn(spawnCondition) {}
        };
    };
};

#endif /* SPAWNER_HPP */
