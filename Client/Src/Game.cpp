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
#include "GameEngineIncludesClient.hpp"
#include "../../Network/Includes/BitConverter.hpp"
#include "ClientSceneLoader.hpp"


namespace Network
{
    Game::Game(std::string ip, std::string port, std::shared_ptr<ClientSceneLoader> sceneLoader) :
        _socket(std::make_shared<UDP::socket>(_contextIO, UDP::v4())),
        _buffer({}),
        _sceneLoader(sceneLoader),
        _network(std::make_shared<UDPNetwork>(_contextIO)),
        _isRunning(true),
        _bitConverter(BitConverter::getInstance()),
        _registry(Registry::getInstance())
    {
        UDP::resolver resolver(_contextIO);
        _endpoint = *resolver.resolve(UDP::v4(), ip, port).begin();

        if (!_socket->is_open())
            _socket->open(UDP::v4());

        _socket->non_blocking(true);
        _bitConverter
            .setID(CONNECTION_GAME)
            .compactMessage(_buffer);
        send();
        _network->receiveMessage(_socket, [this](Packet packet, packetsType type, bool status) {
            if (!status) {
                std::cerr << "Failed to receive from Server" << std::endl;
                return;
            }
            analyseMessage(packet);
        });
        _timerID = _registry.spawnEntity();
        _registry.emplaceComponent<TimerSend>(_timerID);
    }

    Game::~Game()
    {
        _registry.killEntity(_timerID);
        _contextIO.stop();
        _socket->close();
    }

    bool Game::isRunning()
    {
        return _isRunning;
    }

    void Game::run()
    {
        _contextIO.run_for(std::chrono::milliseconds(1));

        for (auto &&[timers, _] : Zipper(_registry.getComponents<TimerContainer>(), _registry.getComponents<TimerSend>())) {
            if (ACTION_READY("TICK_RATE"))
                send();
        }
    }

    void Game::send()
    {
        _network->send(_socket, _buffer, _endpoint);
    }

    void Game::disconnect()
    {
        _isRunning = false;
        _bitConverter
            .setID(DISCONNECTED)
            .compactMessage(_buffer);
        send();
    }

    std::shared_ptr<ClientSceneLoader> Game::getClientSceneLoader()
    {
        return _sceneLoader;
    }

    std::vector<char> &Game::getBuffer()
    {
        return _buffer;
    }

    void Game::analyseMessage(Packet packet)
    {
        std::map<std::pair<unsigned char, unsigned short>, std::function<void(const std::vector<char> &)>> handler = {
            {{SPAWN_ENTITY, ID_CH_VE2}, [this](const std::vector<char> &binary) {
                size_t offset{};
                UUID entityID = BitConverter::getUUID(binary, offset);
                char type = BitConverter::getChar(binary, offset);
                auto [x, y] = BitConverter::getVec2(binary, offset);

                try {
                    size_t entity = UUIDManager::findEntityByUUID(entityID);

                    if (_registry.hasComponent<Transformable>(entity)) {
                        auto &[oldX, oldY, _, __, ___, ____] = _registry.getComponent<Transformable>(entity);
                        oldX = x;
                        oldY = y;
                    }
                    return;
                } catch (const std::exception &e) {}

                _actions++;
                switch (type) {
                    case ID_PLAYER:
                        _sceneLoader->setupPlayer(x, y, entityID);
                        break;
                    case ID_OTHER_PLAYER:
                        _sceneLoader->setupOtherPlayer(x, y, entityID);
                        break;
                    case ID_PLAYER_BULLET:
                        _sceneLoader->setupPlayerBullet(x, y, entityID);
                        break;
                    case ID_BRAIN_MOB:
                        _sceneLoader->setupBrainMob(x, y, entityID);
                        break;
                    case ID_BRAIN_MOB_BULLET:
                        _sceneLoader->setupBrainMobBullet(x, y, entityID);
                        break;
                    case ID_ROLLING_MOB:
                        _sceneLoader->setupRollingMob(x, y, entityID);
                        break;
                    case ID_BRAIN_SEAHORSE_BOSS:
                        _sceneLoader->setupBrainSeahorseBoss(x, y, entityID);
                        break;
                    default:
                        // ? log an error?
                        break;
                }
            }},
            {{KILL_ENTITY, ID}, [this](const std::vector<char> &binary) {
                size_t offset{};
                UUID entityID = BitConverter::getUUID(binary, offset);

                try {
                    size_t entity = UUIDManager::findEntityByUUID(entityID);
                    _registry.killEntity(entity);
                    _actions++;
                } catch (const std::exception &e) {
                    // ? log an error?
                }
            }},
            {{POSITION_ENTITY, ID_VE2}, [this](const std::vector<char> &binary) {
                size_t offset{};
                UUID entityID = BitConverter::getUUID(binary, offset);
                BitConverter::Vec2 position = BitConverter::getVec2(binary, offset); // ! Why i need to precise BitConverter for Vec2

                try {
                    size_t entity = UUIDManager::findEntityByUUID(entityID);

                    if (_registry.hasComponent<Transformable>(entity)) {
                        auto &[x, y, _, __, ___, ____] = _registry.getComponent<Transformable>(entity);

                        x = position.x;
                        y = position.y;
                    }
                    _actions++;
                } catch (const std::exception &e) {
                    // ? log an error?
                }
            }},
            {{ENTITY_DAMAGED, ID}, [this](const std::vector<char> &binary) {
                size_t offset{};
                UUID entityID = BitConverter::getUUID(binary, offset);

                try {
                    size_t entity = UUIDManager::findEntityByUUID(entityID);
                    _registry.safeEmplaceComponent<Red>(entity);
                } catch (const std::exception &e) {}
            }},
            {{SWAP_SCENE, CH}, [this](const std::vector<char> &binary) {
                size_t offset{};
                char sceneID = BitConverter::getChar(binary, offset);

                switch (sceneID) {
                    case ID_SCENE_MENU:
                        _sceneLoader->restartFromScene("menu");
                        break;
                    case ID_SCENE_LEVEL_1:
                        _sceneLoader->restartFromScene("level_1");
                        break;
                    case ID_SCENE_LEVEL_2:
                        _sceneLoader->restartFromScene("level_2");
                        break;
                    default:
                        // ? log an error?
                        break;
                }
            }},
            {{GAME_END, NO_ARGS}, [this](const std::vector<char>&) {
                disconnect();
            }},
            {{START_SNAPSHOT, NO_ARGS}, [this](const std::vector<char>&) {
                _actions = 0;
            }},
            {{END_SNAPSHOT, ID_NUM}, [this](const std::vector<char> &binary) {
                size_t offset{};
                UUID snapshotID = BitConverter::getUUID(binary, offset);
                int actions = BitConverter::getNumber(binary, offset);

                if (actions == _actions) {
                    _bitConverter
                        .setID(CONFIRM_SNAPSHOT)
                        .addUUID(snapshotID)
                        .compactMessage(_buffer);
                }
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
}
