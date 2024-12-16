/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Oct, 2024                                                     *
 * Title           - GameEngine                                                        *
 * Description     -                                                                    *
 *     ClientSceneLoader                                                                *
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

#include "ClientSceneLoader.hpp"
#include "GameEngineIncludesClient.hpp"
#include "Game.hpp"
#include "NetClient.hpp"

ClientSceneLoader::ClientSceneLoader(Registry &registry, const std::string &configFile)
:
_registry(registry),
_bitConverter(Network::BitConverter::getInstance()),
_jsonHandler(configFile),
_sceneStack{},
_keyToString({
    { KEY_ZERO, "0" },
    { KEY_ONE, "1" },
    { KEY_TWO, "2" },
    { KEY_THREE, "3" },
    { KEY_FOUR, "4" },
    { KEY_FIVE, "5" },
    { KEY_SIX, "6" },
    { KEY_SEVEN, "7" },
    { KEY_EIGHT, "8" },
    { KEY_NINE, "9" },
    { KEY_A, "Q" },
    { KEY_B, "B" },
    { KEY_C, "C" },
    { KEY_D, "D" },
    { KEY_E, "E" },
    { KEY_F, "F" },
    { KEY_G, "G" },
    { KEY_H, "H" },
    { KEY_I, "I" },
    { KEY_J, "J" },
    { KEY_K, "K" },
    { KEY_L, "L" },
    { KEY_COMMA, "M" },
    { KEY_N, "N" },
    { KEY_O, "O" },
    { KEY_P, "P" },
    { KEY_Q, "A" },
    { KEY_R, "R" },
    { KEY_S, "S" },
    { KEY_T, "T" },
    { KEY_U, "U" },
    { KEY_V, "V" },
    { KEY_W, "Z" },
    { KEY_X, "X" },
    { KEY_Y, "Y" },
    { KEY_Z, "W" },
    { KEY_UP, "/\\" },
    { KEY_DOWN, "\\/" },
    { KEY_LEFT, "<" },
    { KEY_RIGHT, ">" }
}),
_keyMaps({
    {KEY::UP, KEY_UP},
    {KEY::DOWN, KEY_DOWN},
    {KEY::LEFT, KEY_LEFT},
    {KEY::RIGHT, KEY_RIGHT},
    {KEY::ATTACK, KEY_E}
}),
_keyStrings({
    {KEY::UP, "up"},
    {KEY::DOWN, "down"},
    {KEY::LEFT, "left"},
    {KEY::RIGHT, "right"},
    {KEY::ATTACK, "attack"}
}),
_admin(false),
_difficulty(Difficulty::NORMAL),
_friendlyFire(false)
{
    setupEntityMapping();
    setupComponentRemovalMap();
}

void ClientSceneLoader::setGame(std::shared_ptr<Network::Game> game)
{
    _game = game;
}

std::shared_ptr<Network::Game> ClientSceneLoader::getGame()
{
    return _game;
}

bool ClientSceneLoader::loadScene(const std::string &sceneID)
{
    if (!_jsonHandler.contains({"scenes", sceneID})) {
        std::cerr << "Failed to load scene " << sceneID << std::endl;
        return false;
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
        if (_entitySetupMap.find(entityType) != _entitySetupMap.end())
            _entitySetupMap[entityType](entityData);
        else
            std::cerr << "Unknown entity type: " << entityType << std::endl;
    }

    return true;
}

bool ClientSceneLoader::enterScene(const std::string &sceneID)
{
    _sceneStack.push(sceneID);
    return loadScene(sceneID);
}

bool ClientSceneLoader::leaveScene()
{
    if (_sceneStack.size() == 1)
        return false;

    _sceneStack.pop();
    return loadScene(_sceneStack.top());
}

bool ClientSceneLoader::restartFromScene(const std::string &sceneID)
{
    while (!_sceneStack.empty())
        _sceneStack.pop();
    return enterScene(sceneID);
}

