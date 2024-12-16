/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     Acceleration                                                                     *
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

#ifndef ACCELERATION_HPP
    #define ACCELERATION_HPP

namespace ecs
{
    namespace cmp
    {
        struct Acceleration {
            float ax;
            float ay;
            float angularAcc;
            float decelerationFactor; // ? lower = slower / 1.0f = stable

            Acceleration(float ax_ = 0.0f, float ay_ = 0.0f, float angularAcc_ = 0.0f, float decFactor = 0.97f)
                : ax(ax_), ay(ay_), angularAcc(angularAcc_), decelerationFactor(decFactor) {}
        };
    };
};

#endif /* ACCELERATION_HPP */
