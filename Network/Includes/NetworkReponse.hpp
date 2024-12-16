/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** NetworkReponse
*/

#ifndef NETWORKREPONSE_HPP_
    #define NETWORKREPONSE_HPP_

    #define NO_ARGS 65535
    #define NUM 4095 // * [int]
    #define NUM_NUM 255 // * [int] [int]
    #define NUM_CH 511 // * [int] [char]
    #define NUM_STR 767 // * [int] [string]
    #define CH 8191 // * [char]
    #define STR 12287 // * [string]
    #define STR_STR 8959 // * [string] [string]
    #define FL 16383 // * [float]
    #define VE2 20479 // * [Vec2]
    #define VE3 24575 // * [Vec3]
    #define ID 28671 // * [UUID]
    #define ID_NUM 24831 // * [UUID] [int]
    #define ID_CH 25087 // * [UUID] [char]
    #define ID_VE2 25855 // * [UUID] [Vec2]
    #define ID_CH_VE2 24911 // * [UUID] [char] [Vec2]

    // ! DEFINE CODE VALUE ARGS
    // ? send by server received by client TCP
    #define SERVER_KO 100 // * NO_ARGS -> //
    #define SERVER_SUCCESS 101 // * NO_ARGS -> //
    #define SERVER_CLIENT_JOIN 102 // * NUM -> [game port]
    #define SERVER_GET_PRIVATE_MESSAGE 103 // * STR_STR -> [sender username] [message]
    #define SERVER_GET_GAME_MESSAGE 104 // * NUM_STR -> [game port] [message]
    #define SERVER_SEND_GAME_INFO 105 // *  NUM_NUM -> [game port] [players]
    #define SERVER_CLIENT_ADMIN 106 // * NO_ARGS -> //

    // ? Commands send my client received by server TCP
    #define CLIENT_CREATE_GAME 000 // * NO_ARGS -> //
    #define CLIENT_JOIN_GAME 001 // * NUM -> [game port]
    #define CLIENT_SEND_PRIVATE_MESSAGE 002 // * STR_STR -> [receiver username] [message]
    #define CLIENT_SEND_GAME_MESSAGE 003 // * NUM_STR -> [game port] [message]
    #define CLIENT_ADMIN 004 // * NUM_STR -> [admin number] [password]
    #define CLIENT_GET_GAMES_LIST 005 // * NO_ARGS -> //
    #define CLIENT_STOP_GAME 006 // * NUM -> [game port] //

    // ? Commands send by server received by client UDP
    #define SPAWN_ENTITY 150 // * ID_CH_VE2 -> [uuid] [entitykind_id] [pos (vec2)]
    #define KILL_ENTITY 151 // * ID -> [uuid entity]
    #define POSITION_ENTITY 152 // * ID_VE2 -> [uuid] [new pos (vec2)]
    #define ENTITY_DAMAGED 153 // * ID -> [uuid entity]
    #define SWAP_SCENE 154 // * CH -> [scene id]
    #define GAME_END 155 // * NO_ARGS -> //
    #define START_SNAPSHOT 156 // * NO_ARGS -> //
    #define END_SNAPSHOT 157 // * ID_NUM -> [snapshot id] [informations number]

    // ? Entitykind_id list:
    #define ID_PLAYER (char)0
    #define ID_OTHER_PLAYER (char)1
    #define ID_PLAYER_BULLET (char)2
    #define ID_BRAIN_MOB (char)3
    #define ID_BRAIN_MOB_BULLET (char)4
    #define ID_ROLLING_MOB (char)5
    #define ID_BRAIN_SEAHORSE_BOSS (char)6

    // ? Scene id list :
    #define ID_SCENE_MENU (char)0
    #define ID_SCENE_LEVEL_1 (char)1
    #define ID_SCENE_LEVEL_2 (char)2

    // ? Commands Send by client received by server UDP
    #define CONNECTION_GAME 50 // * NO_ARGS -> //
    #define INPUT_PLAYER 51 // * NUM_CH -> [key] [type (`pressed`(0) / `down_start`(1) / `down_end`(2))]
    #define DISCONNECTED 52 // * NO_ARGS -> //
    #define CONFIRM_SNAPSHOT 53 // * ID -> // [snapshot id]

    // ? Key state
    #define STATE_KEY_PRESSED (char)0
    #define STATE_KEY_DOWN_START (char)1
    #define STATE_KEY_DOWN_END (char)2

    // ? Key kind
    #define ID_KEY_UP 265
    #define ID_KEY_DOWN 264
    #define ID_KEY_LEFT 263
    #define ID_KEY_RIGHT 262
    #define ID_KEY_ATTACK 69 // ? raylib KEY_E

#endif /* !NETWORKREPONSE_HPP_ */
