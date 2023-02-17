#include "Player.h"
#include "RigidBody.h"
#include "TerrainBody.h"
#include "ActionMachine.h"
#include "Rand.h"
#include "ScreenFade.h"
#include "StateMac.h"
#include "SpriteSheetNode.h"
#include <iostream>
#include "Projectile.h"
#include "Game.h"
#include "Attack.h"

#include <limits>
// define pra test -m
using namespace std;
#define p(x) cout << #x << ": " << x <<" ";

#define nysize 0.6
#define nxsize 0.3

#define nyoffset 24
#define nycrouchoffset 43
#define nxoffset 0

Player::Player(GameObject& associated):GameObject(associated){
    // Sprite* pbody = new Sprite(associated, "assets/img/Zidle.png");
    // associated.AddComponent(pbody);
    // pbody->SetScaleX(2,2);
    // associated.AddComponent(new Attack(associated, 10, Vec2(0,0), &associated));
    invul = false;
    isDead = false;
    pause = false;
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
    CAST_TIMELIMIT = 0.4;
    CAST_COOLDOWN = 0.3;

    MAX_HP = 100;

    MAX_GLOBAL_SPEED = 3000;
    DASH_FORCE = 1200;
    JUMP_FORCE = 16 ;
    MAX_FALL_SPEED = 1000;
    FALL_ACCELERATION = 100;

    MAX_DASH_SPEED = 1000;
    MAX_DASHDOWN_SPEED = 2000;
    MAX_MOVE_SPEED = 300;
    MOVE_ACCELERATION = 100;
    LATERAL_FRICTION = 80;
    LATERAL_SPEED_THRESHOLD = 100;

    

}
Player::~Player(){

}

