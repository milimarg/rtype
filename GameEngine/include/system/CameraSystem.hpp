/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Oct, 2024                                                     *
 * Title           - GameEngine                                                        *
 * Description     -                                                                    *
 *     CameraSystem                                                                     *
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

#ifndef B_CPP_500_STG_5_1_RTYPE_NOE_TRITSCH_CAMERASYSTEM_HPP
    #define B_CPP_500_STG_5_1_RTYPE_NOE_TRITSCH_CAMERASYSTEM_HPP

    #include "Using.hpp"
    #include "Camera.hpp"
    #include "Registry.hpp"

namespace ecs
{
    USING(
        ecs::cmp::CameraWrapper
    )

    namespace sys
    {
        class CameraSystem {
        public:
            void operator()(
                    [[maybe_unused]] const Registry &registry,
                    Storage<CameraWrapper> &camera3D
            ) const;
        };
    };
};

#endif /* B_CPP_500_STG_5_1_RTYPE_NOE_TRITSCH_CAMERASYSTEM_HPP */
