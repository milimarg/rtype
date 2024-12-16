/*
** EPITECH PROJECT, 2024
** ECS
** File description:
** ClockSystem.hpp
*/

#ifndef ECS_ANIMATIONSYSTEM_HPP
    #define ECS_ANIMATIONSYSTEM_HPP

    #include "Registry.hpp"
    #include "TimerContainer.hpp"
    #include "Animated.hpp"
    #include "SpriteRender.hpp"
    #include "Using.hpp"

namespace ecs
{
    USING(
        cmp::TimerContainer,
        cmp::Animated,
        cmp::SpriteRender
    );

    namespace sys
    {
        class AnimationSystem {
            public:
                void operator()(
                    [[maybe_unused]] const Registry &registry,
                    Storage<Animated> &animations,
                    Storage<SpriteRender> &sprites,
                    Storage<TimerContainer> &timerContainers
                ) const;
        };
    };
};

#endif /* ECS_ANIMATIONSYSTEM_HPP */