void ClientSceneLoader::setupEntityMapping()
{
    _entitySetupMap["mouse"] = [&](const json &data) { setupMouse(data); };
    _entitySetupMap["3Dcamera"] = [&](const json &data) { setupCamera(data); };
    _entitySetupMap["starfield"] = [&](const json &data) { setupStarfield(data); };
    _entitySetupMap["dense_starfield"] = [&](const json &data) { setupDenseStarfield(data); };
    _entitySetupMap["music"] = [&](const json &data) { setupMusic(data); };
    _entitySetupMap["menu_create_button"] = [&](const json &data) { setupMenuCreateButton(data); };
    _entitySetupMap["menu_difficulty_button"] = [&](const json &data) { setupMenuDifficultyButton(data); };
    _entitySetupMap["menu_friendly_fire_button"] = [&](const json &data) { setupMenuFriendlyFire(data); };
    _entitySetupMap["menu_input_remap_button"] = [&](const json &data) { setupMenuInputRemapButton(data); };
    _entitySetupMap["menu_join_button"] = [&](const json &data) { setupMenuJoinButton(data); };
    _entitySetupMap["menu_join_elements"] = [&](const json &data) { setupMenuJoinElements(data); };
    _entitySetupMap["menu_input_remap_elements"] = [&](const json &data) { setupMenuInputRemap(data); };
    _entitySetupMap["leave_button"] = [&](const json &data) { setupLeaveButton(data); };
    _entitySetupMap["admin_panel_button"] = [&](const json &data) { setupAdminPanelButton(data); };
    _entitySetupMap["admin_panel_elems"] = [&](const json &data) { setupAdminPanelElems(data); };
}

void ClientSceneLoader::setupComponentRemovalMap()
{
    _componentRemovalMap["StarField"] = [&]() { removeEntitiesWithComponent<StarField>(); };
    _componentRemovalMap["CameraWrapper"] = [&]() { removeEntitiesWithComponent<CameraWrapper>(); };
    _componentRemovalMap["Button"] = [&]() { removeEntitiesWithComponent<Button>(); };
    _componentRemovalMap["MusicSource"] = [&]() { removeEntitiesWithComponent<MusicSource>(); };
    _componentRemovalMap["Text"] = [&]() { removeEntitiesWithComponent<Text>(); };
    _componentRemovalMap["Drawable"] = [&]() { removeEntitiesWithComponent<Drawable>(); };
    _componentRemovalMap["CameraWrapper"] = [&]() { removeEntitiesWithComponent<CameraWrapper>(); };
}

void ClientSceneLoader::setupPlayer(float x, float y, const UUID &uuid)
{
    if (!_game)
        return;

    entity_t player = _registry.spawnEntity();

    _registry.emplaceComponent<UUID>(player, uuid);

    std::shared_ptr<Texture2D> playerTexture = AssetManager::getInstance().loadAsset<Texture2D>("./assets/sprites/r-typesheet26.png");
    Rect playerRect(0.0f, 0.0f, 65.3f, 50.0f);

    _registry.emplaceComponent<Transformable>(player, x, y);
    _registry.emplaceComponent<Drawable>(player);
    _registry.emplaceComponent<SpriteRender>(player, playerTexture, playerRect);
    _registry.emplaceComponent<Animated>(player, playerRect, 3, 0.1f);

    _registry.emplaceComponent<Input>(
        player,
        Input::actionList{
            [this](Registry &r, size_t e){

                static thread_local std::unordered_map<KEY, bool> keys = {
                    {KEY::UP, false},
                    {KEY::DOWN, false},
                    {KEY::LEFT, false},
                    {KEY::RIGHT, false}
                };

                for (auto &[key, state] : keys) {
                    if (!state && IsKeyDown(_keyMaps[key])) {
                        START_COMPRESSION
                            .setID(INPUT_PLAYER)
                            .addNumber(static_cast<int>(key))
                            .addChar(STATE_KEY_DOWN_START)
                            .compactMessage(_game->getBuffer());

                        state = true;
                    } else if (state && IsKeyUp(_keyMaps[key])) {
                        START_COMPRESSION
                            .setID(INPUT_PLAYER)
                            .addNumber(static_cast<int>(key))
                            .addChar(STATE_KEY_DOWN_END)
                            .compactMessage(_game->getBuffer());
                        state = false;
                    }
                }
            },
            [this](Registry &r, size_t e){
                if (IsKeyPressed(_keyMaps[KEY::ATTACK])) {
                    START_COMPRESSION
                        .setID(INPUT_PLAYER)
                        .addNumber(static_cast<int>(KEY::ATTACK))
                        .addChar(STATE_KEY_PRESSED)
                        .compactMessage(_game->getBuffer());
                }
            }
        }
    );
    _registry.emplaceComponent<SoundSource>(player);
}

void ClientSceneLoader::setupOtherPlayer(float x, float y, const UUID &uuid)
{
        if (!_game)
        return;

    entity_t player = _registry.spawnEntity();

    _registry.emplaceComponent<UUID>(player, uuid);

    std::shared_ptr<Texture2D> playerTexture = AssetManager::getInstance().loadAsset<Texture2D>("./assets/sprites/r-typesheet26-other.png");
    Rect playerRect(0.0f, 0.0f, 65.3f, 50.0f);

    // TODO: change skin (color?)

    _registry.emplaceComponent<Transformable>(player, x, y);
    _registry.emplaceComponent<Drawable>(player);
    _registry.emplaceComponent<SpriteRender>(player, playerTexture, playerRect);
    _registry.emplaceComponent<Animated>(player, playerRect, 3, 0.1f);
    _registry.emplaceComponent<SoundSource>(player);
}

