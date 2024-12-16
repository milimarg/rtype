/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     Animated                                                                         *
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

#ifndef ANIMATED_HPP
    #define ANIMATED_HPP

    #include <vector>
    #include "RaylibWrapper.hpp"

namespace ecs
{
    namespace cmp
    {
        struct Animated {
            std::vector<Rect> frames;
            float frameDuration;
            size_t currentFrame = 0;

            explicit Animated(float frameDuration)
                : frameDuration(frameDuration) {}

            explicit Animated(const Rect &rect, size_t frameCount, float frameDuration) // ? construct by giving a rectangle and replicating it on the right
                : frameDuration(frameDuration)
            {
                frames.reserve(frameCount);
                for (size_t i = 0; i < frameCount; ++i)
                    frames.emplace_back(rect.x + rect.width * static_cast<float>(i), rect.y, rect.width, rect.height);
            }

            explicit Animated(const Rect &rect, size_t horizontalFrameCount, size_t verticalFrameCount, float frameDuration) // ? construct by giving a rectangle and replicating it horizontally and vertically
                : frameDuration(frameDuration)
            {
                frames.reserve(horizontalFrameCount * verticalFrameCount);
                for (size_t i = 0; i < verticalFrameCount; ++i) {
                    for (size_t j = 0; j < horizontalFrameCount; ++j) {
                        frames.emplace_back(
                            rect.x + rect.width * static_cast<float>(j),
                            rect.y + rect.height * static_cast<float>(i),
                            rect.width,
                            rect.height
                        );
                    }
                }
            }

            void addFrame(const Rect &frame)
            {
                frames.push_back(frame); // TODO: use emplace instead of push -> variadic args to build rectangle in place
            }
        };
    };
};

#endif /* ANIMATED_HPP */
