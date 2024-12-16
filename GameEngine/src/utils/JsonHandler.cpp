/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Oct, 2024                                                     *
 * Title           - GameEngine                                                        *
 * Description     -                                                                    *
 *     JsonHandler                                                                      *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                     *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                     *
 *             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                     *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "JsonHandler.hpp"

JsonHandler::JsonHandler(const std::string &filePath)
{
    loadFromFile(filePath);
}

bool JsonHandler::loadFromFile(const std::string &filePath)
{
    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return false;
    }
    try {
        inputFile >> jsonData;
        this->filePath = filePath;
    } catch (json::exception &e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return false;
    }
    return true;
}

bool JsonHandler::saveToFile(const std::string &filePath) const
{
    std::ofstream outputFile(filePath);
    if (!outputFile) {
        std::cerr << "Error: Could not write to file " << filePath << std::endl;
        return false;
    }
    try {
        outputFile << jsonData.dump(4);
    } catch (json::exception &e) {
        std::cerr << "JSON writing error: " << e.what() << std::endl;
        return false;
    }
    return true;
}

bool JsonHandler::contains(const std::vector<std::string> &keys) const
{
    const json *nestedJson = getNestedJson(keys);
    return nestedJson != nullptr;
}

void JsonHandler::remove(const std::vector<std::string> &keys)
{
    if (keys.empty()) {
        std::cerr << "Error: No keys provided for removal." << std::endl;
        return;
    }

    json *nestedJson = getNestedJson(std::vector<std::string>(keys.begin(), keys.end() - 1));
    if (nestedJson && nestedJson->contains(keys.back())) {
        nestedJson->erase(keys.back());
    } else {
        std::cerr << "Warning: Key path not found for removal." << std::endl;
    }
}

void JsonHandler::print() const
{
    std::cout << jsonData.dump(4) << std::endl;
}

void JsonHandler::clear()
{
    jsonData.clear();
}

json *JsonHandler::getNestedJson(const std::vector<std::string> &keys)
{
    if (keys.empty())
        return nullptr;

    json *current = &jsonData;
    for (const auto &key : keys) {
        if (current->contains(key))
            current = &(*current)[key];
        else
            return nullptr;
    }
    return current;
}

const json *JsonHandler::getNestedJson(const std::vector<std::string> &keys) const
{
    if (keys.empty())
        return nullptr;

    const json *current = &jsonData;
    for (const auto &key : keys) {
        if (current->contains(key))
            current = &current->at(key);
        else
            return nullptr;
    }
    return current;
}
