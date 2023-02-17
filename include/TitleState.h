#pragma once
#include "State.h"
#include "StageState.h"
#include "GameObject.h"
#include "Camera.h"
#include "Sprite.h"
#include "Text.h"
#include "InputManager.h"
#include <string>

class TitleState : public State {
    private:
        Music backgroundMusic;
    public:
        TitleState();
        ~TitleState();
        void LoadAssets();
        void Start();
        void Update(float dt);
        void Render();
        void Pause();
        void Resume();
        void Collision();
};