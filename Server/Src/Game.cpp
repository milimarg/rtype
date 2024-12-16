/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** Game
*/

#if defined(_WIN32)
    #define NOGDI
    #define NOUSER
#endif

#include "Game.hpp"
#include "GameEngineIncludesServer.hpp"
#include "ServerSceneLoader.hpp"

namespace Network
{
    Game::Game(Difficulty difficulty, bool friendlyFire) :
        _socket(std::make_shared<UDP::socket>(_contextIO, UDP::v4())),
        _currentScene(ID_SCENE_MENU),
        _network(std::make_shared<UDPNetwork>(_contextIO)),
        _isRunning(true),
        _gameStart(false),
        _bitConverter(BitConverter::getInstance()),
        _registry(Registry::getInstance()),
        _difficulty(difficulty),
        _friendlyFire(friendlyFire)
    {
        if (!_socket->is_open())
            _socket->open(UDP::v4());
        unsigned port = 2000;
        while (port < MAX_PORT) {
            try {
                _socket->bind(UDP::endpoint(UDP::v4(), port));
                _port = port;
                break;
            } catch (const std::exception& e) {
                port++;
            }
        }
    }

    Game::~Game()
    {
        _contextIO.run_for(std::chrono::milliseconds(1));
        _contextIO.stop();
        _socket->close();
    }

    unsigned short Game::getPort() const
    {
        return _port;
    }

    void Game::run(std::shared_ptr<Game> gamePtr)
    {
        Registry &registry = Registry::getInstance();

        setupGameEngine();
        _sceneLoader = std::make_shared<ServerSceneLoader>(registry, gamePtr, "config/server-config.json", _difficulty, _friendlyFire);
        _sceneLoader->loadScene("level_1");


        _network->receiveMessage(_socket, [this](Packet packet, packetsType type, bool status) {
            try {
                UDP::endpoint endpoint = std::get<UDP::endpoint>(type);
                if (!status) {
                    if (_clients.contains(endpoint))
                        _clients.erase(endpoint);
                    std::cout << "Game - Client have been disconnected" << std::endl;
                    return;
                }
                analyseMessage(packet, endpoint);
            }  catch (const std::bad_variant_access& ex) {
                std::cerr << "Game - Failed to get the TCP socket" << std::endl;
                return;
            }
        });

        entity_t timerSend = registry.spawnEntity();
        registry.emplaceComponent<TimerSend>(timerSend);

        while (_isRunning) {
            _contextIO.run_for(std::chrono::milliseconds(1));
            registry
                .runSystems()
                .update();
            checkEndGame();
        }
        std::cout << "Game end" << std::endl;
    }

    void Game::stop()
    {
        for (auto &[endpoint, client]: _clients) {
            START_COMPRESSION(client)
                .setID(GAME_END)
                .compactMessage(client.buffer);
            _network->send(_socket, client.buffer, endpoint);
        }
    }

    std::map<UDP::endpoint, struct Network::Game::ClientInfo> &Game::getClients()
    {
        return _clients;
    }

    size_t Game::getPlayersNumber() const
    {
        return _clients.size();
    }

    void Game::checkEndGame()
    {
        if (_clients.size() == 0) {
            if (_gameStart)
                _isRunning = false;
            return;
        }
        for (auto &[endpoint, client] : _clients) {
            if (client.alive)
                return;
        }
        _sceneLoader->loadScene("level_1");
    }

    void Game::swapWeapon()
    {
        switch (_currentScene) {
            case ID_SCENE_LEVEL_1:
                _sceneLoader->changePlayerWeapon("straight", 1, Vec2(0.0f, 25.0f));
                break;
            case ID_SCENE_LEVEL_2:
                _sceneLoader->changePlayerWeapon("bouncing", 2, Vec2(0.0f, 25.0f));
                break;
        }
    }

    void Game::swapScene(char sceneID)
    {
        _currentScene = sceneID;
        for (auto &[endpoint, client] : _clients) {
            START_COMPRESSION(client)
                .setID(SWAP_SCENE)
                .addChar(sceneID)
                .compactMessage(client.buffer);
            _network->send(_socket, client.buffer, endpoint);
        }
        for (auto &[endpoint, client] : _clients) {
            if (!client.alive) {
                client.uuid = _sceneLoader->setupPlayer();
                client.alive = true;
            } else {
                try {
                    size_t entity = UUIDManager::findEntityByUUID(client.uuid);
                    Health &life = _registry.getComponent<Health>(entity);
                    life.reset();
                } catch (const std::exception &e) {}
            }
        }
        swapWeapon();
    }

