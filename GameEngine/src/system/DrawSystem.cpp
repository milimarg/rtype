/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     DrawSystem                                                                       *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░             *
 *         ▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓             *
 *         █  ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "DrawSystem.hpp"
#include "Zipper.hpp"
#include "IndexedZipper.hpp"
#include "Using.hpp"
#include "utils.hpp"
#include "HUD.hpp"

USING(
    ecs::sys::DrawSystem,
    ecs::cmp::Red,
    ecs::cmp::HUD
);

DrawSystem::DrawSystem() = default;

void DrawSystem::operator()(
    Registry &registry,
    Storage<Drawable> &drawables,
    Storage<Transformable> &transforms,
    Storage<SpriteRender> &sprites,
    Storage<RectangleRender> &rectangles,
    Storage<Text> &texts
) const
{
    auto display = [&registry, &drawables, &transforms, &sprites, &rectangles, &texts](bool isHud) {
        for (auto &&[i, drawable, transform, sprite] : IndexedZipper(drawables, transforms, sprites)) {
            if (isHud != registry.hasComponent<HUD>(i))
                continue;

            if (registry.hasComponent<Red>(i))
                sprite._color = RED;
            else
                sprite._color = WHITE;
            sprite.draw(transform);
        }
        for (auto &&[i, drawable, transform, rectangle] : IndexedZipper(drawables, transforms, rectangles)) {
            if (isHud != registry.hasComponent<HUD>(i))
                continue;

            if (registry.hasComponent<Red>(i))
                rectangle.setColor(RED);
            else
                rectangle.setColor(WHITE);

            rectangle.draw(transform);
        }
        for (auto &&[i, _, text, transform] : IndexedZipper(drawables, texts, transforms)) {
            if (isHud != registry.hasComponent<HUD>(i))
                continue;

            text.render(transform);
        }
    };

    display(false);
    display(true);

    // ? debug info display
    // DrawFPS(10, 10);
    // DrawText((std::to_string(registry.getComponents<SpriteRender>().size()) + " entities").c_str(), 200, 10, 30, WHITE);
}
