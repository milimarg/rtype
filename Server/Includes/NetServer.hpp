/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** NetServer
*/

#ifndef NETSERVER_HPP_
    #define NETSERVER_HPP_
    #include "Game.hpp"
    #include "../../Network/Includes/TCPNetwork.hpp"

namespace Network {
    class NetServer {
        public:
            struct ClientInfo {
                std::vector<char> buffer;
                bool admin;
                bool gameStart;
                unsigned short gamePort;
            };

            struct GameInfo {
                std::shared_ptr<Game> ptr;
                bool status;
            };


            static NetServer &getInstance(unsigned int port);
            void run();

            static void signalHandler(int signum);
            static bool _shouldStop;
        private:
            NetServer(unsigned int port);
            ~NetServer();

            void acceptConnection();
            void analyseMessage(const Packet &packet, std::shared_ptr<TCP::socket> socket);

            ASIO::io_context _contextIO;
            TCP::acceptor _acceptor;
            std::shared_ptr<INetwork> _network;
            BitConverter &_bitConverter;
            std::unordered_map<std::shared_ptr<TCP::socket>, struct ClientInfo> _clients;
            std::unordered_map<unsigned short, GameInfo> _games;
    };
}

#endif /* !NETSERVER_HPP_ */
