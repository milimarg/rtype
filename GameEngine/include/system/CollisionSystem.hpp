/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
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

#ifndef COLLISIONSYSTEM_HPP
    #define COLLISIONSYSTEM_HPP

    #include "Registry.hpp"
    #include "Transformable.hpp"
    #include "Collision.hpp"
    #include "Velocity.hpp"
    #include "Acceleration.hpp"
    #include "Gravity.hpp"
    #include "Using.hpp"

namespace ecs
{
    USING(
        cmp::Transformable,
        cmp::Collision,
        cmp::BoundingBox,
        cmp::Velocity,
        cmp::Acceleration,
        cmp::Gravity
    );

    namespace sys
    {
        class CollisionSystem {
            public:
                void operator()(
                    Registry &registry,
                    Storage<Transformable> &transformations,
                    Storage<Collision> const &collisions,
                    Storage<Velocity> &velocities,
                    Storage<Acceleration> &accelerations,
                    Storage<Gravity> &gravityList
                ) const;
        };
    };
};

#endif /* COLLISIONSYSTEM_HPP */