    void Game::entityDamaged(size_t collidedEntity)
    {
        if (!_registry.hasComponent<UUID>(collidedEntity))
            return;

        UUID tmp = _registry.getComponent<UUID>(collidedEntity);
        for (auto &[endpoint, client] : _clients) {
            START_COMPRESSION(client)
                .setID(ENTITY_DAMAGED)
                .addUUID(tmp)
                .compactMessage(client.buffer);
        }
    }

    void Game::setupGameEngine()
    {
        setupComponents();
        setupSystems();
        _registry.update();
    }

    void Game::setupComponents()
    {
        _registry.registerComponent<Acceleration>();
        _registry.registerComponent<Collision>();
        _registry.registerComponent<Jump>();
        _registry.registerComponent<Gravity>();
        _registry.registerComponent<Transformable>();
        _registry.registerComponent<Velocity>();
        _registry.registerComponent<TimerContainer>();
        _registry.registerComponent<Player>();
        _registry.registerComponent<BrainEnemy>();
        _registry.registerComponent<RollingEnemy>();
        _registry.registerComponent<Bullet>();
        _registry.registerComponent<Health>();
        _registry.registerComponent<Spawner>();
        _registry.registerComponent<Wall>();
        _registry.registerComponent<BottomWall>();
        _registry.registerComponent<EnemySpawner>();
        _registry.registerComponent<Enemy>();
        _registry.registerComponent<UUID>();
        _registry.registerComponent<BossSeahorse>();
        _registry.registerComponent<BrainEnemyBullet>();
        _registry.registerComponent<PlayerBullet>();
    }

    void Game::setupSystems()
    {

        _registry
            // ? time
            .addSystem<TimerContainer>(ClockSystem())

            // ? update
            .addSystem<Player, Velocity, Acceleration, UUID>([this](
                Registry &r,
                Storage<Player> &players,
                Storage<Velocity> &velocities,
                Storage<Acceleration> &accelerations,
                Storage<UUID> &uuids
            ){
                Registry &registry = r;

                for (auto &&[i, player, velocity, acceleration, uuid] : IndexedZipper(players, velocities, accelerations, uuids)) {
                    if (!ACTION_READY_FULL(i, "input action"))
                        continue;

                    auto &[vx, vy, moveSpeed, maxVelocity] = velocity;
                    auto &[ax, ay, ___, decelerationFactor] = acceleration;
                    auto it = std::ranges::find_if(_clients,
                        [this, &uuid](const auto &pair) {
                            return pair.second.uuid == uuid;
                        }
                    );
                    if (it == _clients.end()) {
                        std::cerr << "GAME - Error: don't find the player from his uuid" << std::endl;
                        return;
                    }
                    std::unordered_map<int, bool> &keyStates = it->second.keyStates;

                    if (keyStates[ID_KEY_UP]) {
                        ay = -0.1f;
                    } else if (keyStates[ID_KEY_DOWN]) {
                        ay = 0.1f;
                    } else {
                        ay *= decelerationFactor;
                        if (std::abs(ay) < 0.01f)
                            ay = 0.0f;
                        vy *= decelerationFactor;
                    }

                    if (keyStates[ID_KEY_LEFT]) {
                        ax = -0.1f;
                    } else if (keyStates[ID_KEY_RIGHT]) {
                        ax = 0.1f;
                    } else {
                        ax *= decelerationFactor;
                        if (std::abs(ax) < 0.01f)
                            ax = 0.0f;
                        vx *= decelerationFactor;
                    }

                    vx += ax * moveSpeed;
                    vy += ay * moveSpeed;
                }
            })
            .addSystem<Gravity, Transformable, Acceleration>(GravitySystem())
            .addSystem<Transformable, Velocity>(TransformSystem())
            .addSystem<Transformable, Collision, Velocity, Acceleration, Gravity>(CollisionSystem())
            .addSystem<Health, TimerContainer>(HealthSystem())
            .addSystem<Transformable>([this](Registry &r, Storage<Transformable> &transformations){
                for (auto &&[i, trans] : IndexedZipper(transformations)) {
                    auto &[x, y, __, ___, ____, _____] = trans;
                    if (x < -150.0f || x >= 2500.0f || y >= 1200.0f || y <= -300.0f)
                        r.killEntity(i);
                }
            })
            .addSystem<Spawner, TimerContainer, Transformable>(SpawnerSystem())
            .addSystem<Script, Transformable>(ScriptSystem())
            .addSystem<TimerContainer, TimerSend>([this](Registry &r, Storage<TimerContainer> &timerContainers, Storage<TimerSend> &timerSendContainers) {
                for (auto &&[timers, _] : Zipper(timerContainers, timerSendContainers)) {
                    if (ACTION_READY("TICK_RATE")) {
                        handleSnapshot();
                    }
                }
            });
    }

