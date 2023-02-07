#include "Player.h"
#include "RigidBody.h"
#include "TerrainBody.h"
#include "Rand.h"

#include "StateMac.h"
#include "SpriteSheetNode.h"
#include <iostream>
#include "Projectile.h"
#include "Game.h"
#include <limits>
// define pra test -m
using namespace std;
#define p(x) cout << #x << ": " << x <<" ";

#define nysize 0.7
#define nxsize 0.3

#define nyoffset 18
#define nycrouchoffset 41
#define nxoffset 0

Player::Player(GameObject& associated):GameObject(associated){
    // Sprite* pbody = new Sprite(associated, "assets/img/Zidle.png");
    // associated.AddComponent(pbody);
    // pbody->SetScaleX(2,2);
   
   
    isFiring = false;
    isSlashing = false;
    isAttacking = false;
    speed =  Vec2(0,0);
    oldbox = Vec2(0,0);
    isGrounded = &((RigidBody*)associated.GetComponent(C_ID::RigidBody))->isGrounded;
    hasDash = 2;
    hasDoubleJump = 0;
    inputDone = false;
    crouchHeld =false;
    isDashing = false;
    dreamGround = false;
    jumpStored = false;
    isStunned = false;

    surface_inclination = &((RigidBody*)associated.GetComponent(C_ID::RigidBody))->surface_inclination;

    MAX_DASH_QT = 1;
    MAX_DOUBLE_JUMP_QT = 1;

    JUMP_ACCE_TIMELIMIT = 0.1;
    DASH_TIMELIMIT = 0.26;
    JUMP_STORED_TIMELIMIT = 0.15;
    DASH_COOLDOWN = 0.15;
    STUN_TIMELIMIT = 0.3;

    MAX_HP = 100;

    MAX_GLOBAL_SPEED = 3000;
    DASH_FORCE = 1200;
    JUMP_FORCE = 1000 ;
    MAX_FALL_SPEED = 1000;
    FALL_ACCELERATION = 100;

    MAX_DASH_SPEED = 1000;
    MAX_DASHDOWN_SPEED = 1600;
    MAX_MOVE_SPEED = 300;
    MOVE_ACCELERATION = 100;
    LATERAL_FRICTION = 80;
    LATERAL_SPEED_THRESHOLD = 100;

    

}
Player::~Player(){

}

