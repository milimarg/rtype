/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     TransformSystem                                                                  *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                     *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                     *
 *             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                     *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "TransformSystem.hpp"
#include "IndexedZipper.hpp"
#include "TimerContainer.hpp"
#include "Using.hpp"
#include "utils.hpp"

USING(
    ecs::sys::TransformSystem,
    ecs::cmp::Transformable,
    ecs::cmp::Velocity,
    ecs::cmp::TimerContainer
);

void TransformSystem::operator()(
    Registry &registry,
    Storage<Transformable> &transformations,
    Storage<Velocity> &velocities
) const
{
    for (auto &&[i, transformation, velocity] : IndexedZipper(transformations, velocities)) {
        if (!ACTION_READY_FULL(i, "transform update"))
            continue;

        auto &[x, y, _, __, ___, canMove] = transformation;
        auto &[vx, vy, ____, maxVelocity] = velocity;

        if (!canMove)
            return;

        if (maxVelocity != 0.0f) {
            vx = std::clamp(vx, -maxVelocity, maxVelocity);
            vy = std::clamp(vy, -maxVelocity, maxVelocity);
        }

        x += vx;
        y += vy;
    }
}
