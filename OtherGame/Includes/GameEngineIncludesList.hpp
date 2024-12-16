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

#ifndef GAME_ENGINE_INCLUDES_LIST_HPP
    #define GAME_ENGINE_INCLUDES_LIST_HPP

    #include <iostream>
    #define NOGDI
    #include <raylib.h>

    #include "Registry.hpp"
    #include "Zipper.hpp"

    // ? components
    #include "TimerContainer.hpp"
    #include "Transformable.hpp"
    #include "Velocity.hpp"
    #include "DrawableCmp.hpp"
    #include "Input.hpp"
    #include "Acceleration.hpp"
    #include "Collision.hpp"
    #include "Animated.hpp"
    #include "NamedFrames.hpp"
    #include "Health.hpp"
    #include "Spawner.hpp"
    #include "SoundSource.hpp"
    #include "MusicSource.hpp"
    #include "Mouse.hpp"
    #include "Button.hpp"
    #include "TextCmp.hpp"
    #include "HUD.hpp"

    // ? system list
    #include "ClockSystem.hpp"
    #include "TransformSystem.hpp"
    #include "InputSystem.hpp"
    #include "DrawSystem.hpp"
    #include "CollisionSystem.hpp"
    #include "AnimationSystem.hpp"
    #include "NamedFramesSystem.hpp"
    #include "HealthSystem.hpp"
    #include "MouseSystem.hpp"
    #include "ButtonSystem.hpp"
    #include "MusicSystem.hpp"
    #include "SpawnerSystem.hpp"

    #include "Using.hpp"
    #include "utils.hpp"
    #include "StringHash.hpp"
    #include "Vec2.hpp"

    #include "AssetManager.hpp"
    #include "SceneLoader.hpp"

    #include "Math.hpp"

    #include "CustomComponents.hpp"

USING(
    ecs::Registry,
    ecs::AssetManager,

    ecs::cmp::Acceleration,
    ecs::cmp::Animated,
    ecs::cmp::Collision,
    ecs::cmp::Drawable,
    ecs::cmp::Input,
    ecs::cmp::Gravity,
    ecs::cmp::RectangleRender,
    ecs::cmp::SpriteRender,
    ecs::cmp::Transformable,
    ecs::cmp::Velocity,
    ecs::cmp::Red,
    ecs::cmp::TimerContainer,
    ecs::cmp::NamedFrames,
    ecs::cmp::Health,
    ecs::cmp::Spawner,
    ecs::cmp::SoundSource,
    ecs::cmp::MusicSource,
    ecs::cmp::Mouse,
    ecs::cmp::Button,
    ecs::cmp::Text,
    ecs::cmp::HUD,

    ecs::sys::TransformSystem,
    ecs::sys::InputSystem,
    ecs::sys::DrawSystem,
    ecs::sys::CollisionSystem,
    ecs::sys::ClockSystem,
    ecs::sys::AnimationSystem,
    ecs::sys::NamedFramesSystem,
    ecs::sys::HealthSystem,
    ecs::sys::MouseSystem,
    ecs::sys::ButtonSystem,
    ecs::sys::SpawnerSystem,
    ecs::sys::MusicSystem
);

#endif /* GAME_ENGINE_INCLUDES_LIST_HPP */
