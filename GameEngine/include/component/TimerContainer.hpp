/*
** EPITECH PROJECT, 2024
** ECS
** File description:
** Clock.hpp
*/

#ifndef ECS_CLOCK_HPP
    #define ECS_CLOCK_HPP
    #include <unordered_map>
    #include <string>

namespace ecs
{
    namespace cmp
    {
        struct Timer {
            // ? time are expressed in second
            float _current = 0.0f;
            float _threshold = 0.0f;
            size_t _reached = 0;

            Timer(float threshold = 0.0f) : _threshold(threshold) {}

            bool consume()
            {
                if (_reached) {
                    _reached -= 1;
                    return true;
                }
                return false;
            }

            bool consumeAll()
            {
                if (_reached) {
                    _reached = 0;
                    return true;
                }
                return false;
            }

            void resetTime()
            {
                _current = 0.0f;
            }

            void resetReached()
            {
                _reached = 0;
            }

            void updateTime(const float deltaTime)
            {
                if (_threshold == 0.0f) {
                    ++_reached;
                    return;
                }

                _current += deltaTime;
                while (_current >= _threshold) {
                    _current -= _threshold;
                    ++_reached;
                }
            }
        };

        struct TimerContainer {
            std::unordered_map<std::string, Timer> _timers;

            TimerContainer() = default;

            void add(const std::string &id, float threshold)
            {
                _timers[id] = Timer(threshold);
            }

            bool contains(const std::string &id) const
            {
                return _timers.contains(id);
            }

            Timer &safeGet(const std::string &id, float threshold)
            {
                if (!contains(id))
                    add(id, threshold);
                return _timers[id];
            }

            bool safeConsume(const std::string &id, float threshold)
            {
                return safeGet(id, threshold).consume();
            }

            void erase(const std::string &id)
            {
                _timers.erase(id);
            }

            bool safeUniqueConsume(const std::string &id, float threshold)
            {
                if (safeGet(id, threshold).consume()) {
                    erase(id);
                    return true;
                }
                return false;
            }
        };
    };
};

#endif /* ECS_CLOCK_HPP */
