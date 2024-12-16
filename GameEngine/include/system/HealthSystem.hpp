/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     HealthSystem                                                                     *
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

#ifndef HEALTHSYSTEM_HPP
    #define HEALTHSYSTEM_HPP

    #include "Registry.hpp"
    #include "Health.hpp"
    #include "TimerContainer.hpp"
    #include "Using.hpp"

namespace ecs
{
    USING(
        cmp::Health,
        cmp::TimerContainer
    );

    namespace sys
    {
        class HealthSystem {
            public:
                void operator()(
                    Registry &registry,
                    Storage<Health> &healthCounts,
                    Storage<TimerContainer> &timerContainers
                ) const;
        };
    };
};

#endif /* HEALTHSYSTEM_HPP */
