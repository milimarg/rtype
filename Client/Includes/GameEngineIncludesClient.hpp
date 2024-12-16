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

#ifndef GAME_ENGINE_INCLUDES_CLIENT_HPP
    #define GAME_ENGINE_INCLUDES_CLIENT_HPP

    #include <iostream>
    #define NOGDI

    #include "Registry.hpp"
    #include "Zipper.hpp"

    // ? component list
    #include "Transformable.hpp"
    #include "Velocity.hpp"
    #include "DrawableCmp.hpp"
    #include "RectangleRender.hpp"
    #include "Input.hpp"
    #include "Acceleration.hpp"
    #include "Gravity.hpp"
    #include "Collision.hpp"
    #include "Red.hpp"
    #include "Animated.hpp"
    #include "TimerContainer.hpp"
    #include "NamedFrames.hpp"
    #include "Health.hpp"
    #include "Spawner.hpp"
    #include "Camera.hpp"
    #include "StarField.hpp"
    #include "SoundSource.hpp"
    #include "MusicSource.hpp"
    #include "Mouse.hpp"
    #include "Button.hpp"
    #include "UUID.hpp"
    #include "TextCmp.hpp"
    #include "HUD.hpp"

    // ? system list
    #include "TransformSystem.hpp"
    #include "InputSystem.hpp"
    #include "DrawSystem.hpp"
    #include "GravitySystem.hpp"
    #include "CollisionSystem.hpp"
    #include "ClockSystem.hpp"
    #include "AnimationSystem.hpp"
    #include "NamedFramesSystem.hpp"
    #include "HealthSystem.hpp"
    #include "CameraSystem.hpp"
    #include "StarFieldSystem.hpp"
    #include "MouseSystem.hpp"
    #include "ButtonSystem.hpp"
    #include "MusicSystem.hpp"

    #include "Using.hpp"
    #include "utils.hpp"
    #include "Vec2.hpp"

    #include "AssetManager.hpp"
    #include "ClientSceneLoader.hpp"

    #include "TestError.hpp"
    #include "Math.hpp"

    #include "UUIDManager.hpp"

USING(
    ecs::Registry,
    ecs::AssetManager,
    ecs::UUIDManager,

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
    ecs::cmp::CameraWrapper,
    ecs::cmp::StarField,
    ecs::cmp::SoundSource,
    ecs::cmp::MusicSource,
    ecs::cmp::Mouse,
    ecs::cmp::Button,
    ecs::cmp::UUID,
    ecs::cmp::Text,
    ecs::cmp::HUD,

    ecs::sys::TransformSystem,
    ecs::sys::InputSystem,
    ecs::sys::DrawSystem,
    ecs::sys::GravitySystem,
    ecs::sys::CollisionSystem,
    ecs::sys::ClockSystem,
    ecs::sys::AnimationSystem,
    ecs::sys::NamedFramesSystem,
    ecs::sys::HealthSystem,
    ecs::sys::CameraSystem,
    ecs::sys::StarFieldSystem,
    ecs::sys::MouseSystem,
    ecs::sys::ButtonSystem,
    ecs::sys::MusicSystem
);

#endif /* GAME_ENGINE_INCLUDES_CLIENT_HPP */
