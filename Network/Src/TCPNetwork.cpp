/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** TCPNetwork
*/

#include "TCPNetwork.hpp"

namespace Network {
    TCPNetwork::TCPNetwork(ASIO::io_context &context) :
        _timeoutHandler(context)
    {
    }

    TCPNetwork::~TCPNetwork()
    {}

    void TCPNetwork::setTimeout(socketPtr var)
    {
        try {
            std::shared_ptr<TCP::socket> socket = std::get<std::shared_ptr<TCP::socket>>(var);
            _timeoutHandler.expires_after(std::chrono::seconds(5));
            _timeoutHandler.async_wait(
            [socket](const SYSTEM::error_code &ec) {
                    if (!ec) {
                        std::cerr << "TCPNetwork - Timeout: Send operation took too long." << std::endl;
                        socket->cancel();
                    }
                }
            );
        } catch (const std::bad_variant_access& ex) {
            std::cerr << "TCPNetwork - Failed to get the TCP socket" << std::endl;
            return;
        }
    }

    void TCPNetwork::send(socketPtr var, std::vector<char> &buffer, UDP::endpoint endpoint)
    {
        try {
            std::shared_ptr<TCP::socket> socket = std::get<std::shared_ptr<TCP::socket>>(var);
            try {
                if (buffer.size() == 0)
                    return;
                setTimeout(socket);

                std::vector<char> binary = BitConverter::zip(buffer);
                buffer = {};

                socket->async_write_some(ASIO::buffer(binary, binary.size()),
                    [this](boost::system::error_code ec, std::size_t bytes) {
                        _timeoutHandler.cancel();
                        if (ec) {
                            std::cerr << "TCPNetwork - Send error: " << ec.message() << std::endl;
                        }
                    }
                );
            } catch (const std::runtime_error &e) {
                std::cerr << "TCPNetwork - Error when zip: " << e.what() << std::endl;
            }
        }  catch (const std::bad_variant_access& ex) {
            std::cerr << "TCPNetwork - Failed to get the TCP socket" << std::endl;
            return;
        }
    }

    void TCPNetwork::receiveMessage(socketPtr var, receiveHandler onReceive)
    {
        try {
            std::shared_ptr<TCP::socket> socket = std::get<std::shared_ptr<TCP::socket>>(var);
            auto buffer = std::make_shared<std::vector<char>>(MAX_SHORT);

            socket->async_read_some(ASIO::buffer(buffer->data(), buffer->size()),
                [this, buffer, socket, onReceive](boost::system::error_code ec, std::size_t bytes) {
                    if(!ec) {
                        buffer->resize(bytes);
                        readBinary(socket, buffer, {}, onReceive);
                    } else {
                        if (ec.value() == 2) {
                            std::cout << "TCPNetwork - Client disconnect." << std::endl;
                            onReceive({}, socket, false);
                            return;
                        }
                        std::cerr << "TCPNetwork - Receive error: " << ec.message() << " " << ec.value() << std::endl;
                        receiveMessage(socket, onReceive);
                    }
                }
            );
        }  catch (const std::bad_variant_access& ex) {
            std::cerr << "TCPNetwork - Failed to get the TCP socket" << std::endl;
            return;
        }
    }

    void TCPNetwork::readBinary(socketPtr var, std::shared_ptr<std::vector<char>> buffer, UDP::endpoint endpoint, receiveHandler onReceive)
    {
        try {
            std::shared_ptr<TCP::socket> socket = std::get<std::shared_ptr<TCP::socket>>(var);

            try {

                std::vector<char> unzipBuffer = BitConverter::unzip(buffer);

                size_t offset = 0;
                while (offset + sizeof(CommandInstruction) <= unzipBuffer.size()) {
                    auto receivedCommand = std::make_shared<CommandInstruction>();
                    std::memcpy(receivedCommand.get(), unzipBuffer.data() + offset, sizeof(CommandInstruction));
                    offset += sizeof(CommandInstruction);

                    if (offset + receivedCommand->bufferSize > unzipBuffer.size()) {
                        std::cerr << "TCPNetwork - Error when trying to get the buffer" << std::endl;
                        break;
                    }

                    std::vector<char> binary(unzipBuffer.begin() + offset, unzipBuffer.begin() + offset + receivedCommand->bufferSize);
                    offset += receivedCommand->bufferSize;
                    onReceive({receivedCommand, binary}, socket, true);
                }
            } catch (std::runtime_error &e) {
                std::cerr << "Error when unzip: " << e.what() << std::endl;
            }
            receiveMessage(socket, onReceive);
        }  catch (const std::bad_variant_access& ex) {
            std::cerr << "TCPNetwork - Failed to get the TCP socket" << std::endl;
            return;
        }
    }
}