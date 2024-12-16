/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sun, Oct, 2024                                                     *
 * Title           - OtherGame                                                          *
 * Description     -                                                                    *
 *     SceneLoader                                                                      *
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

#if defined(_WIN32)
    #define NOGDI
    #define NOUSER
#endif

#include "SceneLoader.hpp"
#include "GameEngineIncludesList.hpp"

SceneLoader::SceneLoader(Registry &registry, const std::string &configFile)
:
_registry(registry),
_jsonHandler(configFile),
_currentScene("")
{
    setupEntityMapping();
    setupComponentRemovalMap();
}

void SceneLoader::loadScene(const std::string &sceneID)
{
    if (!_jsonHandler.contains({"scenes", sceneID})) {
        std::cerr << "Failed to load scene " << sceneID << std::endl;
        return;
    }

    auto sceneData = _jsonHandler.get<json>({"scenes", sceneID}, json{});

    for (const auto &componentName : sceneData["remove_components"]) {
        if (_componentRemovalMap.contains(componentName))
            _componentRemovalMap[componentName]();
        else
            std::cerr << "Unknown component type for deletion: " << componentName << std::endl;
    }

    for (const auto &entityData : sceneData["entities"]) {
        std::string entityType = entityData["type"];
        if (_entitySetupMap.contains(entityType))
            _entitySetupMap[entityType](entityData);
        else
            std::cerr << "Unknown entity type: " << entityType << std::endl;
    }

    _currentScene = sceneID;

    _registry.update();
    _registry.optimizeStorages();
}

void SceneLoader::setupEntityMapping()
{
    _entitySetupMap["mouse"] = [this](const json &data) { setupMouse(data); };
    _entitySetupMap["music player"] = [this](const json &data) { setupMusicPlayer(data); };
    _entitySetupMap["player data"] = [this](const json &data) { setupPlayerData(data); };
    _entitySetupMap["undyne"] = [this](const json &data) { setupUndyne(data); };
    _entitySetupMap["texts"] = [this](const json &data) { setupTexts(data); };
    _entitySetupMap["action buttons"] = [this](const json &data) { setupActionButtons(data); };
    _entitySetupMap["action rectangle"] = [this](const json &data) { setupActionRectangle(data); };
    _entitySetupMap["attack slider"] = [this](const json &data) { setupAttackSlider(data); };
    _entitySetupMap["arrow spawners"] = [this](const json &data) { setupArrowSpawners(data); };
    _entitySetupMap["pad"] = [this](const json &data) { setupPad(data); };
    _entitySetupMap["pad square"] = [this](const json &data) { setupPadSquare(data); };
    _entitySetupMap["game over message"] = [this](const json &data) { setupGameOverMessage(data); };
}

void SceneLoader::setupComponentRemovalMap()
{
    _componentRemovalMap["ActionMenuRemovable"] = [this]() { removeEntitiesWithComponent<ActionMenuRemovable>(); };
    _componentRemovalMap["AttackRemovable"] = [this]() { removeEntitiesWithComponent<AttackRemovable>(); };
    _componentRemovalMap["FightRemovable"] = [this]() { removeEntitiesWithComponent<FightRemovable>(); };
    _componentRemovalMap["TimerContainer"] = [this]() { removeEntitiesWithComponent<TimerContainer>(); };
}

void SceneLoader::setupMouse(const json &)
{
    entity_t mouse = _registry.spawnEntity();

    _registry.emplaceComponent<ActionMenuRemovable>(mouse).emplaceComponent<AttackRemovable>(mouse).emplaceComponent<FightRemovable>(mouse);

    Vector2 mousePos = GetMousePosition();
    float scale = 0.05f;
    Rect rect{0.0f, 0.0f, 43.0f, 68.05f};
    Rect trueRect{0.0f, 0.0f, rect.width / scale, rect.height / scale};

    _registry.emplaceComponent<Transformable>(mouse, mousePos.x, mousePos.y, 0.0f, scale, scale, false);
    _registry.emplaceComponent<Mouse>(mouse);
    _registry.emplaceComponent<Collision>(
        mouse,
        1.0f, 1.0f,
        Vec2(rect.width, rect.height),
        [](Registry &r, size_t, size_t collidedEntity) {
            auto handleButtonHover = [&r, collidedEntity](){
                if (!r.hasComponent<Button>(collidedEntity))
                    return;

                Button &button = r.getComponent<Button>(collidedEntity);
                button.hovered = true;
            };

            handleButtonHover();
            return false;
        }
    );
    _registry.emplaceComponent<Drawable>(mouse).emplaceComponent<SpriteRender>(
        mouse,
        AssetManager::getInstance().loadAsset<Texture2D>("assets/sprites/papy_russe.png"),
        trueRect
    ).emplaceComponent<Animated>(mouse, trueRect, 2, 0.1f);

    SetMouseOffset(static_cast<int>(-(rect.width / 2.0f)), static_cast<int>(-(rect.height / 1.5f)));
    HideCursor();
}

