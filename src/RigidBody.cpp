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
    collider->SetScale(Vec2(1,1));
    collider->SetOffset(Vec2(0,0));
    
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
    MOVE_ACCELERATION = 100;
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
    Physics(dt);//!!check weird hitbox when this is off -m


    isGrounded = false;
    inputDone = false;
    
   
}
void RigidBody::Render(){

}
bool RigidBody::Is(std::string type){
    return type == "RigidBody";
}


void RigidBody::NotifyCollision(GameObject& other,Vec2 sep){
    
    Collider * ass_collider = (Collider*)associated.GetComponent("Collider");
    Collider * terrain_collider = (Collider*)other.GetComponent("Collider");
    auto GoUp = [&](float d){
        associated.box.y -= d;
    };
    auto GoDown = [&](float d){
        associated.box.y += d;
    };
    auto GoLeft = [&](float d){
        associated.box.x -= d;
    };
    auto GoRight = [&](float d){
        associated.box.x += d;
    };
    if(TerrainBody * terrain = (TerrainBody*)other.GetComponent("TerrainBody")){
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
        
        int ang = (int)other.angleDeg % 360; // limitar angulo a 360°
        ang = (ang+45)/90;                   //  tecnicamente um cubo girado a 40° eh o mesmo que um em 95º .
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
        

       

        if((((int)other.angleDeg+45)%90)==0) idx == 4;

        switch(idx){//up
            case 0:
                
                GoUp(b);
                if(speed.y >= 0){
                    isGrounded = true; 
                    speed.y =0;
                }    
                

            break;
            case 1://right
                GoRight(b);
               

            break;
            case 2://down
                GoDown(b);
                
                if(speed.y<0){
                    
                    jumpTimer.Set(JUMP_TIMER);
                    speed.y =0;
                }
                

            break;
            case 3://left
                GoLeft(b);

              

            break;
            case 4://45° degree
                if(idx == 0 || idx == 1){//up
             
                    
                    GoUp(b);
                
                    if(speed.y >= 0){//down
                        isGrounded = true; 
                        speed.y =0;
                    }    
                }else{
                    GoDown(b);
                    if(speed.y<0){
                        
                        // jumpTimer.Set(JUMP_TIMER);
                        // speed.y =0;
                    }
                    
                  
                }

            break;
        }
        
     


        Camera::Update(0);
        if(Collider* collider = (Collider*)associated.GetComponent("Collider")){
            collider->Update(0);
        }
            
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
    // p(jumpTimer.Get())cout << endl;
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

Vec2 RigidBody::Bcurve(Vec2 a ,Vec2 b, Vec2 c, Vec2 d,float t) {
    return Vec2( std::pow((1 - t), 3) * a.x + 3 * std::pow((1 - t), 2) * t * b.x + 3 * std::pow((1 - t), 1) * std::pow(t, 2) * c.x + std::pow(t, 3) * d.x
                ,std::pow((1 - t), 3) * a.y + 3 * std::pow((1 - t), 2) * t * b.y + 3 * std::pow((1 - t), 1) * std::pow(t, 2) * c.y + std::pow(t, 3) * d.y);
}

Vec2 RigidBody::Bcurve(std::vector<Vec2> vec, float t) {
    
   
    auto fac = [](const int n){
        int ret = n;
        if(n == 0) return 1;
        for(int i = n-1; i>0 ;i--)ret = ret * i;
        return ret;
    };

    int n = vec.size();
    float x,y = 0;
    
    for (int k = 0; k<n; k++){
        x += fac(n)/fac(k)*fac(n-k) * std::pow((1-t),(n-k)) * std::pow((t),(k)) * vec[k].x;
        y += fac(n)/fac(k)*fac(n-k) * std::pow((1-t),(n-k)) * std::pow((t),(k)) * vec[k].y;
    }

    return Vec2(x,y);

}

