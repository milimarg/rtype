/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Nov, 2024                                                     *
 * Title           - GameEngine                                                         *
 * Description     -                                                                    *
 *     RaylibWrapper                                                                    *
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

#include "RaylibWrapper.hpp"

RaylibWrapper &RaylibWrapper::getInstance()
{
    static thread_local RaylibWrapper instance;
    return instance;
}

void RaylibWrapper::init()
{
    InitWindow(800, 600, "Raylib Window");
    InitAudioDevice();
}

void RaylibWrapper::end()
{
    CloseAudioDevice();
    CloseWindow();
}

Vec2 RaylibWrapper::getMousePosition()
{
    Vector2 pos = GetMousePosition();
    return { pos.x, pos.y };
}

bool RaylibWrapper::isKeyPressed(int key)
{
    return IsKeyPressed(key);
}

bool RaylibWrapper::isKeyDown(int key)
{
    return IsKeyDown(key);
}

bool RaylibWrapper::isMouseButtonPressed(int button)
{
    return IsMouseButtonPressed(button);
}

void RaylibWrapper::drawText(const std::string &text, int posX, int posY, int fontSize, Col &color)
{
    DrawText(text.c_str(), posX, posY, fontSize, { color.r, color.g, color.b, color.a });
}

void RaylibWrapper::drawTexture(Texture2D &texture, Rect &source, Rect &dest, Vec2 &origin, float rotation, Col &tint)
{
    Rectangle src = { source.x, source.y, source.width, source.height };
    Rectangle dst = { dest.x, dest.y, dest.width, dest.height };
    Vector2 orig = { origin.x, origin.y };
    Color raylibTint = { tint.r, tint.g, tint.b, tint.a };
    DrawTexturePro(texture, src, dst, orig, rotation, raylibTint);
}

void RaylibWrapper::playSound(Sound &sound)
{
    PlaySound(sound);
}

void RaylibWrapper::stopSound(Sound &sound)
{
    StopSound(sound);
}

void RaylibWrapper::pauseSound(Sound &sound)
{
    PauseSound(sound);
}

void RaylibWrapper::resumeSound(Sound &sound)
{
    ResumeSound(sound);
}

void RaylibWrapper::setSoundVolume(Sound &sound, float v)
{
    SetSoundVolume(sound, v);
}

void RaylibWrapper::setSoundPan(Sound &sound, float p)
{
    SetSoundPan(sound, p);
}

void RaylibWrapper::playMusicStream(Music &music)
{
    PlayMusicStream(music);
}

void RaylibWrapper::stopMusicStream(Music &music)
{
    StopMusicStream(music);
}

void RaylibWrapper::pauseMusicStream(Music &music)
{
    PauseMusicStream(music);
}

void RaylibWrapper::resumeMusicStream(Music &music)
{
    ResumeMusicStream(music);
}

void RaylibWrapper::setMusicVolume(Music &music, float v)
{
    SetMusicVolume(music, v);
}

void RaylibWrapper::setMusicPitch(Music &music, float p)
{
    SetMusicPitch(music, p);
}

void RaylibWrapper::setMusicPan(Music &music, float p)
{
    SetMusicPan(music, p);
}

void RaylibWrapper::updateMusicStream(Music &music)
{
    UpdateMusicStream(music);
}

void RaylibWrapper::seekMusicStream(Music &music, float s)
{
    SeekMusicStream(music, s);
}

float RaylibWrapper::getMusicTimeLength(Music &music)
{
    return GetMusicTimeLength(music);
}

float RaylibWrapper::getMusicTimePlayed(Music &music)
{
    return GetMusicTimePlayed(music);
}
