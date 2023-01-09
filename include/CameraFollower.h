#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Camera.h"
#include <string>

class CameraFollower : public GameObject {
    public:
        CameraFollower(GameObject& go);
        void Update(float dt);
        void Render();
        bool Is(std::string type);
        bool Is(C_ID);
};