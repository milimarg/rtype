/*
** EPITECH PROJECT, 2024
** ECS
** File description:
** ScriptSystem.cpp
*/

#include "ScriptSystem.hpp"
#include "IndexedZipper.hpp"
#include "utils.hpp"
#include "TimerContainer.hpp"

USING(
    ecs::sys::ScriptSystem,
    ecs::cmp::TimerContainer
);

void ScriptSystem::operator()(
        Registry &registry,
        Storage<Script> &scripts,
        Storage<Transformable> &transformables
) const
{
    for (auto &&[i, script, transformable] : IndexedZipper(scripts, transformables)) {
        if (!ACTION_READY_FULL(i, "script update"))
            continue;

        script.scriptVariables["x"] = transformable.x;
        script.scriptVariables["y"] = transformable.y;
        script.runScripts();
        transformable.x = script.scriptVariables["x"];
        transformable.y = script.scriptVariables["y"];
    }
}
