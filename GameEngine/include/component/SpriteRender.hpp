/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     SpriteRender                                                                     *
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

#ifndef SPRITE_RENDER_HPP
    #define SPRITE_RENDER_HPP

    #include <memory>
    #include "Transformable.hpp"
    #include "RaylibWrapper.hpp"
    #include "Vec2.hpp"

namespace ecs
{
    namespace cmp
    {
        struct SpriteRender {
            std::shared_ptr<Texture2D> texture;
            Rect _textureRect = {0.0f, 0.0f, 1.0f, 1.0f};
            Col _color = WHITE;

            explicit SpriteRender(const std::shared_ptr<Texture2D> &tex) : texture(tex)
            {
                _textureRect = {0.0f, 0.0f, static_cast<float>(tex->width), static_cast<float>(tex->height)};
            }

            explicit SpriteRender(
                const std::shared_ptr<Texture2D> &tex,
                const Rect &rectangle
            ) : texture(tex)
            {
                _textureRect = rectangle;
            }

            void setTexture(const std::shared_ptr<Texture2D> &tex)
            {
                texture = tex;
            }

            void setTextureRect(const Rect &rectangle)
            {
                _textureRect = rectangle;
            }

            void draw(const Transformable &transformable)
            {
                Rect source = _textureRect;
                Rect dest = {
                    transformable.x,
                    transformable.y,
                    _textureRect.width * transformable.scaleX,
                    _textureRect.height * transformable.scaleY
                };

                Vec2 origin = { 0.0f, 0.0f };
                float rotation = transformable.rotation;

                G.drawTexture(
                    *texture,
                    source,
                    dest,
                    origin,
                    rotation,
                    _color
                );
            }
        };
    };
};

#endif /* SPRITE_RENDER_HPP */