void ClientSceneLoader::setupPlayerBullet(float x, float y, const UUID &uuid)
{
    entity_t newBullet = _registry.spawnEntity();

    _registry.emplaceComponent<UUID>(newBullet, uuid);

    _registry.emplaceComponent<Transformable>(newBullet, x - 5.0f, y);
    _registry.emplaceComponent<Drawable>(newBullet);

    Rect newBulletRect(0.0f, 0.0f, 33.31f, 36.0f);

    std::shared_ptr<Texture2D> texture = AssetManager::getInstance().loadAsset<Texture2D>("./assets/sprites/r-typesheet5.png");
    _registry.emplaceComponent<SpriteRender>(newBullet, texture, newBulletRect);
    _registry.emplaceComponent<Animated>(newBullet, newBulletRect, 8, 0.1f);

    // TODO: play sound (for later)
}

void ClientSceneLoader::setupMouse(const json &data)
{
    entity_t mouse = _registry.spawnEntity();
    _registry.emplaceComponent<Mouse>(mouse);
    _registry.emplaceComponent<Transformable>(mouse, 0.0f, 0.0f);
    _registry.emplaceComponent<Collision>(
        mouse,
        1.0f,
        1.0f,
        Vec2(0.0f, 0.0f),
        [](Registry &r, size_t entity, size_t collidedEntity) {
            if (!r.hasComponent<Button>(collidedEntity))
                return false;

            Button &button = r.getComponent<Button>(collidedEntity);
            button.hovered = true;
            return false;
        }
    );
}

void ClientSceneLoader::setupMusic(const json &data)
{
    entity_t music = _registry.spawnEntity();
    const std::string url = data["url"];
    const std::string id = data["music_id"];
    _registry.emplaceComponent<MusicSource>(music, id, url);
}

void ClientSceneLoader::setupCamera(const json &data)
{
    entity_t camera = _registry.spawnEntity();
    _registry.emplaceComponent<CameraWrapper>(camera);
}

void ClientSceneLoader::setupStarfield(const json &data)
{
    int starAmount = 5000;
    if (data.contains("star_amount") && data.at("star_amount").is_number_integer())
        starAmount = data.at("star_amount");

    entity_t starfield = _registry.spawnEntity();
    _registry.emplaceComponent<StarField>(
        starfield,
        0.05f,
        starAmount,
        Vec2{25.0f, 250.0f},
        Vector3{0.0f, 0.0f, 0.0f},
        1000.0f
    );
    _registry.emplaceComponent<Drawable>(starfield);
}

void ClientSceneLoader::setupDenseStarfield(const json &data)
{
    int starAmount = 250;
    if (data.contains("star_amount") && data.at("star_amount").is_number_integer())
        starAmount = data.at("star_amount");

    entity_t starfield = _registry.spawnEntity();
    _registry.emplaceComponent<StarField>(
        starfield,
        -0.15f,
        starAmount,
        Vec2{2.5f, 25.0f},
        Vector3{0.0f, 0.0f, 0.0f},
        10.0f
    );
    _registry.emplaceComponent<Drawable>(starfield);
}

void ClientSceneLoader::setupBrainMob(float x, float y, const UUID &uuid)
{
    std::shared_ptr<Texture2D> enemyTexture = AssetManager::getInstance().loadAsset<Texture2D>("./mods/assets/sprites/r-typesheet17.png", "./assets/sprites/r-typesheet17.png");
    Rect enemyRect(0.0f, 0.0f, 65.25f, 132.0f);

    entity_t enemy = _registry.spawnEntity();

    _registry.emplaceComponent<UUID>(enemy, uuid);

    _registry.emplaceComponent<Transformable>(enemy, x, y);
    _registry.emplaceComponent<Drawable>(enemy);
    _registry.emplaceComponent<SpriteRender>(enemy, enemyTexture, enemyRect);
    _registry.emplaceComponent<Animated>(enemy, enemyRect, 8, 0.05f);
}

