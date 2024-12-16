/*
** EPITECH PROJECT, 2024
** ECS
** File description:
** ClockSystem.hpp
*/

#ifndef NAMEDFRAMESSYSTEM_HPP
    #define NAMEDFRAMESSYSTEM_HPP

    #include "Registry.hpp"
    #include "NamedFrames.hpp"
    #include "SpriteRender.hpp"
    #include "Using.hpp"

namespace ecs
{
    USING(
        cmp::NamedFrames,
        cmp::SpriteRender
    );

    namespace sys
    {
        class NamedFramesSystem {
            public:
                void operator()(
                    Registry &registry,
                    Storage<NamedFrames> &namedFrames,
                    Storage<SpriteRender> &sprites
                ) const;
        };
    };
};

#endif /* NAMEDFRAMESSYSTEM_HPP */
