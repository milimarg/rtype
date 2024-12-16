/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     GravitySystem                                                                    *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄             *
 *      ▐  ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀            *
 *        █▄▄▄▄▄  ▐  █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             *
 *        █    ▌     █          █        █        █    ▌    █         █   █             *
 *       ▄▀▄▄▄▄    ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             *
 *       █    ▐   █         █       █ █          █    ▐   █     ▐   █   █               *
 *       ▐        ▐         ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "GravitySystem.hpp"
#include "IndexedZipper.hpp"
#include "TimerContainer.hpp"
#include "Using.hpp"
#include "utils.hpp"
#include <Velocity.hpp>

USING(
    ecs::sys::GravitySystem,
    ecs::cmp::TimerContainer,
    ecs::cmp::Velocity
);

void GravitySystem::operator()(
    Registry &registry,
    Storage<Gravity> &gravityList,
    Storage<Transformable> &transformations,
    Storage<Acceleration> const &accelerations
) const
{
    for (auto &&[i, gravity, transformation, acceleration] : IndexedZipper(gravityList, transformations, accelerations)) {
        if (!ACTION_READY_FULL(i, "gravity update"))
            continue;

        auto &[x, y, _, __, ___, canMove] = transformation;
        auto &[velocityY, accelerationY] = gravity;
        const auto &[____, ay, _____, maxSpeed] = acceleration;

        if (!canMove)
            continue;

        velocityY += accelerationY;

        velocityY += ay;

        try {
            float maxVelocity = registry.getComponent<Velocity>(i).maxVelocity;
            if (maxVelocity != 0.0f)
                velocityY = std::clamp(velocityY, -maxVelocity, maxVelocity);
        } catch (...) {}

        y += velocityY;
    }
}