    void Game::analyseMessage(const Packet &packet, UDP::endpoint tmpEndpoint)
    {
        std::map<std::pair<unsigned char, unsigned short>, std::function<void(const std::vector<char>&)>> handler = {
            {{CONNECTION_GAME, NO_ARGS}, [this, tmpEndpoint](const std::vector<char>&) {
                std::clog << "Client have been connected" << std::endl;
                if (_clients.size() == 4) {
                    return;
                }
                if (!_clients.contains(tmpEndpoint)) {
                    UUID uuid = _sceneLoader->setupPlayer();
                    _gameStart = true;
                    _clients[tmpEndpoint] = ClientInfo {
                        tmpEndpoint,
                        uuid,
                        {
                            {ID_KEY_UP, false},
                            {ID_KEY_DOWN, false},
                            {ID_KEY_LEFT, false},
                            {ID_KEY_RIGHT, false}
                        },
                        {},
                        true,
                        {}
                    };
                    struct ClientInfo &client = _clients[tmpEndpoint];
                    START_COMPRESSION(client)
                        .setID(SWAP_SCENE)
                        .addChar(_currentScene)
                        .compactMessage(client.buffer);
                    _network->send(_socket, client.buffer, tmpEndpoint);
                }
            }},
            {{INPUT_PLAYER, NUM_CH}, [this, tmpEndpoint](const std::vector<char> &binary) {
                if (!_clients.contains(tmpEndpoint)) {
                    std::cerr << "GAME - Client is not connected to this game" << std::endl;
                    return;
                }
                ClientInfo &client = _clients[tmpEndpoint];
                size_t offset = 0;
                int key = BitConverter::getNumber(binary, offset);
                char state = BitConverter::getChar(binary, offset);

                if (client.keyStates.contains(key)) {
                    client.keyStates[key] = state - 1;
                    client.keyStates[key] = !client.keyStates[key];
                } else if (state == 0) {
                    if (key == ID_KEY_ATTACK) {
                        Registry &r = Registry::getInstance();

                        try {
                            size_t entity = UUIDManager::findEntityByUUID(client.uuid);

                            if (r.hasComponent<Player>(entity)) {
                                Player &player = r.getComponent<Player>(entity);
                                player.shooting = true;
                            }
                        } catch (const std::exception &e) {
                        }
                    }
                }
            }},
            {{DISCONNECTED, NO_ARGS}, [this, tmpEndpoint](const std::vector<char> &) {
                if (!_clients.contains(tmpEndpoint))
                    return;
                ClientInfo &disconnected = _clients[tmpEndpoint];
                try {
                    _registry.killEntity(UUIDManager::findEntityByUUID(disconnected.uuid));
                } catch (const std::exception &e) {}
                _clients.erase(tmpEndpoint);
                if (_clients.empty())
                    _isRunning = false;
            }},
            {{CONFIRM_SNAPSHOT, ID}, [this, tmpEndpoint](const std::vector<char> &binary) {
                if (!_clients.contains(tmpEndpoint)) {
                    std::cerr << "GAME - Client is not connected to this game" << std::endl;
                    return;
                }
                ClientInfo &client = _clients[tmpEndpoint];
                size_t offset = 0;
                UUID snapshotID = BitConverter::getUUID(binary, offset);

                for (auto &snapshot: client.snapshots)
                    if (snapshot.uuid == snapshotID)
                        snapshot.checked = true;
            }}
        };

        unsigned char actionID = packet.instruction->actionID;
        unsigned short argsTypes = packet.instruction->argsTypes;
        if (handler.find({actionID, argsTypes}) != handler.end()) {
            handler.at({actionID, argsTypes})(packet.buffer);
        } else {
            std::cerr << "GAME - Wrong instruction with this ID [" << static_cast<int>(actionID) << "] and this types [" << argsTypes << "]" << std::endl;
        }
    }