void ClientSceneLoader::setupBrainMobBullet(float x, float y, const UUID &uuid)
{
    entity_t newBullet = _registry.spawnEntity();

    _registry.emplaceComponent<UUID>(newBullet, uuid);


    _registry.emplaceComponent<Transformable>(newBullet, x - 5.0f, y, 0.0f, 2.0f, 2.0f);
    _registry.emplaceComponent<Drawable>(newBullet);

    Rect newBulletRect(0.0f, 0.0f, 17.08f, 18.0f);

    std::shared_ptr<Texture2D> texture = AssetManager::getInstance().loadAsset<Texture2D>("./assets/sprites/r-typesheet3.png");
    _registry.emplaceComponent<SpriteRender>(newBullet, texture, newBulletRect);
    _registry.emplaceComponent<Animated>(newBullet, newBulletRect, 12, 0.3f);
}

void ClientSceneLoader::setupRollingMob(float x, float y, const UUID &uuid)
{
    std::shared_ptr<Texture2D> enemyTexture = AssetManager::getInstance().loadAsset<Texture2D>("./assets/sprites/r-typesheet13.png");
    const Rect enemyRect(0.0f, 0.0f, 33.125f, 33.4f);

    entity_t enemy = _registry.spawnEntity();

    _registry.emplaceComponent<UUID>(enemy, uuid);

    _registry.emplaceComponent<Transformable>(enemy, x, y);
    _registry.emplaceComponent<Drawable>(enemy);
    _registry.emplaceComponent<SpriteRender>(enemy, enemyTexture, enemyRect);
    _registry.emplaceComponent<Animated>(
        enemy,
        enemyRect,
        8,
        2,
        0.025f
    );
}

void ClientSceneLoader::setupBrainSeahorseBoss(float x, float y, const UUID &uuid)
{
    std::shared_ptr<Texture2D> bossTexture = AssetManager::getInstance().loadAsset<Texture2D>("./mods/assets/sprites/r-typesheet30.png", "./assets/sprites/r-typesheet30.png");
    const Rect bossRect(20.0f, 212.0f, 162.25f, 212.0f);

    entity_t boss = _registry.spawnEntity();

    _registry.emplaceComponent<UUID>(boss, uuid);

    float scale = 4.5f;

    _registry.emplaceComponent<Transformable>(
        boss,
        x,
        y,
        15.0f,
        scale,
        scale,
        false
    );
    _registry.emplaceComponent<Drawable>(boss);
    _registry.emplaceComponent<SpriteRender>(boss, bossTexture, bossRect);
    _registry.emplaceComponent<Animated>(
        boss,
        bossRect,
        3,
        1,
        0.1f
    );

}

void ClientSceneLoader::setupMenuCreateButton(const json &data)
{
    entity_t createGameButton = _registry.spawnEntity();
    Rect createRect{0.0f, 0.0f, 150.0f, 60.0f};
    std::shared_ptr<Texture2D> createTexture = AssetManager::getInstance().loadAsset<Texture2D>("./assets/sprites/button_create.png");

    _registry.emplaceComponent<Button>(
        createGameButton,
        [this](){
            if (_sceneStack.top() == "menu") {
                enterScene("menu_create");
                return;
            }

            if (_sceneStack.top() == "menu_create") {
                Network::NetClient &channel = Network::NetClient::getInstance();
                _bitConverter
                    .setID(CLIENT_CREATE_GAME)
                    .addChar(static_cast<int>(_difficulty) | (_friendlyFire ? 0b00000100 : 0b00000000))
                    .compactMessage(channel.getBuffer());
            }
        }
    );
    _registry.emplaceComponent<Transformable>(
        createGameButton,
        224.0f,
        420.0f,
        0.0f,
        4.0f,
        4.0f,
        false
    );
    _registry.emplaceComponent<Drawable>(createGameButton);
    _registry.emplaceComponent<SpriteRender>(createGameButton, createTexture, createRect);
    _registry.emplaceComponent<Collision>(createGameButton, createRect.width, createRect.height);
}

