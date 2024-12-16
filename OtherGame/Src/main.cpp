/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sun, Oct, 2024                                                     *
 * Title           - OtherGame                                                          *
 * Description     -                                                                    *
 *     main                                                                             *
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

#include "GameEngineIncludesList.hpp"

template <typename... Components>
void registerComponents(Registry &registry) // TODO: make it a part of the base registry
{
    (registry.registerComponent<Components>(), ...);
}

void registerComponents(Registry &registry)
{
    registerComponents<
        TimerContainer,
        Animated, NamedFrames,
        Drawable, SpriteRender, Red, Text, HUD,
        Transformable, Collision, Velocity, Acceleration,
        Input,
        SoundSource, MusicSource,
        Mouse, Button,
        Health,
        PlayerData, ActionMenuRemovable, AttackRemovable, FightRemovable, ActionButton
    >(registry);
}

void registerSystems(Registry &registry)
{
    registry
        // ? time
        .addSystem<TimerContainer>(ClockSystem())

        // ? events
        .addSystem<Input>(InputSystem())
        .addSystem<Button>(ButtonSystem())

        // ? update
        .addSystem<Transformable, Velocity>(TransformSystem())
        .addSystem<Transformable, Collision, Velocity, Acceleration, Gravity>(CollisionSystem())
        .addSystem<Health, TimerContainer>(HealthSystem())
        .addSystem<Mouse, Transformable>(MouseSystem())
        .addSystem<MusicSource>(MusicSystem())
        .addSystem<Spawner, TimerContainer, Transformable>(SpawnerSystem())

        // ? render
        .addSystem<Animated, SpriteRender, TimerContainer>(AnimationSystem())
        .addSystem<NamedFrames, SpriteRender>(NamedFramesSystem())
        .addSystem<Drawable, Transformable, SpriteRender, RectangleRender, Text>(DrawSystem());
}

int main(void)
{
    Registry &registry = Registry::getInstance();

    registerComponents(registry);
    registerSystems(registry);

    SetTraceLogLevel(LOG_WARNING);
    InitWindow(1920, 1080, "my_hunter");
    SetWindowState(FLAG_WINDOW_ALWAYS_RUN);
    InitAudioDevice();
    SetTargetFPS(FPS);
    SetExitKey(0);

    SceneLoader sceneLoader(registry, "./config/config.json");
    sceneLoader.loadScene("setup");
    sceneLoader.loadScene("action menu");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        registry
            .runSystems()
            .update();
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
