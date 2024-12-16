/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** Music.hpp
*/

#ifndef MUSIC_SOURCE_HPP
    #define MUSIC_SOURCE_HPP

    #include <string>
    #include "RaylibWrapper.hpp"
    #include "AssetManager.hpp"

namespace ecs
{
    namespace cmp
    {
        struct MusicSource
        {
            std::unordered_map<std::string, std::string> musics;
            float volumeSource = 1.0f;
            float pitchSource = 0.0f;
            float panSource = 0.0f;
            float position = 0.0f;

            MusicSource() {}

            MusicSource(const std::string &id, const std::string &file)
            {
                addMusic(id, file);
                play(id);
            }

            void addMusic(const std::string &id, const std::string &file)
            {
                AssetManager::getInstance().loadAsset<Music>(file);
                musics[id] = file;
            }

            bool removeSound(const std::string &id)
            {
                auto it = musics.find(id);

                if (musics.find(id) == musics.end())
                    return false;
                musics.erase(it);
                return true;
            }

            bool play(const std::string &id)
            {
                auto it = musics.find(id);

                if (it == musics.end())
                    return false;
                G.playMusicStream(*AssetManager::getInstance().loadAsset<Music>(it->second));
                return true;
            }

            bool stop(const std::string &id)
            {
                auto it = musics.find(id);

                if (it == musics.end())
                    return false;
                G.stopMusicStream(*AssetManager::getInstance().loadAsset<Music>(it->second));
                return true;
            }

            bool pause(const std::string &id)
            {
                auto it = musics.find(id);

                if (it == musics.end())
                    return false;
                G.pauseMusicStream(*AssetManager::getInstance().loadAsset<Music>(it->second));
                return true;
            }

            bool resume(const std::string &id)
            {
                auto it = musics.find(id);

                if (it == musics.end())
                    return false;
                G.resumeMusicStream(*AssetManager::getInstance().loadAsset<Music>(it->second));
                return true;
            }

            bool setVolume(const std::string &id, const float volume)
            {
                auto it = musics.find(id);

                if (it == musics.end())
                    return false;
                G.setMusicVolume(*AssetManager::getInstance().loadAsset<Music>(it->second), volume);
                volumeSource = volume;
                return true;
            }

            bool setPitch(const std::string &id, const float pitch)
            {
                auto it = musics.find(id);

                if (it == musics.end())
                    return false;
                G.setMusicPitch(*AssetManager::getInstance().loadAsset<Music>(it->second), pitch);
                pitchSource = pitch;
                return true;
            }

            bool setPan(const std::string &id, const float pan)
            {
                auto it = musics.find(id);

                if (it == musics.end())
                    return false;
                G.setMusicPan(*AssetManager::getInstance().loadAsset<Music>(it->second), pan);
                panSource = pan;
                return true;
            }

            bool update(const std::string &id)
            {
                auto it = musics.find(id);

                if (it == musics.end())
                    return false;
                G.updateMusicStream(*AssetManager::getInstance().loadAsset<Music>(it->second));
                return true;
            }

            void updateAll()
            {
                AssetManager &a = AssetManager::getInstance();

                for (auto &[_, musicPath] : musics)
                    G.updateMusicStream(*a.loadAsset<Music>(musicPath));
            }

            bool seek(const std::string &id, const float pos)
            {
                auto it = musics.find(id);

                if (it == musics.end())
                    return false;
                G.seekMusicStream(*AssetManager::getInstance().loadAsset<Music>(it->second), pos);
                position = pos;
                return true;
            }

            float getLength(const std::string &id) const
            {
                auto it = musics.find(id);

                if (it == musics.end())
                    return 0.0f;
                return G.getMusicTimeLength(*AssetManager::getInstance().loadAsset<Music>(it->second));
            }

            float getPlayedLength(const std::string &id) const
            {
                auto it = musics.find(id);

                if (it == musics.end())
                    return 0.0f;
                return G.getMusicTimePlayed(*AssetManager::getInstance().loadAsset<Music>(it->second));
            }

            float getRemainingLength(const std::string &id) const
            {
                return getLength(id) - getPlayedLength(id);
            }
        };
    };
};

#endif /* MUSIC_SOURCE_HPP */
