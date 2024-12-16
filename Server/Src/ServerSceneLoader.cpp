/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Oct, 2024                                                     *
 * Title           - GameEngine                                                        *
 * Description     -                                                                    *
 *     ServerSceneLoader                                                                *
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

#include "ServerSceneLoader.hpp"
#include "GameEngineIncludesServer.hpp"
#include "Game.hpp"

ServerSceneLoader::ServerSceneLoader(
    Registry &registry,
    std::shared_ptr<Network::Game> game,
    const std::string &configFile,
    Difficulty difficulty,
    bool friendlyFire
)
:
_registry(registry),
_bitConverter(Network::BitConverter::getInstance()),
_game(game),
_jsonHandler(configFile),
_difficulty(difficulty),
_friendlyFire(friendlyFire),
_playerHp(20)
{
    Py_Initialize();
    PyObject *sysPath = PySys_GetObject("path");
    PyObject *curDir = PyUnicode_FromString("./scripts");
    PyList_Append(sysPath, curDir);
    Py_DECREF(curDir);

    setupEntityMapping();
    setupComponentRemovalMap();

    switch (_difficulty) {
        case Difficulty::EASY:
            _spawnIntervalMultiplier = 1.5f;
            break;
        case Difficulty::NORMAL:
            _spawnIntervalMultiplier = 1.0f;
            break;
        case Difficulty::HARD:
            _spawnIntervalMultiplier = 0.5f;
            break;
    }
}

ServerSceneLoader::~ServerSceneLoader()
{
    Py_Finalize();
}

void ServerSceneLoader::loadScene(const std::string &sceneID)
{
    if (!_jsonHandler.contains({"scenes", sceneID})) {
        std::cerr << "Failed to load scene " << sceneID << std::endl;
        return;
    }

    if (_game) {
        if (sceneID == "menu") {
            _game->swapScene(ID_SCENE_MENU);
        } else if (sceneID == "level_1") {
            _game->swapScene(ID_SCENE_LEVEL_1);
        } else if (sceneID == "level_2") {
            _game->swapScene(ID_SCENE_LEVEL_2);
        }
    }

    auto sceneData = _jsonHandler.get<json>({"scenes", sceneID}, json{});
    if (_jsonHandler.contains({"player_hp"}))
        _playerHp = _jsonHandler.get<json>({"player_hp"}, json{});

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
}

void ServerSceneLoader::setupEntityMapping()
{
    _entitySetupMap["brain_mob_spawner"] = [&](const json &data) { setupBrainMobSpawner(data); };
    _entitySetupMap["rolling_mob_spawner"] = [&](const json &data) { setupRollingMobSpawner(data); };
    _entitySetupMap["screen_walls"] = [&](const json &data) { setupScreenWalls(data); };
    _entitySetupMap["brain_seahorse_boss"] = [&](const json &data) { setupBrainSeahorseBoss(data); };
}

void ServerSceneLoader::setupComponentRemovalMap()
{
    _componentRemovalMap["Enemy"] = [&]() { removeEntitiesWithComponent<Enemy>(); };
    _componentRemovalMap["Bullet"] = [&]() { removeEntitiesWithComponent<Bullet>(); };
    _componentRemovalMap["EnemySpawner"] = [&]() { removeEntitiesWithComponent<EnemySpawner>(); };
}

