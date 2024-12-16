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

#include "CameraSystem.hpp"
#include "Zipper.hpp"

USING(
    ecs::cmp::CameraWrapper,
    ecs::sys::CameraSystem
);

void CameraSystem::operator()(
    [[maybe_unused]] const ecs::Registry &registry,
    Storage<CameraWrapper> &cameras3D
) const
{
    for (auto &&[camera3D] : Zipper(cameras3D)) {
        auto &[camera, mode] = camera3D;

        UpdateCamera(&camera, mode);
    }
}
