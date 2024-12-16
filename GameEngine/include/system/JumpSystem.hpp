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

#ifndef JUMPSYSTEM_HPP
    #define JUMPSYSTEM_HPP

    #include "Registry.hpp"
    #include "Velocity.hpp"
    #include "Acceleration.hpp"
    #include "Jump.hpp"
    #include "Using.hpp"

namespace ecs
{
    USING(
        cmp::Jump,
        cmp::Velocity,
        cmp::Acceleration
    );

    namespace sys
    {
        class JumpSystem {
            public:
                void operator()(
                    [[maybe_unused]] const Registry &registry,
                    Storage<Jump> &jumps,
                    Storage<Velocity> &velocities,
                    Storage<Acceleration> &accelerations
                ) const;
        };
    };
};

#endif /* JUMPSYSTEM_HPP */
