#include "RigidBody.h"
#include "TerrainBody.h"
#include <iostream>
#include "Game.h"
// define pra test -m
using namespace std;
#define p(x) cout << #x << ": " << x <<" ";



RigidBody::RigidBody(GameObject& associated, int modo):GameObject(associated), modo(modo){
    Sprite* pbody = new Sprite(associated, "assets/img/Zidle.png");
    associated.AddComponent(pbody);
    pbody->SetScaleX(2,2);
    Collider* collider = new Collider(associated);
    collider->SetScale(Vec2(2,2));
    associated.AddComponent(collider);

    speed =  Vec2(0,0);
    oldbox = Vec2(0,0);
    isGrounded = false;
    hasDoubleJump = true;
    inputDone = false;


    JUMP_TIMER = 0.2;
    // jumpTimer.Update(0);
    MAX_GLOBAL_SPEED = 3000;

    JUMP_FORCE = 1200;
    MAX_FALL_SPEED = 800;
    FALL_ACCELERATION = 100;

    MAX_MOVE_SPEED = 600;
    MOVE_ACCELERATION = 400;
    LATERAL_FRICTION = 80;
    LATERAL_SPEED_THRESHOLD = 100;

}
RigidBody::~RigidBody(){

}

void RigidBody::Start(){

}
void RigidBody::Update(float dt){


    Animation(dt);
    Controls(dt);
    Physics(dt);


    isGrounded = false;
    inputDone = false;
    
   
}
void RigidBody::Render(){

}
bool RigidBody::Is(std::string type){
    return type == "RigidBody";
}


void RigidBody::NotifyCollision(GameObject& other,Vec2 sep){
 
    if(TerrainBody * body = (TerrainBody*)other.GetComponent("TerrainBody")){
        // Lidar com Contato na Direita -m
        if(associated.box.GetCenter().x > other.box.GetCenter().x){
            
            associated.box.x += sep.x;
        }

        // Lidar com Contato na Esquerda -m
        else{

            associated.box.x -= sep.x;
        }

        // Lidar com Contato no Topo -m
        if(associated.box.GetCenter().y < other.box.GetCenter().y){
            if(abs(sep.x) < abs(sep.y)){
                
                
                isGrounded = true; 
                associated.box.y += sep.y;
                speed.y = 0;
            }
            
            
            

        }
        // Lidar com Contato a baixo -m
        else{
            associated.box.y -= sep.y;
        }
        Camera::Update(0);
        if(Collider* collider = (Collider*)associated.GetComponent("Collider")){
        collider->Update(0);
        }
            
        }
        else{
           
        }
}

void RigidBody::Animation(float dt){
    Sprite * spr = (Sprite*) associated.GetComponent(SPRITE_T);
    
    if(speed.y < 0 && !isGrounded){
        spr->Open("assets/img/Zjump.png");
    }
    if(speed.y > 2 && !isGrounded){
        spr->Open("assets/img/Zfall.png");
    }
    if(speed.x != 0 && isGrounded){   
        spr->Open("assets/img/Zrun.png");
    }
    if(speed.x == 0 && isGrounded){
        spr->Open("assets/img/Zidle.png");
        speed.x = 0;
    }
    if(speed.x < 0){
        spr->SetFliped(true);
    }else if (speed.x > 0){
        spr->SetFliped(false);
    }
}

void RigidBody::Controls(float dt){
    p(jumpTimer.Get())cout << endl;
    jumpTimer.Update(dt);
    InputManager& inManager = InputManager::GetInstance();
    if(inManager.IsKeyDown(W_KEY)){  
        if (isGrounded ){
            jumpTimer.Restart();
            isGrounded = false;
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
        speed.x -= MOVE_ACCELERATION*dt;
        inputDone = true;
    }
    if(inManager.IsKeyDown(D_KEY)){
       
        speed.x += MOVE_ACCELERATION*dt;
        inputDone = true;
        
    }
    
    if(inManager.MousePress(LEFT_MOUSE_BUTTON) && modo == 1){
        associated.box.x = inManager.GetMouseX() + Camera::pos.x;
        associated.box.y = inManager.GetMouseY() + Camera::pos.y;
        speed = Vec2(0,0);
    }

    if(inManager.MousePress(RIGHT_MOUSE_BUTTON) && modo == 1){
        associated.box.x = 0;
        associated.box.y = 0;
        // speed = Vec2(-100,0);
        
    }
}

void RigidBody::Physics(float dt){
      
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

    // // limitar velocidade global
    // if(speed.Magnitude() > MAX_GLOBAL_SPEED*dt){         
    //     speed = speed.Normalize()*MAX_GLOBAL_SPEED*dt;          
    // }

    //limitar velocidade lateral
    if(abs(speed.x) > MAX_MOVE_SPEED*dt){ 
        speed = Vec2((speed.x/abs(speed.x))*MAX_MOVE_SPEED*dt, speed.y );          
    }

    //limitar velocidade de queda
    if(speed.y > MAX_FALL_SPEED*dt){         
        speed.y =  MAX_FALL_SPEED*dt;          
    }

     
    // mova-se de acordo com a velocidade 
    if(isGrounded) speed.y = 0;

    Vec2 center = Vec2(associated.box.GetCenter() + speed);
    associated.box.SetCenter(center.x,center.y);

}
void RigidBody::MoveonTopof(GameObject& target){

}

void RigidBody::Jump (float dt){
    speed.y = -JUMP_FORCE*dt;
}