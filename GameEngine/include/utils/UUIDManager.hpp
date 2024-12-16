/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Oct, 2024                                                     *
 * Title           - GameEngine                                                        *
 * Description     -                                                                    *
 *     UUIDManager                                                                      *
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

#ifndef INCLUDED_UUIDMANAGER_HPP
    #define INCLUDED_UUIDMANAGER_HPP

    #include <iostream>
    #include <iomanip>
    #include <sstream>
    #include <random>
    #include <array>
    #include <stdexcept>
    #include <Registry.hpp>
    #include "IndexedZipper.hpp"
    #include "UUID.hpp"
    #include "Using.hpp"

namespace ecs
{
    USING(cmp::UUID, cmp::UUIDStr);

    class UUIDManager {
        public:
            static UUID generateUUID();
            static size_t findEntityByUUID(const UUID &uuid);
            static size_t findEntityByUUID(const UUIDStr &uuid);
            static UUID toUUIDNumber(const UUIDStr &uuid);
            static UUIDStr toUUIDString(const UUID &uuid);

        private:
            UUIDManager() = delete;
    };
}

#endif /* INCLUDED_UUIDMANAGER_HPP */
