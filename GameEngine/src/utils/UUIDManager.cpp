/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Oct, 2024                                                     *
 * Title           - GameEngine                                                        *
 * Description     -                                                                    *
 *     UUIDManager                                                                      *
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

#include "UUIDManager.hpp"
#include <cstring>

USING(
    ecs::UUIDManager,
    ecs::cmp::UUID,
    ecs::cmp::UUIDStr
);

UUID UUIDManager::generateUUID()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dist(0, std::numeric_limits<uint32_t>::max());

    uint32_t data1 = dist(gen);
    uint16_t data2 = dist(gen) & 0xFFFF;
    uint16_t data3 = dist(gen) & 0xFFFF;
    uint16_t data4 = (dist(gen) & 0x0FFF) | 0x4000;
    uint16_t data5 = (dist(gen) & 0x3FFF) | 0x8000;
    uint16_t data6 = dist(gen) & 0xFFFF;
    uint16_t data7 = dist(gen) & 0xFFFF;

    UUID uuid;
    std::memcpy(&uuid[0], &data1, sizeof(data1));
    std::memcpy(&uuid[4], &data2, sizeof(data2));
    std::memcpy(&uuid[6], &data3, sizeof(data3));
    std::memcpy(&uuid[8], &data4, sizeof(data4));
    std::memcpy(&uuid[10], &data5, sizeof(data5));
    std::memcpy(&uuid[12], &data6, sizeof(data6));
    std::memcpy(&uuid[14], &data7, sizeof(data7));

    // _uuidRegistry.insert(uuid);
    return uuid;
}

size_t UUIDManager::findEntityByUUID(const UUID &uuid)
{
    Registry &r = Registry::getInstance();

    for (auto &&[i, entityUUID] : IndexedZipper(r.getComponents<UUID>()))
        if (uuid == entityUUID)
            return i;
    throw std::runtime_error("UUID is not assigned to any entity");
}

size_t UUIDManager::findEntityByUUID(const UUIDStr &uuid)
{
    return findEntityByUUID(toUUIDNumber(uuid));
}

UUID UUIDManager::toUUIDNumber(const UUIDStr &uuid)
{
    if (uuid.length() != 36 || uuid[8] != '-' || uuid[13] != '-' ||
        uuid[18] != '-' || uuid[23] != '-')
        throw std::invalid_argument("Invalid UUID format");

    UUID data{};
    int byteIndex = 0;

    for (size_t i = 0; i < uuid.length(); ++i) {
        if (uuid[i] == '-')
            continue;

        std::string byteStr = uuid.substr(i, 2);
        data[byteIndex] = static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16));
        ++byteIndex;
        ++i;
    }

    return data;
}

UUIDStr UUIDManager::toUUIDString(const UUID &uuid)
{
    std::ostringstream ss;
    ss << std::hex << std::setfill('0');

    for (size_t i = 0; i < uuid.size(); ++i) {
        ss << std::setw(2) << static_cast<int>(uuid[i]);
        if (i == 3 || i == 5 || i == 7 || i == 9)
            ss << '-';
    }

    return ss.str();
}