void SceneLoader::setupMusicPlayer(const json &data)
{
    entity_t music = _registry.spawnEntity();
    const std::string url = data["url"];
    const std::string id = data["music_id"];
    _registry.emplaceComponent<MusicSource>(music, id, url);
}

void SceneLoader::setupPlayerData(const json &)
{
    entity_t playerData = _registry.spawnEntity();

    _registry.emplaceComponent<PlayerData>(playerData).emplaceComponent<Health>(
        playerData,
        25,
        [this](Registry &, size_t) {
            loadScene("game over");
        },
        [](Registry &r, size_t e) {
            const Health &h = r.getComponent<Health>(e);

            for (auto &&[text] : Zipper(r.getComponents<Text>())) {
                if (text.id != "player hp")
                    continue;
                text.getPart(0) = std::to_string(h.current);
                // TODO: fix hp loss
            }
        },
        0.0f
    );
}

void SceneLoader::setupUndyne(const json &)
{
    entity_t undyne = _registry.spawnEntity();

    // Rect rect(0.0f, 0.0f, 600.0f, 470.0f);
    Rect rect{0.0f, 470.0f, 600.0f, 470.0f};

    _registry.emplaceComponent<Transformable>(undyne, 755.0f, 0.0f);
    _registry.emplaceComponent<Drawable>(undyne).emplaceComponent<SpriteRender>(
        undyne,
        AssetManager::getInstance().loadAsset<Texture2D>("assets/sprites/undyne.png"),
        rect
    ).emplaceComponent<Animated>(undyne, rect, 27, 3, 0.025f);
    // _registry.emplaceComponent<Animated>(undyne, rect, 27, 4, 0.025f);

    _registry.emplaceComponent<Health>(
        undyne,
        50,
        [this](Registry &, size_t) {
            loadScene("game over");
            loadScene("setup");
            loadScene("action menu");
        },
        [](Registry &r, size_t e) {
            const Health &h = r.getComponent<Health>(e);

            for (auto &&[text] : Zipper(r.getComponents<Text>())) {
                if (text.id != "undyne hp")
                    continue;
                if (text._parts.empty())
                    text.addPart("");
                text.getPart(0) = std::to_string(h.current);
            }

            r.safeEmplaceComponent<Red>(e);
        }
    );
}

void SceneLoader::setupTexts(const json &)
{
    setupText({1600.0f, 0.0f}, "score", "score: 0");
    setupText({750.0f, 0.0f}, "undyne hp", "50");
    setupText({100.0f, 900.0f}, "player hp", "25");
}

