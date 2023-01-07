#pragma once
#include "State.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Music.h"
#include "TileSet.h"
#include "TileMap.h"
#include "Camera.h"
#include "CameraFollower.h"
#include "Alien.h"
#include "PenguinBody.h"
#include "Collider.h"
#include "Collision.h"
#include "GameData.h"
#include "EndState.h"
#include "InputManager.h"
#include "Vec2.h"
#include <vector>
#include <memory>

class StageState : public State {
    private:
        TileSet* tileSet;
        Music backgroundMusic;
    public:
        StageState();
        ~StageState();
        void LoadAssets();
        void Start();
        void Update(float dt);
        void Render();
        void Pause();
        void Resume();
        void Collision();
};