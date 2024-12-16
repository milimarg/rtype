/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** main
*/

#if defined(_WIN32)
    #define NOGDI
    #define NOUSER
#endif

#include "NetServer.hpp"

int main(int argc, char **argv)
{
    try {
        if (argc != 2)
            throw std::runtime_error("Not enough arguments");
        int port = static_cast<unsigned int>(std::stoul(argv[1]));

        Network::NetServer &channel = Network::NetServer::getInstance(port);
        std::cout << "Starting loop server!" << std::endl;
        while (!Network::NetServer::_shouldStop)
            channel.run();
        std::cout << "Server have been stopped" << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
