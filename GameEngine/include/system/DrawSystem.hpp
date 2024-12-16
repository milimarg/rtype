/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     DrawSystem                                                                       *
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

#ifndef DRAW_SYSTEM_HPP
    #define DRAW_SYSTEM_HPP

    #include "Registry.hpp"
    #include "Transformable.hpp"
    #include "DrawableCmp.hpp"
    #include "SpriteRender.hpp"
    #include "RectangleRender.hpp"
    #include "Red.hpp"
    #include "Using.hpp"
    #include "Camera.hpp"
    #include "TextCmp.hpp"

namespace ecs
{
    USING(
        cmp::Drawable,
        cmp::Transformable,
        cmp::SpriteRender,
        cmp::RectangleRender,
        cmp::Red,
        cmp::Text
    );

    namespace sys
    {
        class DrawSystem {
            public:
                explicit DrawSystem();

                void operator()(
                    Registry &registry,
                    Storage<Drawable> &drawables,
                    Storage<Transformable> &transforms,
                    Storage<SpriteRender> &sprites,
                    Storage<RectangleRender> &rectangles,
                    Storage<Text> &texts
                ) const;
        };
    };
};

#endif /* DRAW_SYSTEM_HPP */
