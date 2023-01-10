#include "Player.h"
#include "RigidBody.h"
#include "TerrainBody.h"
#include "ActionMachine.h"
#include "StateMac.h"
#include "SpriteSheetNode.h"
#include <iostream>
#include "Game.h"
// define pra test -m
using namespace std;
#define p(x) cout << #x << ": " << x <<" ";



Player::Player(GameObject& associated):GameObject(associated){
    // Sprite* pbody = new Sprite(associated, "assets/img/Zidle.png");
    // associated.AddComponent(pbody);
    // pbody->SetScaleX(2,2);
   
   

    speed =  Vec2(0,0);
    oldbox = Vec2(0,0);
    isGrounded = &((RigidBody*)associated.GetComponent(C_ID::RigidBody))->isGrounded;
    hasDoubleJump = true;
    inputDone = false;
    surface_inclination = 0;

    JUMP_TIMER = 0.2;
    
    MAX_GLOBAL_SPEED = 3000;

    JUMP_FORCE = 1200;
    MAX_FALL_SPEED = 800;
    FALL_ACCELERATION = 100;

    MAX_MOVE_SPEED = 600;
    MOVE_ACCELERATION = 100;
    LATERAL_FRICTION = 80;
    LATERAL_SPEED_THRESHOLD = 100;

    

}
Player::~Player(){

}

