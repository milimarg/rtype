/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     RectangleRender                                                                  *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄             *
 *      ▐  ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀            *
 *        █▄▄▄▄▄  ▐  █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             *
 *        █    ▌     █          █        █        █    ▌    █         █   █             *
 *       ▄▀▄▄▄▄    ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             *
 *       █    ▐   █         █       █ █          █    ▐   █     ▐   █   █               *
 *       ▐        ▐         ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#ifndef RECTANGLE_RENDER_HPP
    #define RECTANGLE_RENDER_HPP

    #include "RaylibWrapper.hpp"
    #include "Transformable.hpp"
    #include "Vec2.hpp"

namespace ecs
{
    namespace cmp
    {
        struct RectangleRender {
            Vec2 _size = {10.0f, 10.0f};
            Color _color = WHITE;
            float _rotation = 0.0f;

            void setSize(const Vec2 &size)
            {
                _size = size;
            }

            void setColor(const Color &color)
            {
                _color = color;
            }

            void draw(const Transformable &transformable) const
            {
                DrawRectanglePro(
                    Rectangle(transformable.x, transformable.y, _size.x * transformable.scaleX, _size.y * transformable.scaleY),
                    {0, 0},
                    transformable.rotation,
                    _color
                );
            }
        };
    };
};

#endif /* RECTANGLE_RENDER_HPP */
