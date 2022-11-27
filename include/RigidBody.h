// babys first rigid body 2D  pra dizer que um objeto tem colisao -m

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


class RigidBody : public Component{
private:
public:
    RigidBody(GameObject& associated,int modo);
    ~RigidBody();

    void Start();
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    void NotifyCollision(GameObject& other);

    int modo;
   
};