/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sun, Oct, 2024                                                     *
 * Title           - OtherGame                                                          *
 * Description     -                                                                    *
 *     SceneLoader                                                                      *
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

#ifndef SCENE_LOADER_HPP
    #define SCENE_LOADER_HPP

    #include <unordered_map>
    #include <unordered_set>
    #include <string>
    #include <functional>
    #include "Registry.hpp"
    #include "JsonHandler.hpp"
    #include "IndexedZipper.hpp"
    #include "Vec2.hpp"
    #include "Using.hpp"
    #include "RaylibWrapper.hpp"
    #include "StringHash.hpp"

class SceneLoader {
    public:
        SceneLoader(Registry &registry, const std::string &configFile);

        void loadScene(const std::string &sceneID);

    private:
        Registry &_registry;
        JsonHandler _jsonHandler;

        using EntitySetupFunction = std::function<void(const json &)>;
        std::unordered_map<std::string, EntitySetupFunction, StringHash, std::equal_to<>> _entitySetupMap;
        std::unordered_map<std::string, std::function<void()>, StringHash, std::equal_to<>> _componentRemovalMap;

        std::string _currentScene;

        void setupEntityMapping();
        void setupComponentRemovalMap();

        template <typename ComponentType>
        void removeEntitiesWithComponent()
        {
            for (auto &&[i, _] : IndexedZipper(_registry.getComponents<ComponentType>()))
                _registry.killEntity(i);
        }

        void setupMouse(const json &data);
        void setupMusicPlayer(const json &data);
        void setupPlayerData(const json &data);
        void setupUndyne(const json &data);
        void setupTexts(const json &data);
        void setupActionButtons(const json &data);
        void setupActionRectangle(const json &data);
        void setupAttackSlider(const json &data);
        void setupArrowSpawners(const json &data);
        void setupPad(const json &data);
        void setupPadSquare(const json &data);
        void setupGameOverMessage(const json &data);

        void setupText(
            const Vec2 &position,
            const std::string &id,
            const std::string &defaultText = ""
        );
        void setupActionButton(
            const Vec2 &position,
            const Rect &rect,
            size_t offsetMultiplier,
            const std::function<void(entity_t)> &onClick
        );
        void setupArrowSpawner(
            const Vec2 &position,
            float rotation,
            const std::string &direction
        );
        void setupArrow(
            const Vec2 &position,
            float rotation,
            const std::string &direction
        );
};

#endif /* SCENE_LOADER_HPP */
