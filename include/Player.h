// babys first rigid body 2D  pra dizer que um objeto tem colisao -m

#pragma once

#include "Component.h"
#include "GameObject.h"
#include "InputManager.h"
#include "StateMac.h"
#include "Camera.h"
#include "Sprite.h"
#include "Sound.h"
#include "Collider.h"
#include "Vec2.h"
#include <string>
#include <memory>
#include <algorithm>

enum RBSTATE
{
    LEFT, RIGHT, STILL, CROUCH, IDLE, WALK, RUN, JUMP, FALL, DASH , MELEE, SHOT, SKID
};

class Player : public GameObject{
private:
    void Controls(float dt);
    void Physics(float dt);
    void Animation(float dt);
    void MoveonTopof(GameObject& target);
    
public:
    

    Player(GameObject& associated);
    ~Player();

    void Start();
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    bool Is(C_ID);


    void Jump(float dt);
    void JustGrounded();
    Vec2 Bcurve(Vec2 a ,Vec2 b, Vec2 c, Vec2 d,float dt);
    Vec2 Bcurve(std::vector<Vec2> vec,float dt);
    int GetState();

    
    bool isFiring;
    bool isSlashing;

    Vec2 speed;
    Vec2 oldbox;
    bool* isGrounded;
    int hasDoubleJump;
    int hasDash;
    bool inputDone;
    bool isDashing;
    bool crouchHeld;
    bool jumpStored;
    bool dreamGround;
    bool isDreamDashing;
    bool canDash;
    Timer JumpStoredTimer;
    Timer jumpTimer;
    Timer dashTimer;
    Timer dashCooldown;

    float* surface_inclination;

    int MAX_DASH_QT;
    int MAX_DOUBLE_JUMP_QT;
    float JUMP_ACCE_TIMELIMIT;
    float DASH_TIMELIMIT;
    float JUMP_STORED_TIMELIMIT;
    float DASH_COOLDOWN;
    StateMachine * state_machine;
    //na verção final esses provavelmente serão constantes por isso os nomes UPPER CASE
    float MAX_GLOBAL_SPEED;// = 1200;
    float MAX_MOVE_SPEED;// = 400;
    float MAX_FALL_SPEED;// = 700;

    float MOVE_ACCELERATION;// = 800;
    float FALL_ACCELERATION;// = 700;

    float LATERAL_FRICTION;
    float LATERAL_SPEED_THRESHOLD;
    float DASH_FORCE;
    float JUMP_FORCE;

};