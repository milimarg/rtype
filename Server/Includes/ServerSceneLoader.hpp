/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Oct, 2024                                                     *
 * Title           - GameEngine                                                        *
 * Description     -                                                                    *
 *     ServerSceneLoader                                                                *
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

#ifndef INCLUDED_SERVERSCENELOADER_HPP
    #define INCLUDED_SERVERSCENELOADER_HPP

    #include <unordered_map>
    #include <string>
    #include <functional>
    #include "Registry.hpp"
    #include "JsonHandler.hpp"
    #include "IndexedZipper.hpp"
    #include "Zipper.hpp"
    #include "Vec2.hpp"
    #include "Using.hpp"
    #include "UUIDManager.hpp"
    #include "CustomComponents.hpp"
    #include "Difficulty.hpp"

USING(
    ecs::cmp::UUID,
    Network::Difficulty
);

namespace Network
{
    class Game;
    class BitConverter;
}

class ServerSceneLoader {
    public:
        ServerSceneLoader(Registry &registry, std::shared_ptr<Network::Game> game, const std::string &configFile, Difficulty difficulty, bool friendlyFire);
        ~ServerSceneLoader();

        void loadScene(const std::string &sceneID);

        UUID setupPlayer();
        void changePlayerWeapon(const std::string &weaponID, int shotCount = 1, const Vec2 &offset = {0.0f, 0.0f});

    private:
        Registry &_registry;
        Network::BitConverter &_bitConverter;
        std::shared_ptr<Network::Game> _game;
        JsonHandler _jsonHandler;

        using EntitySetupFunction = std::function<void(const json &)>;
        std::unordered_map<std::string, EntitySetupFunction> _entitySetupMap;
        std::unordered_map<std::string, std::function<void()>> _componentRemovalMap;

        Difficulty _difficulty;
        bool _friendlyFire;
        float _spawnIntervalMultiplier;

        void setupEntityMapping();
        void setupComponentRemovalMap();

        template <typename ComponentType>
        void removeEntitiesWithComponent()
        {
            for (auto &&[i, _] : IndexedZipper(_registry.getComponents<ComponentType>()))
                _registry.killEntity(i);
        }

        void setupBrainMobSpawner(const json &data);
        void setupRollingMobSpawner(const json &data);
        void setupScreenWalls(const json &data);
        void setupBrainSeahorseBoss(const json &data);

        void spawnRollingMob(const Vec2 &coords, float velocity = -10.0f, float acceleration = -5.0f, int hp = 3, bool hasGravity = true);

        std::function<void(Registry &, const Vec2 &)> getPlayerWeaponFromID(
            size_t playerID,
            const std::string &weaponID,
            int shotCount = 1,
            const Vec2 &offset = {0.0f, 0.0f}
        );
        void swapPlayerWeapon(
            size_t playerID,
            const std::string &weaponID,
            int shotCount = 1,
            const Vec2 &offset = {0.0f, 0.0f}
        );

        int _playerHp;
};

#endif /* INCLUDED_SERVERSCENELOADER_HPP */
