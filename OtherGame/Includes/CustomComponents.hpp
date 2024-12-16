/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sun, Oct, 2024                                                     *
 * Title           - OtherGame                                                          *
 * Description     -                                                                    *
 *     CustomComponents                                                                 *
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

#ifndef INCLUDED_CUSTOMCOMPONENTS_HPP
    #define INCLUDED_CUSTOMCOMPONENTS_HPP

    #include <string>

struct PlayerData {
    int score = 0;

    PlayerData() = default;

    void updateScore(int value)
    {
        score += value;
    }
};

struct ActionMenuRemovable {};
struct AttackRemovable {};
struct FightRemovable {};

struct ActionButton {};
struct Arrow {
    int damage;
    std::string direction;

    Arrow(int damage, const std::string &direction) : damage(damage), direction(direction) {}
};

#endif
