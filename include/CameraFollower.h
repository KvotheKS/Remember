#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Camera.h"
#include <string>

class CameraFollower : public Component {
    public:
        CameraFollower(GameObject& go);
        void Update(float dt);
        void Render();
        bool Is(std::string type);
};