void Player::Start(){
    
    // ------------------ STATE MACHINE SETUP --------------------
    
    StateMachine * state_machine = (StateMachine*) associated.GetComponent(C_ID::StateMachine);
    if(state_machine == nullptr) cout << "erro pegando statemachine em player start2\n";
    
    
    // state creation
    SSNode* sprite_sheet_node = new SSNode("assets/img/Zrun.png", {0, 0, 60, 80}, 1, 0.3,Vec2(2,2));
    state_machine->AddNode(RBSTATE::RUN, sprite_sheet_node); 
        
    sprite_sheet_node = new SSNode("assets/img/Zidle.png",  {0, 0, 60, 80}, 1, 1,Vec2(2,2));
    state_machine->AddNode(RBSTATE::IDLE, sprite_sheet_node); 
    
    sprite_sheet_node = new SSNode("assets/img/Zjump.png",  {0, 0, 60, 80}, 1, 1,Vec2(2,2));
    state_machine->AddNode(RBSTATE::JUMP, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Zfall.png",  {0, 0, 60, 80}, 1, 1,Vec2(2,2));
    state_machine->AddNode(RBSTATE::FALL, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Zdash.png",  {0, 0, 60, 80}, 1, 1,Vec2(2,2));
    state_machine->AddNode(RBSTATE::DASH, sprite_sheet_node); 

    // transition creation
    state_machine->AddTransition(RBSTATE::RUN,RBSTATE::DASH);

    // set first colider e state
    Collider * ass_collider = (Collider*)associated.GetComponent(C_ID::Collider);
    if(ass_collider == nullptr) cout << "erro pegando collider em player start\n";
    
    ass_collider->SetScale(Vec2(0.65,0.5)); 
    ass_collider->SetOffset(Vec2(0,8));

    state_machine->ChangeState(RBSTATE::IDLE);//st->AddTransition(0, RBSTATE::RUN); st->AddTransition(RBSTATE::RUN, 0);
    
}

/// @brief Player Update
void Player::Update(float dt){
    
    Animation(dt);
    Controls(dt);
    Physics(dt);//!!check weird hitbox when this is off -m


    *isGrounded = false;
    inputDone = false;
    
   
}
void Player::Render(){

}
bool Player::Is(std::string type){
    return type == "Player";
}

bool Player::Is(C_ID type){
    return type == C_ID::Player;
}



void Player::Animation(float dt){
    
    StateMachine * spr = (StateMachine*) associated.GetComponent(C_ID::StateMachine);
    Collider * ass_collider = (Collider*)associated.GetComponent(C_ID::Collider);
    // ActionMachine * act = (ActionMachine*) associated.GetComponent("ActionMachine");
    if(speed.y < 0 && !*isGrounded){
        spr->ChangeState(RBSTATE::JUMP);
        ass_collider->SetScale(Vec2(0.65,0.5)); 
        ass_collider->SetOffset(Vec2(0,8));
    }
    if(speed.y > 2 && !*isGrounded){
        spr->ChangeState(RBSTATE::FALL);
        ass_collider->SetScale(Vec2(0.65,0.5)); 
        ass_collider->SetOffset(Vec2(0,8));
    }
    auto [idx, cr_state] = spr->GetCurrent();
    // auto [idxa, cr_statea] = act->GetCurrent();
    
    if(speed.x != 0 && *isGrounded && idx != RBSTATE::RUN && idx != RBSTATE::DASH){
        
        spr->ChangeState(RBSTATE::RUN);
        ass_collider->SetScale(Vec2(0.65,0.5)); 
        ass_collider->SetOffset(Vec2(0,8));
    }
        

    if(speed.x == 0 && *isGrounded && idx != RBSTATE::IDLE){
        spr->ChangeState(RBSTATE::IDLE);
        ass_collider->SetScale(Vec2(0.65,0.5)); 
        ass_collider->SetOffset(Vec2(0,8));
    }
        


    if(speed.x < 0) cr_state->SetFliped(true);

    else if (speed.x > 0)
        cr_state->SetFliped(false);
    
}

void Player::Controls(float dt){
    // p(jumpTimer.Get())cout << endl;
    jumpTimer.Update(dt);
    InputManager& inManager = InputManager::GetInstance();

    if(inManager.IsKeyDown(W_KEY)){  
        if (*isGrounded ){
            jumpTimer.Restart();
            *isGrounded = false;
            Jump(dt);
        }else if (jumpTimer.Get()<JUMP_TIMER){
            
            Jump(dt);
        }
    }else{
        jumpTimer.Update(JUMP_TIMER);
    }

    if(inManager.IsKeyDown(S_KEY)){
        
    }
    
    if(inManager.IsKeyDown(A_KEY)){
        if(speed.x>20){
            
        }
        speed.x -= MOVE_ACCELERATION*dt;

         
        // grudar em rampas
        if(*isGrounded&& (surface_inclination != 0)){
            
            associated.box.y += MAX_MOVE_SPEED* 0.030 ;

        }

        inputDone = true;
    }
    if(inManager.IsKeyDown(D_KEY)){
        if(*isGrounded ){
            
            associated.box.y += MAX_MOVE_SPEED* 0.030 ;
                  
        }
        speed.x += MOVE_ACCELERATION*dt;
        inputDone = true;
        
    }
    
    if(inManager.MousePress(LEFT_MOUSE_BUTTON) ){
        associated.box.x = inManager.GetMouseX() + Camera::pos.x;
        associated.box.y = inManager.GetMouseY() + Camera::pos.y;
        speed = Vec2(0,0);
    }

    if(inManager.MousePress(RIGHT_MOUSE_BUTTON) ){
        associated.box.x = 0;
        associated.box.y = 0;
       
        
    }
}

void Player::Physics(float dt){
      
    // Queda 
    speed.y += FALL_ACCELERATION*dt; 
   

    // desacelerar
    if(!inputDone){   
        if(abs(speed.x) < LATERAL_SPEED_THRESHOLD*dt)
            speed.x = 0;
        else if(speed.x > 0){
            speed.x = speed.x - LATERAL_FRICTION*dt; 
        }else{
            speed.x = speed.x + LATERAL_FRICTION*dt; 
        }
    }

    // limitar velocidade global
    if(speed.Magnitude() > MAX_GLOBAL_SPEED*dt){         
        speed = speed.Normalize()*MAX_GLOBAL_SPEED*dt;          
    }

    //limitar velocidade lateral
    if(abs(speed.x) > MAX_MOVE_SPEED*dt){ 
        speed = Vec2((speed.x/abs(speed.x))*MAX_MOVE_SPEED*dt, speed.y );          
    }

    //limitar velocidade de queda
    if(speed.y > MAX_FALL_SPEED*dt){         
        speed.y =  MAX_FALL_SPEED*dt;          
    }

     
    // mova-se de acordo com a velocidade 
    if(*isGrounded) speed.y = 0;

    Vec2 center = Vec2(associated.box.GetCenter() + speed);
    associated.box.SetCenter(center.x,center.y);

}

void Player::Jump (float dt){
    speed.y = -JUMP_FORCE*dt;
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