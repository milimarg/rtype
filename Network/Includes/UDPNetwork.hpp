/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** UDPNetwork
*/

#ifndef UDPNETWORK_HPP_
    #define UDPNETWORK_HPP_

    #include "INetwork.hpp"

namespace Network {
    class UDPNetwork : public INetwork {
        public:

            UDPNetwork(ASIO::io_context &context);
            ~UDPNetwork();

            void setTimeout(socketPtr var) override;
            void send(socketPtr var, std::vector<char> &buffer, UDP::endpoint endpoint = {}) override;
            void receiveMessage(socketPtr var, receiveHandler onReceive) override;
            void readBinary(socketPtr var, std::shared_ptr<std::vector<char>> buffer, UDP::endpoint endpoint, receiveHandler onReceive) override;

        protected:
        private:
            ASIO::steady_timer _timeoutHandler;
            UDP::endpoint _tmpEndpoint;

    };
}

#endif /* !UDPNETWORK_HPP_ */
