/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     HealthSystem                                                                       *
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

#include "HealthSystem.hpp"
#include "IndexedZipper.hpp"
#include "Using.hpp"
#include "utils.hpp"
#include "Red.hpp"

USING(
    ecs::sys::HealthSystem,
    ecs::cmp::Red
);

void HealthSystem::operator()(
    Registry &registry,
    Storage<Health> &healthCounts,
    Storage<TimerContainer> &timerContainers
) const
{
    for (auto &&[i, timers, healthCount] : IndexedZipper(timerContainers, healthCounts))
        if (timers.safeUniqueConsume("damage display", healthCount.damageDisplayDuration))
            registry.removeComponent<Red>(i);

    for (auto &&[i, healthCount, timers] : IndexedZipper(healthCounts, timerContainers)) {
        if (!ACTION_READY("health update"))
            continue;

        auto &[_, current, damage, __, onDeath, onDamageTaken] = healthCount;

        if (damage) {
            current -= damage;
            damage = 0;
            onDamageTaken(registry, i);
        }

        if (current <= 0) {
            onDeath(registry, i);
            registry.killEntity(i);
        }
    }
}
