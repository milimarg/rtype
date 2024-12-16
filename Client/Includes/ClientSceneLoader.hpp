/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Oct, 2024                                                     *
 * Title           - GameEngine                                                        *
 * Description     -                                                                    *
 *     ClientSceneLoader                                                                *
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

#ifndef INCLUDED_CLIENTSCENELOADER_HPP
    #define INCLUDED_CLIENTSCENELOADER_HPP

    #include <unordered_map>
    #include <string>
    #include <functional>
    #include <stack>
    #include "Registry.hpp"
    #include "JsonHandler.hpp"
    #include "IndexedZipper.hpp"
    #include "Vec2.hpp"
    #include "Using.hpp"
    #include "UUID.hpp"
    #include "RaylibWrapper.hpp"
    #include "Difficulty.hpp"

    #define START_COMPRESSION if (_game->getBuffer().size() > 1500) _game->send(); _bitConverter

namespace Network
{
    class Game;
    class BitConverter;
}

USING(
    ecs::cmp::UUID,
    Network::Difficulty
);

class ClientSceneLoader {
    public:
        ClientSceneLoader(Registry &registry, const std::string &configFile);

        void setGame(std::shared_ptr<Network::Game> game);
        std::shared_ptr<Network::Game> getGame();

        bool enterScene(const std::string &sceneID);
        bool leaveScene();
        bool restartFromScene(const std::string &sceneID);

        void setupPlayer(float x, float y, const UUID &uuid);
        void setupOtherPlayer(float x, float y, const UUID &uuid);
        void setupPlayerBullet(float x, float y, const UUID &uuid);
        void setupBrainMob(float x, float y, const UUID &uuid);
        void setupBrainMobBullet(float x, float y, const UUID &uuid);
        void setupRollingMob(float x, float y, const UUID &uuid);
        void setupBrainSeahorseBoss(float x, float y, const UUID &uuid);
        void setAdmin(bool state);

    private:
        bool loadScene(const std::string &sceneID);

        template <typename ComponentType>
        void removeEntitiesWithComponent()
        {
            for (auto &&[i, _] : IndexedZipper(_registry.getComponents<ComponentType>()))
                _registry.killEntity(i);
        }

        void setupMouse(const json &data);
        void setupCamera(const json &data);
        void setupStarfield(const json &data);
        void setupDenseStarfield(const json &data);
        void setupMusic(const json &data);
        void setupMenuCreateButton(const json &data);
        void setupMenuDifficultyButton(const json &data);
        void setupMenuFriendlyFire(const json &data);
        void setupMenuInputRemapButton(const json &data);
        void setupMenuJoinButton(const json &data);
        void setupMenuJoinElements(const json &data);
        void setupMenuInputRemap(const json &data);
        void setupLeaveButton(const json &data);
        void setupAdminPanelButton(const json &data);
        void setupAdminPanelElems(const json &data);

        Registry &_registry;
        Network::BitConverter &_bitConverter;
        std::shared_ptr<Network::Game> _game;
        JsonHandler _jsonHandler;

        using EntitySetupFunction = std::function<void(const json &)>;
        std::unordered_map<std::string, EntitySetupFunction> _entitySetupMap;
        std::unordered_map<std::string, std::function<void()>> _componentRemovalMap;

        std::stack<std::string> _sceneStack;

        void setupEntityMapping();
        void setupComponentRemovalMap();

        enum class KEY {
            UP = 265,
            DOWN = 264,
            LEFT = 263,
            RIGHT = 262,
            ATTACK = 69
        };

        std::unordered_map<int, std::string> _keyToString;
        std::unordered_map<KEY, int> _keyMaps;
        std::unordered_map<KEY, std::string> _keyStrings;

        bool _admin;

        void inputTextRemapHelper(
            KEY key,
            const Vector2 &pos,
            std::unordered_map<KEY, bool> &clickedStates
        );

        Difficulty _difficulty;
        bool _friendlyFire;
};

#endif /* INCLUDED_CLIENTSCENELOADER_HPP */
