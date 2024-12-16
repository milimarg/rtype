/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** mainClient
*/

#if defined(_WIN32)
    #define NOGDI
    #define NOUSER
#endif

#include "NetClient.hpp"
#include "GameEngineIncludesClient.hpp"

template <typename... Components>
void registerComponents(Registry &registry)
{
    (registry.registerComponent<Components>(), ...);
}

void registerComponents(Registry &registry)
{
    registerComponents<
        TimerContainer,
        Acceleration,
        Collision,
        Input,
        Gravity,
        Drawable, RectangleRender, SpriteRender, Text,
        Animated, NamedFrames,
        Transformable,
        Velocity,
        CameraWrapper, StarField,
        SoundSource, MusicSource,
        Mouse, Button,
        Red,
        HUD
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
        .addSystem<Transformable, Collision, Velocity, Acceleration, Gravity>(CollisionSystem())
        .addSystem<StarField, CameraWrapper, Drawable>(StarFieldSystem())
        .addSystem<Mouse, Transformable>(MouseSystem())
        .addSystem<Red, TimerContainer>([](Registry &r, Storage<Red> &reds, Storage<TimerContainer> &timers) {
            for (auto &&[entity, red, timer] : IndexedZipper(reds, timers)) {
                if (timer.safeUniqueConsume("ENTITY_DAMAGED", 0.2f)) {
                    try {
                        r.removeComponent<Red>(entity);
                    } catch (const std::exception &e) {}
                }
            }
        })
        .addSystem<MusicSource>(MusicSystem())

        // ? render
        .addSystem<Animated, SpriteRender, TimerContainer>(AnimationSystem())
        .addSystem<NamedFrames, SpriteRender>(NamedFramesSystem())
        .addSystem<Drawable, Transformable, SpriteRender, RectangleRender, Text>(DrawSystem());
}

int main(int argc, char **argv)
{
    Registry &registry = Registry::getInstance();

    try {
        if (argc != 3)
            throw std::runtime_error("Not enough arguments");
        std::string ip = argv[1];
        std::string port = argv[2];
        registerComponents(registry);
        registerSystems(registry);

        SetTraceLogLevel(LOG_WARNING);
        InitWindow(1920, 1080, "r-type");
        SetWindowState(FLAG_WINDOW_ALWAYS_RUN);
        InitAudioDevice();
        SetTargetFPS(FPS);
        SetExitKey(0);

        std::shared_ptr<ClientSceneLoader> sceneLoader = std::make_shared<ClientSceneLoader>(registry, "./config/client-config.json");
        sceneLoader->restartFromScene("menu");
        registry.update();

        ASIO::io_context contextIO;
        Network::NetClient &channel = Network::NetClient::getInstance();
        channel.setAdress(ip, port);
        channel.setClientSceneLoader(sceneLoader);
        std::cout << "Starting loop client!" << std::endl;
        while (!WindowShouldClose() && !Network::NetClient::_shouldStop) {
            channel.run();
            BeginDrawing();
            ClearBackground(BLACK);
            registry
                .runSystems()
                .update();
            EndDrawing();
        }
        channel.stop();
        CloseWindow();
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
