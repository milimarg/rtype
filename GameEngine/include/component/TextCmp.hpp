/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Oct, 2024                                                     *
 * Title           - GameEngine                                                        *
 * Description     -                                                                    *
 *     Text                                                                             *
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

#ifndef INCLUDED_TEXT_HPP
    #define INCLUDED_TEXT_HPP

    #include <iostream>
    #include <vector>
    #include <memory>
    #include "Transformable.hpp"
    #include "RaylibWrapper.hpp"

namespace ecs
{
    namespace cmp
    {
        struct Text {
            std::string id{};
            std::vector<std::shared_ptr<std::string>> _parts;
            int fontSize;
            Color color;

            Text(const std::string &id, const std::string &initialPart, int size = 20, Color textColor = WHITE)
                : id(id), fontSize(size), color(textColor)
            {
                if (!initialPart.empty())
                    addPart(initialPart);
            }

            Text(const std::string &id, const std::vector<std::string> &initialParts, int size = 20, Color textColor = WHITE)
                : id(id), fontSize(size), color(textColor)
            {
                for (auto &part : initialParts)
                    if (!part.empty())
                        addPart(part);
            }

            void addPart(const std::string &part)
            {
                _parts.emplace_back(std::make_shared<std::string>(part));
            }

            void addPart(std::shared_ptr<std::string> part)
            {
                _parts.emplace_back(part);
            }

            const std::string &getPart(size_t index) const
            {
                if (index >= _parts.size())
                    throw std::out_of_range("Index out of range");
                return *_parts[index];
            }

            std::string &getPart(size_t index)
            {
                if (index >= _parts.size())
                    throw std::out_of_range("Index out of range");
                return *_parts[index];
            }

            void removePart(size_t index)
            {
                if (index >= _parts.size())
                    throw std::out_of_range("Index out of range");
                _parts.erase(_parts.begin() + index);
            }

            void render(Transformable &pos)
            {
                std::string fullText{};
                for (const auto &part : _parts)
                    fullText += *part;

                DrawText(fullText.c_str(), static_cast<int>(pos.x), static_cast<int>(pos.y), fontSize, color);
            }
        };
    };
};

#endif
