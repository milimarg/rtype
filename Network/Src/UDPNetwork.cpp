/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** UDPNetwork
*/

#include "UDPNetwork.hpp"

namespace Network {
    UDPNetwork::UDPNetwork(ASIO::io_context &context) :
        _timeoutHandler(context)
    {
    }

    UDPNetwork::~UDPNetwork()
    {}

    void UDPNetwork::setTimeout(socketPtr var)
    {
        try {
            std::shared_ptr<UDP::socket> socket = std::get<std::shared_ptr<UDP::socket>>(var);
            _timeoutHandler.expires_after(std::chrono::seconds(5));
            _timeoutHandler.async_wait(
                [socket](const SYSTEM::error_code &ec) {
                    if (!ec) {
                        std::cerr << "UDPNetwork - Timeout: Send operation took too long." << std::endl;
                        socket->cancel();
                    }
                }
            );
        } catch (const std::bad_variant_access& ex) {
            std::cerr << "UDPNetwork - Failed to get the UDP socket" << std::endl;
            return;
        }
    }

    void UDPNetwork::send(socketPtr var, std::vector<char> &buffer, UDP::endpoint endpoint)
    {
        try {
            std::shared_ptr<UDP::socket> socket = std::get<std::shared_ptr<UDP::socket>>(var);
            try {
                if (buffer.size() == 0)
                    return;
                setTimeout(socket);

                std::vector<char> zipBuffer = BitConverter::zip(buffer);
                buffer = {};

                socket->async_send_to(ASIO::buffer(zipBuffer, zipBuffer.size()), endpoint,
                    [this, endpoint](boost::system::error_code ec, std::size_t) {
                        _timeoutHandler.cancel();
                        if (ec) {
                            std::cerr << "UDPNetwork - Send error: " << ec.message() << std::endl;
                        }
                    }
                );
            } catch (const std::runtime_error &e) {
                std::cerr << "UDPNetwork - Error when zip: " << e.what() << std::endl;
            }
        }  catch (const std::bad_variant_access& ex) {
            std::cerr << "UDPNetwork - Failed to get the UDP socket" << std::endl;
            return;
        }
    }

    void UDPNetwork::receiveMessage(socketPtr var, receiveHandler onReceive)
    {
        try {
            std::shared_ptr<UDP::socket> socket = std::get<std::shared_ptr<UDP::socket>>(var);
            auto buffer = std::make_shared<std::vector<char>>(MAX_SHORT);

            socket->async_receive_from(ASIO::buffer(buffer->data(), buffer->size()), _tmpEndpoint,
                [this, buffer, socket, onReceive](boost::system::error_code ec, std::size_t bytes) {
                    if(!ec) {
                        buffer->resize(bytes);
                        readBinary(socket, buffer, _tmpEndpoint, onReceive);
                    } else {
                        if (ec.value() == 2) {
                            std::cout << "UDPNetwork - Client disconnect." << std::endl;
                            onReceive({}, _tmpEndpoint, false);
                            return;
                        }
                        std::cerr << "UDPNetwork - Receive error: " << ec.message() << " " << ec.value() << std::endl;
                        receiveMessage(socket, onReceive);
                    }
                }
            );
        }  catch (const std::bad_variant_access& ex) {
            std::cerr << "UDPNetwork - Failed to get the UDP socket" << std::endl;
            return;
        }
    }

    void UDPNetwork::readBinary(socketPtr var, std::shared_ptr<std::vector<char>> buffer, UDP::endpoint endpoint, receiveHandler onReceive)
    {
        try {
            std::shared_ptr<UDP::socket> socket = std::get<std::shared_ptr<UDP::socket>>(var);

            try {

                std::vector<char> unzipBuffer = BitConverter::unzip(buffer);
                size_t offset = 0;
                while (offset + sizeof(CommandInstruction) <= unzipBuffer.size()) {
                    auto receivedCommand = std::make_shared<CommandInstruction>();
                    std::memcpy(receivedCommand.get(), unzipBuffer.data() + offset, sizeof(CommandInstruction));
                    offset += sizeof(CommandInstruction);

                    if (offset + receivedCommand->bufferSize > unzipBuffer.size()) {
                        std::cerr << "UDPNetwork - Error when trying to get the buffer" << std::endl;
                        break;
                    }

                    std::vector<char> binary(unzipBuffer.begin() + offset, unzipBuffer.begin() + offset + receivedCommand->bufferSize);
                    offset += receivedCommand->bufferSize;
                    onReceive({receivedCommand, binary}, endpoint, true);
                }
            } catch (std::runtime_error &e) {
                std::cerr << "Error when unzip: " << e.what() << std::endl;
            }
            receiveMessage(socket, onReceive);
        }  catch (const std::bad_variant_access& ex) {
            std::cerr << "UDPNetwork - Failed to get the UDP socket" << std::endl;
        }
    }
}