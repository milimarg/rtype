/*
** EPITECH PROJECT, 2024
** ECS
** File description:
** ClockSystem.hpp
*/

#ifndef ECS_SCRIPTSYSTEM_HPP
#define ECS_SCRIPTSYSTEM_HPP

#include "Registry.hpp"
#include "Script.hpp"
#include "Using.hpp"
#include "Transformable.hpp"

namespace ecs
{
    USING(
        cmp::Script,
        cmp::Transformable
    );

    namespace sys
    {
        class ScriptSystem {
            public:
                void operator()(
                        Registry &registry,
                        Storage<Script> &scripts,
                        Storage<Transformable> &transformables
                ) const;
            };
    };
};

#endif /* ECS_SCRIPTSYSTEM_HPP */
