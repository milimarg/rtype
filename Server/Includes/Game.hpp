/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** Game
*/

#ifndef GAME_HPP_
    #define GAME_HPP_
    #include <queue>
    #include <iostream>
    #include <map>
    #include <algorithm>
    #include "BitConverter.hpp"
    #include "UDPNetwork.hpp"
    #include "Difficulty.hpp"
    #define START_COMPRESSION(client) if (client.buffer.size() > 1500) _network->send(_socket, client.buffer, client.endpoint); _bitConverter

class ServerSceneLoader;

namespace Network
{
    class Game {
        public:
            struct Entity {
                char type;
                BitConverter::Vec2 position;
            };

            using Snapshot = std::unordered_map<UUID, Entity>;

            struct SnapshotState {
                UUID uuid;
                Snapshot snapshot;
                bool checked;
            };

            struct ClientInfo {
                UDP::endpoint endpoint;
                UUID uuid;
                std::unordered_map<int, bool> keyStates;
                std::vector<char> buffer;
                bool alive;
                std::deque<SnapshotState> snapshots;
            };

            Game(Difficulty difficulty, bool friendlyFire);
            ~Game();

            unsigned short getPort() const;
            void run(std::shared_ptr<Game>);
            void stop();

            std::map<UDP::endpoint, ClientInfo> &getClients();
            size_t getPlayersNumber() const;
            void checkEndGame();
            void swapWeapon();
            void swapScene(char sceneID);
            void entityDamaged(size_t collidedEntity);

        private:
            void setupGameEngine();
            void setupComponents();
            void setupSystems();
            void analyseMessage(const Packet &packet, UDP::endpoint tmpEndpoint);

            char getEntityType(size_t entity);
            Snapshot getNewSnapshot(ClientInfo &client);
            int compareSnapshot(ClientInfo &client, Snapshot &latest, Snapshot &newSnapshot);
            Snapshot &getLatestCheckSnapshot(ClientInfo &client);
            void handleSnapshot();

            ASIO::io_context _contextIO;
            std::shared_ptr<UDP::socket> _socket;
            unsigned short _port;
            unsigned char _currentScene;
            std::shared_ptr<INetwork> _network;
            bool _isRunning;
            bool _gameStart;
            BitConverter &_bitConverter;
            Registry &_registry;

            std::map<UDP::endpoint, struct ClientInfo> _clients;

            std::shared_ptr<ServerSceneLoader> _sceneLoader;
            Difficulty _difficulty;
            bool _friendlyFire;

            struct TimerSend {};
    };
}

#endif /* !GAME_HPP_ */