void ClientSceneLoader::setupMenuDifficultyButton(const json &data)
{
    entity_t difficultyButton = _registry.spawnEntity();
    Rect difficultyRect{0.0f, 0.0f, 600.0f, 240.0f};
    std::shared_ptr<Texture2D> difficultyTexture = AssetManager::getInstance().loadAsset<Texture2D>("./assets/sprites/empty_button.png");

    entity_t difficultyText = _registry.spawnEntity();
    std::string difficultyLabel{};
    switch (_difficulty) {
        case Difficulty::EASY:
            difficultyLabel = "Easy";
            break;
        case Difficulty::NORMAL:
            difficultyLabel = "Normal";
            break;
        case Difficulty::HARD:
            difficultyLabel = "Hard";
            break;
    }
    _registry.emplaceComponent<Text>(difficultyText, "difficulty text", difficultyLabel, 150);
    _registry.emplaceComponent<Drawable>(difficultyText);
    _registry.emplaceComponent<Transformable>(difficultyText, 1120.0f, 150.0f);

    _registry.emplaceComponent<Button>(
        difficultyButton,
        [this, difficultyText](){
            if (!_registry.hasComponent<Text>(difficultyText))
                return;
            Text &text = _registry.getComponent<Text>(difficultyText);
            std::string &difficultyStr = text.getPart(0);

            switch (_difficulty) {
                case Difficulty::EASY:
                    _difficulty = Difficulty::NORMAL;
                    difficultyStr = "Normal";
                    break;
                case Difficulty::NORMAL:
                    _difficulty = Difficulty::HARD;
                    difficultyStr = "Hard";
                    break;
                case Difficulty::HARD:
                    _difficulty = Difficulty::EASY;
                    difficultyStr = "Easy";
                    break;
            }
        }
    );
    _registry.emplaceComponent<Transformable>(difficultyButton, 1096.0f, 120.0f);
    _registry.emplaceComponent<Drawable>(difficultyButton);
    _registry.emplaceComponent<SpriteRender>(difficultyButton, difficultyTexture, difficultyRect);
    _registry.emplaceComponent<Collision>(difficultyButton, difficultyRect.width, difficultyRect.height);
}

void ClientSceneLoader::setupMenuFriendlyFire(const json &data)
{
    entity_t friendlyFireButton = _registry.spawnEntity();
    Rect friendlyFireRect{0.0f, 0.0f, 600.0f, 240.0f};
    std::shared_ptr<Texture2D> fierndlyFireTexture = AssetManager::getInstance().loadAsset<Texture2D>("./assets/sprites/empty_button.png");

    entity_t friendlyFireText = _registry.spawnEntity();
    _registry.emplaceComponent<Text>(friendlyFireText, "friendly fire text", std::vector<std::string>{"Friendly fire: \n", _friendlyFire ? "on" : "off"}, 80);
    _registry.emplaceComponent<Drawable>(friendlyFireText);
    _registry.emplaceComponent<Transformable>(friendlyFireText, 1120.0f, 430.0f);

    _registry.emplaceComponent<Button>(
        friendlyFireButton,
        [this, friendlyFireText](){
            if (!_registry.hasComponent<Text>(friendlyFireText))
                return;
            Text &text = _registry.getComponent<Text>(friendlyFireText);
            if (text._parts.size() == 1)
                text.addPart("off");
            std::string &friendlyFireStr = text.getPart(1);

            _friendlyFire = !_friendlyFire;
            friendlyFireStr = _friendlyFire ? "on" : "off";
        }
    );
    _registry.emplaceComponent<Transformable>(friendlyFireButton, 1096.0f, 420.0f);
    _registry.emplaceComponent<Drawable>(friendlyFireButton);
    _registry.emplaceComponent<SpriteRender>(friendlyFireButton, fierndlyFireTexture, friendlyFireRect);
    _registry.emplaceComponent<Collision>(friendlyFireButton, friendlyFireRect.width, friendlyFireRect.height);
}

void ClientSceneLoader::setupMenuInputRemapButton(const json &data)
{
    entity_t inputRemapButton = _registry.spawnEntity();
    Rect inputRemapRect{0.0f, 0.0f, 600.0f, 240.0f};
    std::shared_ptr<Texture2D> inputRemapTexture = AssetManager::getInstance().loadAsset<Texture2D>("./assets/sprites/empty_button.png");

    entity_t inputRemapText = _registry.spawnEntity();
    _registry.emplaceComponent<Text>(inputRemapText, "input remap text", "Inputs", 150);
    _registry.emplaceComponent<Drawable>(inputRemapText);
    _registry.emplaceComponent<Transformable>(inputRemapText, 1120.0f, 750.0f);

    _registry.emplaceComponent<Button>(
        inputRemapButton,
        [this](){
            enterScene("menu_input_remap");
        }
    );
    _registry.emplaceComponent<Transformable>(inputRemapButton, 1096.0f, 720.0f);
    _registry.emplaceComponent<Drawable>(inputRemapButton);
    _registry.emplaceComponent<SpriteRender>(inputRemapButton, inputRemapTexture, inputRemapRect);
    _registry.emplaceComponent<Collision>(inputRemapButton, inputRemapRect.width, inputRemapRect.height);
}

void ClientSceneLoader::setupMenuJoinButton(const json &data)
{
    entity_t joinGameButton = _registry.spawnEntity();
    Rect joinRect{0.0f, 0.0f, 150.0f, 60.0f};
    std::shared_ptr<Texture2D> joinTexture = AssetManager::getInstance().loadAsset<Texture2D>("./assets/sprites/button_join.png");

    _registry.emplaceComponent<Button>(
        joinGameButton,
        [this](){
            enterScene("menu_join");
        }
    );
    _registry.emplaceComponent<Transformable>(
        joinGameButton,
        1096.0f,
        420.0f,
        0.0f,
        4.0f,
        4.0f,
        false
    );
    _registry.emplaceComponent<Drawable>(joinGameButton);
    _registry.emplaceComponent<SpriteRender>(joinGameButton, joinTexture, joinRect);
    _registry.emplaceComponent<Collision>(joinGameButton, joinRect.width, joinRect.height);
}

