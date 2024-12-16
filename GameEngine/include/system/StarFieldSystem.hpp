/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2024                                                     *
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

#ifndef STARFIELD_SYSTEM_HPP
    #define STARFIELD_SYSTEM_HPP

    #include "Registry.hpp"
    #include "StarField.hpp"
    #include "Using.hpp"
    #include "DrawableCmp.hpp"

namespace ecs
{
    USING(
        cmp::StarField,
        cmp::CameraWrapper,
        cmp::Drawable
    );

    namespace sys
    {
        class StarFieldSystem {
            public:
                explicit StarFieldSystem();

                void operator()(
                    Registry &registry,
                    Storage<StarField> &stars,
                    Storage<CameraWrapper> &cameras,
                    Storage<Drawable> &drawables
                ) const;
        };
    };
};

#endif /* STARFIELD_SYSTEM_HPP */
