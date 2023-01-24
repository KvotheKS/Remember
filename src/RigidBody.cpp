#include "RigidBody.h"
#include "TerrainBody.h"
#include "ActionMachine.h"
#include "StateMac.h"
#include "Player.h"
#include "SpriteSheetNode.h"
#include <iostream>
#include "Game.h"
// define pra test -m
using namespace std;
#define p(x) cout << #x << ": " << x <<" ";



RigidBody::RigidBody(GameObject& associated):GameObject(associated){
    // Sprite* pbody = new Sprite(associated, "assets/img/Zidle.png");
    // associated.AddComponent(pbody);
    // pbody->SetScaleX(2,2);
   
    speed =  Vec2(0,0);
    oldbox = Vec2(0,0);
    isGrounded = false;
    justGrounded = false;
    hasDoubleJump = true;
    inputDone = false;
    surface_inclination = 0;

    updateCounter = 0;

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
RigidBody::~RigidBody(){

}

void RigidBody::Start(){
  
}

/// @brief RigidBody Update
void RigidBody::Update(float dt){
    // isGrounded = false;
   
}
void RigidBody::Render(){

}
bool RigidBody::Is(std::string type){
    return type == "RigidBody";
}

bool RigidBody::Is(C_ID type){
    return type == C_ID::RigidBody;
}

void RigidBody::NotifyCollision(GameObject& other,Vec2 sep){
    
    Collider * ass_collider = (Collider*)associated.GetComponent(C_ID::Collider);
    
    auto GoUp = [&](float d){
        associated.box.y -= d;
    };
    auto GoDown = [&](float d){
        associated.box.y += d;
    };
    auto GoLeft = [&](float d){
        associated.box.x -= d+1;
    };
    auto GoRight = [&](float d){
        associated.box.x += d+1;
    };

    if(TerrainBody * terrain = (TerrainBody*)other.GetComponent(C_ID::Collider)){

        Collider * terrain_collider = (Collider*)other.GetComponent(C_ID::Collider);
        /* pegar ponto central de cada aresta */
        Vec2 top = (Vec2(0,-1).Rotate(other.angleDeg*PI/180))*(terrain_collider->box.h/2) + terrain_collider->box.GetCenter();
        Vec2 right = (Vec2(1,0).Rotate(other.angleDeg*PI/180))*(terrain_collider->box.w/2)+ terrain_collider->box.GetCenter();
        Vec2 down = (Vec2(0,1).Rotate(other.angleDeg*PI/180))*(terrain_collider->box.h/2)+ terrain_collider->box.GetCenter();
        Vec2 left = (Vec2(-1,0).Rotate(other.angleDeg*PI/180))*(terrain_collider->box.w/2)+ terrain_collider->box.GetCenter();
       
        
        
        /* achar distancia entre esse RigidBody e cada ponto central de aresta */
        vector<float> distances;
        
        distances.push_back(top.Distance(ass_collider->box.GetCenter()));     
        distances.push_back(right.Distance(ass_collider->box.GetCenter()));
        distances.push_back(down.Distance(ass_collider->box.GetCenter()));
        distances.push_back(left.Distance(ass_collider->box.GetCenter()));

        /*achar menor das distancia e cconverter em index */ 
        min_element(distances.begin(), distances.end());
        auto it = std::min_element(std::begin(distances), std::end(distances));
        int idx = std::distance(std::begin(distances), it); 
        
        float ang = fmod(other.angleDeg, 360); // limitar angulo a 360°
        ang = (ang+45)/90;                   //  tecnicamente um cubo girado a 45° eh o mesmo que um em 90º .
        idx += ang;             

        idx = idx%4;

        /* calcular a distancia que o Rigidbody devera se mover.*/
        float C = abs(180 - ((int)other.angleDeg%45) - 90) ;
        float b = sin(C*PI/180);
        if(b == 0){         //evitar div por 0
            b =  sep.Magnitude();
        }else{
            b = sin(90*PI/180) * sep.Magnitude()/b;
        }
        


        

        if((((int)other.angleDeg+45)%90)!=0) {
            switch(idx){
            
            case 0://up
                // cout << "up:";
                associated.box.y -= b;
                if(speed.y >= 0){
                    if(isGrounded == false){
                        if( Player * l_pl = ((Player*)associated.GetComponent(C_ID::Player))) l_pl->JustGrounded(); 
                    }
                    isGrounded = true; 
                    
                    
                    speed.y =0;
                }    

            break;
            case 1://right
                // cout << "right\n";
                GoRight(b);
               

            break;
            case 2://down]
                // cout << "down\n";
                GoDown(b);
                
                if(speed.y<0){
                    
                    jumpTimer.Set(JUMP_TIMER);
                    speed.y =0;
                }
                

            break;
            case 3://left
                // cout << "left\n";
                GoLeft(b);

              

            break;
            }
        }else{//45° degree
            if(idx == 0 || idx == 1){//up
                    
            GoUp(b);
        
            if(speed.y >= 0){//down
                isGrounded = true; 
                
                if( Player * l_pl = ((Player*)associated.GetComponent(C_ID::Player))) l_pl->JustGrounded(); 
                speed.y =0;
            }    
            }else{
                GoDown(b);
                if(speed.y<0){
                    
                    // jumpTimer.Set(JUMP_TIMER);
                    // speed.y =0;
                }
                
                
            }
        }

       
           
        

        
        surface_inclination =  other.angleDeg;
     
        // Camera::Update(0);
        if(Collider* collider = (Collider*)associated.GetComponent(C_ID::Collider)){
            collider->Update(0);
        }
        
            
            
    }
        
}

