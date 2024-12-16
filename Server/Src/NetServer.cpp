/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** NetServer
*/

#include "NetServer.hpp"
#include "ServerSceneLoader.hpp"

bool Network::NetServer::_shouldStop = false;

// ! NETSERVER
namespace Network {
    NetServer &NetServer::getInstance(unsigned int port)
    {
        static NetServer instance(port);
        return instance;
    }

    NetServer::NetServer(unsigned int port) :
        _acceptor(_contextIO, TCP::endpoint(TCP::v4(), port)),
        _network(std::make_shared<TCPNetwork>(_contextIO)),
        _bitConverter(BitConverter::getInstance())
    {
        signal(SIGINT, signalHandler);
        acceptConnection();
    }

    NetServer::~NetServer()
    {
        _shouldStop = true;
        for (auto &[port, game]: _games)
            game.ptr->stop();
        _contextIO.run_for(std::chrono::milliseconds(1));
        _contextIO.stop();
    }

    void NetServer::signalHandler(int signum)
    {
        (void)signum;
        _shouldStop = true;
    }

    void NetServer::run()
    {
        _contextIO.run_for(std::chrono::milliseconds(1));

        std::vector<unsigned short> needToRemove;

        for (auto &[port, _games] : _games)
            if (!_games.status)
                needToRemove.push_back(port);
        for (auto &port : needToRemove)
            _games.erase(port);
    }

    void NetServer::acceptConnection()
    {
        std::shared_ptr<TCP::socket> newSocket = std::make_shared<TCP::socket>(_contextIO);

        _acceptor.async_accept(*newSocket,
            [this, newSocket](SYSTEM::error_code ec) {
                if (!ec) {
                    std::cout << "NETSERVER - Client connected." << std::endl;
                    _clients[newSocket] = ClientInfo {
                        {},
                        false,
                        false,
                        0
                    };
                    _network->receiveMessage(newSocket, [this](Packet packet, packetsType type, bool status) {
                        try {
                            std::shared_ptr<TCP::socket> socket = std::get<std::shared_ptr<TCP::socket>>(type);
                            if (!status) {
                                if (_clients.contains(socket))
                                    _clients.erase(socket);
                                std::cout << "NETSERVER - Client have been disconnected" << std::endl;
                                return;
                            }
                            if (!_shouldStop)
                                analyseMessage(packet, socket);
                        }  catch (const std::bad_variant_access& ex) {
                            std::cerr << "NETSERVER - Failed to get the TCP socket" << std::endl;
                            return;
                        }
                    });
                }
                acceptConnection();
            }
        );
    }