UUID ServerSceneLoader::setupPlayer()
{
    entity_t player = _registry.spawnEntity();

    UUID playerUUID = UUIDManager::generateUUID();
    _registry.emplaceComponent<UUID>(player, playerUUID);

    float playerWidth = 65.3f;
    float playerHeight = 50.0f;

    _registry.emplaceComponent<Transformable>(player, 80.0f, 487.0f);
    _registry.emplaceComponent<Velocity>(player, 0.0f, 0.0f, 1.0f, 5.0f);

    _registry.emplaceComponent<Acceleration>(player);
    _registry.emplaceComponent<Collision>(player, playerWidth, playerHeight);
    _registry.emplaceComponent<Player>(player);
    _registry.emplaceComponent<Health>(
        player,
        _playerHp,
        [this](Registry &r, size_t entity) {
            if (_game && r.hasComponent<UUID>(entity)) {
                UUID uuid = r.getComponent<UUID>(entity);
                for (auto &[endpoint, client] : _game->getClients())
                    if (uuid == client.uuid)
                        client.alive = false;
            }
        }
    );

    try {
        _registry.emplaceComponent<Spawner>(
            player,
            getPlayerWeaponFromID(player, "straight", 1, Vec2(0.0f, 25.0f)),
            0.1f,
            [](Transformable &){},
            [](Registry &r, size_t e){
                if (r.hasComponent<Player>(e)) {
                    Player &p = r.getComponent<Player>(e);
                    bool shooting = p.shooting;
                    p.shooting = false;
                    return shooting;
                }
                return false;
            }
        );
    } catch (...) {
        std::cerr << "Error during bullet spawner creation" << std::endl;
    }

    return playerUUID;
}

void ServerSceneLoader::changePlayerWeapon(const std::string &weaponID, int shotCount, const Vec2 &offset)
{
    for (auto &&[e, _] : IndexedZipper(_registry.getComponents<Player>()))
        swapPlayerWeapon(e, weaponID, shotCount, offset);
}

void ServerSceneLoader::setupBrainMobSpawner(const json &data)
{
    entity_t spawner = _registry.spawnEntity();

    auto spawnEnemy = [this](Registry &r, const Vec2 &coords) {
        float enemyWidth = 65.25f;
        float enemyHeight = 132.0f;

        entity_t enemy = r.spawnEntity();

        UUID uuid = UUIDManager::generateUUID();
        r.emplaceComponent<UUID>(enemy, uuid);
        r.emplaceComponent<Transformable>(enemy, coords.x, coords.y);
        r.emplaceComponent<Velocity>(enemy);
        r.emplaceComponent<Acceleration>(enemy);
        r.emplaceComponent<Collision>(enemy, enemyWidth, enemyHeight);
        r.emplaceComponent<BrainEnemy>(enemy);
        r.emplaceComponent<Script>(
            enemy,
            "brain_enemy_system",
            Script::scriptData{
                {"ascending", false}
            }
        );
        r.emplaceComponent<Enemy>(enemy);
        r.emplaceComponent<Health>(enemy, 5);

        auto spawnBullet = [this, enemy](Registry &r, const Vec2 &coords) {
            auto [x, y] = coords;

            entity_t newBullet = r.spawnEntity();

            UUID bulletUuid = UUIDManager::generateUUID();
            r.emplaceComponent<UUID>(newBullet, bulletUuid);

            r.emplaceComponent<BrainEnemyBullet>(newBullet);
            r.emplaceComponent<Bullet>(newBullet, enemy, 1);
            r.emplaceComponent<Transformable>(newBullet, x, y + 35.0f, 0.0f, 2.0f, 2.0f);
            r.emplaceComponent<Acceleration>(newBullet, -10.0f, 0.0f, 0.0f, 1.0f);
            r.emplaceComponent<Velocity>(newBullet, -20.0f);
            r.emplaceComponent<Gravity>(newBullet);
            r.emplaceComponent<Collision>(newBullet,
                17.08f,
                18.0f,
                Vec2(0.0f, 0.0f),
                [this](Registry &r, size_t entity, size_t collidedEntity) {
                    if (!r.hasComponent<Player>(collidedEntity)
                        || !r.hasComponent<Health>(collidedEntity)
                        || !r.hasComponent<Bullet>(entity))
                        return false;
                    Bullet &b = r.getComponent<Bullet>(entity);
                    Health &h = r.getComponent<Health>(collidedEntity);

                    h.takeDamage(b.damage);

                    if (_game)
                        _game->entityDamaged(collidedEntity);

                    r.killEntity(entity);

                    return false;
                }
            );
        };

        r.emplaceComponent<Spawner>(enemy, spawnBullet, 0.7f * _spawnIntervalMultiplier);
    };

    auto moveSpawner = [](Transformable &t) {
        static thread_local auto move = [](float &coord, bool &direction, float minVal, float maxVal, float step) {
            if (direction) {
                if (coord <= minVal)
                    direction = false;
                coord -= step;
            } else {
                if (coord >= maxVal)
                    direction = true;
                coord += step;
            }
        };

        static thread_local bool ascending = false;
        static thread_local bool goingForward = false;

        move(t.y, ascending, 100.0f, 900.0f, 5.0f);
        move(t.x, goingForward, 2000.0f, 2100.0f, 5.0f);
    };

    _registry.emplaceComponent<Spawner>(
        spawner,
        spawnEnemy,
        2.5f * _spawnIntervalMultiplier,
        // 0.01f,
        0.5f,
        moveSpawner
    );
    _registry.emplaceComponent<EnemySpawner>(spawner);
    _registry.emplaceComponent<Transformable>(spawner, Math::randomDouble(1700.0f, 1900.0f), Math::randomDouble(200.0f, 800.0f));
}