void Player::Start(){
    invulTimer.SetFinish(2.0);
    deathTimer.SetFinish(1.3);
    pause = false;
    hp = MAX_HP;
    // ------------------ STATE MACHINE SETUP --------------------
    
    state_machine = (StateMachine*) associated.GetComponent(C_ID::StateMachine);
    if(state_machine == nullptr) cout << "erro pegando statemachine em player start2\n";
    
    // state creation
    Vec2 scale = Vec2(2,2);

    SSNode* sprite_sheet_node = new SSNode("assets/img/Ype/Ycrouch_loop.png",  {0, 0, 80, 80}, 1, 1,scale);
    state_machine->AddNode(RBSTATE::CROUCH, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Ype/Ycrouch.png",  {0, 0, 80*5, 80}, 5, 0.075,scale);
    state_machine->AddNode(RBSTATE::CROUCHSTART, sprite_sheet_node); 



    sprite_sheet_node = new SSNode("assets/img/Ype/Yidle.png",  {0, 0, 80*18, 80}, 18, 0.1,scale);
    state_machine->AddNode(RBSTATE::IDLE, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Ype/Yidleblink.png",  {0, 0, 80*18, 80}, 18, 0.1,scale);
    state_machine->AddNode(RBSTATE::IDLE_B, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Ype/Yrun.png",  {0, 0, 80*3, 80}, 3, 0.1,scale);
    state_machine->AddNode(RBSTATE::WALK, sprite_sheet_node);

    
    sprite_sheet_node = new SSNode("assets/img/Ype/Yrun_loop.png",  {0, 0, 80*8, 80}, 8, 0.1,scale);
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

    sprite_sheet_node = new SSNode("assets/img/Ype/Ymorte.png",  {0, 0, 80*13, 80}, 13, 0.1,scale);
    state_machine->AddNode(RBSTATE::DIE, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Ype/Ydano.png",  {0, 0, 80, 80}, 1, 1,scale);
    state_machine->AddNode(RBSTATE::STUN, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Ype/YmagicR.png",  {0, 0, 80*4, 80}, 4, 0.1,scale);
    state_machine->AddNode(RBSTATE::CASTR, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Ype/YmagicL.png",  {0, 0, 80*6, 80}, 6, 0.1,scale);
    state_machine->AddNode(RBSTATE::CASTL, sprite_sheet_node); 




    // transition creation
    state_machine->AddTransition(RBSTATE::WALK,RBSTATE::RUN);
    state_machine->AddTransition(RBSTATE::IDLE_B,RBSTATE::IDLE);
    state_machine->AddTransition(RBSTATE::CROUCHSTART,RBSTATE::CROUCH);
    

    // set first colider e state
    Collider * ass_collider = (Collider*)associated.GetComponent(C_ID::Collider);
    if(ass_collider == nullptr) cout << "erro pegando collider em player start\n";
    
    ass_collider->SetScale(Vec2(nxsize,nysize)); 
    ass_collider->SetOffset(Vec2(nxoffset,nyoffset));

    state_machine->ChangeState(RBSTATE::IDLE);//st->AddTransition(0, RBSTATE::RUN); st->AddTransition(RBSTATE::RUN, 0);
}

/// @brief Player Update
void Player::Update(float dt){

    if(pause)return;
    RunTimers(dt);
    Controls(dt); 
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
    //ATTACK COMMAND
    if(inManager.IsKeyDown(D_KEY) && !isAttacking){
        
        isAttacking = true;
        castTimer.Restart();

        auto& st = Game::GetInstance().GetCurrentState();
        auto proj_go = new GameObject();
            auto proj = new Projectile(*proj_go, 20.0f, 0.0f, 600.0f, 600.0f);
            auto spr = new Sprite(*proj_go, "assets/img/Ype/Ymagic.png", 6,0.1,0);
            auto atk = new Attack(*proj_go, 10000, Vec2(),&associated);
            auto dsp = new DisappearOnHit(*proj_go, &associated);
            auto cld = (Collider*)proj_go->GetComponent(C_ID::Collider);
            cld->type = C_ID::Hitbox;
            proj_go->AddComponents({proj, spr, atk, dsp});
            spr->SetScaleX(3,3);
            proj_go->box.x = associated.box.GetCenter().x;
            proj_go->box.y = associated.box.GetCenter().y;
        st.bulletArray.emplace_back(proj_go);
    }
  
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
       
        
      
        if (*isGrounded || dreamGround){
            
            
            Jump(1);
            *isGrounded = false;
        
            speed.y = -JUMP_FORCE;
            
            if(isDashing){
                isDreamDashing = true;
            }
            isDashing = false;  
            dreamGround = false;
        }
        else if(inManager.KeyPress(S_KEY)){
            if (hasDoubleJump > 0){
                if(isDashing){
                    JumpStoredTimer.Restart();
                    jumpStored = true;
                }else{
                    if(!*isGrounded)Jump(0);
                    
                    hasDoubleJump --;
                    speed.y  = 0;
                    speed.y =  -JUMP_FORCE;
                  
                }
            }
        }
        if (jumpTimer.Get()<JUMP_ACCE_TIMELIMIT){     
            if(!isDashing)speed.y = -JUMP_FORCE;
           
        }
        
    }else{      
        jumpTimer.Update(dt);
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
    
                    
                    Effect* effect = new Effect(*GO_jumpdust_effect,0,0,"assets/img/Effects/dashdust.png",4,0.075);
                    
                    GO_jumpdust_effect->AddComponent(effect);
                    auto ass_center = associated.box.GetCenter();
                   
                
                    int x_offset = -associated.box.w/2.5;
                    if(movement_direction.x == -1){//!!fazer isso no effect
                        ((Sprite*) GO_jumpdust_effect->GetComponent(C_ID::Sprite))->SetFliped(true);
                        x_offset *= -1;
                    }
                    GO_jumpdust_effect->box.SetCenter(ass_center.x+x_offset,ass_center.y+56);
                    State& state = Game::GetInstance().GetCurrentState();
                state.AddObject(GO_jumpdust_effect);
            }
            
            speed = movement_direction.Normalize() * DASH_FORCE* dt;
            if(movement_direction ==  Vec2(0,1))speed.y *= 1.5;
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
        
        

        // GameObject* GO_fade = new GameObject();
        //     GO_fade->depth = 1000;
        //     float fadeinTime = 1; float fadeoutTime = 1;float duration = 4;
        //     ScreenFade* effect = new ScreenFade(*GO_fade, fadeinTime, fadeoutTime, duration);
            
        //     GO_fade->AddComponent(effect);
            
        //     GO_fade->box.SetCenter(0,0);
        

        //     State& state = Game::GetInstance().GetCurrentState();
        // state.AddObject(GO_fade);
       
        // mouse direction code
        // speed = Vec2((inManager.GetMouseX() + Camera::pos.x)-associated.box.GetCenter().x,
        // (inManager.GetMouseY() + Camera::pos.y)-associated.box.GetCenter().y).Normalize() * 1000*dt;

        // GetStunned(Vec2((inManager.GetMouseX() + Camera::pos.x)-associated.box.GetCenter().x, (inManager.GetMouseY() + Camera::pos.y)-associated.box.GetCenter().y));
        

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
        castTimer.Update(dt);
        if(castTimer.Get()>CAST_TIMELIMIT)isAttacking =false;
    }

    if(isStunned){
        stunTimer.Update(dt);
        if(stunTimer.Get()> STUN_TIMELIMIT)isStunned = false; 
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
    if(isDead){
        if(deathTimer.Update(dt)){
            associated.RequestDelete();
        }
    }
    if(invulTimer.Update(dt)){
        invul = false;
    }

}

void Player::Physics(float dt){
    
    // Queda 
    if(!isDashing){
        speed.y += FALL_ACCELERATION*dt; 
    }
    
    if(jumpStored && !isDashing){
        hasDoubleJump = false;
        speed.y = -JUMP_FORCE;
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
        // limitar velocidade de queda //!! isso fode em frammerate alta provavel
        if(speed.y > MAX_FALL_SPEED*dt){         
            speed.y =  MAX_FALL_SPEED*dt;  
            float overspeed = abs(speed.y) - MAX_FALL_SPEED*dt;
            float norm = speed.y/abs(speed.y);
            speed = Vec2( speed.x , (speed.y - overspeed * norm * 0.5));         
        }
    }  
    
    // limitar velocidade global
    // if(speed.Magnitude() > MAX_GLOBAL_SPEED*dt){         
    //     speed = speed.Normalize()*MAX_GLOBAL_SPEED*dt;          
    // }

    // grounded
    if(*isGrounded){
        
        
        if(speed.y>0) speed.y = 0;
        // if(speed.x > 0){
        //     if(*surface_inclination <45)associated.box.y += MAX_MOVE_SPEED * 0.030 ;
        // }
        // if(speed.x < 0){
        //     if(*surface_inclination >45)associated.box.y += MAX_MOVE_SPEED * 0.030 ;
        // }
        if(*surface_inclination !=0)associated.box.y += MAX_MOVE_SPEED * 2 * dt ;//!! isso esconde o bug dele tremer em plataforma, n sei por uqe
    }
    if(isAttacking){
        if(*isGrounded)speed.x = 0;
    }
    // mova-se de acordo com a velocidade 
    Vec2 center = Vec2(associated.box.GetCenter() + speed);
    associated.box.SetCenter(center.x,center.y);
    
    
    Collider * ass_collider = (Collider*)associated.GetComponent(C_ID::Collider);
    associated.box.x = min(Bounds.x + Bounds.w - associated.box.w, max(Bounds.x, associated.box.x));
    associated.box.y = min(Bounds.y + Bounds.h - associated.box.h, max(Bounds.y, associated.box.y));
    
}



void Player::Animation(float dt){

    
    state_machine = (StateMachine*) associated.GetComponent(C_ID::StateMachine);
    auto [state_idx, cr_state] = state_machine->GetCurrent();
    Collider * ass_collider = (Collider*)associated.GetComponent(C_ID::Collider);
    
    if(isAttacking){
        state_machine->ChangeState_s(RBSTATE::CASTR); 
        ass_collider->SetScale(Vec2(nxsize,nysize)); 
        ass_collider->SetOffset(Vec2(nxoffset,nyoffset));
    }else if(isDead){
        state_machine->ChangeState_s(RBSTATE::DIE); 
        ass_collider->SetScale(Vec2(nxsize,nysize)); 
        ass_collider->SetOffset(Vec2(nxoffset,nyoffset));
    }
    else if(isDashing){
        auto v = (Vec2(0,0).AngleLine(speed) * 180 / 3.141592); 
        if(speed.x == 0) {
            state_machine->ChangeState_s(RBSTATE::DASHUP);
            ass_collider->SetScale(Vec2(nxsize,nysize*0.60)); 
        } else{
            state_machine->ChangeState_s(RBSTATE::DASH); 
            ass_collider->SetScale(Vec2(nxsize,nysize*0.60)); 
            ass_collider->SetOffset(Vec2(nxoffset,nycrouchoffset));
            
         
          
            associated.angleDeg = v;
           
            
            if(cr_state->GetFliped()){
                associated.angleDeg -= 180;
                
            }
            
            
        }
        if(Rand::Get_r()<0.20){
            GameObject* GO_dashline_effect = new GameObject();

                Effect* effect = new Effect(*GO_dashline_effect,0,0,"assets/img/Effects/dashline.png",8,0.07);
            
                GO_dashline_effect->AddComponent(effect);
                auto ass_center = associated.box.GetCenter();

                
                int offset_y = (60 * Rand::Get_r())-30;
            
                GO_dashline_effect->angleDeg = v;
                GO_dashline_effect->box.SetCenter(ass_center.x+((speed.x>0)?offset_y:(-offset_y)),ass_center.y+((speed.y<0)?offset_y:(-offset_y)));
            

                State& state = Game::GetInstance().GetCurrentState();
            state.AddObject(GO_dashline_effect);
        }
      
        
        // (speed.x <= 0)?cr_state->SetFliped(false):cr_state->SetFliped(true); 
    }else if(isStunned){
        
        state_machine->ChangeState_s(RBSTATE::STUN);
        ass_collider->SetScale(Vec2(nxsize,nysize)); 
        ass_collider->SetOffset(Vec2(nxoffset,nyoffset));
    
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
        if(crouchHeld && *isGrounded ){
            if(state_idx != RBSTATE::CROUCHSTART && state_idx != RBSTATE::CROUCH){
                state_machine->ChangeState_s(RBSTATE::CROUCHSTART);
                ass_collider->SetScale(Vec2(nxsize,nysize*0.60)); 
                ass_collider->SetOffset(Vec2(nxoffset,nycrouchoffset));
            }
        
        }else if(speed.x != 0 && *isGrounded && state_idx != RBSTATE::WALK && state_idx != RBSTATE::RUN){    
            state_machine->ChangeState_s(RBSTATE::WALK);
            ass_collider->SetScale(Vec2(nxsize,nysize)); 
            ass_collider->SetOffset(Vec2(nxoffset,nyoffset));
            }
            
        else if(speed.x == 0 && *isGrounded && state_idx != RBSTATE::IDLE && state_idx != RBSTATE::IDLE_B){ 
                state_machine->ChangeState_s((Rand::Get_r()<0.30)?RBSTATE::IDLE_B:RBSTATE::IDLE);
                ass_collider->SetScale(Vec2(nxsize,nysize)); 
                ass_collider->SetOffset(Vec2(nxoffset,nyoffset));
            }
        
    }
    

    
}

void Player::Jump (bool type){
    jumpTimer.Restart();

    GameObject* GO_jumpdust_effect = new GameObject();
        string dust_sprite_name = "";
        (type == 1)?dust_sprite_name = "assets/img/Effects/jumpdust.png":dust_sprite_name = "assets/img/Effects/airjumpdust.png";
        Effect* effect = new Effect(*GO_jumpdust_effect,0,0,dust_sprite_name,4,0.075);
        
        GO_jumpdust_effect->AddComponent(effect);
        auto ass_center = associated.box.GetCenter();
        GO_jumpdust_effect->box.SetCenter(ass_center.x,ass_center.y+64);
     

        State& state = Game::GetInstance().GetCurrentState();
    state.AddObject(GO_jumpdust_effect);
    
    // speed.y = -JUMP_FORCE;
}
void Player::JustGrounded(){//!!doesn work, so retorna is grounded
    // isAttacking = false;
    isDreamDashing = false;
    hasDash = MAX_DASH_QT;
    hasDoubleJump = MAX_DOUBLE_JUMP_QT;
    
}

void Player::GetStunned(Vec2 dir){
    auto [state_idx, cr_state] = state_machine->GetCurrent();
    InputManager& inManager = InputManager::GetInstance();
    speed = dir.Normalize()*30;
    
    if(speed.x >0){
        cr_state->SetFliped(false);
    }else{
        cr_state->SetFliped(true);
    }
    isStunned = true;
    isDashing = false;
    isDreamDashing = false;
    stunTimer.Restart();
}
void Player::Die(){
    isStunned = false;
    isDashing = false;
    isDreamDashing = false;
    stunTimer.SetFinish(1);

    isDead = true;
    

}

void Player::NotifyCollision(GameObject& other, Vec2 sep)
{
    auto bingus = (Attack*)other.GetComponent(C_ID::Attack);
    if(!bingus || bingus->OwnedBy(&associated))
        return;
    
    
    TakeDamage(bingus->GetDamage(), (other.box.x>associated.box.x)?0:1);
}

void Player::TakeDamage(int damage, int direction){
    if(invul)return;
    hp -= damage;
    invulTimer.Restart();
    invul = true;

    if(direction == 1){
        GetStunned(Vec2(1,-1));
    }else{
        GetStunned(Vec2(-1,-1));
    }

   if(hp<=0){
        Die();
    }
}

void Player::SetPause(bool pause){
    this->pause = pause;
}

// void Player::Tint(bool trans){
//     auto statelist = state_machine->GetStates();

//     for(auto & node: statelist){
//         node.second->SetTint(255,255,255);
//     }
// }
