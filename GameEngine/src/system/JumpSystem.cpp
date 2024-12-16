/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     JumpSystem                                                                       *
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

#include "JumpSystem.hpp"
#include "Zipper.hpp"
#include "Using.hpp"
#include "RaylibWrapper.hpp"

USING(
    ecs::sys::JumpSystem,
    ecs::cmp::Jump,
    ecs::cmp::Velocity,
    ecs::cmp::Acceleration
);

void JumpSystem::operator()(
    [[maybe_unused]] const Registry &registry,
    Storage<Jump> &jumps,
    Storage<Velocity> &velocities,
    Storage<Acceleration> &accelerations
) const
{
    for (auto &&[jump, velocity, acceleration] : Zipper(jumps, velocities, accelerations)) {
        auto &[jumpStrength, isGrounded] = jump;
        auto &[_, vy, __, ___] = velocity;
        auto &[____, ay, _____, decelerationFactor] = acceleration;

        if (vy == 0.0f)
            isGrounded = true;
        else
            isGrounded = false;

        if (isGrounded && G.isKeyPressed(KEY_SPACE)) {
            ay = -jumpStrength;
            isGrounded = false;
        } else {
            ay = 0.0f;

            if (!isGrounded)
                vy *= decelerationFactor;
        }
    }
}
