/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** Star.hpp
*/

#ifndef B_CPP_500_STG_5_1_RTYPE_NOE_TRITSCH_STARFIELD_HPP
    #define B_CPP_500_STG_5_1_RTYPE_NOE_TRITSCH_STARFIELD_HPP

    #include "Registry.hpp"
    #include "Using.hpp"
    #include "RaylibWrapper.hpp"
    #include <rlgl.h>
    #include <cmath>
    #include "Camera.hpp"
    #include "Vec2.hpp"
    #include "Math.hpp"

static float vector3Distance(Vector3 v1, Vector3 v2)
{
    float result = 0.0f;

    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    float dz = v2.z - v1.z;
    result = sqrtf(dx*dx + dy*dy + dz*dz);

    return result;
}

namespace ecs
{
    namespace cmp
    {
        struct Star
        {
            Color color;
            Vector3 position;
            float radius;
            float twinkleIntensity;
            float twinkleSpeed;

            explicit Star(
                Color color = WHITE,
                Vector3 position = {0.0f, 0.0f, 20.0f},
                float radius = 1.0f
            ) : color(color),
            position(position),
            radius(radius),
            twinkleIntensity(1.0f),
            twinkleSpeed(static_cast<float>(Math::randomInt(1, 20)) / 10.0f)
            {
            }

            inline void updateTwinkle(float globalTwinkleOffset)
            {
                twinkleIntensity = 0.6f + 0.4f * sinf(globalTwinkleOffset * twinkleSpeed);
            }

            inline Color getTwinkleColor() const
            {
                Color twinkleColor = color;
                twinkleColor.a = static_cast<unsigned char>(twinkleIntensity * color.a);
                return twinkleColor;
            }

            void draw(const Model &sphereModel) const
            {
                rlPushMatrix();
                rlTranslatef(position.x, position.y, position.z);
                rlScalef(radius, radius, radius);
                DrawModel(sphereModel, {0, 0, 0}, 1.0f, getTwinkleColor());
                rlPopMatrix();
            }
        };

        struct StarField
        {
            std::vector<Star> stars;
            float baseSpeed = 1.0f;
            float additionalSpeed = 0.0f;
            Vec2 rotationAcc = {0.0f, 0.0f};
            float lastTwinkleUpdate;
            float twinkleUpdateInterval = 0.1f;

            explicit StarField(
                float speed,
                const size_t amount,
                const Vec2 &radiusInterval,
                const Vector3 &position,
                const float radiusPosition
            ) : baseSpeed(speed), lastTwinkleUpdate(0.0f)
            {
                initStars(amount, radiusInterval, position, radiusPosition);
            }

            ~StarField()
            {
                // UnloadModel(sphereModel); // ! unloading it causes it to not be loadable later
            }

            void initStars(size_t amount, const Vec2 &radiusInterval, const Vector3 &position, float radiusPosition)
            {
                float theta{};
                float phi{};
                int radius{};
                Color color{};
                stars.reserve(amount);

                for (size_t i = 0; i < amount; ++i) {
                    theta = static_cast<float>(Math::randomInt(0, 360)) * DEG2RAD;
                    phi = static_cast<float>(Math::randomInt(0, 360)) * DEG2RAD;
                    radius = Math::randomInt(static_cast<int>(radiusInterval.x), static_cast<int>(radiusInterval.y));

                    if (Math::randomInt(1, 100) > 90) {
                        color = {
                            static_cast<unsigned char>(Math::randomInt(10, 50)), 0,
                            static_cast<unsigned char>(Math::randomInt(1, 150)),
                            static_cast<unsigned char>(Math::randomInt(50, 255))
                        };
                    } else {
                        color = {
                            static_cast<unsigned char>(Math::randomInt(180, 255)),
                            static_cast<unsigned char>(Math::randomInt(180, 255)),
                            static_cast<unsigned char>(Math::randomInt(180, 255)),
                            static_cast<unsigned char>(Math::randomInt(200, 255))
                        };
                    }

                    stars.emplace_back(
                        color,
                        Vector3{
                            position.x + radiusPosition * sinf(theta) * cosf(phi),
                            position.y + radiusPosition * sinf(theta) * sinf(phi),
                            position.z + radiusPosition * cosf(theta)
                        },
                        static_cast<float>(radius) / 100.0f
                    );
                }
            }

            void update()
            {
                rotationAcc.y += baseSpeed + additionalSpeed;
                float currentTime = GetTime();
                if (currentTime - lastTwinkleUpdate <= twinkleUpdateInterval)
                    return;

                float globalTwinkleOffset = currentTime;
                for (auto &star : stars)
                    star.updateTwinkle(globalTwinkleOffset);
                lastTwinkleUpdate = currentTime;
            }

            void draw(const CameraWrapper &camera)
            {
                static const Model sphereModel = LoadModelFromMesh(GenMeshSphere(1.0f, 8, 8));

                rlPushMatrix();
                rlRotatef(rotationAcc.x, 1.0f, 0.0f, 0.0f);
                rlRotatef(rotationAcc.y, 0.0f, 1.0f, 0.0f);

                for (auto &star : stars)
                    if (vector3Distance(star.position, camera.camera.position) > 2.0f)
                        star.draw(sphereModel);

                rlPopMatrix();
            }
        };
    };
};

#endif /* B_CPP_500_STG_5_1_RTYPE_NOE_TRITSCH_STARFIELD_HPP */
