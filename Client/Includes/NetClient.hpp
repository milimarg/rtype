/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** NetClient
*/

#ifndef NETCLIENT_HPP_
    #define NETCLIENT_HPP_

    #include "Game.hpp"
    #include "ClientSceneLoader.hpp"
    #include "TCPNetwork.hpp"

namespace Network {
    class NetClient {
        public:
            static NetClient &getInstance();
            void setAdress(std::string ip, std::string port);

            void setClientSceneLoader(std::shared_ptr<ClientSceneLoader> sceneLoader);
            std::vector<char> &getBuffer();

            void run();
            void stop();

            std::unordered_map<int, int> &adminGetGames();

            static bool _shouldStop;
        private:
            NetClient();
            ~NetClient();

            static void signalHandler(int signum);

            void analyseMessage(Packet packet);
            void getGameList();

            // ? Lobby variables
            ASIO::io_context _contextIO;
            std::shared_ptr<TCP::socket> _socket;
            std::string _ip;
            std::string _port;
            std::vector<char> _buffer;
            std::shared_ptr<INetwork> _network;
            BitConverter &_bitConverter;
            Registry &_registry;

            // ? Game variables
            bool _gameStart;
            bool _admin;
            std::shared_ptr<Game> _game;
            std::shared_ptr<ClientSceneLoader> _sceneLoader;
            std::unordered_map<int, int> _gameList;

            // ? Timer
            size_t _timerID;

            struct TimerSend {};
    };
}
#endif /* !NETCLIENT_HPP_ */
