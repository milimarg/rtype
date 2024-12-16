/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** Game
*/

#ifndef GAME_HPP_
    #define GAME_HPP_

    #include <map>
    #include "BitConverter.hpp"
    #include "UDPNetwork.hpp"

class ClientSceneLoader;

namespace Network
{
    class Game {
        public:
            Game(std::string ip, std::string port, std::shared_ptr<ClientSceneLoader> sceneLoader);
            ~Game();

            void run();
            bool isRunning();

            void send();
            void disconnect();
            std::shared_ptr<ClientSceneLoader> getClientSceneLoader();
            std::vector<char> &getBuffer();

        private:
            void analyseMessage(Packet packet);

            ASIO::io_context _contextIO;
            std::shared_ptr<UDP::socket> _socket;
            UDP::endpoint _endpoint;
            std::vector<char> _buffer;
            std::shared_ptr<INetwork> _network;
            bool _isRunning;
            BitConverter &_bitConverter;
            Registry &_registry;

            std::shared_ptr<ClientSceneLoader> _sceneLoader;

            int _actions;
            size_t _timerID;
            struct TimerSend {};
    };
}

#endif /* !GAME_HPP_ */
