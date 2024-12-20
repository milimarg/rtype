/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Nov, 2024                                                     *
 * Title           - GameEngine                                                         *
 * Description     -                                                                    *
 *     Vec2                                                                             *
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

#include "Vec2.hpp"
#include "Math.hpp"

Vec2::Vec2(float e0, float e1)
    : x(e0), y(e1)
{}

Vec2 Vec2::operator-() const
{
    return Vec2(-x, -y);
}

Vec2 &Vec2::operator+=(const Vec2 &v)
{
    x += v.x;
    y += v.y;
    return *this;
}

Vec2 &Vec2::operator*=(float t)
{
    x *= t;
    y *= t;
    return *this;
}

Vec2 &Vec2::operator/=(float t)
{
    return *this *= 1 / t;
}
