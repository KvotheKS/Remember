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
#include "RigidBody.h"
#include "TerrainBody.h"

#include "Collider.h"
#include "Collision.h"
#include "GameData.h"
#include "EndState.h"
#include "InputManager.h"
#include "Vec2.h"
#include <vector>
#include <memory>

class TestState : public State {
private:
    TileSet* tileSet;
    Music backgroundMusic;
public:
    TestState();
    ~TestState();
    void LoadAssets();
    void Start();
    void Update(float dt);
    void Render();
    void Pause();
    void Resume();

    Vec2 Bcurve(Vec2 a ,Vec2 b, Vec2 c, Vec2 d,float dt);
    Vec2 Bcurve2(std::vector<Vec2> vec,float dt);
};