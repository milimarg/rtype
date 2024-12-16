/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** CameraComponent.hpp
*/

#ifndef B_CPP_500_STG_5_1_RTYPE_NOE_TRITSCH_CAMERA_HPP
    #define B_CPP_500_STG_5_1_RTYPE_NOE_TRITSCH_CAMERA_HPP

    #include "RaylibWrapper.hpp"

namespace ecs
{
    namespace cmp
    {
        struct CameraWrapper
        {
            Camera3D camera;
            CameraMode mode;

            explicit CameraWrapper(
                Vector3 position = {0.0f, 0.0f, 0.0f},
                Vector3 target = {0.0f, 0.0f, 10.0f},
                Vector3 up = {0.0f, 1.0f, 0.0f},
                float fovy = 90.0f,
                CameraProjection projection = CAMERA_PERSPECTIVE,
                CameraMode mode = CAMERA_CUSTOM)
                : camera({position, target, up, fovy, projection}),
                mode(mode)
            {}
        };
    };
};

#endif /* B_CPP_500_STG_5_1_RTYPE_NOE_TRITSCH_CAMERA_HPP */
