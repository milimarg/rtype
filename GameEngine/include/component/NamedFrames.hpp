/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-rtype-noe.tritsch                                *
 * Description     -                                                                    *
 *     NamedFrames                                                                      *
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

#ifndef NAMED_FRAMES_HPP
    #define NAMED_FRAMES_HPP

    #include <unordered_map>
    #include <string>
    #include "RaylibWrapper.hpp"
    #include "StringHash.hpp"

namespace ecs
{
    namespace cmp
    {
        struct NamedFrames {
            std::unordered_map<std::string, Rect, StringHash, std::equal_to<>> frames{};
            std::string currentFrame{};
            bool updatedFrame{};

            NamedFrames(const std::unordered_map<std::string, Rect, StringHash, std::equal_to<>> &framesToAdd = {}, const std::string &startFrame = "")
            {
                for (const auto &[id, rect] : framesToAdd)
                    frames[id] = rect;
                if (!startFrame.empty())
                    setCurrentFrame(startFrame);
            }

            void addFrame(const std::string &id, const Rect &frame)
            {
                frames[id] = frame;
            }

            Rect getFrame(const std::string &id) const
            {
                auto it = frames.find(id);
                if (it != frames.end())
                    return it->second;
                return Rect{0, 0, 0, 0};
            }

            void setCurrentFrame(const std::string &id)
            {
                if (!frames.contains(id))
                    return;

                currentFrame = id;
                updatedFrame = true;
            }

            Rect getCurrentFrame()
            {
                auto it = frames.find(currentFrame);
                if (it != frames.end()) {
                    updatedFrame = false;
                    return it->second;
                }
                return Rect{0, 0, 0, 0};
            }
        };
    };
};

#endif /* NAMED_FRAMES_HPP */
