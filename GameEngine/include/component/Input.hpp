/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     Input                                                                            *
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

#ifndef INPUT_HPP
    #define INPUT_HPP

    #include <unordered_map>
    #include <functional>
    #include "RaylibWrapper.hpp"
    #include "Registry.hpp"

namespace ecs
{
    namespace cmp
    {
        struct Input {
            enum class ActionKind {
                KEY_DOWN,
                KEY_PRESSED
            };

            using actionFunc = std::function<void(Registry &, size_t)>;
            using funcWrapperType = std::pair<Input::ActionKind, actionFunc>;
            using keyBindMapType = std::unordered_map<KeyboardKey, funcWrapperType>;
            using actionList = std::vector<actionFunc>;

            keyBindMapType keyBindings;
            actionList actions;

            explicit Input(keyBindMapType bindings = {}, actionList actions = {})
                : keyBindings(std::move(bindings)), actions(std::move(actions)) {}

            explicit Input(actionList actions)
                : keyBindings{}, actions(std::move(actions)) {}

            void bindKey(KeyboardKey key, ActionKind actionKind, std::function<void(Registry &, size_t)> func)
            {
                keyBindings[key] = std::make_pair(actionKind, func);
            }

            void unbindKey(KeyboardKey key)
            {
                keyBindings.erase(key);
            }

            void addAction(actionFunc action)
            {
                actions.emplace_back(action);
            }

            void checkInputs(Registry &r, size_t i)
            {
                for (auto &[key, funcInfo] : keyBindings) {
                    auto &[actionKind, func] = funcInfo;
                    switch (actionKind) {
                        case ActionKind::KEY_DOWN:
                            if (G.isKeyDown(key))
                                func(r, i);
                            break;
                        case ActionKind::KEY_PRESSED:
                            if (G.isKeyPressed(key))
                                func(r, i);
                            break;
                    }
                }
            }

            void checkActions(Registry &r, size_t i)
            {
                for (auto &action : actions)
                    action(r, i);
            }
        };
    };
};

#endif /* INPUT_HPP */