    char Game::getEntityType(size_t entity)
    {
        static std::vector<std::pair<bool(*)(Registry &, size_t), char>> types = {
            { [](Registry &r, size_t e) { return r.hasComponent<Player>(e); }, ID_OTHER_PLAYER },
            { [](Registry &r, size_t e) { return r.hasComponent<PlayerBullet>(e); }, ID_PLAYER_BULLET },
            { [](Registry &r, size_t e) { return r.hasComponent<BrainEnemy>(e); }, ID_BRAIN_MOB },
            { [](Registry &r, size_t e) { return r.hasComponent<BrainEnemyBullet>(e); }, ID_BRAIN_MOB_BULLET },
            { [](Registry &r, size_t e) { return r.hasComponent<RollingEnemy>(e); }, ID_ROLLING_MOB },
            { [](Registry &r, size_t e) { return r.hasComponent<BossSeahorse>(e); }, ID_BRAIN_SEAHORSE_BOSS },
        };

        for (const auto &[hasComponentFunc, type] : types)
            if (hasComponentFunc(_registry, entity))
                return type;

        return -1;
    }

    Game::Snapshot Game::getNewSnapshot(ClientInfo &client)
    {
        auto &uuids = _registry.getComponents<UUID>();
        auto &transformables = _registry.getComponents<Transformable>();
        Snapshot newSnapshot = {};

        for (auto &&[e, uuid, transformable] : IndexedZipper(uuids, transformables)) {
            auto &[x, y, __, ___, ____, _____] = transformable;
            char id = (uuid == client.uuid) ? ID_PLAYER : getEntityType(e);
            if (id == -1)
                continue;
            newSnapshot[uuid] = {id, {x, y}};
        }
        return newSnapshot;
    }

    int Game::compareSnapshot(ClientInfo &client, Snapshot &latest, Snapshot &newSnapshot)
    {
        int actions = 0;
        for (auto &[uuid, entity]: latest) {
            if (!newSnapshot.contains(uuid)) {
                START_COMPRESSION(client)
                    .setID(KILL_ENTITY)
                    .addUUID(uuid)
                    .compactMessage(client.buffer);
                actions++;
            } else {
                BitConverter::Vec2 &newPos = newSnapshot[uuid].position;
                BitConverter::Vec2 &oldPos = latest[uuid].position;
                if (newPos.x != oldPos.x || newPos.y != oldPos.y) {
                    START_COMPRESSION(client)
                        .setID(POSITION_ENTITY)
                        .addUUID(uuid)
                        .addVec2({newPos.x, newPos.y})
                        .compactMessage(client.buffer);
                    actions++;
                }

            }
        }
        for (auto &[uuid, entity]: newSnapshot) {
            if (!latest.contains(uuid)) {
                START_COMPRESSION(client)
                    .setID(SPAWN_ENTITY)
                    .addUUID(uuid)
                    .addChar(entity.type)
                    .addVec2(entity.position)
                    .compactMessage(client.buffer);
                actions++;
            }
        }
        return actions;
    }

    Game::Snapshot &Game::getLatestCheckSnapshot(ClientInfo &client)
    {
        static thread_local Snapshot empty = {};

        if (client.snapshots.empty())
            return empty;
        for (auto &elem: client.snapshots) {
            if (elem.checked)
                return elem.snapshot;
        }
        return empty;
    }

    void Game::handleSnapshot()
    {
        for (auto &[endpoint, client] : _clients) {
            UUID snapshotUUID = UUIDManager::generateUUID();
            START_COMPRESSION(client)
                .setID(START_SNAPSHOT)
                .compactMessage(client.buffer);
            Snapshot newSnapshot = getNewSnapshot(client);
            Snapshot &latest = getLatestCheckSnapshot(client);
            int actions = compareSnapshot(client, latest, newSnapshot);
            START_COMPRESSION(client)
                .setID(END_SNAPSHOT)
                .addUUID(snapshotUUID)
                .addNumber(actions)
                .compactMessage(client.buffer);
            client.snapshots.push_front(SnapshotState {
                snapshotUUID,
                newSnapshot,
                true // TODO: set to true if the snapshot checked doesn't work properly
            });
            _network->send(_socket, client.buffer, endpoint);
            if (client.snapshots.size() == 17)
                client.snapshots.pop_back();
        }
    }
}