void Player::Start(){
    hp = MAX_HP;
    // ------------------ STATE MACHINE SETUP --------------------
    
    state_machine = (StateMachine*) associated.GetComponent(C_ID::StateMachine);
    if(state_machine == nullptr) cout << "erro pegando statemachine em player start2\n";
    
    
    // state creation
    Vec2 scale = Vec2(2,2);
    SSNode* sprite_sheet_node = new SSNode("assets/img/Ype/Ycrouch.png",  {0, 0, 80*5, 80}, 5, 0.1,scale);
    state_machine->AddNode(RBSTATE::CROUCH, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Ype/Yidle.png",  {0, 0, 80*18, 80}, 18, 0.1,scale);
    state_machine->AddNode(RBSTATE::IDLE, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Ype/Yidleblink.png",  {0, 0, 80*18, 80}, 18, 0.1,scale);
    state_machine->AddNode(RBSTATE::IDLE_B, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Ype/Ywalk.png",  {0, 0, 80, 80}, 1, 1,scale);
    state_machine->AddNode(RBSTATE::WALK, sprite_sheet_node);

    sprite_sheet_node = new SSNode("assets/img/Ype/Yrun.png",  {0, 0, 80*8, 80}, 8, 0.1,scale);
    state_machine->AddNode(RBSTATE::RUN, sprite_sheet_node); 
        
    sprite_sheet_node = new SSNode("assets/img/Ype/Yjump.png",  {0, 0, 80, 80}, 1, 1,scale);
    state_machine->AddNode(RBSTATE::JUMP, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Ype/Yfall.png",  {0, 0, 80, 80}, 1, 1,scale);
    state_machine->AddNode(RBSTATE::FALL, sprite_sheet_node); 
    
    sprite_sheet_node = new SSNode("assets/img/Ype/Ydash.png",  {0, 0, 80*3, 80}, 3, 0.1,scale);
    state_machine->AddNode(RBSTATE::DASH, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Ype/Ydashup.png",  {0, 0, 80*4, 80},4,0.1,scale);
    state_machine->AddNode(RBSTATE::DASHUP, sprite_sheet_node);

    sprite_sheet_node = new SSNode("assets/img/Ype/Yidle.png",  {0, 0, 80, 80}, 1, 1,scale);
    state_machine->AddNode(RBSTATE::MELEE, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Ype/Yidle.png",  {0, 0, 80, 80}, 1, 1,scale);
    state_machine->AddNode(RBSTATE::SHOT, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Ype/Yidle.png",  {0, 0, 80, 80}, 1, 1,scale);
    state_machine->AddNode(RBSTATE::SKID, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Ype/Yidle.png",  {0, 0, 80, 80}, 1, 1,scale);
    state_machine->AddNode(RBSTATE::STUN, sprite_sheet_node); 



    // transition creation
    state_machine->AddTransition(RBSTATE::WALK,RBSTATE::RUN);
    state_machine->AddTransition(RBSTATE::IDLE_B,RBSTATE::IDLE);
    

    // set first colider e state
    Collider * ass_collider = (Collider*)associated.GetComponent(C_ID::Collider);
    if(ass_collider == nullptr) cout << "erro pegando collider em player start\n";
    
    ass_collider->SetScale(Vec2(nxsize,nysize)); 
    ass_collider->SetOffset(Vec2(nxoffset,nyoffset));

    state_machine->ChangeState(RBSTATE::IDLE);//st->AddTransition(0, RBSTATE::RUN); st->AddTransition(RBSTATE::RUN, 0);
    
}

/// @brief Player Update
void Player::Update(float dt){
  
    Controls(dt);
    RunTimers(dt);
    Physics(dt);
    Animation(dt);

    *isGrounded = false;
    inputDone = false;
    crouchHeld = false;
}
void Player::Render(){
}
bool Player::Is(std::string type){
    return type == "Player";
}

bool Player::Is(C_ID type){
    return type == C_ID::Player;
}

void Player::Controls(float dt){

    if(isStunned)return;

    auto [state_idx, cr_state] = state_machine->GetCurrent();
    jumpTimer.Update(dt);
    InputManager& inManager = InputManager::GetInstance();
    Vec2 movement_direction = Vec2(0,0);


    
    bool space_pressed = false;
    
    
    // UP COMMAND
    if(inManager.IsKeyDown(UP_ARROW_KEY)){  
        movement_direction.y -= 1;

    }
  
    // DOWN COMMAND
    if(inManager.IsKeyDown(DOWN_ARROW_KEY) ){
        movement_direction.y += 1;
       
        
        if(*isGrounded){   
            crouchHeld = true;              
        }      
    }
    
    // LEFT COMMAND
    if(inManager.IsKeyDown(LEFT_ARROW_KEY) ){ 
        if(!isDashing && !(*isGrounded && isAttacking)){
            movement_direction.x -= 1;
            cr_state->SetFliped(true); 

            if(!crouchHeld ){
            
               
                speed.x -= MOVE_ACCELERATION*dt;
                inputDone = true;
            }
            // grudar em rampas
            // if(*isGrounded && (*surface_inclination != 0)){//!!deveria fazer uma função em rigidbody que devolve esses valores em vez de usar ponteiros
                   
            //     associated.box.y += MAX_MOVE_SPEED * 0.030 ;
                
            // }
        }
        
    }
    
    // RIGHT COMMAND
    if(inManager.IsKeyDown(RIGHT_ARROW_KEY)){
        if(!isDashing && !(*isGrounded && isAttacking)){
            movement_direction.x += 1;
            cr_state->SetFliped(false);
            
            if(!crouchHeld ){
         
                speed.x += MOVE_ACCELERATION*dt;

                inputDone = true;
            }
            // if(*isGrounded  && (*surface_inclination != 0)){
            //     associated.box.y += MAX_MOVE_SPEED * 0.030 ;
                    
            // }
        }
  
    }

    // JUMP COMMAND
    if(inManager.IsKeyDown(S_KEY)){
       
        if(inManager.KeyPress(S_KEY)){
            if (hasDoubleJump > 0){
                if(isDashing){
                    JumpStoredTimer.Restart();
                    jumpStored = true;
                }else{
                    if(!*isGrounded)Jump(0);
                    
                    hasDoubleJump --;
                    speed.y  = 0;
                    speed.y =  -JUMP_FORCE*dt;
                  
                }
            }
        }
      
        if (*isGrounded || dreamGround){
            
            hasDoubleJump = MAX_DOUBLE_JUMP_QT;
            Jump(1);
            *isGrounded = false;
        
            speed.y = -JUMP_FORCE*dt;
            
            if(isDashing){
                isDreamDashing = true;
            }
            isDashing = false;  
            dreamGround = false;
        }
        if (jumpTimer.Get()<JUMP_ACCE_TIMELIMIT){     
            if(!isDashing)speed.y = -JUMP_FORCE*dt;
           
        }
        
    }else{      
        jumpTimer.Update(JUMP_ACCE_TIMELIMIT);
    }
    
    // DASH COMMAND
    if(inManager.IsKeyDown(A_KEY)){ 
        if(hasDash > 0 &&  canDash && !isDashing && !isDreamDashing){
            
            
            hasDash--;
            if(movement_direction.y >0 && *isGrounded){
                movement_direction.y = 0;
            }
            /* Dash Based on Movement Direction*/
            
            if(movement_direction == Vec2(0,0)){ // if no directed, dash toward facing direction
                (cr_state->GetFliped())?movement_direction.x = -1:movement_direction.x = 1;
            }

            if(*isGrounded ){
                dreamGround = true;
                GameObject* GO_jumpdust_effect = new GameObject();
    
                    
                    Effect* effect = new Effect(*GO_jumpdust_effect,0,0,"assets/img/Ype/dashdust.png",4,0.075);
                    
                    GO_jumpdust_effect->AddComponent(effect);
                    auto ass_center = associated.box.GetCenter();
                    GO_jumpdust_effect->box.SetCenter(ass_center.x,ass_center.y+56);
                
                    if(movement_direction.x == -1){//!!fazer isso no effect
                        ((Sprite*) GO_jumpdust_effect->GetComponent(C_ID::Sprite))->SetFliped(true);
                    }
                    State& state = Game::GetInstance().GetCurrentState();
                state.AddObject(GO_jumpdust_effect);
            }
            
            speed = movement_direction.Normalize() * DASH_FORCE * dt;
            /* Dash Based on Mouse Position    */
            // speed = Vec2((inManager.GetMouseX() + Camera::pos.x)-associated.box.GetCenter().x,
            //             (inManager.GetMouseY() + Camera::pos.y)-associated.box.GetCenter().y).Normalize() * DASH_FORCE*dt;
            isDashing = true;
            dashTimer.Restart();

            canDash = false;
            dashCooldown.Restart();
        }
       
    }
    // LEFT CLICK COMAND
    if(inManager.MousePress(LEFT_MOUSE_BUTTON) ){
        
        isAttacking = true;
        atackTimer.Restart();
        // mouse direction code
        // speed = Vec2((inManager.GetMouseX() + Camera::pos.x)-associated.box.GetCenter().x,
        // (inManager.GetMouseY() + Camera::pos.y)-associated.box.GetCenter().y).Normalize() * 1000*dt;

        //GetStunned(Vec2((inManager.GetMouseX() + Camera::pos.x)-associated.box.GetCenter().x, (inManager.GetMouseY() + Camera::pos.y)-associated.box.GetCenter().y),dt);
        
        /* teleport to mouse click position*/ 
        // associated.box.x = inManager.GetMouseX() + Camera::pos.x;
        // associated.box.y = inManager.GetMouseY() + Camera::pos.y;
        // speed = Vec2(0,0);
    }
    // RIGHT CLICK COMAND
    if(inManager.MousePress(RIGHT_MOUSE_BUTTON) ){
        associated.box.x = 0;
        associated.box.y = 0;
    }

    
}

void Player::RunTimers(float dt){
    if(isAttacking){
        atackTimer.Update(dt);
        if(atackTimer.Get()>1)isAttacking =false;
    }

    if(isStunned){
        stunTimer.Update(dt);
        if(stunTimer.Get()> STUN_TIMELIMIT) isStunned = false;
    }
    

    if(!canDash){
        dashCooldown.Update(dt);
        if(dashCooldown.Get() > DASH_TIMELIMIT+DASH_COOLDOWN) canDash = true;      
    }

    if(isDashing){
        dashTimer.Update(dt);
        if(dashTimer.Get() > DASH_TIMELIMIT)isDashing = false;
    }

    if(jumpStored){
        JumpStoredTimer.Update(dt);
        if(JumpStoredTimer.Get() > JUMP_STORED_TIMELIMIT)jumpStored = false;
    }
}

void Player::Physics(float dt){
   
    // Queda 
    if(!isDashing){
        speed.y += FALL_ACCELERATION*dt; 
    }
    
    if(jumpStored && !isDashing){
        hasDoubleJump = false;
        speed.y = -JUMP_FORCE*dt;
        jumpStored = false;
    }

    // desacelerar
    if(!inputDone && !isDashing ){   
        if(abs(speed.x) < LATERAL_SPEED_THRESHOLD*dt)
            speed.x = 0;
        else if(speed.x > 0){
            speed.x = speed.x - LATERAL_FRICTION*dt; 
            
        }else{
            speed.x = speed.x + LATERAL_FRICTION*dt; 
        }
    }

    
    float local_max;
    (isDreamDashing)?local_max = MAX_DASH_SPEED:local_max = MAX_MOVE_SPEED;
    
    
    if(isDashing){
        if(speed.y > 0 && *isGrounded){
            isDashing = false;
        }
        float lmax_dash_speed = (speed.x == 0 && speed.y>0)?MAX_DASHDOWN_SPEED:MAX_DASH_SPEED;
        
        if(speed.Magnitude() > lmax_dash_speed*dt){ 
        
            float overspeed = speed.Magnitude() - lmax_dash_speed*dt;
            speed = speed - overspeed * speed.Normalize() * 0.5;          
        }
        
        // cout << lmax_dash_speed << endl;
        
    }else if (isStunned){
    }else{
        // limitar velocidade lateral
        if(abs(speed.x) > local_max*dt){ 
            float overspeed = abs(speed.x) - local_max*dt;
            float norm = speed.x/abs(speed.x);
            speed = Vec2( (speed.x - overspeed * norm * 0.5) , speed.y );          
        }
        // limitar velocidade de queda
        if(speed.y > MAX_FALL_SPEED*dt){         
            speed.y =  MAX_FALL_SPEED*dt;  
            float overspeed = abs(speed.y) - MAX_FALL_SPEED*dt;
            float norm = speed.y/abs(speed.y);
            speed = Vec2( speed.x , (speed.y - overspeed * norm * 0.5));         
        }
    }  
    
    // limitar velocidade global
    if(speed.Magnitude() > MAX_GLOBAL_SPEED*dt){         
        speed = speed.Normalize()*MAX_GLOBAL_SPEED*dt;          
    }

    // grounded
    if(*isGrounded){
        if(speed.y>0) speed.y = 0;
        // if(speed.x > 0){
        //     if(*surface_inclination <45)associated.box.y += MAX_MOVE_SPEED * 0.030 ;
        // }
        // if(speed.x < 0){
        //     if(*surface_inclination >45)associated.box.y += MAX_MOVE_SPEED * 0.030 ;
        // }
        if(*surface_inclination !=0)associated.box.y += MAX_MOVE_SPEED * 0.030 ;//!! isso esconde o bug dele tremer em plataforma, n sei por uqe
    }
    if(isAttacking){
        if(*isGrounded)speed.x = 0;
    }
    // mova-se de acordo com a velocidade 
    Vec2 center = Vec2(associated.box.GetCenter() + speed);
    associated.box.SetCenter(center.x,center.y);

}



void Player::Animation(float dt){
    state_machine = (StateMachine*) associated.GetComponent(C_ID::StateMachine);
    auto [state_idx, cr_state] = state_machine->GetCurrent();
    Collider * ass_collider = (Collider*)associated.GetComponent(C_ID::Collider);
    

    if(isDashing){
        
        if(speed.x == 0) {
            state_machine->ChangeState_s(RBSTATE::DASHUP);
            ass_collider->SetScale(Vec2(nxsize,nysize*0.60)); 
        } else{
            state_machine->ChangeState_s(RBSTATE::DASH); 
            ass_collider->SetScale(Vec2(nxsize,nysize*0.60)); 
            ass_collider->SetOffset(Vec2(nxoffset,nycrouchoffset));

            auto v = (Vec2(0,0).AngleLine(speed) * 180 / 3.141592); 
            associated.angleDeg = v;
            
            if(cr_state->GetFliped()){
                associated.angleDeg -= 180;
            }
        }
        
        
        

        
        
        
        // (speed.x <= 0)?cr_state->SetFliped(false):cr_state->SetFliped(true); 
    }else{

        associated.angleDeg = 0;

        if(speed.y < 0 && !*isGrounded){
            state_machine->ChangeState_s(RBSTATE::JUMP);
            ass_collider->SetScale(Vec2(nxsize,nysize)); 
            ass_collider->SetOffset(Vec2(nxoffset,nyoffset));
        }
        if(speed.y > 2 && !*isGrounded){
            state_machine->ChangeState_s(RBSTATE::FALL);
            ass_collider->SetScale(Vec2(nxsize,nysize)); 
            ass_collider->SetOffset(Vec2(nxoffset,nyoffset));
        }
        if(crouchHeld && *isGrounded  ){
            state_machine->ChangeState_s(RBSTATE::CROUCH);
            ass_collider->SetScale(Vec2(nxsize,nysize*0.60)); 
            ass_collider->SetOffset(Vec2(nxoffset,nycrouchoffset));
        }else{
            if(speed.x != 0 && *isGrounded && state_idx != RBSTATE::RUN && state_idx != RBSTATE::RUN){    
            state_machine->ChangeState_s(RBSTATE::RUN);
            ass_collider->SetScale(Vec2(nxsize,nysize)); 
            ass_collider->SetOffset(Vec2(nxoffset,nyoffset));
            }
            
            if(speed.x == 0 && *isGrounded && state_idx != RBSTATE::IDLE && state_idx != RBSTATE::IDLE_B){
                
                
                state_machine->ChangeState_s((Rand::Get_r()<0.30)?RBSTATE::IDLE_B:RBSTATE::IDLE);
                ass_collider->SetScale(Vec2(nxsize,nysize)); 
                ass_collider->SetOffset(Vec2(nxoffset,nyoffset));
            }
        }
    }
    if(isStunned){
        
        state_machine->ChangeState_s(RBSTATE::STUN);
        ass_collider->SetScale(Vec2(nxsize,nysize)); 
        ass_collider->SetOffset(Vec2(0,8));
    }

    
}

void Player::Jump (bool type){
    jumpTimer.Restart();

    GameObject* GO_jumpdust_effect = new GameObject();
        string dust_sprite_name = "";
        (type == 1)?dust_sprite_name = "assets/img/Ype/jumpdust.png":dust_sprite_name = "assets/img/Ype/airjumpdust.png";
        Effect* effect = new Effect(*GO_jumpdust_effect,0,0,dust_sprite_name,4,0.075);
        
        GO_jumpdust_effect->AddComponent(effect);
        auto ass_center = associated.box.GetCenter();
        GO_jumpdust_effect->box.SetCenter(ass_center.x,ass_center.y+64);
     

        State& state = Game::GetInstance().GetCurrentState();
    state.AddObject(GO_jumpdust_effect);
    
    // speed.y = -JUMP_FORCE*dt;
}
void Player::JustGrounded(){//!!doesn work
    // isAttacking = false;
    isDreamDashing = false;
    hasDash = MAX_DASH_QT;
    
}

void Player::GetStunned(Vec2 dir, float dt){
    auto [state_idx, cr_state] = state_machine->GetCurrent();
    InputManager& inManager = InputManager::GetInstance();
    speed = dir.Normalize()*1000*dt;
    
    if(speed.x >0){
        cr_state->SetFliped(true);
    }else{
        cr_state->SetFliped(false);
    }
    isStunned = true;
    isDashing = false;
    isDreamDashing = false;
    stunTimer.Restart();
}

int Player::GetState()
{
    return  RBSTATE::RIGHT*(speed.x > 0 && *isGrounded) +
            RBSTATE::LEFT*(speed.x < 0 && *isGrounded) +
            RBSTATE::STILL*(speed.x == 0 && *isGrounded) +
            RBSTATE::JUMP*(speed.y < 0 && !*isGrounded) +
            RBSTATE::FALL*(speed.y > 2 && !*isGrounded) +
            RBSTATE::RUN*(speed.x != 0 && *isGrounded) +
            RBSTATE::IDLE*(speed.x == 0 && *isGrounded);
}