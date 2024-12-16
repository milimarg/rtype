/*
** EPITECH PROJECT, 2024
** ECS
** File description:
** ClockSystem.cpp
*/

#include "ClockSystem.hpp"
#include "Zipper.hpp"

USING(
    ecs::cmp::TimerContainer,
    ecs::sys::ClockSystem
);

ClockSystem::ClockSystem() : lastTime(std::chrono::steady_clock::now()) {}

void ClockSystem::operator()(
    [[maybe_unused]] const Registry &registry,
    Storage<TimerContainer> &timerContainers
) const
{
    auto currentTime = std::chrono::steady_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
    lastTime = currentTime;

    for (auto &&[timerContainer] : Zipper(timerContainers)) {
        auto &[timers] = timerContainer;

        for (auto &[_, timer] : timers)
            timer.updateTime(deltaTime);
    }
}
