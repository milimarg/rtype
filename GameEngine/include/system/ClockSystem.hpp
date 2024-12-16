/*
** EPITECH PROJECT, 2024
** ECS
** File description:
** ClockSystem.hpp
*/

#ifndef ECS_CLOCKSYSTEM_HPP
    #define ECS_CLOCKSYSTEM_HPP

    #include "TimerContainer.hpp"
    #include "Using.hpp"
    #include "Registry.hpp"
    #include <chrono>

namespace ecs
{
    USING(
        cmp::TimerContainer
    );

    namespace sys
    {
        class ClockSystem {
            public:
                ClockSystem();

                void operator()(
                    [[maybe_unused]] const Registry &registry,
                    Storage<TimerContainer> &timerContainers
                ) const;

            private:
                mutable std::chrono::steady_clock::time_point lastTime;
        };
    };
};

#endif /* ECS_CLOCKSYSTEM_HPP */