    void NetServer::analyseMessage(const Packet &packet, std::shared_ptr<TCP::socket> socket)
    {
        ClientInfo &client = _clients[socket];
        std::map<std::pair<unsigned char, unsigned short>, std::function<void(const std::vector<char>&)>> handler = {
            {{CLIENT_CREATE_GAME, CH}, [this, socket, &client](const std::vector<char> &binary) {
                size_t offset{};
                char settings = BitConverter::getChar(binary, offset);
                Difficulty difficulty = static_cast<Difficulty>(settings & 0b00000011);
                bool friendlyFire = (settings & 0b00000100) != 0;

                std::thread gameThread(
                    [this, socket, &client, difficulty, friendlyFire]() {
                        try {
                            std::shared_ptr<Game> newGame = std::make_shared<Game>(difficulty, friendlyFire);
                            unsigned short port = newGame->getPort();

                            _games[port] = {
                                newGame,
                                true
                            };
                            _bitConverter
                                .setID(SERVER_CLIENT_JOIN)
                                .addNumber(port)
                                .compactMessage(client.buffer);
                            _network->send(socket, client.buffer);
                            newGame->run(newGame);
                            _games[port].status = false;
                        } catch (const std::exception& e) {
                            std::cerr << "NETSERVER - failed to execute thread" << std::endl;
                        }
                    }
                );
                gameThread.detach();
            }},
            {{CLIENT_JOIN_GAME, NUM}, [this, socket, &client](const std::vector<char> &binary) {
                size_t offset{};
                int gamePort = BitConverter::getNumber(binary, offset);
                std::cout << "NETSERVER - player tried to join a game with port " << gamePort << std::endl;

                if (_games.contains(gamePort) && _games[gamePort].ptr->getPlayersNumber() < 4) {
                    _bitConverter
                        .setID(SERVER_CLIENT_JOIN)
                        .addNumber(gamePort)
                        .compactMessage(client.buffer);
                } else {
                    _bitConverter
                        .setID(SERVER_KO)
                        .compactMessage(client.buffer);
                }
                _network->send(socket, client.buffer);
            }},
            {{CLIENT_SEND_PRIVATE_MESSAGE, STR_STR}, [this](const std::vector<char> &binary) {
                size_t offset{};
                std::string username = BitConverter::getString(binary, offset);
                std::string message = BitConverter::getString(binary, offset);
                std::cout << "NETSERVER - player send {" << message << "} to " << username << std::endl;
            }},
            {{CLIENT_SEND_GAME_MESSAGE, NUM_STR}, [this](const std::vector<char> &binary) {
                size_t offset{};
                int gamePort = BitConverter::getNumber(binary, offset);
                std::string message = BitConverter::getString(binary, offset);
                std::cout << "NETSERVER - player send {" << message << "{ in game chat with port " << gamePort << std::endl;
            }},
            {{CLIENT_ADMIN, NUM_STR}, [this, socket, &client](const std::vector<char> &binary) {
                size_t offset{};
                int adminNumber = BitConverter::getNumber(binary, offset);
                std::string password = BitConverter::getString(binary, offset);

                if (adminNumber == 42 && password == "MIAOU") {
                    _bitConverter
                        .setID(SERVER_CLIENT_ADMIN)
                        .compactMessage(client.buffer);
                    client.admin = true;
                } else {
                    _bitConverter
                        .setID(SERVER_KO)
                        .compactMessage(client.buffer);
                }
                _network->send(socket, client.buffer);
            }},
            {{CLIENT_GET_GAMES_LIST, NO_ARGS}, [this, socket, &client](const std::vector<char> &binary) {
                if (_games.empty())
                    return;
                if (!client.admin) {
                    _bitConverter
                        .setID(SERVER_KO)
                        .compactMessage(client.buffer);
                    _network->send(socket, client.buffer);
                    return;
                }
                for (auto &[port, game]: _games) {
                    _bitConverter
                        .setID(SERVER_SEND_GAME_INFO)
                        .addNumber(port)
                        .addNumber(game.ptr->getPlayersNumber())
                        .compactMessage(client.buffer);
                }
                _network->send(socket, client.buffer);
            }},
            {{CLIENT_STOP_GAME, NUM}, [this, socket, &client](const std::vector<char> &binary) {
                size_t offset{};
                int gamePort = BitConverter::getNumber(binary, offset);
                if (!client.admin || !_games.contains(gamePort)) {
                    _bitConverter
                        .setID(SERVER_KO)
                        .compactMessage(client.buffer);
                    _network->send(socket, client.buffer);
                    return;
                }
                _bitConverter
                    .setID(SERVER_KO)
                    .compactMessage(client.buffer);
                _network->send(socket, client.buffer);
                _games[gamePort].ptr->stop();
                _games.erase(gamePort);
            }},
        };

        unsigned char actionID = packet.instruction->actionID;
        unsigned short argsTypes = packet.instruction->argsTypes;
        auto it = handler.find({actionID, argsTypes});
        if (it != handler.end())
            it->second(packet.buffer);
        else
            std::cerr << "NETSERVER - Wrong instruction with this ID [" << static_cast<int>(actionID) << "] and this types [" << argsTypes << "]" << std::endl;
    }
}
