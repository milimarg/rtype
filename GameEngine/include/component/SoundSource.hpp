/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** Music.hpp
*/

#ifndef B_CPP_500_STG_5_1_RTYPE_NOE_TRITSCH_SOUND_HPP
    #define B_CPP_500_STG_5_1_RTYPE_NOE_TRITSCH_SOUND_HPP

    #include <string>
    #include "RaylibWrapper.hpp"
    #include "AssetManager.hpp"

namespace ecs
{
    namespace cmp
    {
        struct SoundSource
        {
            std::unordered_map<std::string, std::string> sounds;
            float volumeSource = 1.0f;
            float pitchSource = 0.0f;
            float panSource = 0.0f;
            float position = 0.0f;

            SoundSource() = default;

            void addSound(const std::string &id, const std::string &filePath)
            {
                AssetManager::getInstance().loadAsset<Sound>(filePath);
                sounds[id] = filePath;
            }

            bool removeSound(const std::string &id)
            {
                auto it = sounds.find(id);

                if (sounds.find(id) == sounds.end())
                    return false;
                sounds.erase(it);
                return true;
            }

            bool checkSound(const std::string &id)
            {
                return sounds.contains(id);
            }

            bool play(const std::string &id)
            {
                auto it = sounds.find(id);

                if (it == sounds.end())
                    return false;
                G.playSound(*AssetManager::getInstance().loadAsset<Sound>(it->second));
                return true;
            }

            void safePlay(const std::string &id, const std::string &filePath)
            {
                if (!checkSound(id))
                    addSound(id, filePath);
                play(id);
            }

            bool stop(const std::string &id)
            {
                auto it = sounds.find(id);

                if (it == sounds.end())
                    return false;
                G.stopSound(*AssetManager::getInstance().loadAsset<Sound>(it->second));
                return true;
            }

            bool pause(const std::string &id)
            {
                auto it = sounds.find(id);

                if (it == sounds.end())
                    return false;
                G.pauseSound(*AssetManager::getInstance().loadAsset<Sound>(it->second));
                return true;
            }

            bool resume(const std::string &id)
            {
                auto it = sounds.find(id);

                if (it == sounds.end())
                    return false;
                G.resumeSound(*AssetManager::getInstance().loadAsset<Sound>(it->second));
                return true;
            }

            bool setVolume(const std::string &id, const float volume)
            {
                auto it = sounds.find(id);

                if (it == sounds.end())
                    return false;
                G.setSoundVolume(*AssetManager::getInstance().loadAsset<Sound>(it->second), volume);
                volumeSource = volume;
                return true;
            }

            bool setPitch(const std::string &id, const float pitch)
            {
                auto it = sounds.find(id);

                if (it == sounds.end())
                    return false;
                G.setSoundVolume(*AssetManager::getInstance().loadAsset<Sound>(it->second), pitch);
                pitchSource = pitch;
                return true;
            }

            bool setPan(const std::string &id, const float pan)
            {
                auto it = sounds.find(id);

                if (it == sounds.end())
                    return false;
                G.setSoundPan(*AssetManager::getInstance().loadAsset<Sound>(it->second), pan);
                panSource = pan;
                return true;
            }
        };
    };
};

#endif /* B_CPP_500_STG_5_1_RTYPE_NOE_TRITSCH_SOUND_HPP */
