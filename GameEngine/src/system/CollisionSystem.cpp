/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-rtype-noe.tritsch                                *
 * Description     -                                                                    *
 *     CollisionSystem                                                                  *
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

#include "CollisionSystem.hpp"
#include "IndexedZipper.hpp"
#include "Using.hpp"
#include "utils.hpp"
#include "TimerContainer.hpp"

USING(
    ecs::IndexedZipper,
    ecs::sys::CollisionSystem,
    ecs::cmp::Transformable,
    ecs::cmp::Acceleration,
    ecs::cmp::BoundingBox,
    ecs::cmp::Collision,
    ecs::cmp::Gravity,
    ecs::cmp::Velocity,
    ecs::cmp::TimerContainer
);

void CollisionSystem::operator()(
    Registry &registry,
    Storage<Transformable> &transformations,
    Storage<Collision> const &collisions,
    Storage<Velocity> &velocities,
    Storage<Acceleration> &accelerations,
    Storage<Gravity> &gravityList
) const
{
    for (auto &&[i, collision1, transformation1] : IndexedZipper(collisions, transformations)) {
        BoundingBox box1 = collision1.getBoundingBox(transformation1);
        auto &onCollideLeft = collision1.onCollideLeft;

        for (auto &&[i2, collision2, transformation2] : IndexedZipper(collisions, transformations)) {
            if (i == i2)
                continue;

            BoundingBox box2 = collision2.getBoundingBox(transformation2);
            auto &onCollideRight = collision2.onCollideRight;

            if (box1.intersects(box2)) {
                bool ignoreBaseLogicLeft = !onCollideLeft(registry, i, i2);
                bool ignoreBaseLogicRight = !onCollideRight(registry, i2, i);
                if (ignoreBaseLogicLeft || ignoreBaseLogicRight)
                    continue;

                float overlapX = std::min(box1.x + box1.width, box2.x + box2.width) - std::max(box1.x, box2.x);
                float overlapY = std::min(box1.y + box1.height, box2.y + box2.height) - std::max(box1.y, box2.y);
                bool overlapInX = overlapX < overlapY;

                float xAdjustment1 = (transformation1.x < transformation2.x ? -overlapX : overlapX) * static_cast<float>(overlapInX);
                float yAdjustment1 = (transformation1.y < transformation2.y ? -overlapY : overlapY) * static_cast<float>(!overlapInX);

                if (transformation1.canMove) {
                    transformation1.x += xAdjustment1;
                    transformation1.y += yAdjustment1;

                    if (velocities[i].has_value()) {
                        auto &[vx, vy, _, __] = velocities[i].value();
                        if (overlapInX)
                            vx = 0.0f;
                        else
                            vy = 0.0f;
                    }

                    if (accelerations[i].has_value()) {
                        auto &[ax, ay, _, __] = accelerations[i].value();
                        if (overlapInX)
                            ax = 0.0f;
                        else
                            ay = 0.0f;
                    }

                    if (!overlapInX && gravityList[i].has_value()) {
                        auto &[velocityY, _] = gravityList[i].value();
                        velocityY = 0.0f;
                    }
                }

                float xAdjustment2 = (transformation2.x < transformation1.x ? -overlapX : overlapX) * static_cast<float>(overlapInX);
                float yAdjustment2 = (transformation2.y < transformation1.y ? -overlapY : overlapY) * static_cast<float>(!overlapInX);

                if (transformation2.canMove) {
                    transformation2.x += xAdjustment2;
                    transformation2.y += yAdjustment2;

                    if (velocities[i2].has_value()) {
                        auto &[vx, vy, _, __] = velocities[i2].value();
                        if (overlapInX)
                            vx = 0.0f;
                        else
                            vy = 0.0f;
                    }

                    if (accelerations[i2].has_value()) {
                        auto &[ax, ay, _, __] = accelerations[i2].value();
                        if (overlapInX)
                            ax = 0.0f;
                        else
                            ay = 0.0f;
                    }

                    if (!overlapInX && gravityList[i2].has_value()) {
                        auto &[velocityY, _] = gravityList[i2].value();
                        velocityY = 0.0f;
                    }
                }
            }
        }
    }
}
