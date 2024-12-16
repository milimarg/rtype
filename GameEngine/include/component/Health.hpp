/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-rtype-noe.tritsch                                *
 * Description     -                                                                    *
 *     Health                                                                           *
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

#ifndef HEALTH_HPP
    #define HEALTH_HPP

    #include <iostream>
    #include <functional>
    #include "Red.hpp"
    #include "Registry.hpp"

namespace ecs
{
    namespace cmp
    {
        struct Health {
            using onDeathFunc = std::function<void(Registry &, size_t)>;
            using onDamageTakenFunc = std::function<void(Registry &, size_t)>;

            int max;
            int current;
            int damageToTake;
            float damageDisplayDuration;
            onDeathFunc onDeath;
            onDamageTakenFunc onDamageTaken;

            Health(
                int max_,
                onDeathFunc onDeath = [](Registry &, size_t){},
                onDamageTakenFunc onDamageTaken = [](Registry &r, size_t i){ r.safeEmplaceComponent<Red>(i); },
                float damageDisplayDuration_ = 0.2f
            )
                : max(max_),
                current(max_),
                damageToTake(0),
                damageDisplayDuration(damageDisplayDuration_),
                onDeath(onDeath),
                onDamageTaken(onDamageTaken)
                {}

            void takeDamage(int damage)
            {
                damageToTake += damage;
            }

            void reset()
            {
                current = max;
            }
        };
    };
};

#endif /* HEALTH_HPP */
