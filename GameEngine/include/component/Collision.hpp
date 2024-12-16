/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     Collision                                                                        *
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

#ifndef COLLISION_HPP
    #define COLLISION_HPP

    #define M_PI 3.14159265358979323846

    #include "Transformable.hpp"
    #include <cmath>
    #include <array>
    #include <ranges>
    #include <functional>
    #include "Registry.hpp"
    #include "Vec2.hpp"

inline float toRadians(float degrees)
{
    return degrees * ((float)M_PI / 180.0f);
}

inline std::pair<float, float> rotatePoint(float x, float y, float angleRad)
{
    float cosAngle = std::cos(angleRad);
    float sinAngle = std::sin(angleRad);

    return {x * cosAngle - y * sinAngle, x * sinAngle + y * cosAngle};
}

namespace ecs
{
    namespace cmp
    {
        struct BoundingBox {
            float x;
            float y;
            float width;
            float height;
            float rotation;

            BoundingBox(float x_ = 0.0f, float y_ = 0.0f, float w = 0.0f, float h = 0.0f, float rot = 0.0f)
                : x(x_), y(y_), width(w), height(h), rotation(rot) {}

            std::array<std::pair<float, float>, 4> getCorners() const
            {
                float angleRad = toRadians(rotation);
                float halfWidth = width / 2.0f;
                float halfHeight = height / 2.0f;

                std::array<std::pair<float, float>, 4> corners = {
                    rotatePoint(-halfWidth, -halfHeight, angleRad),
                    rotatePoint(halfWidth, -halfHeight, angleRad),
                    rotatePoint(halfWidth, halfHeight, angleRad),
                    rotatePoint(-halfWidth, halfHeight, angleRad)
                };

                for (auto &[xPos, yPos] : corners) {
                    xPos += x + halfWidth;
                    yPos += y + halfHeight;
                }

                return corners;
            }

            bool intersects(const BoundingBox &other) const
            {
                auto cornersA = getCorners();
                auto cornersB = other.getCorners();

                for (const auto &[xPos, yPos] : cornersA)
                    if (xPos >= other.x && xPos <= other.x + other.width &&
                        yPos >= other.y && yPos <= other.y + other.height)
                        return true;

                return std::ranges::any_of(cornersB, [this](const auto &corner) {
                    const auto &[xPos, yPos] = corner;
                    return xPos >= x && xPos <= x + width && yPos >= y && yPos <= y + height;
                });
            }
        };

        struct Collision {
            using onCollideFunc = std::function<bool(Registry &, size_t, size_t)>; // ? returns a boolean to know if we need to also execute the default logic

            float width;
            float height;
            onCollideFunc onCollideLeft;
            onCollideFunc onCollideRight;
            Vec2 offset;

            Collision(
                float w,
                float h,
                Vec2 offset = Vec2(0.0f, 0.0f),
                onCollideFunc onCollideLeft = [](Registry &, size_t, size_t){ return true; },
                onCollideFunc onCollideRight = [](Registry &, size_t, size_t){ return false; }
            )
                : width(w), height(h), onCollideLeft(onCollideLeft), onCollideRight(onCollideRight), offset(offset) {}

            ~Collision() = default;

            BoundingBox getBoundingBox(const Transformable &transform) const { // TODO: avoid building everytime we use it -> cache state depending on hashed transform
                float scaledWidth = width * transform.scaleX;
                float scaledHeight = height * transform.scaleY;

                return BoundingBox(
                    transform.x + offset.x,
                    transform.y + offset.y,
                    scaledWidth,
                    scaledHeight,
                    transform.rotation
                );
            }
        };
    };
};

#endif /* COLLISION_HPP */