void ClientSceneLoader::setupMenuJoinElements(const json &data)
{
    entity_t menuJoinText = _registry.spawnEntity();
    entity_t menuJoinInput = _registry.spawnEntity();

    Rect menuJoinRect{0.0f, 0.0f, 600.0f, 240.0f};
    std::shared_ptr<Texture2D> menuJoinTexture = AssetManager::getInstance().loadAsset<Texture2D>("./assets/sprites/empty_button.png");

    _registry.emplaceComponent<Button>(
        menuJoinInput,
        [this, menuJoinText](){
            if (!_registry.hasComponent<Text>(menuJoinText))
                return;
            Text &text = _registry.getComponent<Text>(menuJoinText);
            if (text._parts.size() != 1)
                return;
            text.addPart("");
        },
        [this, menuJoinText](){
            if (!_registry.hasComponent<Text>(menuJoinText))
                return;
            Text &text = _registry.getComponent<Text>(menuJoinText);
            if (text._parts.size() < 2)
                return;
            text.removePart(1);
        }
    );
    _registry.emplaceComponent<Transformable>(menuJoinInput, 224.0f, 420.0f);
    _registry.emplaceComponent<Drawable>(menuJoinInput);
    _registry.emplaceComponent<SpriteRender>(menuJoinInput, menuJoinTexture, menuJoinRect);
    _registry.emplaceComponent<Collision>(menuJoinInput, menuJoinRect.width, menuJoinRect.height);


    _registry.emplaceComponent<Transformable>(menuJoinText, 245.0f, 465.0f);
    _registry.emplaceComponent<Text>(menuJoinText, "menu join", "", 150);
    _registry.emplaceComponent<Drawable>(menuJoinText);
    _registry.emplaceComponent<Input>(
        menuJoinText,
        Input::actionList{
            [this](Registry &r, size_t e){
                if (!r.hasComponent<Text>(e) || !r.hasComponent<TimerContainer>(e))
                    return;
                Text &text = r.getComponent<Text>(e);
                TimerContainer &timers = r.getComponent<TimerContainer>(e);
                if (text._parts.empty())
                    text.addPart("2000");
                std::string &numberStr = text.getPart(0);

                if (text._parts.size() >= 2 && timers.safeUniqueConsume("input - underscore display", 0.5f)) {
                    std::string &underscoreStr = text.getPart(1);

                    if (underscoreStr == "_")
                        underscoreStr = "";
                    else
                        underscoreStr = "_";
                }

                for (int key = KEY_ZERO; key <= KEY_NINE; ++key) {
                    if (!IsKeyDown(key))
                        continue;
                    if (numberStr.length() >= 6)
                        break;
                    bool timerAlreadyExisted = timers.contains("input - repeat key pressed");
                    if (timers.safeUniqueConsume("input - repeat key pressed", 0.3f)) {
                        numberStr += static_cast<char>(key);
                    } else if (!timerAlreadyExisted) {
                        // *numberStr += static_cast<char>(key);
                    }
                }
                if (IsKeyDown(KEY_BACKSPACE) && !numberStr.empty() && timers.safeUniqueConsume("input - remove delay", 0.5f))
                    numberStr.pop_back();
            }
        }
    );

    entity_t joinGameButton = _registry.spawnEntity();
    Rect joinRect{0.0f, 0.0f, 150.0f, 60.0f};
    std::shared_ptr<Texture2D> joinTexture = AssetManager::getInstance().loadAsset<Texture2D>("./assets/sprites/button_join.png");

    _registry.emplaceComponent<Button>(
        joinGameButton,
        [this, menuJoinText](){
            if (!_registry.hasComponent<Text>(menuJoinText))
                return;
            Text &text = _registry.getComponent<Text>(menuJoinText);
            if (text._parts.empty())
                return;
            try {
                int port = std::stoi(text.getPart(0));

                Network::NetClient &channel = Network::NetClient::getInstance();
                _bitConverter
                    .setID(CLIENT_JOIN_GAME)
                    .addNumber(port)
                    .compactMessage(channel.getBuffer());
            } catch (...) {
            }
        }
    );
    _registry.emplaceComponent<Transformable>(
        joinGameButton,
        1096.0f,
        420.0f,
        0.0f,
        4.0f,
        4.0f,
        false
    );
    _registry.emplaceComponent<Drawable>(joinGameButton);
    _registry.emplaceComponent<SpriteRender>(joinGameButton, joinTexture, joinRect);
    _registry.emplaceComponent<Collision>(joinGameButton, joinRect.width, joinRect.height);
}

