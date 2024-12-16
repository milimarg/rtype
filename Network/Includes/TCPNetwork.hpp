/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** TCPNetwork
*/

#ifndef TCPNETWORK_HPP_
    #define TCPNETWORK_HPP_

    #include "INetwork.hpp"

namespace Network {
    class TCPNetwork : public INetwork {
        public:

            TCPNetwork(ASIO::io_context &context);
            ~TCPNetwork();

            void setTimeout(socketPtr var) override;
            void send(socketPtr var, std::vector<char> &buffer, UDP::endpoint endpoint = {}) override;
            void receiveMessage(socketPtr var, receiveHandler onReceive) override;
            void readBinary(socketPtr var, std::shared_ptr<std::vector<char>> buffer, UDP::endpoint endpoint, receiveHandler onReceive) override;

        protected:
        private:
            ASIO::steady_timer _timeoutHandler;

    };
}
#endif /* !TCPNETWORK_HPP_ */
