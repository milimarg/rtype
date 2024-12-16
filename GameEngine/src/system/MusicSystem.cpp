/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sun, Oct, 2024                                                     *
 * Title           - GameEngine                                                         *
 * Description     -                                                                    *
 *     MusicSystem                                                                      *
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

#include "MusicSystem.hpp"
#include "Zipper.hpp"

USING(ecs::sys::MusicSystem);

void MusicSystem::operator()(
    [[maybe_unused]] const Registry &registry,
    Storage<MusicSource> &musicSources
) const
{
    for (auto &&[musicSource] : Zipper(musicSources))
        musicSource.updateAll();
}