void SceneLoader::setupActionButtons(const json &data)
{
    std::unordered_map<std::string, std::tuple<Rect, size_t, std::function<void(entity_t)>>, StringHash, std::equal_to<>> buttonMapping = {
        {"fight", {{0.0f, 0.0f, 187.0f, 73.0f}, 1, [this](entity_t e){
            for (auto &&[i, _, frames] : IndexedZipper(_registry.getComponents<ActionButton>(), _registry.getComponents<NamedFrames>()))
                if (e != i)
                    frames.setCurrentFrame("base");

            loadScene("attack");
        }}},
        {"act", {{238.0f, 0.0f, 187.0f, 73.0f}, 2, [](entity_t){}}},
        {"item", {{499.0f, 0.0f, 187.0f, 73.0f}, 3, [](entity_t){}}},
        {"mercy", {{747.0f, 0.0f, 190.0f, 73.0f}, 4, [](entity_t){}}}
    };

    try {
        if (!data.contains("list")) {
            throw std::runtime_error("Invalid JSON structure for action buttons.");
        }

        std::unordered_set<std::string, StringHash, std::equal_to<>> foundButtons;

        for (const auto &button : data.at("list")) {
            std::string id = button.at("id").get<std::string>();
            if (!buttonMapping.contains(id)) {
                std::cerr << "Unknown button id: " << id << std::endl;
                continue;
            }

            Vec2 pos{};
            if (button.contains("pos") && button.at("pos").contains("x") && button.at("pos").contains("y")) {
                pos.x = button.at("pos").at("x").get<float>();
                pos.y = button.at("pos").at("y").get<float>();
            } else {
                throw std::runtime_error("Missing or invalid position data for button: " + id);
            }

            const auto &[rect, offsetMultiplier, onClick] = buttonMapping[id];
            setupActionButton(pos, rect, offsetMultiplier, onClick);

            foundButtons.insert(id);
        }

        for (const auto &[id, _] : buttonMapping)
            if (!foundButtons.contains(id))
                throw std::runtime_error("Missing required button: " + id);
    } catch (const json::exception &e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error setting up action buttons: " << e.what() << std::endl;
    }
}

void SceneLoader::setupActionRectangle(const json &)
{
    entity_t actionRect = _registry.spawnEntity();

    float scale = 1.5f;
    Rect rect{0.0f, 0.0f, 960.0f, 284.0f};

    _registry.emplaceComponent<FightRemovable>(actionRect);

    _registry.emplaceComponent<Transformable>(actionRect, 240.0f, 480.0f, 0.0f, scale, scale);
    _registry.emplaceComponent<Drawable>(actionRect).emplaceComponent<SpriteRender>(
        actionRect,
        AssetManager::getInstance().loadAsset<Texture2D>("assets/sprites/rectangle.png"),
        rect
    );
}

void SceneLoader::setupAttackSlider(const json &)
{
    entity_t attackSlider = _registry.spawnEntity();

    _registry.emplaceComponent<FightRemovable>(attackSlider);

    Rect rect{0.0f, 0.0f, 1798.0f, 458.0f};
    float scale = 0.75f;

    _registry.emplaceComponent<Transformable>(attackSlider, 285.0f, 520.0f, 0.0f, scale, scale);
    _registry.emplaceComponent<Drawable>(attackSlider).emplaceComponent<SpriteRender>(
        attackSlider,
        AssetManager::getInstance().loadAsset<Texture2D>("assets/sprites/fight.png"),
        rect
    );
    _registry.emplaceComponent<Input>(
        attackSlider,
        Input::actionList{
            [this](Registry &, size_t){
                if (!IsKeyDown(KEY_ENTER))
                    return;
                loadScene("fight");
            }
        }
    );
}

void SceneLoader::setupArrowSpawners(const json &)
{
    float arrowWidth = 50.0f;
    float arrowHeight = 26.0f;
    float middleX = (1920.0f - arrowHeight) / 2.0f;
    float middleY = (1080.0f - arrowWidth) / 2.0f;
    float startX = -100.0f;
    float startY = -50.0f;
    float endX = 1920.0f + 100.0f;
    float endY = 1080.0f + 50.0f;

    setupArrowSpawner({middleX + arrowWidth / 2.0f, startY}, 90.0f, "up");
    setupArrowSpawner({endX, middleY + arrowHeight * 3.0f}, 180.0f, "right");
    setupArrowSpawner({middleX, endY}, 270.0f, "down");
    setupArrowSpawner({startX, middleY + arrowHeight * 2.0f}, 0.0f, "left");
}

