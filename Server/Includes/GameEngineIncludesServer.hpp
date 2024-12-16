/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-rtype-noe.tritsch                                *
 * Description     -                                                                    *
 *     main                                                                             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░             *
 *         ▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓             *
 *         █  ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#ifndef GAME_ENGINE_INCLUDES_SERVER_HPP
    #define GAME_ENGINE_INCLUDES_SERVER_HPP

    #include <iostream>

    #include "Registry.hpp"

    // ? component list
    #include "Transformable.hpp"
    #include "Velocity.hpp"
    #include "Acceleration.hpp"
    #include "Gravity.hpp"
    #include "Collision.hpp"
    #include "Jump.hpp"
    #include "Red.hpp"
    #include "TimerContainer.hpp"
    #include "Health.hpp"
    #include "Spawner.hpp"
    #include "UUID.hpp"
    #include "Script.hpp"

    // ? system list
    #include "TransformSystem.hpp"
    #include "GravitySystem.hpp"
    #include "CollisionSystem.hpp"
    #include "ClockSystem.hpp"
    #include "HealthSystem.hpp"
    #include "SpawnerSystem.hpp"
    #include "ScriptSystem.hpp"

    #include "Using.hpp"
    #include "utils.hpp"
    #include "Vec2.hpp"

    #include "ServerSceneLoader.hpp"

    #include "TestError.hpp"
    #include "Math.hpp"

    #include "UUIDManager.hpp"

    #include "CustomComponents.hpp"

USING(
    ecs::Registry,
    ecs::UUIDManager,

    ecs::cmp::Acceleration,
    ecs::cmp::Collision,
    ecs::cmp::Jump,
    ecs::cmp::Gravity,
    ecs::cmp::Transformable,
    ecs::cmp::Velocity,
    ecs::cmp::Red,
    ecs::cmp::TimerContainer,
    ecs::cmp::Health,
    ecs::cmp::Spawner,
    ecs::cmp::UUID,
    ecs::cmp::Script,

    ecs::sys::TransformSystem,
    ecs::sys::GravitySystem,
    ecs::sys::CollisionSystem,
    ecs::sys::ClockSystem,
    ecs::sys::HealthSystem,
    ecs::sys::SpawnerSystem,
    ecs::sys::ScriptSystem
);

#endif /* GAME_ENGINE_INCLUDES_SERVER_HPP */
