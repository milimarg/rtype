/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     Transformable                                                                        *
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

#ifndef TRANSFORM_HPP
    #define TRANSFORM_HPP

namespace ecs
{
    namespace cmp
    {
        struct Transformable {
            float x = 0.0f;
            float y = 0.0f;
            float rotation = 0.0f;
            float scaleX = 1.0f;
            float scaleY = 1.0f;
            bool canMove = true;

            Transformable(float x, float y, float rotation = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f, bool canMove = true) :
                x(x), y(y),
                rotation(rotation),
                scaleX(scaleX), scaleY(scaleY),
                canMove(canMove)
            {}

            Transformable(float x, float y, bool canMove) :
                x(x), y(y),
                canMove(canMove)
            {}
        };
    };
};

#endif /* TRANSFORM_HPP */