void SceneLoader::setupPad(const json &)
{
    entity_t pad = _registry.spawnEntity();

    _registry.emplaceComponent<ActionMenuRemovable>(pad);

    Rect rect{0.0f, 0.0f, 122.0f, 122.0f};
    float scale = 1.2f;

    _registry.emplaceComponent<Transformable>(
        pad,
        885.0f, 505.0f, 0.0f,
        scale, scale,
        false
    );

    std::unordered_map<std::string, Rect, StringHash, std::equal_to<>> framesList = {
        {"up", rect},
        {"right", {rect.x + 122.0f, rect.y, rect.width, rect.height}},
        {"down", {rect.x + 122.0f * 2.0f, rect.y, rect.width, rect.height}},
        {"left", {rect.x + 122.0f * 3.0f, rect.y, rect.width, rect.height}}
    };

    _registry.emplaceComponent<Drawable>(pad).emplaceComponent<SpriteRender>(
        pad,
        AssetManager::getInstance().loadAsset<Texture2D>("assets/sprites/pad.png"),
        rect
    ).emplaceComponent<NamedFrames>(pad, framesList, "up");

    _registry.emplaceComponent<Collision>(pad, rect.width, rect.height);
    _registry.emplaceComponent<Input>(
        pad,
        Input::actionList{
            [](Registry &r, size_t e){
                if (!r.hasComponent<NamedFrames>(e))
                    return;
                NamedFrames &f = r.getComponent<NamedFrames>(e);

                static const std::unordered_map<KeyboardKey, std::string> keyToFrame{
                    {KEY_UP, "up"},
                    {KEY_RIGHT, "right"},
                    {KEY_DOWN, "down"},
                    {KEY_LEFT, "left"}
                };

                for (const auto &[key, frame] : keyToFrame)
                    if (IsKeyPressed(key))
                        f.setCurrentFrame(frame);
            }
        }
    );
    _registry.emplaceComponent<SoundSource>(pad);
}

void SceneLoader::setupPadSquare(const json &)
{
    entity_t padSquare = _registry.spawnEntity();

    _registry.emplaceComponent<ActionMenuRemovable>(padSquare);

    Rect rect{0.0f, 0.0f, 164.0f, 158.0f};

    _registry.emplaceComponent<Transformable>(padSquare, 875.0f, 500.0f);
    _registry.emplaceComponent<Drawable>(padSquare).emplaceComponent<SpriteRender>(
        padSquare,
        AssetManager::getInstance().loadAsset<Texture2D>("assets/sprites/square.png"),
        rect
    );
}

void SceneLoader::setupGameOverMessage(const json &)
{
    entity_t gameOverMessage = _registry.spawnEntity();

    Rect rect{0.0f, 0.0f, 1920.0f, 1440.0f};
    float scale = 0.7f;

    _registry.emplaceComponent<Transformable>(
        gameOverMessage,
        300.0f, 0.0f, 0.0f,
        scale, scale
    );
    _registry.emplaceComponent<Drawable>(gameOverMessage).emplaceComponent<SpriteRender>(
        gameOverMessage,
        AssetManager::getInstance().loadAsset<Texture2D>("assets/sprites/game_over.png"),
        rect
    );
}

void SceneLoader::setupText(
    const Vec2 &position,
    const std::string &id,
    const std::string &defaultText
)
{
    entity_t text = _registry.spawnEntity();

    _registry.emplaceComponent<Transformable>(text, position.x, position.y);
    _registry.emplaceComponent<Drawable>(text).emplaceComponent<Text>(text, id, defaultText, 40);
}

void SceneLoader::setupActionButton(
    const Vec2 &position,
    const Rect &rect,
    size_t offsetMultiplier,
    const std::function<void(entity_t)> &onClick
)
{
    entity_t actionButton = _registry.spawnEntity();

    _registry.emplaceComponent<FightRemovable>(actionButton).emplaceComponent<ActionButton>(actionButton);

    float selectedOffset = 937.0f * static_cast<float>(offsetMultiplier);
    float scale = 1.5f;

    // TODO: setup input and sounds
    _registry.emplaceComponent<Transformable>(actionButton, position.x, position.y, 0.0f, scale, scale, false);

    std::unordered_map<std::string, Rect, StringHash, std::equal_to<>> framesList = {
        {"base", rect},
        {"selected", {rect.x + selectedOffset, rect.y, rect.width, rect.height}}
    };

    _registry.emplaceComponent<Drawable>(actionButton).emplaceComponent<SpriteRender>(
        actionButton,
        AssetManager::getInstance().loadAsset<Texture2D>("assets/sprites/menu.png"),
        rect
    ).emplaceComponent<NamedFrames>(actionButton, framesList, "base");

    _registry.emplaceComponent<Button>(
        actionButton,
        [this, actionButton, onClick](){
            if (_currentScene != "action menu")
                return;

            if (!_registry.hasComponent<NamedFrames>(actionButton))
                return;
            NamedFrames &frames = _registry.getComponent<NamedFrames>(actionButton);

            frames.setCurrentFrame("selected");
            onClick(actionButton);
        },
        [this, actionButton](){
            if (_currentScene != "action menu")
                return;

            if (!_registry.hasComponent<NamedFrames>(actionButton))
                return;
            NamedFrames &frames = _registry.getComponent<NamedFrames>(actionButton);

            frames.setCurrentFrame("base");
        }
    );
    _registry.emplaceComponent<Collision>(actionButton, rect.width, rect.height);
    // _registry.emplaceComponent<Input>(actionButton);
    _registry.emplaceComponent<SoundSource>(actionButton);
}

