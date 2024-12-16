/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Nov, 2024                                                     *
 * Title           - GameEngine                                                         *
 * Description     -                                                                    *
 *     AssetManager                                                                     *
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

#ifndef ECS_ASSETMANAGER_HPP
    #define ECS_ASSETMANAGER_HPP

    #include "RaylibWrapper.hpp"
    #include <string>
    #include <unordered_map>
    #include <memory>
    #include <variant>
    #include <filesystem>

namespace ecs
{
    struct Texture3D
    {
        TextureCubemap texture;
    };

    class AssetManager {
        public:
            AssetManager(const AssetManager &) = delete;
            AssetManager &operator=(const AssetManager &) = delete;
            static AssetManager &getInstance();

            template <typename T>
            std::shared_ptr<T> loadAsset(const std::string &filename, const std::string &altFilename = "")
            {
                std::shared_ptr<T> asset = tryLoadAsset<T>(filename);

                if (!asset && !altFilename.empty())
                    asset = tryLoadAsset<T>(altFilename);

                return asset;
            }

            template <typename T>
            std::shared_ptr<T> tryLoadAsset(const std::string &filename)
            {
                if (!std::filesystem::exists(filename))
                    return nullptr;

                std::shared_ptr<T> cache = getFromCache<T>(filename);
                if (cache)
                    return cache;

                if constexpr (std::is_same_v<T, Texture2D>) {
                    Image image = LoadImage(filename.c_str());
                    std::shared_ptr<T> texture = std::make_shared<T>(LoadTextureFromImage(image));
                    _assetCache[filename] = texture;
                    UnloadImage(image);
                    return texture;
                }

                if constexpr (std::is_same_v<T, Texture3D>) {
                    Image image = LoadImage(filename.c_str());
                    TextureCubemap texture = LoadTextureCubemap(image, CUBEMAP_LAYOUT_LINE_VERTICAL);
                    std::shared_ptr<T> texture3d = std::make_shared<Texture3D>(texture);
                    _assetCache[filename] = texture3d;
                    UnloadImage(image);
                    return texture3d;
                }

                if constexpr (std::is_same_v<T, Music>) {
                    std::shared_ptr<T> music = std::make_shared<T>(LoadMusicStream(filename.c_str()));
                    _assetCache[filename] = music;
                    return music;
                }

                if constexpr (std::is_same_v<T, Sound>) {
                    std::shared_ptr<T> sound = std::make_shared<T>(LoadSound(filename.c_str()));
                    _assetCache[filename] = sound;
                    return sound;
                }

                return nullptr;
            }

            bool removeAsset(const std::string &filename);

        private:
            AssetManager() = default;
            ~AssetManager() = default;

            template <typename T>
            std::shared_ptr<T> getFromCache(const std::string &filename)
            {
                auto it = _assetCache.find(filename);
                if (it != _assetCache.end())
                    if (std::holds_alternative<std::shared_ptr<T>>(it->second))
                        return std::get<std::shared_ptr<T>>(it->second);
                return nullptr;
            }

            std::unordered_map<std::string,
            std::variant<
                std::shared_ptr<Texture2D>,
                std::shared_ptr<Texture3D>,
                std::shared_ptr<Music>,
                std::shared_ptr<Sound>
            >> _assetCache;
    };
};

#endif /* ECS_ASSETMANAGER_HPP */
