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


class RigidBody : public GameObject{
private:
    void Controls(float dt);
    void Physics(float dt);
    void Animation(float dt);
    void MoveonTopof(GameObject& target);
    
public:
    RigidBody(GameObject& associated,int modo);
    ~RigidBody();

    void Start();
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    void NotifyCollision(GameObject& other,Vec2 sep);

    int modo;

    Vec2 speed;
    Vec2 oldbox;
    bool isGrounded;
    bool hasDoubleJump;
    bool inputDone;
    //na verção final esses provavelmente serão constantes por isso os nomes UPPER CASE
    float MAX_GLOBAL_SPEED;// = 1200;
    float MAX_MOVE_SPEED;// = 400;
    float MAX_FALL_SPEED;// = 700;

    float MOVE_ACCELERATION;// = 800;
    float FALL_ACCELERATION;// = 700;

    float LATERAL_FRICTION;
    float LATERAL_SPEED_THRESHOLD;

    float JUMP_FORCE;
    
    

};