void SceneLoader::setupArrowSpawner(
    const Vec2 &position,
    float rotation,
    const std::string &direction
)
{
    entity_t arrowSpawner = _registry.spawnEntity();

    _registry.emplaceComponent<Transformable>(arrowSpawner, position.x, position.y);
    _registry.emplaceComponent<Spawner>(
        arrowSpawner,
        [this, rotation, direction](Registry &, const Vec2 &pos){
            setupArrow(pos, rotation, direction);
        },
        0.1f, 0.05f
    );
}

void SceneLoader::setupArrow(
    const Vec2 &position,
    float rotation,
    const std::string &direction
)
{
    entity_t arrow = _registry.spawnEntity();

    _registry.emplaceComponent<ActionMenuRemovable>(arrow);

    Rect rect{50.0f, 0.0f, 50.0f, 26.0f};

    _registry.emplaceComponent<Transformable>(arrow, position.x, position.y, rotation);
    _registry.emplaceComponent<Drawable>(arrow).emplaceComponent<SpriteRender>(
        arrow,
        AssetManager::getInstance().loadAsset<Texture2D>("assets/sprites/arrow.png"),
        rect
    );

    static const std::unordered_map<std::string, Vec2> directionVelocities = {
        {"up", {0.0f, 10.0f}},
        {"right", {-10.0f, 0.0f}},
        {"down", {0.0f, -10.0f}},
        {"left", {10.0f, 0.0f}}
    };
    Vec2 velocity = directionVelocities.count(direction) ? directionVelocities.at(direction) : Vec2{0.0f, 0.0f};
    _registry.emplaceComponent<Velocity>(
        arrow,
        velocity.x * Math::randomDouble(0.5f, 2.0f),
        velocity.y * Math::randomDouble(0.5f, 2.0f)
    ).emplaceComponent<Acceleration>(arrow, 0.0f, 0.0f, 0.0f, 1.0f)
    .emplaceComponent<Collision>(
        arrow,
        rect.width, rect.height,
        Vec2(0.0f, 0.0f),
        [](Registry &r, size_t entity, size_t collidedEntity) {
            if (!r.hasComponent<Arrow>(entity)
                || !r.hasComponent<NamedFrames>(collidedEntity)
                || !r.hasComponent<SoundSource>(collidedEntity))
                return false;

            const Arrow &a = r.getComponent<Arrow>(entity);
            SoundSource &s = r.getComponent<SoundSource>(collidedEntity);

            if (r.getComponent<NamedFrames>(collidedEntity).currentFrame != a.direction) {
                for (auto &&[_, h] : Zipper(r.getComponents<PlayerData>(), r.getComponents<Health>()))
                    h.takeDamage(a.damage);
                s.safePlay("hearth hit", "assets/audio/heart_hit.ogg");
            } else {
                for (auto &&[text] : Zipper(r.getComponents<Text>())) {
                    if (text.id != "score")
                        continue;
                    if (text._parts.size() == 1)
                        text.addPart("");
                    for (auto &&[data] : Zipper(r.getComponents<PlayerData>())) {
                        ++data.score;
                        text.getPart(1) = std::to_string(data.score);
                        break;
                    }
                }
                // s.safePlay("pad hit", "assets/audio/ahit.ogg"); // TODO: play appropriate sound
            }

            r.killEntity(entity);
            return false;
        }
    );
    _registry.emplaceComponent<Arrow>(arrow, 1, direction);
}
