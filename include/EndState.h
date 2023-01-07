#pragma once
#include "State.h"
#include "GameObject.h"
#include "Music.h"
#include "Text.h"
#include "Sprite.h"
#include "GameData.h"
#include "InputManager.h"
#include <string>

class EndState : public State {
    private:
        Music backgroundMusic;
    public:
        EndState();
        ~EndState();
        void LoadAssets();
        void Start();
        void Update(float dt);
        void Render();
        void Pause();
        void Resume();
        void Collision();
};