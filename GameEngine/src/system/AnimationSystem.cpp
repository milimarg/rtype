/*
** EPITECH PROJECT, 2024
** ECS
** File description:
** AnimationSystem.cpp
*/

#include "AnimationSystem.hpp"
#include "Zipper.hpp"
#define NOGDI
#include "utils.hpp"

USING(
    ecs::sys::AnimationSystem,
    ecs::cmp::Timer
);

void AnimationSystem::operator()(
    [[maybe_unused]] const Registry &registry,
    Storage<Animated> &animations,
    Storage<SpriteRender> &sprites,
    Storage<TimerContainer> &timerContainers
) const
{
    for (auto &&[timers, animation, sprite] : Zipper(timerContainers, animations, sprites)) {
        if (!ACTION_READY("animation update"))
            continue;

        auto &[frames, frameDuration, currentFrame] = animation;

        if (timers.safeConsume("animation", frameDuration)) {
            currentFrame = (currentFrame + 1) % frames.size();
            sprite.setTextureRect(frames[currentFrame]);
        }
    }
}
