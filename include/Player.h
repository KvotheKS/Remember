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
    LEFT, RIGHT, STILL, CROUCH, IDLE, IDLE_B, WALK, RUN, JUMP, FALL, DASH ,DASHUP, MELEE, SHOT, SKID, STUN
};

class Player : public GameObject{
private:
    void Controls(float dt);
    void RunTimers(float dt);
    void Physics(float dt);
    void Animation(float dt);
    
    
public:
    

    Player(GameObject& associated);
    ~Player();

    void Start();
    void Update(float dt);
    void Render();
    
    bool Is(std::string type);
    bool Is(C_ID);


    void Jump(bool type);
    void Shoot(float dt);
    void bonkHead();
    /// @brief stuns and push player 
    /// @param dir direction of push
    void GetStunned(Vec2 dir,float dt);
    void JustGrounded();

    int GetState();
    
    

    int hp;
    bool isFiring;
    bool isSlashing;

    Vec2 speed;
    Vec2 oldbox;
    bool* isGrounded;
    float* surface_inclination;
    int hasDoubleJump;
    int hasDash;
    bool inputDone;
    bool isDashing;
    bool isAttacking;
    bool crouchHeld;
    bool jumpStored;
    bool dreamGround;
    bool isDreamDashing;
    bool isStunned;
    bool canDash;

    Timer JumpStoredTimer;
    Timer jumpTimer;
    Timer dashTimer;
    Timer dashCooldown;
    Timer stunTimer;
    Timer atackTimer;
    


    int MAX_HP;


    int MAX_DASH_QT;
    int MAX_DOUBLE_JUMP_QT;
    float JUMP_ACCE_TIMELIMIT;
    float DASH_TIMELIMIT;
    float JUMP_STORED_TIMELIMIT;
    float DASH_COOLDOWN;
    float STUN_TIMELIMIT;
    float JUMP_TIMER;


    StateMachine * state_machine;
    //na verção final esses provavelmente serão constantes por isso os nomes UPPER CASE
    float MAX_GLOBAL_SPEED;// = 1200;

    float MAX_DASH_SPEED;
    float MAX_DASHDOWN_SPEED;
    float MAX_MOVE_SPEED;// = 400;
    float MAX_FALL_SPEED;// = 700;

    float MOVE_ACCELERATION;// = 800;
    float FALL_ACCELERATION;// = 700;

    float LATERAL_FRICTION;
    float LATERAL_SPEED_THRESHOLD;
    float DASH_FORCE;
    float JUMP_FORCE;

};