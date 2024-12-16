/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Thu, Oct, 2024                                                     *
 * Title           - GameEngine                                                         *
 * Description     -                                                                    *
 *     IGraphicalLibraryWrapper                                                         *
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

#ifndef INCLUDED_IGRAPHICALLIBRARYWRAPPER_HPP
    #define INCLUDED_IGRAPHICALLIBRARYWRAPPER_HPP

    #include <iostream>
    #include "Vec2.hpp"
    #define NOGDI
    #include "raylib.h"

using Col = Color;

class IGraphicalLibraryWrapper
{
    public:
        virtual ~IGraphicalLibraryWrapper() = default;

        struct Rect {
            float x;
            float y;
            float width;
            float height;
        };

        virtual void init() = 0;
        virtual void end() = 0;

        virtual Vec2 getMousePosition() = 0;
        virtual bool isKeyPressed(int) = 0;
        virtual bool isKeyDown(int) = 0;
        virtual bool isMouseButtonPressed(int) = 0;
        virtual void drawText(
            const std::string &text,
            int posX,
            int posY,
            int fontSize,
            Col &color
        ) = 0;
        virtual void drawTexture(
            Texture2D &texture,
            Rect &source,
            Rect &dest,
            Vec2 &origin,
            float rotation,
            Col &tint
        ) = 0;
        virtual void playSound(Sound &) = 0;
        virtual void stopSound(Sound &) = 0;
        virtual void pauseSound(Sound &) = 0;
        virtual void resumeSound(Sound &) = 0;
        virtual void setSoundVolume(Sound &, float) = 0;
        virtual void setSoundPan(Sound &, float) = 0;
        virtual void playMusicStream(Music &) = 0;
        virtual void stopMusicStream(Music &) = 0;
        virtual void pauseMusicStream(Music &) = 0;
        virtual void resumeMusicStream(Music &) = 0;
        virtual void setMusicVolume(Music &, float) = 0;
        virtual void setMusicPitch(Music &, float) = 0;
        virtual void setMusicPan(Music &, float) = 0;
        virtual void updateMusicStream(Music &) = 0;
        virtual void seekMusicStream(Music &, float) = 0;
        virtual float getMusicTimeLength(Music &) = 0;
        virtual float getMusicTimePlayed(Music &) = 0;
};

#endif /* INCLUDED_IGRAPHICALLIBRARYWRAPPER_HPP */