void ClientSceneLoader::setupMenuInputRemap(const json &data)
{
    static std::unordered_map<KEY, bool> clickedStates{};
    static std::array<std::pair<Vector2, KEY>, 5> inputPositions = {
            std::make_pair(Vector2(600, 100), KEY::UP),
            std::make_pair(Vector2(1100, 100), KEY::DOWN),
            std::make_pair(Vector2(600, 500), KEY::LEFT),
            std::make_pair(Vector2(1100, 500), KEY::RIGHT),
            std::make_pair(Vector2(850, 800), KEY::ATTACK)
    };

    for (auto &[pos, key] : inputPositions)
        inputTextRemapHelper(key, pos, clickedStates);
}

void ClientSceneLoader::setupLeaveButton(const json &data)
{
    entity_t leaveButton = _registry.spawnEntity();
    Rect leaveRect{0.0f, 0.0f, 80.0f, 80.0f};
    std::shared_ptr<Texture2D> leaveTexture = AssetManager::getInstance().loadAsset<Texture2D>("./assets/sprites/quit.png");

    _registry.emplaceComponent<HUD>(leaveButton);
    _registry.emplaceComponent<Button>(
        leaveButton,
        [this](){
            if (_sceneStack.top() == "menu") {
                Network::NetClient::_shouldStop = true;
                return;
            }

            if (_sceneStack.top().starts_with("menu")) {
                leaveScene();
                return;
            }

            if (_sceneStack.top().starts_with("level")) {
                Network::NetClient &channel = Network::NetClient::getInstance();
                channel.stop();
                restartFromScene("menu");
                return;
            }
        }
    );
    _registry.emplaceComponent<Transformable>(
        leaveButton,
        1880.0f,
        10.0f,
        0.0f,
        0.5f,
        0.5f,
        false
    );

    _registry.emplaceComponent<Drawable>(leaveButton);
    _registry.emplaceComponent<SpriteRender>(leaveButton, leaveTexture, leaveRect);
    _registry.emplaceComponent<Collision>(leaveButton, leaveRect.width, leaveRect.height);
}

void ClientSceneLoader::setupAdminPanelButton(const json &data)
{
    if (!_admin) {
        try {
            if (!data.contains("id") || !data.contains("password")
                || !data.at("id").is_number_integer() || !data.at("password").is_string())
                return;

            int id = data.at("id");
            const std::string &password = data.at("password");

            Network::NetClient &channel = Network::NetClient::getInstance();
            _bitConverter
                .setID(CLIENT_ADMIN)
                .addNumber(id)
                .addString(password)
                .compactMessage(channel.getBuffer());
        } catch (const json::exception &e) {
            std::cerr << "JSON parsing error: " << e.what() << std::endl;
            return;
        } catch (const std::exception &e) {
            std::cerr << "Error setting up action buttons: " << e.what() << std::endl;
            return;
        }
    }

    entity_t adminPanelButton = _registry.spawnEntity();
    Rect adminPanelRect{0.0f, 0.0f, 240.0f, 240.0f};
    std::shared_ptr<Texture2D> adminPanelTexture = AssetManager::getInstance().loadAsset<Texture2D>("./assets/sprites/empty_square_button.png");

    _registry.emplaceComponent<Button>(
        adminPanelButton,
        [this](){
            if (!_admin) {
                std::cout << "Not admin!" << std::endl;
                return;
            }
            enterScene("menu_admin_panel");
        }
    );
    _registry.emplaceComponent<Transformable>(
        adminPanelButton,
        1820.0f,
        10.0f,
        0.0f,
        0.16f,
        0.16f,
        false
    );

    _registry.emplaceComponent<Drawable>(adminPanelButton);
    _registry.emplaceComponent<SpriteRender>(adminPanelButton, adminPanelTexture, adminPanelRect);
    _registry.emplaceComponent<Collision>(adminPanelButton, adminPanelRect.width, adminPanelRect.height);
}