void ServerSceneLoader::setupRollingMobSpawner(const json &data)
{
    entity_t spawner = _registry.spawnEntity();

    auto spawnEnemy = [this](Registry &r, const Vec2 &coords) {
        spawnRollingMob(coords);
    };

    auto moveSpawner = [](Transformable &t) {
        static auto move = [](float &coord, bool &direction, float minVal, float maxVal, float step) {
            if (direction) {
                if (coord <= minVal)
                    direction = false;
                coord -= step;
            } else {
                if (coord >= maxVal)
                    direction = true;
                coord += step;
            }
        };

        static thread_local bool ascending = false;

        move(t.y, ascending, 100.0f, 900.0f, 5.0f);
    };

    _registry.emplaceComponent<Spawner>(
        spawner,
        spawnEnemy,
        0.5f * _spawnIntervalMultiplier,
        0.3f,
        moveSpawner
    );
    _registry.emplaceComponent<EnemySpawner>(spawner);
    _registry.emplaceComponent<Transformable>(spawner, 1920.0f, 500.0f);
}

void ServerSceneLoader::setupScreenWalls(const json &data)
{
    auto wallCollision = [](Registry &r, size_t, size_t collidedEntity) {
        return r.hasComponent<Player>(collidedEntity);
    };

    auto downWallCollision = [](Registry &r, size_t, size_t collidedEntity) {
        return r.hasComponent<Player>(collidedEntity) || r.hasComponent<RollingEnemy>(collidedEntity);
    };

    auto createWall = [&](
        float width,
        float height,
        float x,
        float y,
        std::function<bool(Registry &, size_t, size_t)> onCollideLeft = [](Registry &, size_t, size_t) { return false; },
        std::function<bool(Registry &, size_t, size_t)> onCollideRight = [](Registry &, size_t, size_t) { return false; }
    ) {
        entity_t wall = _registry.spawnEntity();
        _registry.emplaceComponent<Collision>(wall, width, height, Vec2(0.0f, 0.0f), onCollideLeft, onCollideRight);
        _registry.emplaceComponent<Transformable>(wall, x, y, false);
        _registry.emplaceComponent<Wall>(wall);
        return wall;
    };

    createWall(100.0f, 1080.0f, -100.0f, 0.0f, wallCollision, wallCollision);
    createWall(100.0f, 1080.0f, 1920.0f, 0.0f, wallCollision, wallCollision);
    createWall(1920.0f + 100.0f * 2.0f, 100.0f, -100.0f, -100.0f, wallCollision, wallCollision);
    entity_t bottomWall = createWall(1920.0f + 100.0f * 2.0f, 100.0f, -100.0f, 1080.0f - 75.0f, downWallCollision, downWallCollision);
    _registry.emplaceComponent<BottomWall>(bottomWall);
}

