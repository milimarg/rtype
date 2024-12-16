/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Oct, 2024                                                     *
 * Title           - GameEngine                                                        *
 * Description     -                                                                    *
 *     ButtonSystem                                                                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄             *
 *      ▐  ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀            *
 *        █▄▄▄▄▄  ▐  █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             *
 *        █    ▌     █          █        █        █    ▌    █         █   █             *
 *       ▄▀▄▄▄▄    ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             *
 *       █    ▐   █         █       █ █          █    ▐   █     ▐   █   █               *
 *       ▐        ▐         ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "ButtonSystem.hpp"
#include "Zipper.hpp"
#include "RaylibWrapper.hpp"

USING(ecs::sys::ButtonSystem);

void ButtonSystem::operator()(
    [[maybe_unused]] const ecs::Registry &registry,
    Storage<Button> &buttons
) const
{
    for (auto &&[button] : Zipper(buttons)) {
        auto &[onClick, onClickOutside, hovered, clicked] = button;

        if (G.isMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (hovered)
                button.onClick();
            else if (clicked)
                button.onClickOutside();
            clicked = hovered;
        }
        hovered = false;
    }
}
