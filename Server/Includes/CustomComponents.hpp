/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Oct, 2024                                                     *
 * Title           - Server                                                             *
 * Description     -                                                                    *
 *     CustomComponents                                                                 *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _|           *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _|           *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_|           *
 *       _|        _|          _|        _|      _|        _|        _|    _|           *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _|           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#ifndef INCLUDED_CUSTOMCOMPONENTS_HPP
    #define INCLUDED_CUSTOMCOMPONENTS_HPP

    #include <iostream>

struct BottomWall {};

struct Wall {};

struct BossSeahorse {};

struct Enemy {};

struct EnemySpawner {};

struct Player {
    bool shooting;

    Player() : shooting(false) {}
};

struct Bullet {
    size_t originEntity;
    int damage;

    Bullet(size_t originEntity, int damage = 0) : originEntity(originEntity), damage(damage) {}
};

struct PlayerBullet {};

struct BrainEnemyBullet {};

struct RollingEnemy {};

struct BrainEnemy {};

#endif /* INCLUDED_CUSTOMCOMPONENTS_HPP */