void ServerSceneLoader::setupBrainSeahorseBoss(const json &data)
{


    entity_t spawner = _registry.spawnEntity();
    std::string nextLevel{};
    float bossTimer = 30.0f;
    float bossHp = 100.0f;

    if (data.contains("next_level") && data.at("next_level").is_string())
        nextLevel = data["next_level"];
    if (data.contains("timer") && data.at("timer").is_number_float())
        bossTimer = data.at("timer");
    if (data.contains("hp") && data.at("hp").is_number_integer())
        bossHp = data.at("hp");

    auto spawnBoss = [this, nextLevel, bossHp](Registry &r, const Vec2 &coords) {
        static thread_local bool firstPass = true;

        if (firstPass) {
            firstPass = false;
            return;
        }

        float bossWidth = 162.25f;
        float bossHeight = 212.0f;

        entity_t boss = r.spawnEntity();

        UUID uuid = UUIDManager::generateUUID();
        r.emplaceComponent<UUID>(boss, uuid);

        r.emplaceComponent<BossSeahorse>(boss);

        float scale = 4.5f;

        r.emplaceComponent<Transformable>(
            boss,
            coords.x,
            coords.y,
            15.0f,
            scale,
            scale,
            false
        );
        r.emplaceComponent<Collision>(
            boss,
            bossWidth,
            bossHeight,
            Vec2(bossWidth * scale / 3.0f, 0.0f),
            [this](Registry &r, size_t entity, size_t collidedEntity) {
                if (!r.hasComponent<Player>(collidedEntity)
                    || !r.hasComponent<Health>(collidedEntity)
                    || !r.hasComponent<TimerContainer>(entity))
                    return false;
                Health &h = r.getComponent<Health>(collidedEntity);
                TimerContainer &t = r.getComponent<TimerContainer>(entity);

                if (!t.safeConsume("boss-player collision", 0.5f))
                    return true;
                h.takeDamage(1);
                if (_game)
                    _game->entityDamaged(collidedEntity);
                return true;
            }
        );
        r.emplaceComponent<Health>(
            boss,
            bossHp,
            [this, nextLevel](Registry &r, size_t entity) {
                loadScene(nextLevel);
            }
        );

        auto spawnRolling = [this, boss](Registry &r, const Vec2 &coords) {
            if (!r.hasComponent<TimerContainer>(boss) || !r.hasComponent<Health>(boss))
                return;
            TimerContainer &timer = r.getComponent<TimerContainer>(boss);
            Health &health = r.getComponent<Health>(boss);

            if (timer.contains("boss cooldown") && !timer.safeUniqueConsume("boss cooldown", 1.0f))
                return;

            if (timer.safeUniqueConsume("boss burst interval", 1.0f)) {
                timer.add("boss cooldown", 1.0f);
                return;
            }

            double offset = Math::randomDouble(0.0f, 1000.0f);
            bool hasGravity = Math::randomInt(0, 1);

            Vec2 newCoords(coords.x + offset / 20.0f, coords.y + offset);

            float velocity = Math::randomDouble(-10.0f, -30.0f);
            float acceleration = Math::randomDouble(-5.0f, -15.0f);

            int hp = 1;
            if (health.current <= 30) {
                velocity *= 2.5f;
                acceleration *= 2.5f;
                hp = 3;
            }

            spawnRollingMob(newCoords, velocity, acceleration, hp, hasGravity);
        };

        r.emplaceComponent<Spawner>(
            boss,
            spawnRolling,
            0.05f * _spawnIntervalMultiplier
        );
        _registry.emplaceComponent<Enemy>(boss);

        removeEntitiesWithComponent<EnemySpawner>();
    };

    _registry.emplaceComponent<Spawner>(
        spawner,
        spawnBoss,
        bossTimer
    );
    _registry.emplaceComponent<EnemySpawner>(spawner);
    _registry.emplaceComponent<Transformable>(spawner, 1570.0f, -50.0f);
}

