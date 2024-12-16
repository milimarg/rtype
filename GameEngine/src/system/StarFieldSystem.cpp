/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     StarFieldSystem                                                                       *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░             *
 *         ▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓             *
 *         █  ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "StarFieldSystem.hpp"
#include "Zipper.hpp"
#include "IndexedZipper.hpp"
#include "Using.hpp"
#include "utils.hpp"
#include "Camera.hpp"
#include "Velocity.hpp"

USING(
    ecs::IndexedZipper,
    ecs::sys::StarFieldSystem,
    ecs::cmp::Velocity
);

StarFieldSystem::StarFieldSystem() = default;

void StarFieldSystem::operator()(
    Registry &registry,
    Storage<StarField> &starfield,
    Storage<CameraWrapper> &cameras,
    Storage<Drawable> &drawables
) const
{
    for (auto &&[stars, _] : Zipper(starfield, drawables)) {
        stars.update();

        for (auto &&[camera] : Zipper(cameras)) {
            BeginMode3D(camera.camera);
            stars.draw(camera);
            EndMode3D();
        }
    }
}
