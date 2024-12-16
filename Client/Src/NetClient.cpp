/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** NetClient
*/

#if defined(_WIN32)
    #define NOGDI
    #define NOUSER
#endif
#include "NetClient.hpp"
#include "TimerContainer.hpp"
#include "Using.hpp"
#include "utils.hpp"
#include "Zipper.hpp"

USING(
    ecs::Zipper,
    ecs::cmp::TimerContainer
);

bool Network::NetClient::_shouldStop = false;

// ! NETCLIENT
namespace Network
{
    NetClient &NetClient::getInstance()
    {
        static NetClient instance;
        return instance;
    }

    NetClient::NetClient() :
        _socket(std::make_shared<TCP::socket>(_contextIO, TCP::v4())),
        _buffer({}),
        _network(std::make_shared<TCPNetwork>(_contextIO)),
        _bitConverter(BitConverter::getInstance()),
        _registry(Registry::getInstance()),
        _gameStart(false),
        _admin(false)
    {
        if (!_socket->is_open())
            _socket->open(TCP::v4());
        _socket->non_blocking(true);
        signal(SIGINT, signalHandler);
    }

    NetClient::~NetClient()
    {
        _contextIO.stop();
        _socket->close();
    }

    void NetClient::setAdress(std::string ip, std::string port)
    {
        _ip = ip;
        _port = port;
        if (_ip == "LOCALHOST" || _ip == "localhost")
            _ip = "0.0.0.0";
        TCP::resolver resolver(_contextIO);
        TCP::resolver::results_type endpoints = resolver.resolve(_ip, _port);
        ASIO::connect(*_socket, endpoints);

        _network->receiveMessage(_socket, [this](Packet packet, packetsType type, bool status) {
            if (!status) {
                std::cerr << "Failed to receive from Server" << std::endl;
                return;
            }
            analyseMessage(packet);
        });
    }

    void NetClient::setClientSceneLoader(std::shared_ptr<ClientSceneLoader> sceneLoader)
    {
        _sceneLoader = sceneLoader;
    }

    std::vector<char> &NetClient::getBuffer()
    {
        return _buffer;
    }

    void NetClient::stop()
    {
        if (_game) {
            _game->disconnect();
            _game = nullptr;
            _sceneLoader->setGame(nullptr);
        }
    }

    void NetClient::signalHandler(int signum)
    {
        (void)signum;
        _shouldStop = true;
    }

    void NetClient::run()
    {
        if (_game) {
            if (!_game->isRunning()) {
                _game = nullptr;
                _sceneLoader->setGame(nullptr);
                _sceneLoader->restartFromScene("menu");
                return;
            }
            _game->run();
        }
        else {
            _contextIO.run_for(std::chrono::milliseconds(1));
            for (auto &&[timers, _] : Zipper(_registry.getComponents<TimerContainer>(), _registry.getComponents<TimerSend>())) {
                if (ACTION_READY("TICK_RATE"))
                    getGameList();
            }
        }
        if (!_buffer.empty())
            _network->send(_socket, _buffer);
    }

    void NetClient::getGameList()
    {
        if (!_admin)
            return;
        // if (!_gameList.empty()) // ! How to prevent keeping delete element ??
        //     _gameList = {};
        _bitConverter
            .setID(CLIENT_GET_GAMES_LIST)
            .compactMessage(_buffer);
    }

    std::unordered_map<int, int> &NetClient::adminGetGames()
    {
        return _gameList;
    }

    void NetClient::analyseMessage(Packet packet)
    {
        std::map<std::pair<unsigned char, unsigned short>, std::function<void(const std::vector<char> &)>> handler = {
            {{SERVER_KO, NO_ARGS}, [this](const std::vector<char>&) {
                std::cout << "Server failed" << std::endl;
            }},
            {{SERVER_SUCCESS, NO_ARGS}, [this](const std::vector<char>&) {
                std::cout << "Server Succeed" << std::endl;
            }},
            {{SERVER_CLIENT_JOIN, NUM}, [this](const std::vector<char>&binary) {
                try {
                    if (!_sceneLoader) {
                        std::cerr << "SceneLoader have not been set" << std::endl;
                        return;
                    }
                    size_t offset{};
                    int port = BitConverter::getNumber(binary, offset);
                    std::cout << "Game is created at this port: " << port << std::endl;
                    _game = std::make_shared<Game>(_ip, std::to_string(port), _sceneLoader);
                    _sceneLoader->setGame(_game);
                    _gameStart = true;
                } catch (const std::invalid_argument &e) {
                    std::cerr << "NETCLIENT - " << e.what() << std::endl;
                }
            }},
            {{SERVER_GET_PRIVATE_MESSAGE, STR_STR}, [this](const std::vector<char>&binary) {
                size_t offset{};
                std::string username = BitConverter::getString(binary, offset);
                std::string message = BitConverter::getString(binary, offset);
                std::cout << "I received {" << message << "} from " << username << std::endl;
            }},
            {{SERVER_GET_GAME_MESSAGE, NUM_STR}, [this](const std::vector<char>&binary) {
                size_t offset{};
                int gamePort = BitConverter::getNumber(binary, offset);
                std::string message = BitConverter::getString(binary, offset);
                std::cout << "I received {" << message << "} from game message at this port" << gamePort << std::endl;
            }},
            {{SERVER_SEND_GAME_INFO, NUM_NUM}, [this](const std::vector<char>&binary) {
                size_t offset{};
                int gamePort = BitConverter::getNumber(binary, offset);
                int players = BitConverter::getNumber(binary, offset);
                _gameList[gamePort] = players;
            }},
            {{SERVER_CLIENT_ADMIN, NO_ARGS}, [this](const std::vector<char>&) {
                if (_admin)
                    return;
                std::cout << "Successfully set admin" << std::endl;
                _sceneLoader->setAdmin(true);
                _admin = true;

                _timerID = _registry.spawnEntity();
                _registry.emplaceComponent<TimerSend>(_timerID);
            }}
        };

        unsigned char actionID = packet.instruction->actionID;
        unsigned short argsTypes = packet.instruction->argsTypes;
        if (handler.find({actionID, argsTypes}) != handler.end()) {
            handler.at({actionID, argsTypes})(packet.buffer);
        } else {
            std::cerr << "NETCLIENT - Wrong instruction with this ID [" << static_cast<int>(actionID) << "] and this types [" << argsTypes << "]" << std::endl;
        }
    }
}
