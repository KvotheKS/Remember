#include "Player.h"
#include "RigidBody.h"
#include "TerrainBody.h"

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
    hasDash = 2;
    hasDoubleJump = 0;
    inputDone = false;
    crouchHeld =false;
    isDashing = false;
    
    jumpStored = false;
    
    surface_inclination = 0;

    MAX_DASH_QT = 2;
    MAX_DOUBLE_JUMP_QT = 2;

    JUMP_ACCE_TIMELIMIT = 0.1;
    DASH_TIMELIMIT = 0.25;

    MAX_GLOBAL_SPEED = 3000;
    DASH_FORCE = 1200;
    JUMP_FORCE = 1200;
    MAX_FALL_SPEED = 800;
    FALL_ACCELERATION = 100;

    MAX_MOVE_SPEED = 300;
    MOVE_ACCELERATION = 100;
    LATERAL_FRICTION = 80;
    LATERAL_SPEED_THRESHOLD = 100;

    

}
Player::~Player(){

}

void Player::Start(){
    
    // ------------------ STATE MACHINE SETUP --------------------
    
    state_machine = (StateMachine*) associated.GetComponent(C_ID::StateMachine);
    if(state_machine == nullptr) cout << "erro pegando statemachine em player start2\n";
    
    
    // state creation
    Vec2 scale = Vec2(2,2);
    SSNode* sprite_sheet_node = new SSNode("assets/img/Zcrouch.png",  {0, 0, 60, 80}, 1, 1,scale);
    state_machine->AddNode(RBSTATE::CROUCH, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Zidle.png",  {0, 0, 60, 80}, 1, 1,scale);
    state_machine->AddNode(RBSTATE::IDLE, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Zwalk.png", {0, 0, 60, 80}, 1, 0.2,scale);
    state_machine->AddNode(RBSTATE::WALK, sprite_sheet_node);

    sprite_sheet_node = new SSNode("assets/img/Zrun.png", {0, 0, 60, 80}, 1, 1,scale);
    state_machine->AddNode(RBSTATE::RUN, sprite_sheet_node); 
        
    sprite_sheet_node = new SSNode("assets/img/Zjump.png",  {0, 0, 60, 80}, 1, 1,scale);
    state_machine->AddNode(RBSTATE::JUMP, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Zfall.png",  {0, 0, 60, 80}, 1, 1,scale);
    state_machine->AddNode(RBSTATE::FALL, sprite_sheet_node); 
    
    sprite_sheet_node = new SSNode("assets/img/Zdash.png",  {0, 0, 60, 80}, 1, 1,scale);
    state_machine->AddNode(RBSTATE::DASH, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Zmelee.png",  {0, 0, 60, 80}, 1, 1,scale);
    state_machine->AddNode(RBSTATE::MELEE, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Zshot.png",  {0, 0, 60, 80}, 1, 1,scale);
    state_machine->AddNode(RBSTATE::SHOT, sprite_sheet_node); 

    sprite_sheet_node = new SSNode("assets/img/Zskid.png",  {0, 0, 60, 80}, 1, 1,scale);
    state_machine->AddNode(RBSTATE::SKID, sprite_sheet_node); 



    // transition creation
    state_machine->AddTransition(RBSTATE::WALK,RBSTATE::RUN);

    // set first colider e state
    Collider * ass_collider = (Collider*)associated.GetComponent(C_ID::Collider);
    if(ass_collider == nullptr) cout << "erro pegando collider em player start\n";
    
    ass_collider->SetScale(Vec2(0.65,0.5)); 
    ass_collider->SetOffset(Vec2(0,8));

    state_machine->ChangeState(RBSTATE::IDLE);//st->AddTransition(0, RBSTATE::RUN); st->AddTransition(RBSTATE::RUN, 0);
    
}

/// @brief Player Update
void Player::Update(float dt){
    
  
    Controls(dt);
    Physics(dt);//!!check weird hitbox when this is off -m
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
    
    auto [state_idx, cr_state] = state_machine->GetCurrent();
    jumpTimer.Update(dt);
    InputManager& inManager = InputManager::GetInstance();
    // UP COMMAND
    bool space_pressed = false;

    
    if(inManager.IsKeyDown(SDLK_SPACE)){
        space_pressed = true;
    }
    if(inManager.IsKeyDown(W_KEY)){  
        if(inManager.KeyPress(W_KEY)){
            if (hasDoubleJump > 0){
                if(isDashing){
                    jumpStored = true;
                }else{
                    jumpTimer.Restart();
                    hasDoubleJump --;
                    speed.y  = 0;
                    speed.y = -JUMP_FORCE*dt;
                    
                }
            }
        }
      
        if (*isGrounded ){
            hasDoubleJump = MAX_DOUBLE_JUMP_QT;
            jumpTimer.Restart();
            *isGrounded = false;
            speed.y = -JUMP_FORCE*dt;
            
            if(isDashing){
                isDreamDashing = true;
            }
            isDashing = false;  
        }
        else if (jumpTimer.Get()<JUMP_ACCE_TIMELIMIT){     
            speed.y = -JUMP_FORCE*dt;
        }
    }else{
        jumpTimer.Update(JUMP_ACCE_TIMELIMIT);
    }
  
    // DOWN COMMAND
    if(inManager.IsKeyDown(S_KEY) ){
        if(inManager.KeyPress(S_KEY)){

        }
        if(*isGrounded ){   
            crouchHeld = true;              
        }      
    }
    
    // LEFT COMMAND
    if(inManager.IsKeyDown(A_KEY) && !crouchHeld && !isDashing){ 
        if(inManager.KeyPress(A_KEY)){
            
        }  
        cr_state->SetFliped(true); 
        
        speed.x -= MOVE_ACCELERATION*dt;

         
        // grudar em rampas
        if(*isGrounded&& (surface_inclination != 0)){
            
            associated.box.y += MAX_MOVE_SPEED * 0.030 ;

        }

        inputDone = true;
    }
    
    // RIGHT COMMAND
    if(inManager.IsKeyDown(D_KEY) && !crouchHeld && !isDashing){
        if(inManager.KeyPress(D_KEY)){
            
        }
        cr_state->SetFliped(false);
        speed.x += MOVE_ACCELERATION*dt;
        

        if(*isGrounded ){
            
            associated.box.y += MAX_MOVE_SPEED * 0.030 ;
                  
        }
        
        inputDone = true;
        
    }
    
    if(inManager.MousePress(LEFT_MOUSE_BUTTON)){
        // associated.box.x = inManager.GetMouseX() + Camera::pos.x;
        // associated.box.y = inManager.GetMouseY() + Camera::pos.y;
        // speed = Vec2(0,0);
       
        
        if(hasDash > 0 && !isDashing){
             hasDash--;
        

            speed = Vec2((inManager.GetMouseX() + Camera::pos.x)-associated.box.GetCenter().x,
                        (inManager.GetMouseY() + Camera::pos.y)-associated.box.GetCenter().y).Normalize() * DASH_FORCE*dt;
            isDashing = true;
            dashTimer.Restart();
        }
       
    }

    if(inManager.MousePress(RIGHT_MOUSE_BUTTON) ){
        associated.box.x = 0;
        associated.box.y = 0;
       
        
    }
}

void Player::Physics(float dt){
    dashTimer.Update(dt);
    if(dashTimer.Get() > DASH_TIMELIMIT){
        
        isDashing = false;
    }
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
    if(!inputDone && !isDashing){   
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
    float local_max = 800;
    if(isDreamDashing){
        local_max = 800;
    }else{
        local_max = MAX_MOVE_SPEED;
    }
    if(isDashing ){
        if(speed.Magnitude() > MAX_MOVE_SPEED*dt){ 
            float overspeed = speed.Magnitude() - MAX_MOVE_SPEED*dt;
            speed = speed - overspeed * speed.Normalize() * 0.05;          
        }
    }else{
        // limitar velocidade lateral
        if(abs(speed.x) > local_max*dt){ 
            float overspeed = abs(speed.x) - local_max*dt;
            float norm = speed.x/abs(speed.x);
            speed = Vec2( (speed.x - overspeed * norm * 0.5) , speed.y );          
        }
        //limitar velocidade de queda
        if(speed.y > MAX_FALL_SPEED*dt){         
            speed.y =  MAX_FALL_SPEED*dt;  
            float overspeed = abs(speed.y) - MAX_FALL_SPEED*dt;
            float norm = speed.y/abs(speed.y);
            speed = Vec2( speed.x , (speed.y - overspeed * norm * 0.5));         
        }
    }  

     
    // mova-se de acordo com a velocidade 
    if(*isGrounded &&speed.y >0) speed.y = 0;

    Vec2 center = Vec2(associated.box.GetCenter() + speed);
    associated.box.SetCenter(center.x,center.y);

}



void Player::Animation(float dt){
    state_machine = (StateMachine*) associated.GetComponent(C_ID::StateMachine);
    auto [state_idx, cr_state] = state_machine->GetCurrent();
    Collider * ass_collider = (Collider*)associated.GetComponent(C_ID::Collider);
  
    if(speed.y < 0 && !*isGrounded){
        state_machine->ChangeState(RBSTATE::JUMP);
        ass_collider->SetScale(Vec2(0.65,0.5)); 
        ass_collider->SetOffset(Vec2(0,8));
    }
    if(speed.y > 2 && !*isGrounded){
        state_machine->ChangeState(RBSTATE::FALL);
        ass_collider->SetScale(Vec2(0.65,0.5)); 
        ass_collider->SetOffset(Vec2(0,8));
    }
    
    if(crouchHeld){
        state_machine->ChangeState(RBSTATE::CROUCH);
        ass_collider->SetScale(Vec2(0.65,0.5)); //!!vai ter que mudar
        ass_collider->SetOffset(Vec2(0,8));
    }else{
        if(speed.x != 0 && *isGrounded && state_idx != RBSTATE::RUN && state_idx != RBSTATE::WALK){    
        state_machine->ChangeState(RBSTATE::WALK);
        ass_collider->SetScale(Vec2(0.65,0.5)); 
        ass_collider->SetOffset(Vec2(0,8));
        }
        

        if(speed.x == 0 && *isGrounded && state_idx != RBSTATE::IDLE){
            state_machine->ChangeState(RBSTATE::IDLE);
            ass_collider->SetScale(Vec2(0.65,0.5)); 
            ass_collider->SetOffset(Vec2(0,8));
        }
    }

    if(isDashing){
        state_machine->ChangeState(RBSTATE::DASH);
        ass_collider->SetScale(Vec2(0.65,0.5)); 
        ass_collider->SetOffset(Vec2(0,8));

        
        associated.angleDeg = (speed.AngleLine(Vec2(1,0)) * 180 / 3.141592);
     
       
        if(abs(associated.angleDeg) > 90){
            associated.angleDeg -= 180;
        }
       
        

        (speed.x < 0)?cr_state->SetFliped(true):cr_state->SetFliped(false); 
    }else{
        associated.angleDeg = 0;
    }
    
    
        


    // if(speed.x < 0) cr_state->SetFliped(true);

    // if (speed.x > 0){
    //     cr_state->SetFliped(false);
    // }
        
    
}

void Player::Jump (float dt){
    speed.y = -JUMP_FORCE*dt;
}
void Player::JustGrounded(){
   
    isDreamDashing = false;
    hasDash = MAX_DASH_QT;
    
    
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