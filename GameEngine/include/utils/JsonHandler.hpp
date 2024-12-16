/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Oct, 2024                                                     *
 * Title           - GameEngine                                                        *
 * Description     -                                                                    *
 *     JsonHandler                                                                      *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░             *
 *         ▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓             *
 *         █  ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#ifndef JSON_HANDLER_HPP
    #define JSON_HANDLER_HPP

    #include <fstream>
    #include <string>
    #include <iostream>
    #include "json.hpp"

using json = nlohmann::json;

class JsonHandler {
    public:
        JsonHandler() = default;
        explicit JsonHandler(const std::string &filePath);

        bool loadFromFile(const std::string &filePath);
        bool saveToFile(const std::string &filePath) const;

        bool contains(const std::vector<std::string> &keys) const;

        template <typename T>
        T get(const std::vector<std::string> &keys, const T &defaultValue) const;

        template <typename T>
        void set(const std::vector<std::string> &keys, const T &value);

        void remove(const std::vector<std::string> &keys);

        void print() const;
        void clear();

    private:
        json jsonData;
        std::string filePath;

        json *getNestedJson(const std::vector<std::string> &keys);
        const json *getNestedJson(const std::vector<std::string> &keys) const;
};

template <typename T>
T JsonHandler::get(const std::vector<std::string> &keys, const T &defaultValue) const
{
    const json *nestedJson = getNestedJson(keys);
    if (nestedJson) {
        try {
            return nestedJson->get<T>();
        } catch (json::exception &e) {
            std::cerr << "Error retrieving key path: " << e.what() << std::endl;
        }
    }
    return defaultValue;
}

template <typename T>
void JsonHandler::set(const std::vector<std::string> &keys, const T &value)
{
    json *nestedJson = getNestedJson(keys);
    if (nestedJson)
        *nestedJson = value;
    else
        std::cerr << "Error: Could not set value for given key path." << std::endl;
}

#endif /* JSON_HANDLER_HPP */