void ClientSceneLoader::setupAdminPanelElems(const json &data)
{
    Network::NetClient &n = Network::NetClient::getInstance();

    // std::unordered_map<int, int> games = {{2000, 3}, {3002, 1}}; // ! Example
    std::unordered_map<int, int> &games = n.adminGetGames();

    float baseY = 100.0f;


    for (auto &[port, playersNumber]: games) {
        entity_t gameText = _registry.spawnEntity();
        std::string gameString = "Game [" + std::to_string(port) + "] with " + std::to_string(playersNumber) + "/4 players";
        _registry.emplaceComponent<Transformable>(gameText, 200.0f, baseY);
        _registry.emplaceComponent<Text>(gameText, "game info", gameString, 50);
        _registry.emplaceComponent<Drawable>(gameText);

        entity_t closeGameButton = _registry.spawnEntity();
        Rect closeGameRect{0.0f, 0.0f, 80.0f, 80.0f};
        std::shared_ptr<Texture2D> closeGameTexture = AssetManager::getInstance().loadAsset<Texture2D>("./assets/sprites/quit.png");

        _registry.emplaceComponent<Button>(
            closeGameButton,
            [this, port, gameText, closeGameButton](){
                Network::NetClient &n = Network::NetClient::getInstance();
                _bitConverter
                    .setID(CLIENT_STOP_GAME)
                    .addNumber(port)
                    .compactMessage(n.getBuffer());
                _registry.killEntity(gameText);
                _registry.killEntity(closeGameButton);
            }
        );
        _registry.emplaceComponent<Transformable>(
            closeGameButton,
            100.0f,
            baseY + 30,
            0.0f,
            0.5f,
            0.5f,
            false
        );

        _registry.emplaceComponent<Drawable>(closeGameButton);
        _registry.emplaceComponent<SpriteRender>(closeGameButton, closeGameTexture, closeGameRect);
        _registry.emplaceComponent<Collision>(closeGameButton, closeGameRect.width, closeGameRect.height);

        baseY += 200;
    }
}

void ClientSceneLoader::setAdmin(bool state)
{
    _admin = state;
}

void ClientSceneLoader::inputTextRemapHelper(
    KEY key,
    const Vector2 &pos,
    std::unordered_map<KEY, bool> &clickedStates
)
{
    entity_t inputRemapText = _registry.spawnEntity();
    entity_t inputRemapButton = _registry.spawnEntity();
    entity_t inputName = _registry.spawnEntity();

    clickedStates[key] = false;

    std::shared_ptr<Texture2D> texture = AssetManager::getInstance().loadAsset<Texture2D>("./assets/sprites/empty_square_button.png");
    Rect rect{0.0f, 0.0f, 240.0f, 240.0f};

    _registry.emplaceComponent<Button>(
            inputRemapButton,
            [this, inputRemapText, &clickedStates, key](){
                clickedStates[key] = true;
                if (!_registry.hasComponent<Text>(inputRemapText))
                    return;
                _registry.getComponent<Text>(inputRemapText).color = RED;
            },
            [this, inputRemapText, &clickedStates, key](){
                clickedStates[key] = false;
                if (!_registry.hasComponent<Text>(inputRemapText))
                    return;
                _registry.getComponent<Text>(inputRemapText).color = WHITE;
            }
    );
    _registry.emplaceComponent<Transformable>(inputRemapButton, pos.x, pos.y);
    _registry.emplaceComponent<Drawable>(inputRemapButton);
    _registry.emplaceComponent<SpriteRender>(inputRemapButton, texture, rect);
    _registry.emplaceComponent<Collision>(inputRemapButton, rect.width, rect.height);

    _registry.emplaceComponent<Text>(inputName, "input name", _keyStrings[key], 70);
    _registry.emplaceComponent<Drawable>(inputName);
    _registry.emplaceComponent<Transformable>(inputName, pos.x, pos.y - 20);

    _registry.emplaceComponent<Transformable>(inputRemapText, pos.x + 50, pos.y + 70);
    _registry.emplaceComponent<Text>(inputRemapText, "input remap text", std::string(_keyToString[_keyMaps[key]]), 100);
    _registry.emplaceComponent<Drawable>(inputRemapText);
    _registry.emplaceComponent<Input>(
            inputRemapText,
            Input::actionList{
                    [this, key, &clickedStates](Registry &r, size_t e){
                        if (!r.hasComponent<Text>(e) || !clickedStates[key])
                            return;
                        Text &text = r.getComponent<Text>(e);
                        std::string &keyStr = text.getPart(0);

                        for (int inputKey = KEY_ZERO; inputKey <= KEY_UP; ++inputKey) {
                            if (!IsKeyDown(inputKey))
                                continue;
                            auto k = static_cast<KeyboardKey>(inputKey);
                            if (!_keyToString.contains(k))
                                continue;
                            keyStr = _keyToString[k];
                            _keyMaps[key] = k;
                            clickedStates[key] = false;
                            text.color = WHITE;
                            break;
                        }
                    }
            }
    );
}
