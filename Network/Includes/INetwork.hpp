/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** INetwork
*/

#ifndef INETWORK_HPP_
    #define INETWORK_HPP_

    #include "BitConverter.hpp"

struct Packet {
    std::shared_ptr<Network::CommandInstruction> instruction;
    std::vector<char> buffer;
};

using socketPtr = std::variant<std::shared_ptr<TCP::socket>, std::shared_ptr<UDP::socket>>;
using packetsType = std::variant<std::shared_ptr<TCP::socket>, UDP::endpoint>;
using receiveHandler = std::function<void(Packet, packetsType, bool)>;

namespace Network {
    class INetwork {
        public:

            virtual ~INetwork() = default;

            virtual void setTimeout(socketPtr var) = 0;
            virtual void send(socketPtr var, std::vector<char> &buffer, UDP::endpoint endpoint = {}) = 0;
            virtual void receiveMessage(socketPtr var, receiveHandler onReceive) = 0;
            virtual void readBinary(socketPtr var, std::shared_ptr<std::vector<char>> buffer, UDP::endpoint endpoint, receiveHandler onReceive) = 0;
        protected:
        private:
    };
}

#endif /* !INETWORK_HPP_ */
