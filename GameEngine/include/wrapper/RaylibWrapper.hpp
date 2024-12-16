/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Thu, Oct, 2024                                                     *
 * Title           - GameEngine                                                         *
 * Description     -                                                                    *
 *     RaylibWrapper                                                                    *
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

#ifndef INCLUDED_RAYLIBWRAPPER_HPP
    #define INCLUDED_RAYLIBWRAPPER_HPP

    #include "IGraphicalLibraryWrapper.hpp"

class RaylibWrapper : public IGraphicalLibraryWrapper
{
    public:
        RaylibWrapper(const RaylibWrapper&) = delete;
        RaylibWrapper &operator=(const RaylibWrapper&) = delete;

        static RaylibWrapper &getInstance();

        void init() override;
        void end() override;

        Vec2 getMousePosition() override;
        bool isKeyPressed(int) override;
        bool isKeyDown(int) override;
        bool isMouseButtonPressed(int) override;
        void drawText(
            const std::string &text,
            int posX,
            int posY,
            int fontSize,
            Col &color
        ) override;
        void drawTexture(
            Texture2D &texture,
            Rect &source,
            Rect &dest,
            Vec2 &origin,
            float rotation,
            Col &tint
        ) override;
        void playSound(Sound &sound) override;
        void stopSound(Sound &sound) override;
        void pauseSound(Sound &sound) override;
        void resumeSound(Sound &sound) override;
        void setSoundVolume(Sound &sound, float v) override;
        void setSoundPan(Sound &sound, float p) override;
        void playMusicStream(Music &music) override;
        void stopMusicStream(Music &music) override;
        void pauseMusicStream(Music &music) override;
        void resumeMusicStream(Music &music) override;
        void setMusicVolume(Music &music, float v) override;
        void setMusicPitch(Music &music, float p) override;
        void setMusicPan(Music &music, float p) override;
        void updateMusicStream(Music &music) override;
        void seekMusicStream(Music &music, float s) override;
        float getMusicTimeLength(Music &music) override;
        float getMusicTimePlayed(Music &music) override;

    private:
        RaylibWrapper() = default;
};

#define G RaylibWrapper::getInstance()

using Rect = RaylibWrapper::Rect;

#endif /* INCLUDED_RAYLIBWRAPPER_HPP */
