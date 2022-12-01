// Essa provavelmente vai ser bem similar a TerrainBody.
// mas a ideia e que terreno ou não se move, ou só se move por vontate propria, mão interaje com nada.
// se um objeto vai reagir a outros ele provavelmente deve ser TerrainBody.                                -m

#pragma once

#include "Component.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Camera.h"
#include "Sprite.h"
#include "Sound.h"
#include "Collider.h"
#include "Vec2.h"
#include <string>
#include <memory>
#include <algorithm>


class TerrainBody : public Component{
private:
public:
    TerrainBody(GameObject& associated);
    ~TerrainBody();

    void Start();
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    void NotifyCollision(GameObject& other,Vec2 sep);

    
   
};