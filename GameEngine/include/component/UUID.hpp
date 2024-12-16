/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Thu, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     UUID                                                                             *
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

#ifndef UUID_HPP
    #define UUID_HPP

    #include <string>
    #include <array>
    #include <cstddef>

namespace ecs
{
    namespace cmp
    {
        using UUID = std::array<unsigned char, 16>;
        using UUIDStr = std::string;
    };
};

namespace std
{
    template <>
    struct hash<ecs::cmp::UUID> {
        std::size_t operator()(const ecs::cmp::UUID &uuid) const noexcept {
            std::size_t hashValue = 0;
            for (const auto  byte : uuid)
                hashValue = (hashValue << 8) ^ byte;
            return hashValue;
        }
    };
}

#endif /* UUID_HPP */
