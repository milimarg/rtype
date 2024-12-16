/*
** EPITECH PROJECT, 2024
** ECS
** File description:
** NamedFramesSystem.cpp
*/

#include "NamedFramesSystem.hpp"
#include "IndexedZipper.hpp"
#define NOGDI
#include "utils.hpp"
#include "TimerContainer.hpp"

USING(
    ecs::sys::NamedFramesSystem,
    ecs::cmp::TimerContainer
);

void NamedFramesSystem::operator()(
    Registry &registry,
    Storage<NamedFrames> &namedFrames,
    Storage<SpriteRender> &sprites
) const
{
    for (auto &&[i, namedFrameList, sprite] : IndexedZipper(namedFrames, sprites)) {
        if (!ACTION_READY_FULL(i, "named frame update"))
            continue;

        auto &[_, __, updatedFrame] = namedFrameList;

        if (updatedFrame)
            sprite.setTextureRect(namedFrameList.getCurrentFrame());
    }
}
