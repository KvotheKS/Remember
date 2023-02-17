// Essa provavelmente vai ser bem similar a Gate.
// mas a ideia e que terreno ou não se move, ou só se move por vontate propria, mão interaje com nada.
// se um objeto vai reagir a outros ele provavelmente deve ser Gate.                                -m

#pragma once

#include "GameObject.h"
#include "Component.h"
#include "InputManager.h"
#include "Camera.h"
#include "Sprite.h"
#include "Sound.h"
#include "Collider.h"
#include "Vec2.h"
#include "ScreenFade.h"
#include <string>
#include <memory>
#include <algorithm>
#include "State.h"
#include "Stage2.h"


class Gate : public GameObject {
private:
    State * target_state;
    int spawn_point;

    Timer exitTimer;
    
public:
    bool active;
    Gate(GameObject& associated, State * target_state, int spawn_point = 0,bool active = true);
    ~Gate();

    void Start();
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    bool Is(C_ID);
    void NotifyCollision(GameObject& other,Vec2 sep);
   
};