#pragma once
#include "GameObject.h"
#include "InputManager.h"
#include "Vec2.h"

typedef void (*CameraFunction)(float);
typedef bool (*ConditionalLambda)();
class Camera {
    private:
        static GameObject* focus;
    public:
        static Vec2 pos;
        static CameraFunction fnc;
        static Vec2 speed;
        static Vec2 lpos;
        static Rect Bounds;
        static int width, height;
        static std::pair<ConditionalLambda, CameraFunction> transition;

        static void Follow(GameObject* newFocus);
        static void Unfollow();
        static void Update(float dt);
        static void FollowTarget(float dt);
        static bool False();
        static void Stationary(float dt);
        static void ControlCamera(float dt);
        static void SetCameraFunction(CameraFunction fnc);
        static void SetCameraTransition(ConditionalLambda = Camera::False, CameraFunction = Stationary);
};