void ServerSceneLoader::spawnRollingMob(const Vec2 &coords, float velocity, float acceleration, int hp, bool hasGravity)
{
    float enemyWidth = 33.125f;
    float enemyHeight = 33.4f;

    entity_t enemy = _registry.spawnEntity();

    UUID uuid = UUIDManager::generateUUID();
    _registry.emplaceComponent<UUID>(enemy, uuid);
    _registry.emplaceComponent<Transformable>(enemy, coords.x, coords.y);
    _registry.emplaceComponent<Velocity>(enemy, velocity);
    _registry.emplaceComponent<Acceleration>(enemy, acceleration, 0.0f, 0.0f, 1.0f);
    _registry.emplaceComponent<Collision>(
        enemy,
        enemyWidth,
        enemyHeight,
        Vec2(0.0f, 0.0f),
        [this](Registry &r, size_t entity, size_t collidedEntity) {
            if (r.hasComponent<BottomWall>(collidedEntity)) {
                if (!r.hasComponent<Gravity>(entity))
                    return true;

                r.getComponent<Gravity>(entity).velocityY -= Math::randomDouble(0.5f, 1.5f);
                return false;
            }

            if (!r.hasComponent<Player>(collidedEntity)
                || !r.hasComponent<Health>(collidedEntity))
                return false;
            Health &h = r.getComponent<Health>(collidedEntity);

            h.takeDamage(1);

            if (_game)
                _game->entityDamaged(collidedEntity);

            r.killEntity(entity);
            return false;
        }
    );
    _registry.emplaceComponent<Health>(enemy, hp);
    _registry.emplaceComponent<RollingEnemy>(enemy);
    _registry.emplaceComponent<Enemy>(enemy);
    if (hasGravity)
        _registry.emplaceComponent<Gravity>(enemy);
}

std::function<void(Registry &, const Vec2 &)> ServerSceneLoader::getPlayerWeaponFromID(
    size_t playerID,
    const std::string &weaponID,
    int shotCount,
    const Vec2 &offset
)
{
    std::unordered_map<std::string, std::function<void(Registry &, const Vec2 &)>> weaponMap = {
        {"straight", [this, playerID](Registry &r, const Vec2 &coords) {
            auto [x, y] = coords;

            entity_t newBullet = r.spawnEntity();

            UUID uuid = UUIDManager::generateUUID();
            r.emplaceComponent<UUID>(newBullet, uuid);

            r.emplaceComponent<PlayerBullet>(newBullet);
            r.emplaceComponent<Bullet>(newBullet, playerID, 1);
            r.emplaceComponent<Transformable>(newBullet, x, y + 10.0f);
            r.emplaceComponent<Acceleration>(newBullet, 10.0f, 0.0f, 0.0f, 0.0f);
            r.emplaceComponent<Velocity>(newBullet, 10.0f);
            r.emplaceComponent<Collision>(newBullet,
                33.31f,
                36.0f,
                Vec2(0.0f, 0.0f),
                [this, playerID](Registry &r, size_t entity, size_t collidedEntity) {
                    if ((!_friendlyFire && !r.hasComponent<Enemy>(collidedEntity))
                        || !r.hasComponent<Health>(collidedEntity)
                        || !r.hasComponent<Bullet>(entity)
                        || playerID == collidedEntity)
                        return false;
                    Bullet &b = r.getComponent<Bullet>(entity);
                    Health &h = r.getComponent<Health>(collidedEntity);

                    h.takeDamage(b.damage);

                    if (_game)
                        _game->entityDamaged(collidedEntity);

                    r.killEntity(entity);

                    return false;
            });
        }},
        {"bouncing", [this, playerID](Registry &r, const Vec2 &coords) {
            auto [x, y] = coords;

            entity_t newBullet = r.spawnEntity();

            UUID uuid = UUIDManager::generateUUID();
            r.emplaceComponent<UUID>(newBullet, uuid);

            r.emplaceComponent<PlayerBullet>(newBullet);
            r.emplaceComponent<Bullet>(newBullet, playerID, 1);
            r.emplaceComponent<Transformable>(newBullet, x, y + 10.0f);
            r.emplaceComponent<Acceleration>(newBullet, 0.0f, 0.0f, 0.0f, 1.0f);
            r.emplaceComponent<Velocity>(newBullet, 10.0f, 0.0f, 1.0f, 10.0f);
            r.emplaceComponent<Collision>(newBullet,
                33.31f,
                36.0f,
                Vec2(0.0f, 0.0f),
                [this, playerID](Registry &r, size_t entity, size_t collidedEntity) {
                    if ((!_friendlyFire && !r.hasComponent<Enemy>(collidedEntity))
                        || !r.hasComponent<Health>(collidedEntity)
                        || !r.hasComponent<Bullet>(entity)
                        || playerID == collidedEntity)
                        return false;
                    Bullet &b = r.getComponent<Bullet>(entity);
                    Health &h = r.getComponent<Health>(collidedEntity);

                    h.takeDamage(b.damage);

                    if (_game)
                        _game->entityDamaged(collidedEntity);
                    r.killEntity(entity);

                    return false;
                },
                [this](Registry &r, size_t entity, size_t collidedEntity) {
                    if (!r.hasComponent<Wall>(collidedEntity)
                        || !r.hasComponent<Gravity>(entity)
                        || !r.hasComponent<Velocity>(entity)
                        || !r.hasComponent<Acceleration>(entity)
                        || !r.hasComponent<TimerContainer>(entity))
                        return false;
                    Gravity &g = r.getComponent<Gravity>(entity);
                    Velocity &v = r.getComponent<Velocity>(entity);
                    Acceleration &a = r.getComponent<Acceleration>(entity);
                    TimerContainer &timers = r.getComponent<TimerContainer>(entity);
                    if (!timers.contains("bouncing bullet - gravity update")) {
                        timers.add("bouncing bullet - gravity update", 1.0f);
                        ++timers._timers["bouncing bullet - gravity update"]._reached;
                    }
                    if (!timers.safeConsume("bouncing bullet - gravity update", 1.0f))
                        return false;
                    g.accelerationY *= -1.0f;
                    g.velocityY *= -1.0f;

                    v.vy = 0.0f;
                    a.ay == 0.0f;

                    return false;
                }
            );
            r.emplaceComponent<Gravity>(newBullet, 0.0f, -0.2f);
        }}
    };

    auto it = weaponMap.find(weaponID);
    if (it == weaponMap.end())
        throw std::invalid_argument("Invalid weapon ID: " + weaponID);
    auto func = it->second;
    if (shotCount <= 1)
        return func;

    return [this, weaponID, shotCount, offset, func](Registry &r, const Vec2 &coords) {
        for (int i = 0; i < shotCount; ++i) {
            Vec2 adjustedCoords = coords;

            if (offset.x == 0.0f && offset.y != 0.0f) {
                adjustedCoords.y += (i - shotCount / 2) * offset.y;
            } else if (offset.x != 0.0f && offset.y == 0.0f) {
                adjustedCoords.x += (i - shotCount / 2) * offset.x;
            } else if (offset.x != 0.0f && offset.y != 0.0f) {
                adjustedCoords.x += (i - shotCount / 2) * offset.x;
                adjustedCoords.y += (i - shotCount / 2) * offset.y;
            }

            func(r, adjustedCoords);
        }
    };
}

void ServerSceneLoader::swapPlayerWeapon(
    size_t playerID,
    const std::string &weaponID,
    int shotCount,
    const Vec2 &offset
)
{
    if (!_registry.hasComponent<Player>(playerID) || !_registry.hasComponent<Spawner>(playerID))
        return;

    Player &player = _registry.getComponent<Player>(playerID);
    Spawner &spawner = _registry.getComponent<Spawner>(playerID);

    try {
        auto newWeapon = getPlayerWeaponFromID(playerID, weaponID, shotCount, offset);

        spawner.onSpawn = newWeapon;
    } catch (const std::invalid_argument &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
